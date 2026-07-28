
# **netX90 Socket Module**

## **Overview**
The `netX90 Socket Module` is designed for managing UDP sockets on the netX90 processor. It allows users to create and handle UDP server sockets, send and receive data, and integrate seamlessly with the netX90's communication stack.

This module is especially useful for applications requiring fast, low-latency communication over UDP.

---

## **Features**
- **Create UDP Server Sockets**: Easily create UDP sockets bound to specific ports.
- **Receive and Send Data**: Supports handling incoming data and sending responses.
- **Callback-Based Design**: Custom callbacks for handling received data.
- **Cyclic Task Execution**: Periodic tasks to process socket communication.

---

## **Usage**
### **1. Integration with netX90 Application**
Below is an example of how to integrate the socket module into a netX90 application:

#### Main Function Example:
```c
#include "nx90_socket.h"

static void socketDataReceived(nx90_hSocket socket, nx90Socket_data_t * data);

int main(void)
{
  N90Cifx_t * nx90Cifx = N90Cifx_Init("has3_pn_pb");

  nx90Socket_Init(nx90Cifx->n90Channels[1]);

  const uint32_t SocketLength = 100;
  const uint32_t SocketPort = 500;

  nx90_hSocket s = nx90Socket_CreateUDPServerSocket(SocketPort, SocketLength, socketDataReceived);

  if(NULL == s)
  {
    while(1)
    {
       nx90Socket_Tasks(s);
    }
  }

  return 0;
}

static void socketDataReceived(nx90_hSocket socket, nx90Socket_data_t * receivedData)
{
   nx90Socket_SendData(socket, receivedData);
}
```

### **2. Test with Python Script**
You can test the UDP server created by the module using the provided Python script. The script sends a message to the server and expects an echo response.

#### **Test Script**
```python
import socket
import time

# Server IP and port
server_ip = "10.200.7.50"
server_port = 500
message = "Hello, UDP Server!"

# Create a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Set a timeout for receiving a response (optional)
sock.settimeout(2)  # Waits for 2 seconds for a response

try:
    # Record the time before sending the message
    send_time = time.time()
    
    # Send the message to the server
    print(f"Sending message to {server_ip}:{server_port}")
    sock.sendto(message.encode(), (server_ip, server_port))

    # Wait for a response from the server
    response, server_address = sock.recvfrom(100)  # Buffer size of 1024 bytes

    # Record the time after receiving the response
    receive_time = time.time()

    # Calculate the round-trip time (RTT)
    rtt = (receive_time - send_time) * 1000  # Convert to milliseconds
    print(f"Received echo from server: {response.decode()}")
    print(f"Round-trip time (latency): {rtt:.2f} ms")

except socket.timeout:
    print("No response received from the server.")

finally:
    # Close the socket
    sock.close()
```

---

## **How to Test**
### **1. Setup**
- Compile and run the netX90 application on the target device.
- Note the IP address of the netX90 device (e.g., `10.200.7.50`).

### **2. Run the Test Script**
- Update the `server_ip` variable in the Python script to match your netX90 device's IP address.
- Execute the Python script on your PC or another device in the same network as the netX90.

### **3. Expected Output**
- The server will echo back the received message.
- The script will display:
  - The echoed message.
  - The calculated round-trip time (latency).

---

## **Example Output**
```plaintext
Sending message to 10.200.7.50:500
Received echo from server: Hello, UDP Server!
Round-trip time (latency): 15.34 ms
```