# ubus-demo

## howto install ubus in traditional linux


```
git clone git://git.openwrt.org/project/libubox.git
cd libubox
# optional turn lua support off
mkdir build && cd build
cmake ../
make
make install

git clone git://nbd.name/luci2/ubus.git
cd ubus
# optional turn lua support off
mkdir build && cd build
cmake ../
make
make install
```
### test ubus

```
systemctl start ubus.service
systemctl status ubus.service
```
### grant non root processes access

```
chmod o+r /var/run/ubus.sock
chmod o+w /var/run/ubus.sock
```

## use ubus

```
$ ubus listen &
$ ubus send foo '{"a":"b"}'
```
