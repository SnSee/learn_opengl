
# OpenGL Specification

## 中英文对照

|EN |CN
|- |-
|framebuffer            |帧缓冲
|antialiasing           |抗锯齿

## 概念

### SPIR-V

SPIR-V（Standard Portable Intermediate Representation - Vulkan）是一种中间语言，专门用于图形渲染和通用计算。它由Khronos Group开发，主要用于Vulkan图形API，但也支持OpenCL和其他Khronos APIs。SPIR-V的主要目的是提供一种标准化的、语言无关的中间表示，使得编译器可以将多种高级编程语言（如GLSL、HLSL）编译成SPIR-V，然后再由GPU的驱动程序将其转换为设备的本地指令集。

## Specification Chapters

### OpenGL Fundamentals

#### Execution Model

* 当图元(primitive)顶点数据(vertex)被剪切(clipped)时会产生新的顶点数据

* GL commands 是顺序执行的，前一条命令执行完毕后下一条命令才会执行
* GL commands 是 client-server 模型: 用户程序(client) 调用命令，命令被 GL (server) 解析并执行，server 可以远程部署

```yml
帧缓冲(framebuffer)有两种:
    1.default framebuffer: window system-provided(窗口系统内部管理)，reading 使用
    2.framebuffer object: application created(用户管理)，drawing 使用
```

#### Command Syntax

* 函数声明使用 ANSI C syntax
* void Uniform{1234}{if}(int location, T value); 代表 1i,1f,2i,2f,... 共 4*2=8 种函数声明，其中 Uniform4i 表示接收 4 个 int 类型参数，可能的类型见 Table 2.1
* GL 基础类型以 GL 开头，如 GLint，其字节数必须和 Table 2.2 一致，并不一定和 C 类型一样

* 参数类型转换

|expected type |conversion
|- |-
|boolean        |0/0.f -> FALSE，nonzero -> TRUE
|integer/enum   |FALSE -> 0, TRUE -> 1, floating -> round(floating)，如果float太大超出范围则结果不确定
|floating       |FALSE -> 0.0, TRUE -> 1.0, integer -> float(integer)/double(integer)

* 查找函数(Query command)以 Get 开头，当实际返回类型和函数需要的返回类型不一致时会进行类型转换

|expected type |conversion
|- |-
|boolean            |0/0.f -> FALSE，nonzero -> TRUE
|unsigned integer   |negative -> 0
|integer            |FALSE -> 0, TRUE -> 1, floating -> round(floating)，例外: RGBA component, DepthRange, depth buffer，详见 Table 18.2
|floating           |FALSE -> 0.0, TRUE -> 1.0, integer -> float(integer)/double(integer)

#### Command Execution

规范中主要讨论单个 CPU 线程中绑定的单个 context。但多线程(各自绑定一个 context)也可以共享 GL 对象。可以将关联较小的任务分发到多个 context 中以提高执行速度和 GPU 利用率

##### Errors

只有可能发生错误时才会检查 error，防止影响 error-free 程序

```c
enum GetError(void);
```

Table 2.3: Summary of GL errors 查看所有 GL 错误

|Implicit Errors |desc
|- |-
|NO_ERROR           |表示无错
|CONTEXT_LOST       |没有当前 context
|INVALID_ENUM       |实参为非法枚举值
|INVALID_VALUE      |形参为 sizei/sizeiptr，实参为负数
|OUT_OF_MEMORY      |内存不足
|INVALID_OPERATION  |当前状态不允许枚举值对应操作

* 如果有 error 但未调用 GetError，该 error 标志会一直存在，且不会被后面的 error 刷新
* 当有 error 时，如果是 OUT_OF_MEMORY 则 GL 行为不确定，其他情况下导致该 error 的 GL 命令无效，即不会影响 GL state 和 framebuffer
* 当命令发生 error 时，如果有返回值一般为 0，按地址传递的值不会被修改
* No Error Mode: 如果发生错误，行为未定义，可能会崩溃; GetError 只会返回 NO_ERROR/OUT_OF_MEMORY
