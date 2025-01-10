App to run zephyr zperf tests with lwIP + FreeRTOS
To build the simulator:

```
git clone https://github.com/kamendov-maxim/FreeRTOS-lwIP-zperf.git
cd FreeRTOS-lwIP-zperf
git submodule update --init --recursive
mkdir build
cd build
cmake ..
cmake --build .
```

Before running test app you should set up tap interface
```
sudo ip tuntap add mode tap tap0
sudo ip -6 address add 2001:db8::2/64 dev tap0
```

Run tests by executing the build/zperf/zperf binary with arguments specifying particular test.

Output of ```zperf --help```:
```
Usage:
udp_upload <address> <port> <duration> <packet size> <baud rate> - udp upload
tcp_upload <address> <port> <duration> <packet size> <baud rate> - tcp upload
udp_download <port> <address> 
tcp_download <port> <address>
```
