# 32Blit Tower

For 32Blit builds this is:
```
mkdir build.32stm
cd build.32stm
cmake .. -DCMAKE_TOOLCHAIN_FILE=/path/to/32blit-sdk/32blit.toolchain
```


For local builds this is:
```
mkdir build
cd build
cmake -D32BLIT_DIR=/path/to/32blit-sdk/ ..
```
