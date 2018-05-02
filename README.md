Simple project that shows how to build the tcpecho example from the [LPCOpen Libraries and Examples](https://www.nxp.com/support/developer-resources/software-development-tools/lpc-developer-resources-/lpcopen-libraries-and-examples:LPC-OPEN-LIBRARIES) kit against the [FreeRTOS simulator](https://github.com/megakilo/FreeRTOS-Sim) in combination with [lwIP](https://savannah.nongnu.org/projects/lwip/).

To build and install the simulator:

```
cd FreeRTOS-lwIP-Sim
mkdir build
cd build
cmake ..
cmake --build . --target install
```

The example will be available in /opt/freertos-lwip-sim/share/examples/lwip_freertos_tcpecho.

To build the example:

```
cp -r /opt/freertos-lwip-sim/share/examples/lwip_freertos_tcpecho ~/
cd ~/lwip_freertos_tcpecho
mkdir build
cd build
cmake ..
cmake --build .
```

Simply run the tcpecho server by executing the resulting binary.

