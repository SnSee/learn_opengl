
# OpenCL

[OpenCL Guide](https://github.com/KhronosGroup/OpenCL-Guide)

OpenCL(Open Computing Language) 是一个免费的跨平台并行编程库。它提供的并行编程机制旨在使得程序能够更快、更平稳运行。

## OpenCL-CTS

### Build

[OpenCL-CTS](https://github.com/KhronosGroup/OpenCL-CTS)

```sh
# 按照 README 提供的方式编译即可
git clone https://github.com/KhronosGroup/OpenCL-CTS.git
git clone https://github.com/KhronosGroup/OpenCL-Headers.git
git clone https://github.com/KhronosGroup/OpenCL-ICD-Loader.git

mkdir OpenCL-ICD-Loader/build
cmake -S OpenCL-ICD-Loader -B OpenCL-ICD-Loader/build \
      -DOPENCL_ICD_LOADER_HEADERS_DIR=$PWD/OpenCL-Headers
cmake --build ./OpenCL-ICD-Loader/build --config Release

mkdir OpenCL-CTS/build
cmake -S OpenCL-CTS -B OpenCL-CTS/build \
      -DCL_INCLUDE_DIR=$PWD/OpenCL-Headers \
      -DCL_LIB_DIR=$PWD/OpenCL-ICD-Loader/build \
      -DOPENCL_LIBRARIES=OpenCL
cmake --build OpenCL-CTS/build --config Release
```

### Run Test

```sh
clinfo                                  # 检查 platform 和 device
sudo apt install rocm-dev               # 如果没有进行安装(amd 显卡)
sudo apt install rocm-opencl-dev
sudo apt install mesa-opencl-icd        # 如果 rocm 不行可以试试 mesa platform
ls /etc/OpenCL/vendors/                 # 检查是否有 ICD 文件

# 执行测试用例
./OpenCL-CTS/build/test_conformance/basic/test_basic
```
