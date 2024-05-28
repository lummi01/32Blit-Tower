![](/assets/image.png)
# 32Blit Tower
a simple reaction game for 32Blit.

Use the A-Button to build your tower to the top of the screen.</BR>
Get points with new steps, positioning the blocks exactly and reaching the top as big as possible.<BR>
![](/assets/Tower0.bmp)
<BR>
For local builds:
```
mkdir build
cd build
cmake -D32BLIT_DIR=/path/to/32blit-sdk/ ..
```

For 32Blit builds:
```
mkdir build.stm32
cd build.stm32
cmake .. -DCMAKE_TOOLCHAIN_FILE=/path/to/32blit-sdk/32blit.toolchain

```
