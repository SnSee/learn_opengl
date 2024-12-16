
# vulkan

## 环境变量

1. vulkan 程序链接的是 libvulkan.so
2. 对于不同的 GPU，会有不同的驱动，如 AMD 的 libvulkan_radeon.so
3. vulkan 使用 GPU 时会自动加载对应的驱动
4. 可通过下面的方式设置驱动路径

```sh
# 指定 ICD 路径，即 mesa 编译出的 libvulkan_radeon.so 路径
export VK_ICD_FILENAMES=build_dir/src/amd/vulkan/radeon_icd.x86_64.json
```
