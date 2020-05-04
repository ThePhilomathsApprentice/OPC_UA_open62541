//#include <open62541/plugin/log_stdout.h>
//#include <open62541/server.h>
//#include <open62541/server_config_default.h>

#include "open62541.h"

#include <signal.h>
#include <stdlib.h>

static volatile UA_Boolean running = true;
static void stopHandler(int sig) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
    running = false;
}


int main(void) {
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);

    UA_Server *server = UA_Server_new();
    UA_ServerConfig_setDefault(UA_Server_getConfig(server));



	//Add a new namespace to the server.
	UA_Int16 nsRoom1 = UA_Server_addNamespace(server, "Room_1");
	UA_LOG_INFO(UA_Log_Stdout,UA_LOGCATEGORY_USERLAND, "New Namespace Added with Number:%d\n", nsRoom1 );

	//Add a new object called Temperature..
	UA_NodeId r1_tempsense_Id; /* get the nodeid assigned by the server */
    UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
    //oAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Pump (Manual)");
    UA_Server_addObjectNode(server, UA_NODEID_STRING(2, "R1_TemperatureSensor" ),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                            UA_QUALIFIEDNAME(2, "Temperature Sensor"), UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
                            oAttr, NULL, &r1_tempsense_Id);

	//Add a variable Vendor Name to server.
	UA_VariableAttributes vnAttr = UA_VariableAttributes_default;
    UA_String vendorName = UA_STRING(":World Temperatures Ltd..:");
    UA_Variant_setScalar(&vnAttr.value, &vendorName, &UA_TYPES[UA_TYPES_STRING]);
    //vnAttr.displayName = UA_LOCALIZEDTEXT("en-US", "ManufacturerName");
    UA_Server_addVariableNode(server, UA_NODEID_STRING(2, "R1_TS1_VendorName" ), r1_tempsense_Id,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(2, "VendorName"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), vnAttr, NULL, NULL);

	//Add a variable Serial Number to server.
	UA_VariableAttributes snAttr = UA_VariableAttributes_default;
    UA_Int32 serialNumber = 0;
	UA_LOG_INFO(UA_Log_Stdout,UA_LOGCATEGORY_USERLAND, "Serial nUmber is %d...\n", serialNumber);
    UA_Variant_setScalar(&snAttr.value, &serialNumber, &UA_TYPES[UA_TYPES_INT32]);
    //snAttr.displayName = UA_LOCALIZEDTEXT("en-US", "ManufacturerName");
    UA_Server_addVariableNode(server, UA_NODEID_STRING(2, "R1_TS1_SerialNumber" ), r1_tempsense_Id,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(2, "SerialNumber"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), snAttr, NULL, NULL);



	UA_LOG_INFO(UA_Log_Stdout,UA_LOGCATEGORY_USERLAND, "Starting Server...\n");
    UA_StatusCode retval = UA_Server_run(server, &running);
	UA_LOG_INFO(UA_Log_Stdout,UA_LOGCATEGORY_USERLAND, "Server Shut Dwon....\n");

    UA_Server_delete(server);
    return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}
