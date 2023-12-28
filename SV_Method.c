//#include "open62541.h"
//#include <xml.h>
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>
#include <libxml2/libxml/xmlreader.h>

//#include "myNewServer.h"
//#include "myNewMethod.h"
#define MAX_STRING_SIZE 64


typedef struct {
char Tag[MAX_STRING_SIZE];
char Name[MAX_STRING_SIZE];
float Probability;
char CASnumber[MAX_STRING_SIZE];
int Concentration;
} AlarmStruct;

typedef struct {
char Tag[MAX_STRING_SIZE];
char Name[MAX_STRING_SIZE];
float Probability;
char CASnumber[MAX_STRING_SIZE];
int Concentration;
} NonAlarmStruct;

extern char SoftwareVersion[MAX_STRING_SIZE];
extern char DataBlockVersion[MAX_STRING_SIZE];
extern char InstrumentTime[MAX_STRING_SIZE];
extern char MeasurementTime[MAX_STRING_SIZE];

extern char BootStatus[MAX_STRING_SIZE];
extern char SnapshotStatus[MAX_STRING_SIZE];
extern char SCPStatus[MAX_STRING_SIZE];
extern char SFTPStatus[MAX_STRING_SIZE];
extern char RunScriptStatus[MAX_STRING_SIZE];
extern char ArchiveStatus[MAX_STRING_SIZE];
extern char AncillarySensorStatus[MAX_STRING_SIZE];

extern char Sensor[MAX_STRING_SIZE];
extern UA_Int16 OperatingTime;
extern char WarningMessage[MAX_STRING_SIZE];

extern UA_Float IgramPP;
extern UA_Float IgramDC;
extern UA_Float LaserPP;
extern UA_Float LaserDC;
extern UA_Float SingleBeamAt900;
extern UA_Float SingleBeamAt2500;
extern UA_Int16 SignalToNoiseAt2500;
extern UA_Float CenterBurstLocation;
extern UA_Float DetectorTemp;
extern UA_Float LaserFrequency;
extern UA_Int16 HardDriveSpace;
extern UA_Int16 Flow;
extern UA_Int16 Temperature;
extern UA_Float Pressure;
extern UA_Int16 TempOptics;
extern UA_Int16 BadScanCounter;
extern UA_Int16 FreeMemorySpace;

extern char LABFilename[MAX_STRING_SIZE];
extern char LOGFilename[MAX_STRING_SIZE];
extern char LgFilename[MAX_STRING_SIZE];
extern char SecondLgFilename[MAX_STRING_SIZE];

extern UA_Float SystemCounter;
extern UA_Float DetectorCounter;
extern UA_Float LaserCounter;
extern UA_Float FlowPumpCounter;
extern UA_Float DesiccantCounter;

extern UA_Int16 NoOfAlarms;
extern UA_Int16 NoOfNonAlarms;
extern AlarmStruct arrayOfAlarm[255];
extern NonAlarmStruct arrayOfNonAlarm[255];

extern int sockfd;
extern int command_sockfd;

    /*
    Namespace 0: is the Standard Server addressSpace given by the OPC UA Specifications.
    Spec Text: The Server exposes a Type System with DataTypes, ReferenceTypes, ObjectTypes and
    VariableTypes including all of the OPC Foundation provided (namespace 0) types that
    are used by the Server, as defined in Part 5. Items that are defined in Namespace 0 but are
    defined in other specification parts are tested as part of the other information models.

    Namespace 1: urn:unconfigured:Application can be configured in the ua_config_default.c file
    */


UA_StatusCode GetSoftwareVersionMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10001), output);
    //UA_String tmp = UA_STRING_ALLOC(SoftwareVersion);
    //UA_Variant_setScalarCopy(output, &tmp, &UA_TYPES[UA_TYPES_STRING]);      // global var$
    //UA_String_clear(&tmp);

    //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, (char*)"GetSoftwareVersionMethodCallback was called");
    return UA_STATUSCODE_GOOD;

}

UA_StatusCode GetDataBlockVersionMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10002), output);
    //UA_String tmp = UA_STRING_ALLOC(DataBlockVersion);
    //UA_Variant_setScalarCopy(output, &tmp, &UA_TYPES[UA_TYPES_STRING]);      // global variable
    //UA_String_clear(&tmp);

    //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, (char*)"GetDataBlockVersionMethodCallback was called");
    return UA_STATUSCODE_GOOD;

}
UA_StatusCode GetInstrumentTimeMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10101), output);
/*
    UA_String tmp = UA_STRING_ALLOC(InstrumentTime);
    UA_Variant_setScalarCopy(output, &tmp, &UA_TYPES[UA_TYPES_STRING]);      // global var$
    UA_String_clear(&tmp);

    //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, (char*)"GetInstrumentTimeMethodCallback was called");
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetMeasurementTimeMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10102), output);
/*
    UA_String tmp = UA_STRING_ALLOC(MeasurementTime);
    UA_Variant_setScalarCopy(output, &tmp, &UA_TYPES[UA_TYPES_STRING]);      // global var$
    UA_String_clear(&tmp);

    //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, (char*)"GetMeasurementTimeMethodCallback was called");
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetBootStatusMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10211), output);
/*
    UA_String tmp = UA_STRING_ALLOC(BootStatus);
    UA_Variant_setScalarCopy(output, &BootStatus, &UA_TYPES[UA_TYPES_STRING]);      // global var$
    UA_String_clear(&tmp);

    //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, (char*)"GetBootStatusMethodCallback was called");
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetSnapshotStatusMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10212), output);
/*
    UA_String tmp = UA_STRING_ALLOC(SnapshotStatus);
    UA_Variant_setScalarCopy(output, &tmp, &UA_TYPES[UA_TYPES_STRING]);      // global var$
    UA_String_clear(&tmp);

    //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, (char*)"GetSnapshotStatusMethodCallback was called");
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetSCPStatusMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10213), output);
/*
    UA_String tmp = UA_STRING_ALLOC(SCPStatus);
    UA_Variant_setScalarCopy(output, &tmp, &UA_TYPES[UA_TYPES_STRING]);      // global var$
    UA_String_clear(&tmp);

    //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, (char*)"GetSCPStatusMethodCallback was called");
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetSFTPStatusMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10214), output);
/*
    UA_String tmp = UA_STRING_ALLOC(SFTPStatus);
    UA_Variant_setScalarCopy(output, &tmp, &UA_TYPES[UA_TYPES_STRING]);      // global var$
    UA_String_clear(&tmp);

    //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, (char*)"GetSFTStatusMethodCallback was called");
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetRunScriptStatusMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10215), output);
/*
    UA_String tmp = UA_STRING_ALLOC(RunScriptStatus);
    UA_Variant_setScalarCopy(output, &tmp, &UA_TYPES[UA_TYPES_STRING]);      // global var$
    UA_String_clear(&tmp);

    //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, (char*)"GetRunScriptStatusMethodCallback was called");
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetArchiveStatusMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10216), output);
/*
    UA_String tmp = UA_STRING_ALLOC(ArchiveStatus);
    UA_Variant_setScalarCopy(output, &tmp, &UA_TYPES[UA_TYPES_STRING]);      // global var$
    UA_String_clear(&tmp);

    //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, (char*)"GetArchiveStatusMethodCallback was called");
    return UA_STATUSCODE_GOOD;
*/
}


UA_StatusCode GetAncillarySensorStatusMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10217), output);
/*
    UA_String tmp = UA_STRING_ALLOC(AncillarySensorStatus);
    UA_Variant_setScalarCopy(output, &tmp, &UA_TYPES[UA_TYPES_STRING]);      // global var$
    UA_String_clear(&tmp);

    //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, (char*)"GetAncillaryStatusMethodCallback was called");
    return UA_STATUSCODE_GOOD;
*/
}


UA_StatusCode GetSensorMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10201), output);
/*
    UA_String tmp = UA_STRING_ALLOC(Sensor);
    UA_Variant_setScalarCopy(output, &tmp, &UA_TYPES[UA_TYPES_STRING]);      // global var$
    UA_String_clear(&tmp);

    //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, (char*)"GetSensorMethodCallback was called");
    return UA_STATUSCODE_GOOD;
*/
}


UA_StatusCode GetOperatingTimeMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10202), output);

/*
    UA_Variant_setScalarCopy(output, &OperatingTime, &UA_TYPES[UA_TYPES_INT16]);      // global var$

    //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, (char*)"GetOperatingTimeMethodCallback was called");
    return UA_STATUSCODE_GOOD;
*/
}


UA_StatusCode GetWarningMessageMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10203), output);
/*
    UA_String tmp = UA_STRING_ALLOC(WarningMessage);
    UA_Variant_setScalarCopy(output, &tmp, &UA_TYPES[UA_TYPES_STRING]);      // global var$
    UA_String_clear(&tmp);

    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, (char*)"GetWarningMessageMethodCallback was called");
    return UA_STATUSCODE_GOOD;
*/
}


UA_StatusCode GetIgramPPMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10301), output);
/*
    UA_Variant_setScalarCopy(output, &IgramPP, &UA_TYPES[UA_TYPES_FLOAT]);      // global variable

    //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, (char*)"GetIgramPPMethodCallback was called");
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetIgramDCMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10302), output);
/*
    UA_Variant_setScalarCopy(output, &IgramDC, &UA_TYPES[UA_TYPES_FLOAT]);      // global variable

    //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, (char*)"GetIgramPPMethodCallback was called");
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetLaserPPMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10303), output);
/*
    UA_Variant_setScalarCopy(output, &LaserPP, &UA_TYPES[UA_TYPES_FLOAT]);      // global variable

    //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, (char*)"GetIgramPPMethodCallback was called");
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetLaserDCMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10304), output);
/*
    UA_Variant_setScalarCopy(output, &LaserDC, &UA_TYPES[UA_TYPES_FLOAT]);      // global variable

    //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, (char*)"GetIgramPPMethodCallback was called");
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetSingleBeamAt900MethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10305), output);
/*
    UA_Variant_setScalarCopy(output, &SingleBeamAt900, &UA_TYPES[UA_TYPES_FLOAT]);      // global variable
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetSingleBeamAt2500MethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10306), output);
/*
    UA_Variant_setScalarCopy(output, &SingleBeamAt2500, &UA_TYPES[UA_TYPES_FLOAT]);      // global variable
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetSignalToNoiseAt2500MethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10307), output);
/*
    UA_Variant_setScalarCopy(output, &SignalToNoiseAt2500, &UA_TYPES[UA_TYPES_INT16]);      // global variable
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetCenterBurstLocationMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10308), output);
/*
    UA_Variant_setScalarCopy(output, &CenterBurstLocation, &UA_TYPES[UA_TYPES_FLOAT]);      // global variable
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetDetectorTempMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10309), output);
/*
    UA_Variant_setScalarCopy(output, &DetectorTemp, &UA_TYPES[UA_TYPES_FLOAT]);      // global variable
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetLaserFrequencyMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10310), output);
/*
    UA_Variant_setScalarCopy(output, &LaserFrequency, &UA_TYPES[UA_TYPES_FLOAT]);      // global variable
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetHardDriveSpaceMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10311), output);
/*
    UA_Variant_setScalarCopy(output, &HardDriveSpace, &UA_TYPES[UA_TYPES_INT16]);      // global variable
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetFlowMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10312), output);
/*
    UA_Variant_setScalarCopy(output, &Flow, &UA_TYPES[UA_TYPES_INT16]);      // global variable
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetTemperatureMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10313), output);
/*
    UA_Variant_setScalarCopy(output, &Temperature, &UA_TYPES[UA_TYPES_FLOAT]);      // global variable
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetPressureMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10314), output);
/*
    UA_Variant_setScalarCopy(output, &Pressure, &UA_TYPES[UA_TYPES_FLOAT]);      // global variable
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetTempOpticsMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10315), output);
/*
    UA_Variant_setScalarCopy(output, &TempOptics, &UA_TYPES[UA_TYPES_FLOAT]);      // global variable
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetBadScanCounterMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10316), output);
/*
    UA_Variant_setScalarCopy(output, &BadScanCounter, &UA_TYPES[UA_TYPES_INT16]);      // global variable
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetFreeMemorySpaceMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10317), output);
/*
    UA_Variant_setScalarCopy(output, &FreeMemorySpace, &UA_TYPES[UA_TYPES_INT16]);      // global variable
    return UA_STATUSCODE_GOOD;
*/
}


UA_StatusCode GetLABFilenameMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10318), output);
/*
    UA_String tmp = UA_STRING_ALLOC(LABFilename);
    UA_Variant_setScalarCopy(output, &tmp, &UA_TYPES[UA_TYPES_STRING]);      // global variable
    UA_String_clear(&tmp);
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetLOGFilenameMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10319), output);
/*
    UA_String tmp = UA_STRING_ALLOC(LOGFilename);
    UA_Variant_setScalarCopy(output, &tmp, &UA_TYPES[UA_TYPES_STRING]);      // global variable
    UA_String_clear(&tmp);
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetLgFilenameMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10320), output);
/*
    UA_String tmp = UA_STRING_ALLOC(LgFilename);
    UA_Variant_setScalarCopy(output, &tmp, &UA_TYPES[UA_TYPES_STRING]);      // global variable
    UA_String_clear(&tmp);
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetSecondLgFilenameMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10321), output);
/*
    UA_String tmp = UA_STRING_ALLOC(SecondLgFilename);
    UA_Variant_setScalarCopy(output, &tmp, &UA_TYPES[UA_TYPES_STRING]);      // global variable
    UA_String_clear(&tmp);
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetSystemCounterMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10322), output);
/*
    UA_Variant_setScalarCopy(output, &SystemCounter, &UA_TYPES[UA_TYPES_FLOAT]);      // global variable
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetDetectorCounterMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10323), output);
/*
    UA_Variant_setScalarCopy(output, &DetectorCounter, &UA_TYPES[UA_TYPES_FLOAT]);      // global variable
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetLaserCounterMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10324), output);
/*
    UA_Variant_setScalarCopy(output, &LaserCounter, &UA_TYPES[UA_TYPES_FLOAT]);      // global variable
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetFlowPumpCounterMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10325), output);
/*
    UA_Variant_setScalarCopy(output, &FlowPumpCounter, &UA_TYPES[UA_TYPES_FLOAT]);      // global variable
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetDesiccantCounterMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10326), output);
/*
    UA_Variant_setScalarCopy(output, &DesiccantCounter, &UA_TYPES[UA_TYPES_FLOAT]);      // global variable
    return UA_STATUSCODE_GOOD;
*/
}


UA_StatusCode GetNoOfAlarmsMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10401), output);
/*
    UA_Variant_setScalarCopy(output, &NoOfAlarms, &UA_TYPES[UA_TYPES_INT16]);      // global variable
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetArrayOfAlarmMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    int retval = 0;

    retval = UA_Variant_setScalarCopy(output, &arrayOfAlarm, &UA_TYPES[UA_TYPES_VARIANT]);      // global variable
    //printf("retval from setScalarCopy : %d \n", retval);
    //sleep(3);

    return UA_STATUSCODE_GOOD;

}

UA_StatusCode GetNoOfNonAlarmsMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
    UA_Server_readValue(server, UA_NODEID_NUMERIC(namespaceIndex, 10402), output);
/*
    UA_Variant_setScalarCopy(output, &NoOfNonAlarms, &UA_TYPES[UA_TYPES_INT16]);      // global variable
    return UA_STATUSCODE_GOOD;
*/
}

UA_StatusCode GetArrayOfNonAlarmMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)

{
    int retval = 0;

    retval = UA_Variant_setScalarCopy(output, &arrayOfNonAlarm, &UA_TYPES[UA_TYPES_VARIANT]);      // global variable
    //printf("retval from setScalarCopy : %d \n", retval);

    return UA_STATUSCODE_GOOD;

}

UA_StatusCode ClearAlarmLEDMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
	// uses command_sockfd : port 10004
	char cmdbuffer[255],respbuffer[255];
	char header[255];
	char response[255];
	char footer[255];
	int numbytes;
	int retval;

	//sprintf(cmdbuffer, "<Command name='ClearAlarmLED' originator='OPCUA'></Command>"); // no effect : <Command name='ClearAlarmLED' originator='OPCUA'></Command>
	//sprintf(cmdbuffer, "<Command name=\"ClearAlarmLED\" originator=\"OPCUA\"></Command>"); // no effect : <Command name='ClearAlarmLED' originator='OPCUA'></Command>

	sprintf(cmdbuffer,"<Command name=\"ClearAlarmLED\" />"); // no effect : <Command name="Shutdown" />
	cmdbuffer[32] = '\0';
	//printf("command_sockfd: %d, Command to be executed is : %s \n\n", command_sockfd, cmdbuffer);

	retval = write(command_sockfd, cmdbuffer, strlen(cmdbuffer)); //sizeof(cmdbuffer));
	if (retval == -1)
        {
		perror("Fail to send command to Airgard");
		exit(-1);
        }
	else
	{
		//printf("==================write outcome is %d \n", retval);
		//sleep(3);
		// check the return value immediately
		//if ((numbytes=recv(command_sockfd, respbuffer, MAX_BUFFER_SIZE, 0)) == -1)
                //{
                //        perror("recv");
                //        exit(-1);
                //}
                //respbuffer[numbytes] = '\0';

		// output sample
		// <AirGard serial="AG-123456789">
		//	<Response name="ClearAlarmLED" accepted="yes"></Response>
		// </AirGard>

		//if (gets(buffer, "%s%s%s", header, response, footer) == 0)
		//{
		//	printf("Response buffer received is %s \n", respbuffer);
		//	printf("Response yes?no is %s \n", response);

			// check the ocurrence of "yes"
		//	if (strstr(response, 'yes') == 0)
		//		printf("Command successfully executed by airgard \n");
		//	else
		//		printf("Command fail to execute by airgard \n");
		//}
    	}
	return UA_STATUSCODE_GOOD;

}

// -------------------------------
// convert regular method calls to event methods
// -------------------------------
/*
UA_StatusCode ResetEventMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)

{
	generateEvent_ResetMethodCallback(server,
                         sessionId, sessionHandle,
                         methodId, methodContext,
                         objectId, objectContext,
                         inputSize, input,
                         outputSize, output);

    return UA_STATUSCODE_GOOD;

}

UA_StatusCode ShutdownEventMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)

{
        generateEvent_ShutdownMethodCallback(server,
                         sessionId, sessionHandle,
                         methodId, methodContext,
                         objectId, objectContext,
                         inputSize, input,
                         outputSize, output);

    return UA_STATUSCODE_GOOD;

}
*/
//-----------------------------------------------------------------------------------------------------------------

void
addGetSoftwareVersionMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetSoftwareVersionOutput");
    outputArgument.name = UA_STRING((char*)"SoftwareVersionMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getSoftwareVersionAttr = UA_MethodAttributes_default;
    getSoftwareVersionAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetSoftwareVersion");
    getSoftwareVersionAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetSoftwareVersion");
    getSoftwareVersionAttr.executable = UA_TRUE;
    getSoftwareVersionAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62528),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetSoftwareVersion"),
                            getSoftwareVersionAttr, &GetSoftwareVersionMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Method Node created : Airgard->Methods->GetSoftwareVersion Node");
}

void
addGetDataBlockVersionMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetDataBlockVersionOutput");
    outputArgument.name = UA_STRING((char*)"DataBlockVersionMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getDataBlockVersionAttr = UA_MethodAttributes_default;
    getDataBlockVersionAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetDataBlockVersion");
    getDataBlockVersionAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetDataBlockVersion");
    getDataBlockVersionAttr.executable = UA_TRUE;
    getDataBlockVersionAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62529),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetDataBlockVersion"),
                            getDataBlockVersionAttr, &GetDataBlockVersionMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetDataBlockVersion Node");

}

void
addGetInstrumentTimeMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetInstrumentTimeOutput");
    outputArgument.name = UA_STRING((char*)"InstrumentTimeMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getInstrumentTimeAttr = UA_MethodAttributes_default;
    getInstrumentTimeAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetInstrumentTime");
    getInstrumentTimeAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetInstrumentTime");
    getInstrumentTimeAttr.executable = UA_TRUE;
    getInstrumentTimeAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62530),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetInstrumentTime"),
                            getInstrumentTimeAttr, &GetInstrumentTimeMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetInstrumentTime Node");

}

void
addGetMeasurementTimeMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetMeasurementTimeOutput");
    outputArgument.name = UA_STRING((char*)"MeasurementTimeOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getMeasurementTimeAttr = UA_MethodAttributes_default;
    getMeasurementTimeAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetMeasurementTime");
    getMeasurementTimeAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetMeasurementTime");
    getMeasurementTimeAttr.executable = UA_TRUE;
    getMeasurementTimeAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62531),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetMeasurementTime"),
                            getMeasurementTimeAttr, &GetMeasurementTimeMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetMeasurementTime Node");

}

void
addGetBootStatusMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetBootStatusOutput");
    outputArgument.name = UA_STRING((char*)"BootStatusOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getBootStatusAttr = UA_MethodAttributes_default;
    getBootStatusAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetBootStatus");
    getBootStatusAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetBootStatus");
    getBootStatusAttr.executable = UA_TRUE;
    getBootStatusAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62532),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetBootStatus"),
                            getBootStatusAttr, &GetBootStatusMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetBootStatus Node");


}

void
addGetSnapshotStatusMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetSnapshotStatusOutput");
    outputArgument.name = UA_STRING((char*)"SnapshotStatusOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getSnapshotStatusAttr = UA_MethodAttributes_default;
    getSnapshotStatusAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetSnapshotStatus");
    getSnapshotStatusAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetSnapshotStatus");
    getSnapshotStatusAttr.executable = UA_TRUE;
    getSnapshotStatusAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62533),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetSnapshotStatus"),
                            getSnapshotStatusAttr, &GetSnapshotStatusMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetSnapshotStatus Node");

}

void
addGetSCPStatusMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetSCPStatusOutput");
    outputArgument.name = UA_STRING((char*)"SCPStatusOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getSCPStatusAttr = UA_MethodAttributes_default;
    getSCPStatusAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetSCPStatus");
    getSCPStatusAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetSCPStatus");
    getSCPStatusAttr.executable = UA_TRUE;
    getSCPStatusAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62534),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetSCPStatusInfo"),
                            getSCPStatusAttr, &GetSCPStatusMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetSCPStatus Node");

}


void
addGetSFTPStatusMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetSFTPStatusOutput");
    outputArgument.name = UA_STRING((char*)"SFTPStatusOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getSFTPStatusAttr = UA_MethodAttributes_default;
    getSFTPStatusAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetSFTPStatus");
    getSFTPStatusAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetSFTPStatus");
    getSFTPStatusAttr.executable = UA_TRUE;
    getSFTPStatusAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62535),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetSFTPStatus"),
                            getSFTPStatusAttr, &GetSFTPStatusMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetSFTPStatus Node");

}


void
addGetRunScriptStatusMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetRunScriptStatusOutput");
    outputArgument.name = UA_STRING((char*)"RunScriptStatusOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getRunScriptStatusAttr = UA_MethodAttributes_default;
    getRunScriptStatusAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetRunScriptStatus");
    getRunScriptStatusAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetRunScriptStatus");
    getRunScriptStatusAttr.executable = UA_TRUE;
    getRunScriptStatusAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62536),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetRunScriptStatus"),
                            getRunScriptStatusAttr, &GetRunScriptStatusMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetRunScriptStatus Node");

}


void
addGetArchiveStatusMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetArchiveStatusOutput");
    outputArgument.name = UA_STRING((char*)"ArchiveStatusOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getArchiveStatusAttr = UA_MethodAttributes_default;
    getArchiveStatusAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetArchiveStatus");
    getArchiveStatusAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetArchiveStatus");
    getArchiveStatusAttr.executable = UA_TRUE;
    getArchiveStatusAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62537),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetArchiveStatus"),
                            getArchiveStatusAttr, &GetArchiveStatusMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetArchiveStatus Node");
}


void
addGetAncillarySensorStatusMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetAncillaryStatusOutput");
    outputArgument.name = UA_STRING((char*)"AncillaryStatusOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getAncillarySensorStatusAttr = UA_MethodAttributes_default;
    getAncillarySensorStatusAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetAncillarySensorStatus");
    getAncillarySensorStatusAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetAncillarySensorStatus");
    getAncillarySensorStatusAttr.executable = UA_TRUE;
    getAncillarySensorStatusAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62538),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetAncillarySensorStatus"),
                            getAncillarySensorStatusAttr, &GetAncillarySensorStatusMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetAncillarySensorStatus Node");
}

void
addGetSensorMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetSensorOutput");
    outputArgument.name = UA_STRING((char*)"SensorOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getSensorAttr = UA_MethodAttributes_default;
    getSensorAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetSensor");
    getSensorAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetSensor");
    getSensorAttr.executable = UA_TRUE;
    getSensorAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62539),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetSensor"),
                            getSensorAttr, &GetSensorMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetSensor Node");
}

void
addGetOperatingTimeMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetOperatingTimeOutput");
    outputArgument.name = UA_STRING((char*)"OperatingTimeOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_INT16].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getOperatingTimeAttr = UA_MethodAttributes_default;
    getOperatingTimeAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetOperatingTime");
    getOperatingTimeAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetOperatingTime");
    getOperatingTimeAttr.executable = UA_TRUE;
    getOperatingTimeAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62540),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetOperatingTime"),
                            getOperatingTimeAttr, &GetOperatingTimeMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetOperatingTime Node");
}

void
addGetWarningMessageMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetWarningMessageOutput");
    outputArgument.name = UA_STRING((char*)"WarningMessageOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getWarningMessageAttr = UA_MethodAttributes_default;
    getWarningMessageAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetWarningMessage");
    getWarningMessageAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetWarningMessage");
    getWarningMessageAttr.executable = UA_TRUE;
    getWarningMessageAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62541),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetWarningMessage"),
                            getWarningMessageAttr, &GetWarningMessageMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetWarningMessage Node");
}



void
addGetIgramPPMethod(UA_Server *server, UA_NodeId parent)
{
        // no effect : not required since we are not querying using a parameter
        /*
    UA_Argument inputArgument;
    UA_Argument_init(&inputArgument);
    inputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetIgramPPInput");
    inputArgument.name = UA_STRING((char*)"IgramPPMethodInput");
    inputArgument.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
    inputArgument.valueRank = UA_VALUERANK_SCALAR;
        */

    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetIgramPPOutput");
    outputArgument.name = UA_STRING((char*)"IgramPPMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getIgramPPAttr = UA_MethodAttributes_default;
    getIgramPPAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetIgramPP");
    getIgramPPAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetIgramPP");
    getIgramPPAttr.executable = UA_TRUE;
    getIgramPPAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62542),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetIgramPP"),
                            getIgramPPAttr, &GetIgramPPMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetIgramPP Node");
}

void
addGetIgramDCMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetIgramDCOutput");
    outputArgument.name = UA_STRING((char*)"IgramDCMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getIgramDCAttr = UA_MethodAttributes_default;
    getIgramDCAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetIgramDC");
    getIgramDCAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetIgramDC");
    getIgramDCAttr.executable = UA_TRUE;
    getIgramDCAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62543),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetIgramDC"),
                            getIgramDCAttr, &GetIgramDCMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetIgramDC Node");
}

void
addGetLaserPPMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetLaserPPOutput");
    outputArgument.name = UA_STRING((char*)"LaserPPMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getLaserPPAttr = UA_MethodAttributes_default;
    getLaserPPAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetLaserPP");
    getLaserPPAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetLaserPP");
    getLaserPPAttr.executable = UA_TRUE;
    getLaserPPAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62544),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetLaserPP"),
                            getLaserPPAttr, &GetLaserPPMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetLaserPP Node");
}

void
addGetLaserDCMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetLaserDCOutput");
    outputArgument.name = UA_STRING((char*)"LaserDCMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getLaserDCAttr = UA_MethodAttributes_default;
    getLaserDCAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetLaserDC");
    getLaserDCAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetLaserDC");
    getLaserDCAttr.executable = UA_TRUE;
    getLaserDCAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62545),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetLaserDC"),
                            getLaserDCAttr, &GetLaserDCMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetLaserDC Node");
}

void
addGetSingleBeamAt900Method(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetSingleBeamAt900Output");
    outputArgument.name = UA_STRING((char*)"SingleBeamAt900MethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getSingleBeamAt900Attr = UA_MethodAttributes_default;
    getSingleBeamAt900Attr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetSingleBeamAt900");
    getSingleBeamAt900Attr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetSingleBeamAt900");
    getSingleBeamAt900Attr.executable = UA_TRUE;
    getSingleBeamAt900Attr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62546),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetSingleBeamAt900"),
                            getSingleBeamAt900Attr, &GetSingleBeamAt900MethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetSingleBeamAt900 Node");
}

void
addGetSingleBeamAt2500Method(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetSingleBeamAt2500Output");
    outputArgument.name = UA_STRING((char*)"SingleBeamAt2500MethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getSingleBeamAt2500Attr = UA_MethodAttributes_default;
    getSingleBeamAt2500Attr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetSingleBeamAt2500");
    getSingleBeamAt2500Attr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetSingleBeamAt2500");
    getSingleBeamAt2500Attr.executable = UA_TRUE;
    getSingleBeamAt2500Attr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62547),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetSingleBeamAt2500"),
                            getSingleBeamAt2500Attr, &GetSingleBeamAt2500MethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetSingleBeamAt2500 Node");
}

void
addGetSignalToNoiseAt2500Method(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetSignalToNoiseAt2500Output");
    outputArgument.name = UA_STRING((char*)"SignalToNoiseAt2500MethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_INT16].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getSignalToNoiseAt2500Attr = UA_MethodAttributes_default;
    getSignalToNoiseAt2500Attr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetSignalToNoiseAt2500");
    getSignalToNoiseAt2500Attr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetSignalToNoiseAt2500");
    getSignalToNoiseAt2500Attr.executable = UA_TRUE;
    getSignalToNoiseAt2500Attr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62548),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetSignalToNoiseAt2500"),
                            getSignalToNoiseAt2500Attr, &GetSignalToNoiseAt2500MethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetSignalToNoiseAt2500 Node");
}

void
addGetCenterBurstLocationMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetCenterBurstLocationOutput");
    outputArgument.name = UA_STRING((char*)"CenterBurstLocationMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getCenterBurstLocationAttr = UA_MethodAttributes_default;
    getCenterBurstLocationAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetCentreBurstLocation");
    getCenterBurstLocationAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetCenterBurstLocation");
    getCenterBurstLocationAttr.executable = UA_TRUE;
    getCenterBurstLocationAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62549),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetCenterBurstLocation"),
                            getCenterBurstLocationAttr, &GetCenterBurstLocationMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);
//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetCenterBurstLocation Node");
}

void
addGetDetectorTempMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetDetectorTempOutput");
    outputArgument.name = UA_STRING((char*)"DetectorTempMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getDetectorTempAttr = UA_MethodAttributes_default;
    getDetectorTempAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetDetectorTemp");
    getDetectorTempAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetDetectorTemp");
    getDetectorTempAttr.executable = UA_TRUE;
    getDetectorTempAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62550),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetDetectorTemp"),
                            getDetectorTempAttr, &GetDetectorTempMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetDetectorTemp Node");
}

void
addGetLaserFrequencyMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetLaserFrequencyOutput");
    outputArgument.name = UA_STRING((char*)"LaserFrequencyMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getLaserFrequencyAttr = UA_MethodAttributes_default;
    getLaserFrequencyAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetLaserFrequency");
    getLaserFrequencyAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetLaserFrequency");
    getLaserFrequencyAttr.executable = UA_TRUE;
    getLaserFrequencyAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62551),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetLaserFrequency"),
                            getLaserFrequencyAttr, &GetLaserFrequencyMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetLaserFrequency Node");
}

void
addGetHardDriveSpaceMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetHardDriveSpaceOutput");
    outputArgument.name = UA_STRING((char*)"HardDriveSpaceMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_INT16].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getHardDriveSpaceAttr = UA_MethodAttributes_default;
    getHardDriveSpaceAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetHardDriveSpace");
    getHardDriveSpaceAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetHardDriveSpace");
    getHardDriveSpaceAttr.executable = UA_TRUE;
    getHardDriveSpaceAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62552),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetHardDriveSpace"),
                            getHardDriveSpaceAttr, &GetHardDriveSpaceMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetHardDriveSpace Node");
}

void
addGetFlowMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetFlowOutput");
    outputArgument.name = UA_STRING((char*)"FlowMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_INT16].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getFlowAttr = UA_MethodAttributes_default;
    getFlowAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetFlow");
    getFlowAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetFlow");
    getFlowAttr.executable = UA_TRUE;
    getFlowAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62553),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetFlow"),
                            getFlowAttr, &GetFlowMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetFlow Node");
}

void
addGetTemperatureMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetTemperatureOutput");
    outputArgument.name = UA_STRING((char*)"TemperatureMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_INT16].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getTemperatureAttr = UA_MethodAttributes_default;
    getTemperatureAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetTemperature");
    getTemperatureAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetTemperature");
    getTemperatureAttr.executable = UA_TRUE;
    getTemperatureAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62554),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetTempeature"),
                            getTemperatureAttr, &GetTemperatureMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetTemperature Node");
}

void
addGetPressureMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetPressureOutput");
    outputArgument.name = UA_STRING((char*)"PressureMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getPressureAttr = UA_MethodAttributes_default;
    getPressureAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetPressure");
    getPressureAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetPressure");
    getPressureAttr.executable = UA_TRUE;
    getPressureAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62555),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetPressure"),
                            getPressureAttr, &GetPressureMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetPressure Node");
}

void
addGetTempOpticsMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetTempOpticsOutput");
    outputArgument.name = UA_STRING((char*)"TempOpticsMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_INT16].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getTempOpticsAttr = UA_MethodAttributes_default;
    getTempOpticsAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetTempOptics");
    getTempOpticsAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetTempOptics");
    getTempOpticsAttr.executable = UA_TRUE;
    getTempOpticsAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62556),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetTempOptics"),
                            getTempOpticsAttr, &GetTempOpticsMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetTempOptics Node");
}

void
addGetBadScanCounterMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetBadScanCounterOutput");
    outputArgument.name = UA_STRING((char*)"BadScanCounterMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_INT16].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getBadScanCounterAttr = UA_MethodAttributes_default;
    getBadScanCounterAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetBadScanCounterOptics");
    getBadScanCounterAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetBadScanCounterOptics");
    getBadScanCounterAttr.executable = UA_TRUE;
    getBadScanCounterAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62557),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetBadScanCounter"),
                            getBadScanCounterAttr, &GetBadScanCounterMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetBadScanCounter Node");
}

void
addGetFreeMemorySpaceMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetFreeMemorySpaceOutput");
    outputArgument.name = UA_STRING((char*)"FreeMemorySpaceMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_INT16].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getFreeMemorySpaceAttr = UA_MethodAttributes_default;
    getFreeMemorySpaceAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetFreeMemorySpace");
    getFreeMemorySpaceAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetFreeMemorySpace");
    getFreeMemorySpaceAttr.executable = UA_TRUE;
    getFreeMemorySpaceAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62558),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetTempOptics"),
                            getFreeMemorySpaceAttr, &GetFreeMemorySpaceMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetFreeMemorySpace Node");
}

void
addGetLABFilenameMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetLABFilenameOutput");
    outputArgument.name = UA_STRING((char*)"LABFilenameMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getLABFilenameAttr = UA_MethodAttributes_default;
    getLABFilenameAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetLABFilename");
    getLABFilenameAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetLABFilename");
    getLABFilenameAttr.executable = UA_TRUE;
    getLABFilenameAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62559),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetLABFilename"),
                            getLABFilenameAttr, &GetLABFilenameMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetLABFilename Node");
}

void
addGetLOGFilenameMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetLOGFilenameOutput");
    outputArgument.name = UA_STRING((char*)"LOGFilenameMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getLOGFilenameAttr = UA_MethodAttributes_default;
    getLOGFilenameAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetLOGFilename");
    getLOGFilenameAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetLOGFilename");
    getLOGFilenameAttr.executable = UA_TRUE;
    getLOGFilenameAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62560),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetLOGFilename"),
                            getLOGFilenameAttr, &GetLOGFilenameMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetLOGFilename Node");
}

void
addGetLgFilenameMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetLgFilenameOutput");
    outputArgument.name = UA_STRING((char*)"LgFilenameMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getLgFilenameAttr = UA_MethodAttributes_default;
    getLgFilenameAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetLgFilename");
    getLgFilenameAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetLgFilename");
    getLgFilenameAttr.executable = UA_TRUE;
    getLgFilenameAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62561),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetLgFilename"),
                            getLgFilenameAttr, &GetLgFilenameMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetLgFilename Node");
}

void
addGetSecondLgFilenameMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetSecondLgFilenameOutput");
    outputArgument.name = UA_STRING((char*)"SecondLgFilenameMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getSecondLgFilenameAttr = UA_MethodAttributes_default;
    getSecondLgFilenameAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetSecondLgFilename");
    getSecondLgFilenameAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetSecondLgFilename");
    getSecondLgFilenameAttr.executable = UA_TRUE;
    getSecondLgFilenameAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62562),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetSecondLgFilename"),
                            getSecondLgFilenameAttr, &GetSecondLgFilenameMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetSecondLgFilename Node");
}


void
addGetSystemCounterMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetSystemCounterOutput");
    outputArgument.name = UA_STRING((char*)"SystemCounterMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getSystemCounterAttr = UA_MethodAttributes_default;
    getSystemCounterAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetSystemCounter");
    getSystemCounterAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetSystemCounter");
    getSystemCounterAttr.executable = UA_TRUE;
    getSystemCounterAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62563),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetSystemCounter"),
                            getSystemCounterAttr, &GetSystemCounterMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetSystemCounter Node");
}

void
addGetDetectorCounterMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetDetectorCounterOutput");
    outputArgument.name = UA_STRING((char*)"DetectorCounterMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getDetectorCounterAttr = UA_MethodAttributes_default;
    getDetectorCounterAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetDetectorCounter");
    getDetectorCounterAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetDetectorCounter");
    getDetectorCounterAttr.executable = UA_TRUE;
    getDetectorCounterAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62564),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)(char*)"GetDetectorCounter"),
                            getDetectorCounterAttr, &GetDetectorCounterMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetDetectorCounter Node");
}

void
addGetLaserCounterMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetLaserCounterOutput");
    outputArgument.name = UA_STRING((char*)"LaserCounterMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getLaserCounterAttr = UA_MethodAttributes_default;
    getLaserCounterAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetLaserCounter");
    getLaserCounterAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetLaserCounter");
    getLaserCounterAttr.executable = UA_TRUE;
    getLaserCounterAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62565),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetLaserCounter"),
                            getLaserCounterAttr, &GetLaserCounterMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetLaserCounter Node");
}

void
addGetFlowPumpCounterMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetFlowPumpCounterOutput");
    outputArgument.name = UA_STRING((char*)"FlowPumpCounterMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getFlowPumpCounterAttr = UA_MethodAttributes_default;
    getFlowPumpCounterAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetFlowPumpCounter");
    getFlowPumpCounterAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetFlowPumpCounter");
    getFlowPumpCounterAttr.executable = UA_TRUE;
    getFlowPumpCounterAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62566),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetFlowPumpCounter"),
                            getFlowPumpCounterAttr, &GetFlowPumpCounterMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetFlowPumpCounter Node");
}

void
addGetDesiccantCounterMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetDesiccantCounterOutput");
    outputArgument.name = UA_STRING((char*)"DesiccantCounterMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getDesiccantCounterAttr = UA_MethodAttributes_default;
    getDesiccantCounterAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetDesiccantCounter");
    getDesiccantCounterAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetDesiccantCounter");
    getDesiccantCounterAttr.executable = UA_TRUE;
    getDesiccantCounterAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62567),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetDesiccantCounter"),
                            getDesiccantCounterAttr, &GetDesiccantCounterMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetDesiccantCounter Node");

}


void addGetNoOfAlarmsMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetNoOfAlarmsOutput");
    outputArgument.name = UA_STRING((char*)"NoOfAlarmsMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_INT16].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getNoOfAlarmsAttr = UA_MethodAttributes_default;
    getNoOfAlarmsAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetNoOfAlarms");
    getNoOfAlarmsAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetNoOfAlarms");
    getNoOfAlarmsAttr.executable = UA_TRUE;
    getNoOfAlarmsAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62568),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetNoOfAlarms"),
                            getNoOfAlarmsAttr, &GetNoOfAlarmsMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetNoOfAlarms Node");

}

void addGetArrayOfAlarmMethod(UA_Server *server, UA_NodeId parent)
{
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetArrayOfAlarmOutput");
    outputArgument.name = UA_STRING((char*)"ArrayOfAlarmMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_VARIANT].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getArrayOfAlarmAttr = UA_MethodAttributes_default;
    getArrayOfAlarmAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetArrayOfAlarm");
    getArrayOfAlarmAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetArrayOfAlarm");
    getArrayOfAlarmAttr.executable = UA_TRUE;
    getArrayOfAlarmAttr.userExecutable = UA_TRUE;
    int retval = UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62569),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetArrayOfAlarm"),
                            getArrayOfAlarmAttr, &GetArrayOfAlarmMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);
    //printf("retval from UA_Server_addMethodNode - GetArrayOfAlarm : %d \n", retval);
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Method Node created : Airgard->Methods->GetArrayOfAlarms Node");
}

void addGetNoOfNonAlarmsMethod(UA_Server *server, UA_NodeId parent)
{

    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetNoOfNonAlarms");
    outputArgument.name = UA_STRING((char*)"NoOfNonAlarmsMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_INT16].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getNoOfNonAlarmsAttr = UA_MethodAttributes_default;
    getNoOfNonAlarmsAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetNoOfNonAlarms");
    getNoOfNonAlarmsAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetNoOfNonAlarms");
    getNoOfNonAlarmsAttr.executable = UA_TRUE;
    getNoOfNonAlarmsAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62570),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetNoOfNonAlarms"),
                            getNoOfNonAlarmsAttr, &GetNoOfNonAlarmsMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                "Method Node created : Airgard->Methods->GetNoOfNonAlarms Node");

}


void addGetArrayOfNonAlarmMethod(UA_Server *server, UA_NodeId parent)
{

    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"GetArrayOfNonAlarmOutput");
    outputArgument.name = UA_STRING((char*)"ArrayOfNonAlarmMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_VARIANT].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes getArrayOfAlarmAttr = UA_MethodAttributes_default;
    getArrayOfAlarmAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetArrayOfNonAlarm");
    getArrayOfAlarmAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"GetArrayOfNonAlarm");
    getArrayOfAlarmAttr.executable = UA_TRUE;
    getArrayOfAlarmAttr.userExecutable = UA_TRUE;
    int retval = UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62571),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"GetArrayOfNonAlarm"),
                            getArrayOfAlarmAttr, &GetArrayOfNonAlarmMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

	//printf("retval from UA_Server_addMethodNode - GetArrayOfNonAlarm : %d \n", retval);
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Method Node created : Airgard->Methods->GetArrayOfNonAlarms Node");

}

void addClearAlarmLEDMethod(UA_Server *server, UA_NodeId parent)
{

    size_t namespaceIndex;
    UA_Server_getNamespaceByName(server, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"ClearAlarmLED");
    outputArgument.name = UA_STRING((char*)"ClearAlarmLEDMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_INT16].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes ClearAlarmLEDAttr = UA_MethodAttributes_default;
    ClearAlarmLEDAttr.description = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"ClearAlarmLED");
    ClearAlarmLEDAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",(char*)"ClearAlarmLED");
    ClearAlarmLEDAttr.executable = UA_TRUE;
    ClearAlarmLEDAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62572),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, (char*)"ClearAlarmLED"),
                            ClearAlarmLEDAttr, &ClearAlarmLEDMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Method Node created : Airgard->Methods->ClearAlarmLED Node");

}

/*
void addResetEventMethod(UA_Server *server, UA_NodeId parent)
{
    UA_UInt16 namespaceIndex = 1;
    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT("en-US", "ResetEvent");
    outputArgument.name = UA_STRING((char*)"ResetEventMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_INT16].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes ResetEventMethodAttr = UA_MethodAttributes_default;
    ResetEventMethodAttr.description = UA_LOCALIZEDTEXT((char*)"en-US","ResetEvent");
    ResetEventMethodAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US","ResetEvent");
    ResetEventMethodAttr.executable = UA_TRUE;
    ResetEventMethodAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62573),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, "ResetEvent"),
                            ResetEventMethodAttr, &ResetEventMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Method Node created : Airgard->Methods->ResetEvent Node\n");

}

void addShutdownEventMethod(UA_Server *server, UA_NodeId parent)
{
    UA_UInt16 namespaceIndex = 1;
    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT((char*)"en-US", "ShutdownEvent");
    outputArgument.name = UA_STRING((char*)"ShutdownEventMethodOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_INT16].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes ShutdownEventMethodAttr = UA_MethodAttributes_default;
    ShutdownEventMethodAttr.description = UA_LOCALIZEDTEXT((char*)"en-US","ShutdownEvent");
    ShutdownEventMethodAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US","ShutdownEvent");
    ShutdownEventMethodAttr.executable = UA_TRUE;
    ShutdownEventMethodAttr.userExecutable = UA_TRUE;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(namespaceIndex,62574),
                            parent,                     //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(namespaceIndex, "ShutdownEvent"),
                            ShutdownEventMethodAttr, &ShutdownEventMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);

    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Method Node created : Airgard->Methods->ShutdownEvent Node\n");

}
*/

void CreateServerMethodItems(UA_Server *server, UA_NodeId r2_airgard_method_Id)
{
        addGetSoftwareVersionMethod(server, r2_airgard_method_Id);
        addGetDataBlockVersionMethod(server, r2_airgard_method_Id);
        addGetInstrumentTimeMethod(server, r2_airgard_method_Id);
        addGetMeasurementTimeMethod(server, r2_airgard_method_Id);

        addGetBootStatusMethod(server, r2_airgard_method_Id);
        addGetSnapshotStatusMethod(server, r2_airgard_method_Id);
        addGetSCPStatusMethod(server, r2_airgard_method_Id);
        addGetSFTPStatusMethod(server, r2_airgard_method_Id);
        addGetRunScriptStatusMethod(server, r2_airgard_method_Id);
        addGetArchiveStatusMethod(server, r2_airgard_method_Id);
        addGetAncillarySensorStatusMethod(server, r2_airgard_method_Id);

        addGetSensorMethod(server, r2_airgard_method_Id);
        addGetOperatingTimeMethod(server, r2_airgard_method_Id);
        addGetWarningMessageMethod(server, r2_airgard_method_Id);

        addGetIgramPPMethod(server, r2_airgard_method_Id);
        addGetIgramDCMethod(server, r2_airgard_method_Id);
        addGetLaserPPMethod(server, r2_airgard_method_Id);
        addGetLaserDCMethod(server, r2_airgard_method_Id);
        addGetSingleBeamAt900Method(server, r2_airgard_method_Id);
        addGetSingleBeamAt2500Method(server, r2_airgard_method_Id);
        addGetSignalToNoiseAt2500Method(server, r2_airgard_method_Id);
        addGetCenterBurstLocationMethod(server, r2_airgard_method_Id);
        addGetDetectorTempMethod(server, r2_airgard_method_Id);
        addGetLaserFrequencyMethod(server, r2_airgard_method_Id);
        addGetHardDriveSpaceMethod(server, r2_airgard_method_Id);
        addGetFlowMethod(server, r2_airgard_method_Id);
        addGetTemperatureMethod(server, r2_airgard_method_Id);
        addGetPressureMethod(server, r2_airgard_method_Id);
        addGetTempOpticsMethod(server, r2_airgard_method_Id);
        addGetBadScanCounterMethod(server, r2_airgard_method_Id);
        addGetFreeMemorySpaceMethod(server, r2_airgard_method_Id);
        addGetLABFilenameMethod(server, r2_airgard_method_Id);
        addGetLOGFilenameMethod(server, r2_airgard_method_Id);
        addGetLgFilenameMethod(server, r2_airgard_method_Id);
        addGetSecondLgFilenameMethod(server, r2_airgard_method_Id);
        addGetSystemCounterMethod(server, r2_airgard_method_Id);
        addGetDetectorCounterMethod(server, r2_airgard_method_Id);
        addGetLaserCounterMethod(server, r2_airgard_method_Id);
        addGetFlowPumpCounterMethod(server, r2_airgard_method_Id);
        addGetDesiccantCounterMethod(server, r2_airgard_method_Id);

        addGetNoOfAlarmsMethod(server, r2_airgard_method_Id);
	addGetNoOfNonAlarmsMethod(server, r2_airgard_method_Id);
        addGetArrayOfAlarmMethod(server, r2_airgard_method_Id);
        addGetArrayOfNonAlarmMethod(server, r2_airgard_method_Id);
        //addGetNonAlarmListMethod(server, r2_airgard_method_Id);

	addClearAlarmLEDMethod(server, r2_airgard_method_Id);


	// methods that invokes Events; event id needs to be sent over the wire
	//addResetEventMethod(server, r2_airgard_method_Id);
	//addShutdownEventMethod(server, r2_airgard_method_Id);
}
