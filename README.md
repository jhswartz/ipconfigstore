# ipconfigstore
Android ipconfig.txt manipulator

### Distribution
```
$ autoreconf -i
$ ./configure
$ make distcheck
$ make maintainer-clean
```

### Installation
```
$ tar xvf ipconfigstore-0.1.tar.gz
$ cd ipconfigstore-0.1
$ ./configure
$ make
# make install
```

### Usage
```
usage: ipconfigstore OPTION

 -p VERSION    Pack IP configuration
 -u            Unpack IP configuration
```

### Examples

##### Packing
```
$ ipconfigstore -p 2 < samples/v2/static.conf > /data/misc/ethernet/ipconfig.txt
```

##### Unpacking
```
$ ipconfigstore -u < /data/misc/ethernet/ipconfig.txt > ipconfig.conf
```

### Notes
Review the [com.android.server.net.IpConfigStore] source code for further information.

### Warning
This program has not been tested extensively, use it at your own risk.

[com.android.server.net.IpConfigStore]: https://android.googlesource.com/platform/frameworks/base/+/refs/heads/master/services/core/java/com/android/server/net/IpConfigStore.java
