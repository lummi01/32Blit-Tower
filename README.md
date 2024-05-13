# 32Blit Tower
Use A-Button to build your tower to the top of the screen.</BR>
Get points with new stage, positioning the blocks exactly and reaching the top.

For local builds:
```
mkdir build
cd build
cmake -D32BLIT_DIR=/path/to/32blit-sdk/ ..
```

For 32Blit builds:
```
mkdir build.32stm
cd build.32stm
cmake .. -DCMAKE_TOOLCHAIN_FILE=/path/to/32blit-sdk/32blit.toolchain

```
