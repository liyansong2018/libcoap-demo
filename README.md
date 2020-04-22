# libcoap-demo

## 安装 libcoap
### 安装
```
git clone https://github.com/obgm/libcoap.git
cd libcoap
./autogen.sh
./configure --disable-doxygen --disable-manpages --disable-dtls
make
make install
```
### 使用
coap-server 创建 coap 服务端

```bash
coap-server
```
客户端获取数据
```bash
root@kali:~# coap-client -m get coap://localhost
This is a test server made with libcoap (see https://libcoap.net)
Copyright (C) 2010--2020 Olaf Bergmann <bergmann@tzi.org> and others
```
注意：libcoap 默认安装在 /usr/local/lib 中，该库目录不在系统默认的库目录中，需要手动指定
```
export LD_LIBRARY_PATH=/usr/local/lib
```

## 使用
源码分析详情，请见 [libcoap 接口分析与 CoAP 协议开发](https://blog.csdn.net/song_lee/article/details/105653196)

