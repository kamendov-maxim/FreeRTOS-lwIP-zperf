LWIP FreeRTOS TCP+UDP Echo example

Example description
Welcome to the LWIP TCP+UDP Echo example with FreeRTOS using LWIP. This
example was ported from the LWIP FreeRTOS TCP Echo example found in the
LPCOpen Libraries and Examples kit.  The example demonstrates how to run a
program using lwIP in the FreeRTOS POSIX simulator.

To use the example:

1 - Start the tcpecho executable with the proper permissions.

    $ ./tcpecho

2 - At this point, the example will respond to TCP and UDP Echo (RFC 862)
    requests.

        - Use a telnet client (such as nc, telnet or putty).
        - Configure it to use telnet, port 7 and the IP address specified on
          the command line.
        - Click Open and the port will be opened, now any key pressed on the
          telnet window will be received by the board and be echoed back.
        - NOTE: Some terminal programs (like telnet in Linux) do not send a
          packet until the user presses Enter.
        - Note also by default the putty program will perform a local echo
          before sending the character to the board, resulting in characters
          being displayed twice to the terminal window.


    E.g. from a Windows Command Prompt:

        C:\>telnet <ipaddr> 7


    E.g. UDP from a GNU/Linux console:

        $ nc -u <ip> 7


Special connection requirements
There are no special connection requirements for this example.

