# iot-watertank-level-zwave

on MacOS part of the ZUNO toolchain is not 64bit compatible so you have to manually update SDCC. 
Download it from [sourceforge](https://sourceforge.net/projects/sdcc/).

locate your Arduino Library and in particular the zuno toolchain. It should be there: 

```
/Users/your_username/Library/Arduino15/packages/Z-Uno/tools/zuno_toolchain/00.08.80/zuno_toolchain/sdcc
```

replace binary files in this directory with the ones from sourceforge. 
