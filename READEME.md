# 性能MAX
### 开启命令行模式，关闭桌面模式
```shell
# 命令行
$ sudo systemctl set-default multi-user.target
# 桌面
$ sudo systemctl set-default graphical.target 
```
# 安装fmt
```shell
$ git clone https://ghproxy.com/https://github.com/fmtlib/fmt.git
$ cd fmt && mkdir build && cd build
$ cmake .. && make -j6
$ sudo make install
```

# 安装Jetson Cpp
```shell
$ git clone https://ghproxy.com/https://github.com/pjueon/JetsonGPIO.git
$ cd JetsonGPIO && mkdir build && cd build
$ cmake .. && make -j6
$ sudo make install
```

# 连接设备
<img src="./images/rpi-pins-40-0.png"/>

# 测试输入输出
```shell
$ ./gpio
```
# 启动
```shell
$ mkdir build && cd build
$ cmake .. && make -j6
$./jetson_cam_gpio --lcam=/dev/video0 --rcam=/dev/video2 --lp=16 --rp=18 --save_path=/home/nvidia/videos
```