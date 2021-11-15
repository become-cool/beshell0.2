
> 文档： https://github.com/espressif/qemu/wiki


## 编译 qemu

```
git clone https://github.com.cnpmjs.org/espressif/qemu.git
cd qemu
./configure --target-list=xtensa-softmmu \
    --enable-gcrypt \
    --disable-debug --enable-sanitizers \
    --disable-strip --disable-user \
    --disable-capstone --disable-vnc \
    --disable-sdl --disable-gtk \
make -j7
```

安装 libgcrypt-dev

```
sudo apt install libgcrypt-dev
```

`make` 遇到 `sizeof undefined` 错误，修改报错文件， `sizeof(xxx)`  改为  `__SIZEOF_XXX__`


## 关闭 qemu

```
ctrl-a x
```

## 支持 PSRAM

启动 qemu 的命令时加入 `-m 4194304`  (4MB)
