
# GLSL

[learnopengl](https://learnopengl-cn.readthedocs.io/zh/latest/01%20Getting%20started/05%20Shaders)

着色器是使用一种叫 GLSL 的类C语言写成的。GLSL 是为图形计算量身定制的，它包含一些针对向量和矩阵操作的有用特性。

着色器的开头总是要声明版本，接着是输入和输出变量、uniform和main函数。每个着色器的入口点都是main函数，在这个函数中我们处理所有的输入变量，并将结果输出到输出变量中。

## 缩写/中英文对照

|EN |CN
|- |-
|Vertex Attribute           |顶点属性: 顶点着色器代码中的输入变量
|Vector Swizzling           |向量重组

## 典型结构

```glsl
#version version_number

in type in_variable_name;
in type in_variable_name;
out type out_variable_name;
uniform type uniform_name;

int main() {
  // 处理输入并进行一些图形操作
  ...
  // 输出处理过的结果到输出变量
  out_variable_name = weird_stuff_we_processed;
}
```

## 概念

### 顶点属性(Vertex Attribute)

OpenGL 中有至少 16 个顶点属性可用

```c
// 查询可用顶点属性数量
GLint nrAttributes;
glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
```

## 数据类型

### 基础类型

C 语言中的 int, float, double, uint, bool

### 向量(Vector)

GLSL中的向量是一个可以包含有1、2、3或者4个分量的容器，分量的类型可以是前面默认基础类型的任意一个。它们可以是下面的形式（n代表分量的数量）

|type |desc
|- |-
|vecn   |包含 n 个 float  分量的默认向量, n 取值范围 **[2, 4]**
|dvecn  |包含 n 个 double 分量的向量
|bvecn  |包含 n 个 bool   分量的向量
|ivecn  |包含 n 个 int    分量的向量
|uvecn  |包含 n 个 unsigned int 分量的向量

#### 获取向量分量

```glsl
vec.x, vec.y, vec.z, vec.w    // 获取第 1, 2, 3, 4 个分量
vec.r, vec.g, vec.b, vec.a    // 对颜色可以使用 rgba
vec.s, vec.t, vec.p, vec.q    // 对纹理坐标可以使用 stpq
```

#### 重组

使用向量中已有分量组合出新的向量，每个分量可引用多次

```glsl
vec2 v2;                        // 2 个分量
vec4 v4 = v2.xyxx;              // 4 个分量
vec3 v3 = v4.zyw;               // 3 个分量
vec4 v42 = v2.xxxx + v3.yxzy;   // 向量加法
```

直接将向量作为参数传递给向量构造函数

```glsl
vec2 v2 = vec2(0.1f, 0.2f);
vec4 v4 = vec4(v2, 0.0f, 0.0f);
vec4 v42 = vec4(v4.xyz, 1.0f);
```

### 矩阵(Matrix)

* mat2: 2 x 2 矩阵
* mat3: 3 x 3 矩阵
* mat4: 4 x 4 矩阵

#### 运算

```glsl
mat4 transform;
vec4 v;
// 使用矩阵操控向量
vec4 ret = transform * v;
```

### 纹理对象采样器(Sampler)

搭配 texture 获取纹理像素颜色

```yml
sampler1D: 一维纹理
sampler2D: 二维纹理(图片)
sampler3D: 三维纹理
```

## 函数

### texture

获取纹理像素

```c
vec4 texture(sampler2D texture, vec2 coordinate);

texture   : 纹理采样对象
coordinate: 图片采样坐标点，(0,0) 对应左下，(1,1)对应右上
```

### mix

对两个数据做线性插值

```c
vec4 mix(vec4 color1, vec4 color2, float v2_ratio);

v2_ratio: 第二个颜色所占比例
output  : color1 * (1 - v2_ratio) + color2 * v2_ratio
```

## 控制语句

### if

```glsl
int abc;
if (abc == 1) {
} else if (abc == 2) {
} else {
}
```

## 输入输出

* 关键字: **in**, **out**
* 前一个着色器的输出是后一个着色器的输入，只要 **变量类型** 和 **变量名** 都一样即可(除顶点着色器的输入和片段着色器的输出)
* 顶点着色器直接从顶点数据接收输入，通过 **location** 指定输入位置
* 片段着色器的输出应是 vec4 颜色变量

### Uniform

Uniform 是一种从 CPU 应用想 GPU 着色器程序发送数据的方式。

* Uniform 是 **全局** 的，因此变量命名需要保证唯一性
* 着色器程序的任意阶段都可以访问 uniform
* Uniform 中的值只有被重置或更新时才会改变
* 如果在 GLSL 中声明了一个 uniform 但未使用，编译器会默认将其移除

```glsl
out vec4 color;
uniform vec4 my_color;    // 声明 uniform 变量
void main() {
    color = my_color;     // 引用 uniform 变量
}
```

```c
// 将绿色设置为随时间变化的函数
GLfloat green = (sin(glfwGetTime()) / 2) + 0.5;
// 从着色器程序中获取 uniform 变量位置
GLint my_color_loc = glGetUniformLocation(shader_program, "my_color");
glUseProgram(shader_program);
// 设置 uniform 变量值
glUniform4f(my_color_loc, 0.0f, green, 0.0f, 1.0f);
```
