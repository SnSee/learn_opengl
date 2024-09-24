
# OpenGL

[官网](https://www.opengl.org)

[API](https://www.khronos.org/opengl/wiki/Category:Core_API_Reference)

[LLVM官网](https://releases.llvm.org)

[glfw官网](https://www.glfw.org)

[glew官网](https://glew.sourceforge.net/index.html)

[Specifications](https://registry.khronos.org/OpenGL/index_gl.php#apispecs)

[Windows OpenGL Learn](https://learn.microsoft.com/zh-cn/windows/win32/opengl/opengl)

[learnopengl](https://learnopengl.com)

[learnopengl-CN](https://learnopengl-cn.readthedocs.io/zh/latest)

[Vulkan Tutorial](https://vulkan-tutorial.com)

[OpenCL Guide](https://github.com/KhronosGroup/OpenCL-Guide)

* OpenGL 只是规范(Specification)，并不提供具体实现(Source Code)
* [Mesa3D](https://www.mesa3d.org) 是一个开源的实现版本, 显卡驱动也是实现
* 显卡供应商如 Nvidia 会提供 SDK 及 demo

## 缩写/中英文对照

|Abbr |Full
|- |-
|OpenGL |Open Graphics Library
|GLUT   |OpenGL Utility Toolkit
|GLw    |OpenGL Widget Library
|GLEW   |OpenGL Extension Wrangler Library
|GLFW   |OpenGL Framework Window
|GLSL   |OpenGL Shading Language
|NDC    |Normalized Device Coordinates
|VBO    |Vertex Buffer Objects
|VAO    |Vertex Array Object
|IBO    |Index Buffer Object
|EBO    |Element Buffer Object

|EN |CN
|- |-
|Immediate mode             |立即渲染模式(固定渲染管线)
|Core-profile               |核心模式
|State Machine              |状态机
|State-changing Function    |状态设置函数
|State-using Function       |状态应用函数
|Primitive Type             |基元类型: GL 定义的基础类型如 GLfloat
|Viewport                   |视口
|Graphics Pipeline          |图形渲染管线
|Shader                     |着色器: 执行渲染工作的程序
|OpenGL Shading Language    |OpenGL 着色器语言
|Vertex Shader              |顶点着色器: 在 **GPU** 显存上存储顶点数据
|Shape(Primitive) Assembly  |形状(图元)装配
|Geometry Shader            |几何着色器
|Rasterization              |光栅化
|Fragment Shader            |片段着色器: 计算像素点最终颜色等
|Tests And Blending         |测试与混合: 检查片段深度和透明度等
|Primitive                  |图元: GL 定义的形状类型如 GL_POINTS
|Fragment                   |片段: 一个片段是OpenGL渲染一个像素所需的所有数据
|Blend                      |混合
|Stencil                    |(深度)模板
|Normalized Device Coordinates|标准化设备坐标: x,y,z 有效范围都是 (-1,1)，范围外会被裁剪掉，对应 glViewport 设置的视口范围
|Screen-space Coordinates   |屏幕空间坐标
|Viewport Transform         |视口变换
|VBO                        |顶点缓冲对象: 将内存数据缓冲后传递到显存中并维护
|VAO                        |[顶点数组对象](#vao)
|EBO/IBO                    |索引缓冲对象: 专门缓冲索引
|Indexed Drawing            |索引绘制: 根据索引获取顶点数据进行绘制
|Wireframe Mode             |线框模式

## 系统自带 OpenGL 库及查看版本

* Windows: opengl32.lib 或 opengl32.dll
* Linux: libGL.so
* 自定义: Mesa

```c
// OpenGL 版本号
glGetIntegerv(GL_MAJOR_VERSION, *);
glGetIntegerv(GL_MINOR_VERSION, *);
glGetString(GL_VERSION);                    // OpenGL 版本号
glGetString(GL_VENDOR);                     // 实现当前版本的厂商
glGetString(GL_RENDERER);                   // 渲染器(GPU)名称
glGetString(GL_SHADING_LANGUAGE_VERSION​);   // 支持的 GLSL 版本
```

## 概念

### EGL/GLES

OpenGL ES (OpenGL for Embedded Systems) 是一种为嵌入式系统设计的图形API。它是OpenGL的一个子集，比桌面版的OpenGL精简，去掉了许多复杂的功能，旨在提供一个更轻量级的图形API，适合性能和资源有限的设备。

EGL (Embedded-System Graphics Library) 是一个中间层接口，主要用于在OpenGL ES（GLES）和本地窗口系统（如X Window系统或Android的Surface）之间提供连接。它负责创建和管理OpenGL ES上下文，处理图形缓冲区的分配以及交换缓冲区等操作。

### GLFW

[download](https://www.glfw.org/download.html)

GLFW 是一个跨平台的库，用于创建窗口和处理输入事件，如键盘、鼠标和游戏手柄。它提供了一系列函数，用于设置窗口的属性、处理窗口的消息循环以及处理各种输入设备的事件。GLFW 的目标是为 OpenGL 程序提供一个轻量级、高效和跨平台的窗口和输入接口。

### GLEW

[download](https://glew.sourceforge.net/index.html)

GLEW 是一个用于 OpenGL 扩展的库，它可以帮助开发者高效地检测和加载 OpenGL 扩展。GLEW 提供了一个简单的接口来检测当前系统上支持的 OpenGL 版本以及可用的扩展，并且能够自动加载所需的函数指针。

### State Machine AND Context

OpenGL自身是一个巨大的状态机：一系列变量描述 OpenGL 此刻应当如何运行。OpenGL的状态通常被称为OpenGL上下文(Context)。我们通常使用如下途径去更改OpenGL状态：设置选项，操作缓冲。最后，我们使用当前OpenGL上下文来渲染。

* 可以将 context 看作是驱动用来保存 GL 程序状态信息的大结构体
* context 包含许多资源，如 driver resources in RAM, texture IDs assigned, VBO IDs assigned, enabled states 等
* 使用 GL function 前必须创建 GL context 并设置为当前 context(可以从窗口创建?)
* 每个线程都有自身的 context，一个 context 不能同时被多个线程设置为当前

### 双缓冲(Double Buffer)

应用程序使用单缓冲绘图时可能会存在图像闪烁的问题。 这是因为生成的图像不是一下子被绘制出来的，而是按照从左到右，由上而下逐像素地绘制而成的。最终图像不是在瞬间显示给用户，而是通过一步一步生成的，这会导致渲染的结果很不真实。为了规避这些问题，我们应用双缓冲渲染窗口应用程序。前缓冲保存着最终输出的图像，它会在屏幕上显示；而所有的的渲染指令都会在后缓冲上绘制。当所有的渲染指令执行完毕后，我们交换(Swap)前缓冲和后缓冲，这样图像就立即呈显出来，之前提到的不真实感就消除了。

### depth / 深度值

通常可以理解为 z 坐标，它代表一个像素在空间中和你的距离，如果离你远就可能被别的像素遮挡，你就看不到它了，它会被丢弃，以节省资源。

### 着色器程序

着色器程序对象(Shader Program Object)是多个着色器合并之后并最终链接完成的版本。如果要使用编译好的着色器必须把它们链接为一个着色器程序对象，然后在渲染对象的时候激活这个着色器程序。已激活着色器程序的着色器将在我们发送渲染调用的时候被使用。

当链接着色器至一个程序的时候，它会把每个着色器的输出链接到下个着色器的输入。当输出和输入不匹配的时候，会得到一个连接错误。

### VAO

一个顶点数组对象会储存以下这些内容：

* **glEnableVertexAttribArray** 和 **glDisableVertexAttribArray** 的调用
* 通过 **glVertexAttribPointer** 设置的顶点属性配置
* 通过 **glVertexAttribPointer** 调用进行的顶点缓冲对象与顶点属性链接
* 绑定的 EBO 对象(EBO 对象在 VAO 解绑前不能解绑)

```cpp
GLuint VAO;
glGenVertexArrays(1, &VAO);
// ..:: 初始化代码 :: ..
// 1. 绑定顶点数组对象
glBindVertexArray(VAO);
    // 2. 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. 复制我们的索引数组到一个索引缓冲中，供OpenGL使用
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 3. 设定顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
// 4. 解绑VAO（不是EBO！）
glBindVertexArray(0);

// ..:: 绘制代码（游戏循环中） :: ..

glUseProgram(shaderProgram);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0)
glBindVertexArray(0);
```

## API / 函数接口

### GL API

[API](https://www.khronos.org/opengl/wiki/Category:Core_API_Reference)

#### GL Core

|function |desc
|- |-
|glGetString                |获取对应 context 信息
|glGetIntegerv              |获取对应参数值
|glGetError                 |获取 GL 函数 [错误码](https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetError.xhtml)
|glDebugMessageCallback     |注册 error 回调函数

#### GL Window

#### GL Buffer

|function |desc
|- |-
|glGenBuffers               |生成 VBO 对象
|glBindBuffer               |设置缓冲类型
|glBufferData               |将顶点数据复制到内存缓冲区
|glGenVertexArrays          |生成 VAO 对象
|glBindVertexArray          |[绑定 VAO 对象](#vao)

#### GL Shader

|function |desc
|- |-
|glCreateShader             |创建一个着色器对象
|glCompileShader            |编译着色器
|glDeleteShader             |删除着色器，把着色器链接到程序对象后即可删除
|glGetShaderiv              |检查着色器是否编译成功
|glGetShaderInfoLog         |获取编译着色器日志
|glCreateProgram            |创建一个程序对象以链接 shader
|glAttachShader             |将 shader 添加到 program
|glLinkProgram              |链接已添加的 shader
|glUseProgram               |应用程序对象
|glGetProgramiv             |检查程序是否执行成功
|glGetProgramInfoLog        |获取程序执行日志
|glEnableVertexAttribArray  |启用顶点属性

#### GL Draw

|function |desc
|- |-
|glDrawArrays               |从 array 获取数据绘制图元
|glDrawElements             |从索引缓冲间接获取数据绘制图元
|glPolygonMode              |设置如何绘制图元，如线框还是填充等
|glClearColor               |设置 glClear 要用的 RGBA 颜色
|glClear                    |将窗口设置为 glClearColor, glClearDepth 和 glClearStencil 指定的值

### GLFW API

[API](https://www.glfw.org/docs/latest/topics.html)

|function |desc
|- |-
|glfwInit                   |初始化
|glfwWindowHint             |[设置窗口属性](https://www.glfw.org/docs/latest/window.html#window_hints)
|glfwCreateWindow           |创建窗口
|glfwGetFramebufferSize     |获取窗口尺寸
|glfwWindowShouldClose      |检查 GLFW 是否应该退出
|glfwPollEvents             |检查是否有键盘、鼠标等事件
|glfwSwapBuffers            |交换颜色缓冲(将缓冲区所有像素颜色输出到屏幕?)
|glfwGetError               |获取错误信息，没有错误为 NULL
|glfwSetKeyCallback         |设置键盘回调函数

### GLEW API

|function |desc
|- |-
|glViewport                 |设置视口
|glVertexAttribPointer      |设置如何解析顶点数据(glBufferData 设置)
|glShaderSource             |设置着色器代码

#### glViewport

```cpp
// * OpenGL 坐标范围为 (-1, 1)，最终会映射到屏幕坐标，即 (0, width) 和 (0, height)
void glViewport(GLint left_bottom_x, GLint left_bottom_y, GLsizei width, GLsizei height);
```

#### glVertexAttribPointer

每个顶点属性从一个 VBO 管理的内存中获得数据，该 VBO 是在调用 glVertexAttribPointer 时绑定到 GL_ARRAY_BUFFER 的那一个。

```yml
arg 1: 要配置的顶点属性编号(N)，即顶点着色器中的 layout(location = N) 对应的属性
arg 2: 顶点属性包含数据个数
arg 3: 数据类型
arg 4: 是否需要标准化数据，标准化后所有数据会被映射为 (0,1) 或 (-1,1)
arg 5: 步长，即顶点属性组之间间隔
arg 6: 位置数据偏移量
```

## Demo

### learnopengl

[绘制三角形](https://learnopengl.com/code_viewer.php?code=getting-started/hellotriangle)

## Mesa

[demo](https://archive.mesa3d.org/demos)

### 编译安装

[官网指南](https://docs.mesa3d.org/install.html)

注意:

```txt
1. 优先使用系统提供的软件版本，没有符合要求的版本时再源码安装
2. 保证 LLVM 相关库版本一致
```

* 遇到缺少的环境时使用 apt 安装，如果没有就前加 lib 后加 -dev 试试
* 使用 apt search 查看哪些包包含所需组件
* 版本都太低就自己找源码编译

#### 安装系统库

```sh
sudo apt install libglvnd-dev
sudo apt install libtizonia-dev
sudo apt install zstd
sudo apt install rust-1.80-all
sudo apt install libelf-dev
sudo apt install byacc
sudo apt install flex
sudo apt install bison
sudo apt install libwayland-dev
sudo apt install libwayland-egl1-mesa
sudo apt install libwayland-egl1-mesa
sudo apt install libwayland-dev
sudo apt install libwayland-egl-backend
sudo apt install libwayland-egl-backend-dev
sudo apt install libxcb-randr0-dev
sudo apt install libxext-dev
sudo apt install libxfixes-dev
sudo apt install libxcb-glx0-dev
sudo apt install libxcb-shm0-dev
sudo apt install libx11-xcb-dev
sudo apt install libxcb-dri2-0-dev
sudo apt install libxcb-dri3-dev
sudo apt install libxcb-present-dev
sudo apt install libxshmfence-dev
sudo apt install libxxf86vm-dev
sudo apt install libxrandr-dev
sudo apt install cbindgen
pip install ply -i http://mirrors.aliyun.com/pypi/simple --trusted-host mirrors.aliyun.com
# 参考 rust#cargo 配置镜像源
cargo install --force cbindgen
# 如果报错使用了 unstable feature
rustup default nightly
```

#### DRM

[drm git仓库](git://anongit.freedesktop.org/mesa/drm)
[github镜像](https://github.com/Distrotech/libdrm)

Direct Rendering Manager，Linux内核中的一个子系统，用于提供对图形硬件的直接访问。

```sh
git clone git://anongit.freedesktop.org/mesa/drm
meson build
sudo ninja -C build install
```

#### 编译 LLVM

```sh
git clone https://github.com/llvm/llvm-project.git
# 根据需要使用 -D 设置宏
cmake -S llvm -B build -G "Unix Makefiles" -DLLVM_ENABLE_PROJECTS="clang;lld;libclc"
cd build
make -j8
```

#### 编译 llvm-spirv

```sh
git clone https://github.com/KhronosGroup/SPIRV-LLVM-Translator.git
mkdir build & cd build
cmake ..
make llvm-spirv
```

#### 编译 libclc

```sh
# 代码就是 llvm-project 代码
mkdir build-libclc & cd build-libclc
# llvm-spirv 使用使用的路径
cmake -S ../libclc -B . -G Ninja -DLLVM_CONFIG="$LLVM_CONFIG" -DLIBCLC_TARGETS_TO_BUILD="spirv-mesa3d-;spirv64-mesa3d-" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr -DLLVM_SPIRV=/usr/bin/llvm-spirv
ninja -j8
ninja install
```

#### 编译 mesa

```sh
# prefix: 安装路径，如系统目录 /usr
# libdir: 一般 /usr/lib 或 /usr/lib64
# dri-drivers: 驱动路径，使用 find -type d -name dri 查找
# meson configure --prefix=/usr --libdir=xxx -D dri-drivers-path=xxx ..
mkdir build & cd build
# 使用 llvm 的选项要和编译时一致，如使用静态库，不使用 RTTI 等
meson setup .. -Dshared-llvm=disabled -Dcpp_rtti=false -Ddefault_library=shared
# 使用指定版本的 llvm
# meson setup .. --native-file ../custom-llvm.ini
ninja -j8       # meson compile -C .
sudo meson install
```

## QA

[gl, glu, glut 区别](https://blog.csdn.net/fanhenghui/article/details/52882837)
