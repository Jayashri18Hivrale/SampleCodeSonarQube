
# **netX90 PROFINET Stack (PNS) Module**

## **Overview**
The `netX90 PROFINET Stack Module` provides an interface for initializing, configuring, and managing PROFINET communication on the netX90 platform. It enables developers to handle cyclic and acyclic communication, register event handlers for PROFINET indications, and manage stack tasks efficiently.

---

## **Features**
- **Initialization and Configuration**: Easily set up the PROFINET stack with custom configuration.
- **Cyclic Data Handling**: Register callbacks for handling cyclic input and output data.
- **Indication Callbacks**: Flexible registration of handlers for specific PROFINET indications.
- **Communication Tasks**: Periodic task execution for stack communication and data exchange.

---

## **Usage**
### **1. Integration with netX90 Application**
Below is an example of how to integrate the PNS module into a netX90 application:

#### Main Function Example:
```c
int main(void)
{
  N90Cifx_t * nx90Cifx = N90Cifx_Init("has3_pn_pb");

  nx90PNS_config_t pns_conf = {inputCyclicHandler, outputCyclicHandler};

  nx90PNS_Init(nx90Cifx, &pns_conf);

  int32_t lRet = nx90PNS_ConfigureStack(configureHas3SlavePN());
  
  if(CIFX_NO_ERROR == lRet)
  {
    while(1)
    {
       nx90PNS_CommStackTasks();
       nx90PNS_CyclicTasks();
    }
  }

  return 0;
}

static void inputCyclicHandler(uint32_t submodule, uint8_t * data, uint32_t dataLength, void * userData)
{
    // Handle cyclic input data
}

static void outputCyclicHandler(uint32_t submodule, uint8_t * data, uint32_t dataLength, void * userData)
{
    // Handle cyclic output data
}
```

---

### **2. Key API Functions**

| **Function**                      | **Description**                                                                 |
|-----------------------------------|---------------------------------------------------------------------------------|
| `nx90PNS_Init`                    | Initializes the PROFINET stack with CIFX handles and cyclic data callbacks.     |
| `nx90PNS_RegisterIndicationCallback` | Registers a handler for specific PROFINET indications.                        |
| `nx90PNS_CommStackTasks`          | Executes periodic communication tasks for the PROFINET stack.                  |
| `nx90PNS_CyclicTasks`             | Processes cyclic communication tasks for input/output data exchanges.          |
| `nx90PNS_ConfigureStack`          | Applies configuration settings, such as station name and device parameters.    |
| `nex90PNS_GetStationName`         | Retrieves the station name of the PROFINET device.                             |

---

## **Testing the Module**
### **1. Setup**
- Compile and run the netX90 application on the target device with the PNS module integrated.
- Use a PROFINET tool or master device to connect to the netX90 device.

### **2. Expected Behavior**
- The device will register on the PROFINET network with the configured parameters.
- Cyclic input and output data will be exchanged as per the application logic.
- Indications such as saving the station name can be tested by registering appropriate callbacks.

---

## **Example Output**
When integrated with a PROFINET master, the following events are expected:
1. Successful configuration of the stack.
2. Proper cyclic data exchange with connected devices.
3. Handling of custom indications through registered callbacks.

---

## **Troubleshooting**
- **Configuration Errors**:
  - Ensure the `PNS_CONFIGURATION_DATA_T` structure is correctly populated.
  - Check for errors returned by `nx90PNS_ConfigureStack`.
- **No Data Exchange**:
  - Verify that the cyclic callbacks are properly registered.
  - Check network connectivity and ensure the master is configured correctly.
- **Indication Failures**:
  - Ensure the correct `indicationId` is used when registering callbacks.
  - Verify that the returned `CIFX_PACKET` is properly set up.

---