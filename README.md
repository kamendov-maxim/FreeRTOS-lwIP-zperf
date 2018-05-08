Simple project that shows how to build the tcpecho example from the [LPCOpen Libraries and Examples](https://www.nxp.com/support/developer-resources/software-development-tools/lpc-developer-resources-/lpcopen-libraries-and-examples:LPC-OPEN-LIBRARIES) kit against the [FreeRTOS simulator](https://github.com/megakilo/FreeRTOS-Sim) in combination with [lwIP](https://savannah.nongnu.org/projects/lwip/).

To build the simulator:

```
git clone https://github.com/k0ekk0ek/FreeRTOS-lwIP-Sim.git
cd FreeRTOS-lwIP-Sim
git submodule update --init --recursive
mkdir build
cd build
cmake ..
cmake --build .
```

To install the simulator:

```
cd FreeRTOS-lwIP-Sim/build
cmake --build . --target install
```

The above step will install the simulator into /opt/freertos-lwip-sim. The location was chosen so that headers and libraries that ship with the simulator will not clash with possibly existing installations. Using /opt/freertos-lwip-sim also ensures that freertos-lwip-sim-config.cmake (ProjectConfig.cmake) will be picked up by ``find_package(FreeRTOS-lwIP-Sim)``.

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

