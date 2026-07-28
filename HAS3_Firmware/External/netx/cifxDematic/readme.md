
# **CifxDematic Documentation**

## **Overview**
This project contains modules and documentation for working with the netX90 platform. It includes functionalities for managing PROFINET stacks (`nx90_pns`) and UDP sockets (`nx90_socket`). These modules provide APIs and usage examples to simplify application development on the netX90 processor.

---

## **Modules**

### **PROFINET Stack (PNS) Module**
The `nx90_pns` module provides APIs for initializing, configuring, and managing the PROFINET stack on the netX90 platform. Key features include:
- **Initialization**: Setup and bind the stack to the CIFX handles.
- **Cyclic Data Management**: Register callbacks for cyclic input and output data handling.
- **Indication Handling**: Flexible registration of callbacks for PROFINET-specific indications.
- **Communication Tasks**: Support for periodic communication and cyclic tasks.

Documentation for this module is available in [doc/nx90_pns.md](doc/nx90_pns.md).

### **UDP Socket Module**
The `nx90_socket` module provides APIs for creating and managing UDP sockets on the netX90 processor. Key features include:
- **Socket Creation**: Create generic UDP sockets or server sockets bound to specific ports.
- **Data Transmission**: Send data to specified destinations.
- **Callback Support**: Handle incoming data with user-defined callback functions.
- **Task Execution**: Periodic task processing for socket communication.

Documentation for this module is available in [doc/nx90_Socket.md](doc/nx90_Socket.md).

---
