
# NIR

[NIR Documentation](https://people.freedesktop.org/~cwabbott0/nir-docs/index.html)

[what-is-nir](https://people.freedesktop.org/~cwabbott0/nir-docs/intro.html#what-is-nir)

[知乎](https://zhuanlan.zhihu.com/p/679966416)

NIR 是 Mesa 中为图形驱动设计的一种中间表示(Intermediate Representation)。在 NIR 之前是多种类别的 frontend IR，之后是特定驱动的 backend IR

NIR 设计原则:

* backend-agnostic: 即和后端操作无关，大多数操作都是使用和 GLSL 中操作变量类似的方式来描述的
* natively understand GPU's constructs: 即使用 GPU 原生结构进行构建
* compatible with literature around compiler technology: 即与 compiler 相关的技术文献兼容
* not designed for users outside of Mesa: 即在 Mesa 项目之外不可用

## 名词

|EN |CN
|- |-
|IR             |Intermediate Representation
|SSA            |Single Static Assignment
|Instruction    |指令
|PRE            |Partial Redundancy Elimination
|GCM            |Global Code Motion
|LICM           |Loop-Invariant Code Motion: 将和循环无关代码移动到循环外

NIR 基础概念

|concept |desc
|- |-
|Basic Block    |基本块: 一段顺序执行的代码，没有分支、跳转、返回
|Block Edge     |连接 block
|Instruction    |指令: 执行特定操作，如算数运算、加载存储、条件分支，包含 opcode 和 operands
|Opcode         |操作码: 表示指令具体操作，如 ADD、IF 等
|Operands       |操作数: 指令的输入输出
|Registers      |寄存器: 生命周期在基本块内
|Expression     |表达式
|CFG            |控制流图(Control Flow Graph): 基本块和控制流边组成的图，描述程序中顺序、条件分支和循环等控制结构。通过分析控制流图，编译器可以进行各种优化，例如循环展开、循环合并、条件分支优化等，以提高着色器程序的性能和效率。
|Control Flow Edge|控制流边: 连接两个基本块，表示程序执行时的跳转关系
|Entry Block    |控制流图起点
|Exit Block     |控制流图终点
|Module         |模块: 表示整个着色器，最顶层结构

## 概念

### GCN/RDNA

### front/backend compiler

将 NIR 转换为特定驱动的 beck-end IR 称为 backend compiler，对应的将 GLSL/frontend IR 转换为 NIR 称为 frontend compiler

### CFG

CFG 是在编译器优化和分析中广泛使用的数据结构。它提供了对程序控制流结构的抽象，允许编译器对程序进行各种优化，如死代码删除、循环展开等。CFG 还用于执行静态分析，识别程序中的控制流特征，为优化提供更多信息。

## NIR Data Structure

* 数据结构在 nir.h 中定义
* 通过聚合模拟继承机制
* 使用 NIR_DEFINE_CAST 宏将对象向上转型

```yaml
nir_shader: 已链接或未连接的 shader
nir_function: 链接阶段 GLSL 格式的重载函数
nir_function_overload: 重载函数的声明或定义
nir_function_impl: 重载函数的实现，包含局部变量和寄存器等，并且是 control-flow-tree 的根节点
nir_cf_node: control-flow-tree 中的一个节点
nir_instr: instruction 基类
```

## NIR Control Flow

[Control Flow](https://people.freedesktop.org/~cwabbott0/nir-docs/control_flow.html)

GPU 的一个特性是它会在不同的 **cores** 中通过 **lock step** (线程锁)方式执行同一个 **program** (比如一段代码)的不同 **instances**(即多线程)。当 control flow **diverges** 时(即两个 instance 走向不同分支)，这两条分支都会执行。

比如: 在 block0(即 basic block) 中有两个线程分别为 thread1 和 thread2，thread1 要跳转到 block1 而 thread2 要跳转到 block2，则 GPU 会 **先跳转** 到 block1(enable thread1，disable thread2) 运行，直到到达 **predefined merge block** 时(两条分支在这里 **converge**)，GPU 会 **再跳转** 到 block2(disable thread1, enable thread2) 运行，也直到到达 predefined merge block，此时 thread1 和 thread2 可同时 enable。

**cross-channel operations:** 在 GPU 的某些操作中，**separate instances(also called channels or lanes)** 可以共享 information。
**uniform control flow:** 所有 channel(thread) 必须同时 enable/disable，即保证所有 instances 使用相同的条件(**invariant**)，走同一条分支路径。多数 backend 都基于这个保证。

### NIR Control Flow Model

NIR control flow model 包含 Structured control flow element，如 loops 和 if

```yml
Control Flow Tree:
    - if statements
    - loop statements
    - basic blocks
Control Flow Graph:
```

* 即使 if 没有 else 分支，也会为其创建一个 empty block: 作用是移除 **critical edge**
* NIR 中没有大多数 GPU 中 **predicated break** 和 **predicated continue** 指令
* loop 前有一个 empty block，作用是在优化下面这种情况: 当 loop 中有 statement 是和 loop 无关的(**loop-independent**)，可以将其移到该 empty block 中，不能移到后面，因为 loop 中可能用到相关变量

#### Control Flow Tree RULES

```yml
block: basic block node
cf node: control flow node
cf list: control flow list, a list of cf nodes
non-block: loop node, if node
```

```yml
nir_cf_node:
    - 所有数据结构的基类，可以派生出 nir_loop, nir_if, nir_block 等
nir_block:
    - 包含 nir_instr list
    - 最多只能包含一条跳转指令，而且只能在末尾
exec_list(cf list):
    - 对应 GLSL statements 的由 cf nodes 组成的链表，可以表示 function body, if/loop statements
    - 必须以 block 开始和结尾
    - 至少需要包含一个 block(可以是空的)
    - non-block node 之间需要有且仅有一个 block
nir_if:
    - 包含 then 和 else 两个分支的 cf list
nir_loop:
    - 是无限循环，需要使用 break 退出
nir_jump_instr:
    - 表示 break, continue, return 等跳转操作
nir_op_info:
    - 存储操作码操作数等信息
```

#### Modifying Control Flow

nir.h 中搜索名称中包含 **insert** 的函数

## NIR Instruction

* instruction 是 NIR 中最小计算单元
* 基类为 **nir_instr**
* 在 basic blocks 中顺序执行

### ALU Instruction

## API

|api |desc
|- |-
|nir_print_shader       |将 shader 内容输出到文件供调试
|nir_print_instr        |调试 instruction
|nir_validate_shader    |检查 shader 是否有问题，调试时避免报错位置和实际导致错误位置不一致
