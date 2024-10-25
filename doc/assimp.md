
# ASSIMP

Open Asset Import Library: 是一个加载模型的库

[github](https://github.com/assimp/assimp)

## 概念

### Scene, Model, Mesh

```yml
Face: 一个面片表示渲染中一个最基本的形状单位，即图元(点、线、三角面片、矩形面片)
Mesh:
    - 一般来说，一个模型(Model)由几个子模型/形状组合而成
    - 子模型/形状即是网格
    - 一个网格(包含顶点，索引，材质)是 OpenGL 绘制物体的最小单位
    - 一个模型通常包含多个网格
```

* 所有模型、场景数据都包含在 Scene 对象中(material, mesh, root-node)
* root-node 可能包含很多 child-nodes
* node 中只保存 mesh 的索引，需要到 scene 中获取 mesh 对象
* node 中只保存 材质 的索引，需要到 scene 中获取 材质 对象
* mesh 对象包含渲染所需相关数据(顶点位置，法向量，纹理坐标，面片，物体材质)
* 一个 mesh 对象会包含多个面片
* 一个面片记录了一个图元的顶点索引，通过该索引在 mesh 列表中查找顶点数据

## API

### stbi_set_flip_vertically_on_load

```cpp
// 翻转 y 坐标，使其与 OpenGL 坐标原点一致
stbi_set_flip_vertically_on_load(true);
Importer::ReadFile(path, aiProcess_FlipUVs);
```

### Importer::ReadFile

加载模型文件

```cpp
const aiScene *ReadFile(const string &pFile, unsigned int pFlags);

pFile: 模型文件路径
pFlags: 使用位运算指定多个选项
    aiProcess_Triangulate: 如果模型包含非三角形，转换为三角形
    aiProcess_FlipUVs: 基于 y 轴翻转坐标(使 y 轴原点和 OpenGL 一致)
    aiProcess_GenNormals: 如果模型没有法线向量，为每个顶点创建法线
    aiProcess_SplitLargeMeshes: 把大的网格成几个小的的下级网格，当渲染有最大数量顶点的限制时或者只能处理小块网格时很有用
    aiProcess_OptimizeMeshes: 和上个选项相反，它把几个网格结合为一个更大的网格, 以减少绘制函数调用的次数的方式来优化
```

## demo

### 加载模型

```cpp
void load_model(std::string path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "Assimp load_model error: " << importer.GetErrorString() << ", file: " << path << std::endl;
    }
}
```

### 遍历节点

```cpp
void process_node(aiNode *node, const aiScene *scene) {
    for (GLuint i = 0; i < node->mNumMeshes; ++i) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    }
    for (GLuint i = 0; i < node->mNumChildren; ++i) {
        process_node(node->mChildren[i], scene);
    }
}
```
