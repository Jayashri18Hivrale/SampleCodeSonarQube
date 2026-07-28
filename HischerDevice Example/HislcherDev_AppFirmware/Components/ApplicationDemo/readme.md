# netX 90 Communication Demo Application

This application demonstrates the **cyclic** and **acyclic data exchange** between a **netX 90-based communication device** and a **PLC** or other host system, utilizing the hardware on the **netX 90 JTAG development board**. The code manages both **real-time data exchange** through cyclic communication and **diagnostic/monitoring data** through acyclic communication.

The figure below illustrates the data model and the overall components involved in the system:

![Data Model](doc/LFW%20example%20data%20model.png)

In this model, we can observe two **sensors** and two **discrete actuators**—each actuator having one discrete input and one discrete output—along with an internal **counter**. These components are controlled using the **cyclic input**, **cyclic output**, and **acyclic data models** as described in the following sections. The diagram also depicts the various elements of the application and their interactions.


## Cyclic Data Model Overview

The **cyclic data model** describes the continuous exchange of real-time data between the **netX 90** and the **PLC**. This data is exchanged at regular intervals and is used for controlling actuators (like LEDs) and retrieving real-time sensor data (like temperature sensors or switches). The structure of the cyclic data is crucial for real-time control and monitoring in industrial applications.

### Cyclic Input Data (From PLC to netX 90)

The **cyclic input data** consists of control signals sent by the PLC to control the hardware on the development board, including **LEDs** and the **cyclic counter**. The input data is structured in **6 bytes**, as follows:

| **Byte Position** | **Field**                | **Size** | **Description**                                  |
|-------------------|--------------------------|----------|--------------------------------------------------|
| 0-1               | **Actuator 1 Output**      | 2 bytes  | Controls **LED 1** (MMIO4), turns it ON/OFF       |
| 2-3               | **Actuator 2 Output**      | 2 bytes  | Controls **LED 2** (MMIO5), turns it ON/OFF       |
| 4                 | **Cyclic Counter Direction** | 1 byte   | Controls the direction of the cyclic counter (up/down) |
| 5                 | **Cyclic Counter Speed**  | 1 byte   | Controls the speed at which the cyclic counter updates |

- **Byte Positions 0-1**: The **Actuator 1 Output** controls **LED 1** (connected to **MMIO4**). This 2-byte field indicates whether the LED should be turned ON or OFF, based on the value received from the PLC.
- **Byte Positions 2-3**: The **Actuator 2 Output** controls **LED 2** (connected to **MMIO5**), similarly determining its ON/OFF state.
- **Byte Position 4**: This byte controls the direction of the **cyclic counter** (increment or decrement).
- **Byte Position 5**: This byte controls the **speed** of the cyclic counter, determining how quickly it increments or decrements.

### Cyclic Output Data (From netX 90 to PLC)

The **cyclic output data** consists of real-time feedback from the **netX 90** sent to the PLC. This feedback includes the **cyclic counter value**, **temperature sensor readings**, and **switch states**. The cyclic output data is structured in **10 bytes**, as follows:

| **Byte Position** | **Field**                     | **Size** | **Description**                                         |
|-------------------|-------------------------------|----------|---------------------------------------------------------|
| 0-1               | **Cyclic Counter Value**       | 2 bytes  | The current value of the cyclic counter                  |
| 2-3               | **Sensor 1 Value (Temp in Celsius)**     | 2 bytes  | The board temperature in degrees Celsius                 |
| 4                 | **Sensor 1 State**     | 1 byte   | OK if temp < threshold, WARNING otherwise |
| 5-6               | **Sensor 2 (Temp in Volts)**       | 2 bytes  | The board temperature in volts, measured via ADC         |
| 7                 | **Sensor 2 State**            | 1 byte   |  OK if voltage < threshold, WARNING otherwise |
| 8                 | **Actuator 1 Input**           | 1 byte   | Status of **Switch 3** (reflects the current state)      |
| 9                 | **Actuator 2 Input**           | 1 byte   | Status of **Switch 4** (reflects the current state)      |

- **Byte Positions 0-1**: The **cyclic counter value** is incremented every **Cyclic Counter Speed** miliseconds, in the defined **Cyclic Counter Speed**.
- **Byte Positions 2-3**: The **Temperature in Celsius** field sends the real-time board temperature, measured by an onboard sensor, to the PLC.
- **Byte Position 4**: The **Temp Celcius State** reflects the state of the temperature sensor. The sensor's state is evaluated against predefined temperature thresholds. The switch state is **OK** (0x0) if the temperature is below a defined limit, and **WARNING** (0x1)  if it exceeds the limit.
- **Byte Positions 5-6**: The **Temperature in Volts** field transmits the board temperature in volts, measured via ADC.
- **Byte Position 7**: The **Temp Volts State** reflects the state of the voltage sensor. The voltage value is compared to predefined voltage thresholds. The switch state is **OK** if the voltage is below a defined limit, and **WARNING** if it exceeds the limit.
- **Byte Position 8**: The **Actuator 1 State** reflects the current status of **Switch 3** (MMIO2), ON (0x1) OFF (0x0).
- **Byte Position 9**: The **Actuator 2 State** reflects the current status of **Switch 4** (MMIO3), ON (0x1) OFF (0x0).

---

## Acyclic Data Model Overview

The **acyclic data model** provides access to **diagnostic and configuration data** on demand. It includes information such as the **status codes** and **modes** for sensors and actuators. This data is not exchanged continuously but can be requested when needed to ensure the system is operating as expected.

### Acyclic Data Structure (12 Bytes)

| **Byte Position** | **Field**                     | **Size** | **Description**                                         |
|-------------------|-------------------------------|----------|---------------------------------------------------------|
| 0                 | **Sensor 1 Mode**             | 1 byte   | Mode of **Sensor 1** (updated when accessed via bus or terminal) |
| 1-2               | **Sensor 1 Status Code**       | 2 bytes  | Status of **Sensor 1** (diagnostics, error/warning codes) |
| 3                 | **Sensor 2 Mode**             | 1 byte   | Mode of **Sensor 2**                                    |
| 4-5               | **Sensor 2 Status Code**       | 2 bytes  | Status of **Sensor 2**                                  |
| 6                 | **Actuator 1 Mode**           | 1 byte   | Mode of **Actuator 1** (updated when accessed via bus or terminal) |
| 7-8               | **Actuator 1 Status Code**     | 2 bytes  | Status of **Actuator 1**                                |
| 9                 | **Actuator 2 Mode**           | 1 byte   | Mode of **Actuator 2**                                  |
| 10-11             | **Actuator 2 Status Code**     | 2 bytes  | Status of **Actuator 2**                                |

- **Byte Position 0**: The **Sensor 1 Mode** reflects how **Sensor 1** is configured or operating. This value is updated whenever the sensor is accessed via bus or terminal.
- **Byte Positions 1-2**: The **Sensor 1 Status Code** provides detailed feedback on **Sensor 1's** health, including diagnostics or error codes.
- **Byte Position 3**: The **Sensor 2 Mode** reflects how **Sensor 2** is operating or configured, updated when accessed.
- **Byte Positions 4-5**: The **Sensor 2 Status Code** provides status feedback for **Sensor 2**.
- **Byte Position 6**: The **Actuator 1 Mode** reflects the mode of **Actuator 1** (e.g., manual or automatic).
- **Byte Positions 7-8**: The **Actuator 1 Status Code** provides diagnostic or performance feedback for **Actuator 1**.
- **Byte Position 9**: The **Actuator 2 Mode** reflects the mode of **Actuator 2**.
- **Byte Positions 10-11**: The **Actuator 2 Status Code** provides feedback for **Actuator 2**.

---

## Code Implementation Description

The provided code implements the **cyclic** and **acyclic data exchange** by utilizing **communication channels**, **timers**, and **packet handling** mechanisms. Below is a detailed explanation of how the code works to meet the cyclic and acyclic communication requirements based on the netX 90's capabilities.

### Communication Channels

The application sets up multiple **communication channels** to facilitate data exchange between the **PLC** and the **netX 90** device. Each channel is dedicated to a different type of communication or protocol.

- **Channel 0 (Real-Time Communication)**: Channel 0 is the primary communication channel used for **cyclic data exchange**. It is configured to handle real-time industrial protocols (e.g., EtherNet/IP or PROFINET). In the code, **Channel 0** is responsible for:
  - **Reading cyclic input data** from the PLC, such as actuator control signals and cyclic counter parameters.
  - **Writing cyclic output data** back to the PLC, including sensor readings (temperature, switch states) and status information.
  
  The **`App_AllChannels_Open()`** function opens and configures the channel, ensuring it is ready for communication. The **`xChannelHostState()`** function is used to set the host state to **CIFX_HOST_STATE_READY**, signaling that communication is ready to proceed.

- **Channel 1 and Channel 2 (Optional)**: These channels are reserved for **network services** or other **protocols** (e.g., NetProxy) but are not used by default. If enabled, these channels can handle non-real-time data exchange such as socket interfaces or web services.

### Cyclic Data Handling

#### Cyclic Input Data (from PLC to netX 90)

The **cyclic input data** is processed by the **`App_IODataHandler()`** function, which is registered as a timer callback function. This ensures that cyclic data is handled at regular intervals, meeting the real-time communication needs.

- **Input Data Read**: The function **`xChannelIORead()`** is used to read input data from **Channel 0**. This data includes:
  - **LED Control**: The control signals for **LED 1** (MMIO4) and **LED 2** (MMIO5) are read from the PLC and determine whether the LEDs should be turned ON or OFF.
  - **Cyclic Counter Control**: The PLC sends control values for the cyclic counter, determining whether it counts up or down and how fast the counter updates.
  
- **LED Updates**: After reading the input data, the code updates the state of **LED 1** and **LED 2** by setting the appropriate MMIO pins. This is done using functions like **`HOSTAL_Actuator_SetData()`**, which sets the state of the LEDs based on the input received from the PLC.

#### Cyclic Output Data (from netX 90 to PLC)

The cyclic output data is also managed by the **`App_IODataHandler()`** function. Once the input data is processed, the output data is updated and written back to the PLC.

- **Output Data Write**: The function **`xChannelIOWrite()`** is used to send output data back to the PLC. This output data includes:
  - **Cyclic Counter Value**: The current value of the cyclic counter is sent back to the PLC.
  - **Sensor Readings**: The code reads data from the onboard sensors:
    - **Temperature in Celsius** and **Temperature in Volts** are obtained via the ADC (Analog-to-Digital Converter) and sent to the PLC.
  - **Switch States**: The states of **Switch 3 (MMIO2)** and **Switch 4 (MMIO3)** are monitored and reported back to the PLC, indicating their current status (ON/OFF).
  - **Status Information**: The code evaluates the health of sensors and switches, sending **OK/WARNING** status codes back to the PLC, ensuring that the PLC is aware of any operational issues.

### Acyclic Data Handling

In addition to cyclic data, the code supports **acyclic data** that provides detailed diagnostic information on-demand. Acyclic data includes status codes and operational modes for the sensors and actuators.

- **Acyclic Data Access**: The **`App_IODataHandler()`** function also updates acyclic data, which can be accessed by other parts of the system when requested. This data includes:
  - **Sensor Status Codes**: These codes provide feedback on the operational state of temperature sensors, such as whether they are within normal operating conditions or have exceeded thresholds (e.g., too hot or too cold).
  - **Actuator Status Codes**: The status of actuators (like LEDs and switches) is also updated, indicating whether the actuators are functioning properly or if there is a warning or error.

### Timer-Driven Cyclic Data Exchange

The cyclic data exchange is driven by a **timer**, ensuring that data is processed at regular intervals. This is crucial for maintaining **real-time communication** between the netX 90 and the PLC.

- **Timer Registration**: The **`HOSTAL_Callback_Register()`** function registers the **`App_IODataHandler()`** as a callback function that is executed at regular intervals.
- **Callback Execution**: Once registered, the function is periodically invoked by the timer to:
  - **Read cyclic input data** from the PLC.
  - **Process the input data**, updating the LED states and cyclic counter.
  - **Write cyclic output data** back to the PLC, including sensor readings, switch states, and status codes.

### Error Handling and Shutdown

The code includes robust error handling and ensures proper shutdown in case of issues:

- **Error Checking**: If an error occurs during data exchange (e.g., failure to read/write data or a communication failure), the application logs the error and attempts to recover. Functions such as **`App_AllChannels_Open()`** and **`App_AllChannels_Close()`** manage the opening and closing of communication channels, ensuring they are handled safely.
  
- **Graceful Shutdown**: When the application is terminated, the code ensures that communication is stopped properly by setting the **bus state** and **host state** to not ready. This is done via the **`xChannelHostState()`** function, which ensures that no further data exchange occurs. All communication channels are closed, and any dynamically allocated memory is freed, preventing memory leaks or corruption.

### Overall Flow

1. **Initialization**:
   - The **cifX driver** is opened.
   - Communication channels (e.g., Channel 0) are opened and configured.
   - The **host state** is set to ready for communication.
   
2. **Data Exchange Loop**:
   - **Cyclic input data** (from the PLC) is continuously read, processed, and used to update actuators like **LEDs** and the **cyclic counter**.
   - **Cyclic output data** (to the PLC) is generated, including sensor readings, switch states, and operational status.
   - **Acyclic data** is updated periodically to provide diagnostic information.

3. **Error Handling**:
   - The application monitors for errors during data exchange and handles them gracefully by stopping communication if necessary.

4. **Shutdown**:
   - Communication channels are closed, memory is freed, and the application shuts down properly, ensuring no resources are left in an inconsistent state.

