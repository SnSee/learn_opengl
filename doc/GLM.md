
# GLM

[github](https://github.com/icaven/glm)

OpenGL Mathematics, GLM 只有头文件，只需要 include 即可

```c
// 常用头文件
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
```

## Important

### 列主序

OpenGL 通常使用一种内部矩阵布局，叫做列主序(Column-major Ordering)，GLM 的默认布局就是列主序。

## functions

### normalized

```c
// 将向量 x 转换为单位向量(方向不变，长度变为 1)
vec3 normalize(vec3 const& x);
```

### 向量运算

```c
glm::vec3 va(1.f);
glm::vec3 vb(2.f);

float dr = glm::dot(va, vb);            // 点乘
glm::vec3 cr = glm::cross(va * vb);     // 叉乘
glm::vec3 cw = va * vb;                 // 分量乘法
```

## demo

### 角度 / 弧度

```c
glm::radians(90.0f);            // 角度转弧度
```

### 获取矩阵对象内存地址

```c
glm::mat4 trans = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
const GLfloat *trans_ptr = glm::value_ptr(trans);
```

### 单位矩阵

```c
glm::mat4 m(1.0f);              // 4 行 4 列单位矩阵
```

### 投影矩阵

```c
// 正交投影矩阵
// near_z: 近平面，far_z: 远平面
glm::ortho(left_x, right_x, bottom_y, top_y, near_z, far_z);

// 透视投影矩阵
// fov: 上下视野夹角，通常取 glm::radians(45.0f)
//     上下视野夹角越大，上下方向上看到的画面越多，物体就显得越矮
// aspect_ratio: 宽高比，该值越大，相当于水平视野夹角越大
//     水平视野夹角越大，水平方向看到的画面越多，物体就显得越窄
//     不影响看到的画面的高度
//     该值一般设为 viewport 宽高比？
//     该值会被取绝对值
// near_z: 近平面，通常取 0.1
// far_z: 远平面，通常取 100.0
glm::mat4 proj = glm::perspective(fov, aspect_ratio, near_z, far_z);
```

### 移动向量

* 旋转操作是绕视口原点进行的
* 缩放操作也是以视口原点为基准：像素坐标距离乘以缩放因子作为新坐标

```c
glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);      // 最后的 1.0 为齐次坐标
// 创建位移矩阵
glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f));
vec = trans * vec;                          // 执行位移操作
```

### 缩放 + 旋转 + 位移 矩阵

```c
glm::mat4 trans(1.0f);
trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
```

### camera

```c
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi

glm::mat4 camera(float Translate, glm::vec2 const& Rotate) {
    glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 100.f);
    glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
    View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
    View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
    return Projection * View * Model;
}
```

#### lookAt

通过创建观察矩阵来模拟摄像机

```c
// 用法: 使用 lookAt 创建 View-Matrix
glm::mat4 view = glm::lookAt(
    glm::vec3(0.0f, 0.0f, 3.0f),    // 摄像机位置 pos
    glm::vec3(0.0f, 0.0f, 0.0f),    // 摄像机指向的位置，一般为 pos + front_vector
    glm::vec3(0.0f, 1.0f, 0.0f)     // 上向量(摄像机坐标系的 y 轴正方向)
);
```

原理

![look_at](./pics/look_at.jpg)

```c
// 实现
glm::mat4 lookAt(const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec3 &up) {
    const glm::vec3 &P = pos;
    glm::vec3 D = glm::normalize(pos - dir);
    glm::vec3 R = glm::normalize(glm::cross(up, D));
    glm::vec3 U = glm::cross(D, R);     // 已经是单位向量了，不需要 normalize
    glm::mat4 lm(1.f);                  // rotation
    glm::mat4 rm(1.f);                  // translation
    lm[0][0] = R.x;
    lm[1][0] = R.y;
    lm[2][0] = R.z;
    lm[0][1] = U.x;
    lm[1][1] = U.y;
    lm[2][1] = U.z;
    lm[0][2] = D.x;
    lm[1][2] = D.y;
    lm[2][2] = D.z;

    rm[3][0] = -P.x;
    rm[3][1] = -P.y;
    rm[3][2] = -P.z;

    return lm * rm;
}
```
