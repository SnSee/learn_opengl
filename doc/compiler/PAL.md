
# PAL

[github](https://github.com/GPUOpen-Drivers/pal)

## 编译

```sh
cd src/util/imported 
git clone https://github.com/GPUOpen-Drivers/MetroHash.git pal_metrohash
git clone https://github.com/GPUOpen-Drivers/CWPack.git pal_cwpack
cd -
pip install jinja2
pip install ruamel.yaml

cmake -DCMAKE_BUILD_TYPE:STRING=Debug -DENABLE_TESTS=ON -DENABLE_TESTCASES=ON -G Ninja \
    -DPAL_CLIENT_INTERFACE_MAJOR_VERSION=888 \
    -DPAL_CLIENT=1
ninja -C build
find build -name "*.a"
```
