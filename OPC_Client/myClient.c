//#include <open62541/client_config.h>
//#include <open62541/client_highlevel.h>
//#include <open62541/plugin/log.h>

#include <stdlib.h>


#include "open62541.h"

//extern const UA_Logger *UA_Log_Stdout ;

int main(void) {
    UA_Client *client = UA_Client_new();
    UA_ClientConfig_setDefault(UA_Client_getConfig(client));
    UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://raspberrypi:4840/");//"opc.tcp://DELL-Milind:4840/", Read ReadMe.txt to know how to find out the name of your computer.
    if(retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int)retval;
    }

    /* Read the value attribute of the node. UA_Client_readValueAttribute is a
     * wrapper for the raw read service available as UA_Client_Service_read. */
    UA_Variant value; /* Variants can hold scalar values and arrays of any type */
    UA_Variant_init(&value);

    /* NodeId of the variable holding the current time */
    const UA_NodeId nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_CURRENTTIME);
    retval = UA_Client_readValueAttribute(client, nodeId, &value);

    if(retval == UA_STATUSCODE_GOOD &&
       UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_DATETIME])) {
        UA_DateTime raw_date = *(UA_DateTime *) value.data;
        UA_DateTimeStruct dts = UA_DateTime_toStruct(raw_date);
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "date is: %u-%u-%u %u:%u:%u.%03u\n",
                    dts.day, dts.month, dts.year, dts.hour, dts.min, dts.sec, dts.milliSec);
    }
    
    //Variables for read access
    UA_String VendorName;
    UA_Int32 SerialNumber=0;
    
    //Read Vendor Name
    retval = UA_Client_readValueAttribute(client, UA_NODEID_STRING(2, "R1_TS1_VendorName" ), &value);
    if(retval == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_STRING])) {

		VendorName = *(UA_String *) value.data;
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Vendor Name is %.*s \n",VendorName.length, VendorName.data );
	}
	
    //Read Serial Number
    retval = UA_Client_readValueAttribute(client, UA_NODEID_STRING(2, "R1_TS1_SerialNumber" ), &value);
    if(retval == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_INT32])) {
		
		SerialNumber = *(UA_Int32 *) value.data;
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Serial Number is %d \n", SerialNumber );
	}
	
	
	
    
    
    

    /* Clean up */
    UA_Variant_clear(&value);
    UA_Client_delete(client); /* Disconnects the client internally */
    return EXIT_SUCCESS;
}
