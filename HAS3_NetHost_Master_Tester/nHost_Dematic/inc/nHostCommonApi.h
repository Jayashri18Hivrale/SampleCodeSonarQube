#define RCX_DEFAULT_DESTINATION                0x20
// RCX Commands for querying slave information
#define RCX_GET_SLAVE_HANDLE_REQ               0x2F08
#define RCX_GET_SLAVE_HANDLE_CNF               0x2F09
#define RCX_GET_SLAVE_CONN_INFO_REQ            0x2F0A
#define RCX_GET_SLAVE_CONN_INFO_CNF            0x2F0B

// Parameters for querying different slave states
#define RCX_LIST_CONF_SLAVES                   0x00000001  // Configured slaves
#define RCX_LIST_ACTV_SLAVES                   0x00000002  // Active slaves
#define RCX_LIST_FAULTED_SLAVES                0x00000003  // Faulted slaves