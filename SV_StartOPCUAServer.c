//#include "open62541.h"
#include <open62541/plugin/network.h>
#include <open62541/client_highlevel.h>
#include <open62541/plugin/log_stdout.h>
#include <open62541/client_config_default.h>
#include <open62541/plugin/create_certificate.h>
#include <open62541/plugin/securitypolicy.h>
#include <open62541/plugin/accesscontrol_default.h>
#include <open62541/plugin/nodestore_default.h>
#include <open62541/plugin/pki_default.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include "SV_NewMonitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <libwebsockets.h>

//#define DEBUG
#define UA_ENABLE_HISTORIZING
//#define UA_ENABLE_DISCOVERY
#define UA_ENABLE_DISCOVERY_MULTICAST

#define MANUFACTURER_NAME "Virtual Skies"
#define PRODUCT_URI "http://virtualskies.com.sg/Airgard/"
#define APPLICATION_NAME "OPCUA Server (Airgard) based on open62541"
#define APPLICATION_URI "urn:virtualskies.com.sg"		// maps to CName : virtualskies.com.sg during SSL cert generation
								// maps to openssl.cnf : subjectAltName = URI:virtualskies.com.sg, IP:192.168.1.115
// To solve the error in UAExpert: The applicationURi of the server certificate does not match the applicationDescription returned by the server, change the following:
//#define APPLICATION_URI_SERVER "urn:virtualskies.server.application" => was defined as DEFAULT_CONFIG
#define APPLICATION_URI_SERVER "urn:virtualskies.com.sg"

#define PRODUCT_NAME "Virtual Skies OPC UA Server"
#define STRINGIFY(arg) #arg
#define VERSION(MAJOR, MINOR, PATCH, LABEL) \
    STRINGIFY(MAJOR) "." STRINGIFY(MINOR) "." STRINGIFY(PATCH) LABEL

// comment out the following if you do not want to call SELFSIGNEDCERT routine
//#define TESTSELFSIGNEDCERT

#define PRIVATEKEYLOC "/usr/local/ssl/private/private-key.pem"
#define SSLCERTIFICATELOC "/etc/ssl/certs/cert115.pem"	// actual location => /usr/local/ssl/certs/cert115.pem "/etc/ssl/certs/62541ServerCert.pem"
//#define SSLCERTIFICATELOC "/etc/ssl/certs/62541ServerCert.pem"
#define TRUSTLISTLOC "/usr/local/ssl/trustlist/trustlist.crl"

// 192.168.1.33 acting as client to 192.168.1.44 (LDS)
// need to copy 192.168.1.44::62541LDSServerCert.pem to /usr/local/ssl/certs/.
// need to copy 192.168.1.44::62541LDSprivate-key.pem to /usr/local/ssl/private/.
#define DISCOVERY_SERVER_APPLICATION_URI "urn:lds.virtualskies.com.sg"
#define DISCOVERY_SERVER_ENDPOINT "opc.tcp://192.168.1.44:4840"
#define DISCOVERY_PRIVATEKEYLOC "/usr/local/ssl/private/ldsprivate-key.pem"
#define DISCOVERY_SSLCERTIFICATELOC "/etc/ssl/certs/ldscert44.pem"
#define DISCOVERY_TRUSTLISTLOC "/usr/local/ssl/trustlist/trustlist.crl"

#define GDS_DISCOVERY_SERVER_APPLICATION_URI "urn:gds.virtualskies.com.sg"
#define GDS_DISCOVERY_SERVER_ENDPPOINT "opc.tcp://192.168.1.88:4842"
#define GDS_DISCOVERY_PRIVATEKEYLOC "/usr/local/ssl/private/gdsprivate-key.pem"
#define GDS_DISCOVERY_SSL_CERTIFICATELOC "/etc/ssl/certs/gdscert88.pem"
#define GDS_DISCOVERY_TRUSTLISTLOC "/usr/local/ssl/trustlist/trustlist.crl"

#define PUBLISH_INTERVAL_WEBSOCKETS            9000

/* Struct initialization works across ANSI C/C99/C++ if it is done when the
 * variable is first declared. Assigning values to existing structs is
 * heterogeneous across the three. */
static UA_INLINE UA_UInt32Range
UA_UINT32RANGE(UA_UInt32 min, UA_UInt32 max) {
    UA_UInt32Range range = {min, max};
    return range;
}

static UA_INLINE UA_DurationRange
UA_DURATIONRANGE(UA_Duration min, UA_Duration max) {
    UA_DurationRange range = {min, max};
    return range;
}

void RegisterOPCServerToLDS(UA_Server *);

//void* StartOPCUAServer(void* x_void_ptr, char *OPCipaddress, char *brokeripadress);
void* StartOPCUAServer(void* x_void_ptr, char* argv[]);
UA_NodeId* CreateOPCUANodes(UA_Server *);
void CreateServerMethodItems(UA_Server *, UA_NodeId);
void CreateServerHistorizingItems(UA_Server *);
//void CreateServerPubSub(UA_Server *, char* , UA_Int16);
void CreateServerPubSub(UA_Server *, char *, int, char *);
//void CreateServerWebSockets(UA_Server *, char *, UA_Int16);

void CreateServerEvents(UA_Server *, UA_NodeId);
void CreateServerAlarmsAndConditions(UA_Server *);

// forward declaration
void GetHistoryDBConnection(void);
void CloseHistoryDBConnection(void);

UA_ByteString loadFile(const char *const path);
static UA_Boolean allowAddNode(UA_Server *server, UA_AccessControl *ac,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_AddNodesItem *item);
static UA_Boolean allowDeleteNode(UA_Server *server, UA_AccessControl *ac,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_DeleteNodesItem *item);
static UA_Boolean allowBrowseNode(UA_Server *server, UA_AccessControl *ac,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_NodeId *nodeId, void *nodeContext);

#ifdef UA_ENABLE_HISTORIZING
static UA_Boolean allowHistoryUpdateUpdateData(UA_Server *server, UA_AccessControl *ac,
		const UA_NodeId *sessionId, void *sessionContext,
		const UA_NodeId *nodeId,
		UA_PerformUpdateType performInsertReplace,
		const UA_DataValue *value);

static UA_Boolean allowHistoryUpdateDeleteRawModified(UA_Server *server, UA_AccessControl *ac,
		const UA_NodeId *sessionId, void *sessionContext,
		const UA_NodeId *nodeId,
		UA_DateTime startTimestamp,
		UA_DateTime endTimestamp,
		bool isDeleteModified);
#endif

//#ifdef UA_ENABLE_WEBSOCKET_SERVER
// the following function is defined in open62541.c
// forward declaration
static int callback_opcua(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in,size_t len);
//#endif

static volatile UA_Boolean running = UA_TRUE;
extern int g_argc;

#ifdef UA_ENABLE_WEBSOCKET_SERVER
// the following function is defined in open62541.c
//static int callback_opcua(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in,size_t len)
//{
//	return ActivateWebSocketStreams();
//}
#endif

// sample found in /open62541/examples/common.h
// parses the certificate file - used in StartOPCUAServer.c
UA_ByteString loadFile(const char *const path)
{
    UA_ByteString fileContents = UA_STRING_NULL;

    /* Open the file */
    FILE *fp = fopen(path, "rb");
    if(!fp) {
        errno = 0; /* We read errno also from the tcp layer... */
        return fileContents;
    }

    /* Get the file length, allocate the data and read */
    fseek(fp, 0, SEEK_END);
    fileContents.length = (size_t)ftell(fp);
    fileContents.data = (UA_Byte *)UA_malloc(fileContents.length * sizeof(UA_Byte));
    if(fileContents.data) {
        fseek(fp, 0, SEEK_SET);
        size_t read = fread(fileContents.data, sizeof(UA_Byte), fileContents.length, fp);
        if(read != fileContents.length)
            UA_ByteString_clear(&fileContents);
    } else {
        fileContents.length = 0;
    }
    fclose(fp);

    return fileContents;
}


static void stopHandler(int sig)
{
	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
	running = UA_FALSE;
}

static UA_Boolean
allowAddNode(UA_Server *server, UA_AccessControl *ac,
		const UA_NodeId *sessionId, void *sessionContext,
		const UA_AddNodesItem *item)
{
	//printf("Called allowAddNode \n");
	return UA_TRUE;
}

static UA_Boolean
allowDeleteNode(UA_Server *server, UA_AccessControl *ac,
		const UA_NodeId *sessionId, void *sessionContext,
		const UA_DeleteNodesItem *item)
{
	//printf("Called allowDeleteNode \n");
	return UA_TRUE;
}

static UA_Boolean
allowBrowseNode(UA_Server *server, UA_AccessControl *ac,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_NodeId *nodeId, void *nodeContext)
{
        //printf("Called allowBrowseNode \n");
        return UA_TRUE;
}

static UA_Boolean allowHistoryUpdateUpdateData(UA_Server *server, UA_AccessControl *ac,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_NodeId *nodeId,
                UA_PerformUpdateType performInsertReplace,
                const UA_DataValue *value)
{
	//printf("Called allowHistoryUpdateUpdateData \n");
	return UA_TRUE;
}

static UA_Boolean allowHistoryUpdateDeleteRawModified(UA_Server *server, UA_AccessControl *ac,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_NodeId *nodeId,
                UA_DateTime startTimestamp,
                UA_DateTime endTimestamp,
                bool isDeleteModified)
{
        //printf("Called allowHistoryUpdateDeleteRawModified \n");
	return UA_TRUE;
}

// global variable //
static const size_t usernamePasswordsSize = 2;
static UA_UsernamePasswordLogin logins[2] = {
        {UA_STRING_STATIC("jackybek"), UA_STRING_STATIC("thisisatestpassword24")},
	{UA_STRING_STATIC("admin"),UA_STRING_STATIC("defaultadminpassword24")}
};

void RegisterOPCServerToLDS(UA_Server *uaServer1)
{
	UA_StatusCode retval;

		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "==========================================================");
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_StartOPCUAServer : Just before LDS registration process");

		#ifdef UA_ENABLE_DISCOVERY
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_StartOPCUAServer : Begin LDS registration process");

		//-------------- register itself to the LDS Server <192.168.1.44>
		// code based on githug/open62541/62541/examples/discovery/server_register.c
		// acting as an OPCUA Client to LDS Server
		UA_ByteString LDScertificate = loadFile(DISCOVERY_SSLCERTIFICATELOC);  //loadFile("/etc/ssl/certs/62541LDSServerCert.pem"); //=> symbolic link
	        //UA_ByteString certificate = loadFile("/usr/local/ssl/certs/ldscert44.pem"); // actual location
		if (LDScertificate.length == 0)
	        {
	                UA_LOG_FATAL(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_StartOPCUAServer (LDS): Unable to load file : %s", DISCOVERY_SSLCERTIFICATELOC);
		        //goto cleanup;
			return;
	        }

        	UA_ByteString LDSprivateKey = loadFile(DISCOVERY_PRIVATEKEYLOC);  //loadFile("/usr/local/ssl/private/62541LDSprivate-key.pem");
	        //UA_ByteString LDSprivateKey = loadFile("/usr/local/ssl/private/ldsprivate-key.pem");
	        if (LDSprivateKey.length == 0)
	        {
	                UA_LOG_FATAL(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_StartOPCUAServer (LDS): Unable to load file : %s", DISCOVERY_PRIVATEKEYLOC);
	                //goto cleanup;
			return;
	        }
	        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_StartOPCUAServer (LDS): successfully loaded LDS certificate and privateKey");

	        // load the trustlist
	        size_t LDStrustListSize = 1;
		UA_ByteString *LDStrustList = (UA_ByteString *) UA_Array_new(1, &UA_TYPES[UA_TYPES_BYTESTRING]);
		UA_ByteString_copy(&LDScertificate,&LDStrustList[0]);
	        //UA_STACKARRAY(UA_ByteString, LDStrustList, LDStrustListSize);
	        //for(size_t LDStrustListCount=0; LDStrustListCount < LDStrustListSize; LDStrustListCount++)
	        //        LDStrustList[LDStrustListCount] = loadFile(DISCOVERY_TRUSTLISTLOC);  //loadFile("/usr/local/ssl/trustlist/trustlist.crl");
	        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_StartOPCUAServer (LDS) : successfully loaded LDS trustList");

	        /* Loading of a revocation list currently unsupported */
	        UA_ByteString *LDSrevocationList = NULL;
	        size_t LDSrevocationListSize = 0;

		UA_Client *LDSclient = UA_Client_new();
		UA_ClientConfig *LDSClient_config1 = UA_Client_getConfig(LDSclient);
		//UA_ClientConfig_setDefault(UA_Client_getConfig(LDSclient));
		//UA_ClientConfig_setDefault(LDSClient_config1);
		LDSClient_config1->localConnectionConfig = UA_ConnectionConfig_default;
	        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_StartOPCUAClient (LDS) : here 1");
		#ifdef __linux__
		UA_ClientConfig_setDefaultEncryption(LDSClient_config1, LDScertificate, LDSprivateKey,
							NULL,0,
							NULL,0);
		#else
		UA_ClientConfig_setDefaultEncryption(LDSClient_config1, LDScertificate, LDSprivateKey,
	                                                        LDStrustList, LDStrustListSize,
	                                                        LDSrevocationList, LDSrevocationListSize);
		#endif
	        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_StartOPCUAServer (LDS) : Finished calling setDefaultEncryption");

		UA_ApplicationDescription_clear(&LDSClient_config1->clientDescription);
		UA_String_clear(&LDSClient_config1->clientDescription.applicationUri);
		LDSClient_config1->clientDescription.applicationUri.length = strlen(DISCOVERY_SERVER_APPLICATION_URI);
		LDSClient_config1->clientDescription.applicationUri = UA_String_fromChars(DISCOVERY_SERVER_APPLICATION_URI); //UA_STRING_ALLOC(DISCOVERY_SERVER_APPLICATION_URI);

		// Secure client connect
		LDSClient_config1->securityMode = UA_MESSAGESECURITYMODE_SIGNANDENCRYPT; //UA_MESSAGESECURITYMODE_SIGNANDENCRYPT;  // require encryption
		LDSClient_config1->securityPolicyUri = UA_STRING_ALLOC("");
	        LDSClient_config1->secureChannelLifeTime = 10 * 60 * 1000;        // 10 minutes
	        LDSClient_config1->securityPolicies = (UA_SecurityPolicy*)UA_malloc(sizeof(UA_SecurityPolicy));
	        if (!LDSClient_config1->securityPolicies)
		{
	               	UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_StartOPCUAServer.c (LDS) Error setting securityPolicies : %s", UA_StatusCode_name(UA_STATUSCODE_BADOUTOFMEMORY));
			//goto cleanup;
			return;
		}
		// no such member in v1.3.4
		//LDSClient_config1->initConnectionFunc = UA_ClientConnectionTCP_init; /* for async client */
		//LDSClient_config1->pollConnectionFunc = UA_ClientConnectionTCP_poll; /* for async connection */
		LDSClient_config1->customDataTypes = NULL;
		LDSClient_config1->connectivityCheckInterval = 0;
                LDSClient_config1->requestedSessionTimeout = 1200000; /* requestedSessionTimeout */
                LDSClient_config1->inactivityCallback = NULL;
                LDSClient_config1->clientContext = NULL;
                LDSClient_config1->connectivityCheckInterval = 3000; // in milliseconds

	        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_StartOPCUAServer (LDS) : Calling UA_CertificateVerification_CertFolders .. ");
		#ifdef __linux__
printf("In __linux \n");
		const char *trustlistFolder = NULL;
		const char *issuerlistFolder = NULL;
                const char *revocationlistFolder = NULL;

		LDSClient_config1->certificateVerification.clear(&LDSClient_config1->certificateVerification);
			#ifdef UA_ENABLE_CERT_REJECTED_DIR
				retval = UA_CertificateVerification_CertFolders(&LDSClient_config1->certificateVerification,
                                                                        trustlistFolder, issuerlistFolder,
                                                                        revocationlistFolder, NULL);
			#else
				retval = UA_CertificateVerification_CertFolders(&LDSClient_config1->certificateVerification,
                                                                        trustlistFolder, issuerlistFolder,
                                                                        revocationlistFolder);
			#endif
		#else
		UA_ByteString_clear(&LDScertificate);
		UA_ByteString_clear(&LDSprivateKey);
		for (size_t deleteCounter=0; deleteCounter < LDStrustListSize; deleteCounter++)
			UA_ByteString_clear(&LDStrustList[deleteCounter]);
		#endif

		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_StartOPCUAServer.c (LDS) : successfully setDefaultEncryption as LDS Client");

		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_StartOPCUAServer.c (LDS): Just about to call UA_Client_connectUsername() at line 564");
	        retval = UA_Client_connectUsername(LDSclient, DISCOVERY_SERVER_ENDPOINT, "admin", "defaultadminpassword24");
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_StartOPCUAServer.c (LDS): Finished calling UA_Client_connectUsername() at line 564");
	        if (retval != UA_STATUSCODE_GOOD)
	        {
	                UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_StartOPCUAServer.c (LDS) : Cannot login securely to OPCUA LDS Server : %s", DISCOVERY_SERVER_ENDPOINT);
			UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_StartOPCUAServer.c (LDS): error is %s", UA_StatusCode_name(retval));
			//goto cleanup;
			return;
	        }
	        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_StartOPCUAServer.c (LDS) : UA_LDS_connectUsername() : success");

		UA_UInt64 callbackId;
		// server registration starts after 0 (5=5 minutes); initial delay for 500ms
		UA_StatusCode LDS_retval = UA_Server_addPeriodicServerRegisterCallback(uaServer1,
									LDSclient,
									DISCOVERY_SERVER_ENDPOINT,		// opc.tcp://192.168.1.44:4840
									0 * 60 * 1000,				// delay after X milliseconds
									500,					// delay first register for 500 ms
									&callbackId);
		// the following check in UA_Server_addPeriodicServerRegisterCallback() failed
		//(client->connection.state != UA_CONNECTIONSTATE_CLOSED)

		if (LDS_retval != UA_STATUSCODE_GOOD)
		{
			UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER,
				"SV_StartOPCUAServer.c (LDS): registering to remote LDS server  : Fail at UA_Server_addPeriodicServerRegisterCallback. StatusCode %s",
				UA_StatusCode_name(LDS_retval));
			UA_Client_disconnect(LDSclient);
			UA_Client_delete(LDSclient);
			//goto cleanup;
			return;
		}
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_StartOPCUAServer (LDS) : End LDS registration process");
	    	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_StartPOCUAServer (LDS) : registered to LDS Server <192.168.1.44:4840>");
		#endif	// UA_ENABLE_DISCOVERY
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "=============================================================");

}


//================================================================
static UA_StatusCode
createEndpoint(UA_ServerConfig *conf, UA_EndpointDescription *endpoint,
               const UA_SecurityPolicy *securityPolicy,
               UA_MessageSecurityMode securityMode)
{
    UA_EndpointDescription_init(endpoint);

    endpoint->securityMode = securityMode;
    UA_String_copy(&securityPolicy->policyUri, &endpoint->securityPolicyUri);
    endpoint->transportProfileUri =
        UA_STRING_ALLOC("http://opcfoundation.org/UA-Profile/Transport/uatcp-uasc-uabinary");

    /* Add security level value for the corresponding message security mode */
    endpoint->securityLevel = (UA_Byte) securityMode;
    UA_String_copy(&securityPolicy->policyUri, &endpoint->securityPolicyUri);
    endpoint->transportProfileUri =
        UA_STRING_ALLOC("http://opcfoundation.org/UA-Profile/Transport/uatcp-uasc-uabinary");

    /* Add security level value for the corresponding message security mode */
    endpoint->securityLevel = (UA_Byte) securityMode;

    /* Enable all login mechanisms from the access control plugin  */
    UA_StatusCode retval = UA_Array_copy(conf->accessControl.userTokenPolicies,
                                         conf->accessControl.userTokenPoliciesSize,
                                         (void **)&endpoint->userIdentityTokens,
                                         &UA_TYPES[UA_TYPES_USERTOKENPOLICY]);

    	if(retval != UA_STATUSCODE_GOOD)
	{
        	UA_String_clear(&endpoint->securityPolicyUri);
        	UA_String_clear(&endpoint->transportProfileUri);
        	return retval;
    	}
    	endpoint->userIdentityTokensSize = conf->accessControl.userTokenPoliciesSize;

    	UA_String_copy(&securityPolicy->localCertificate, &endpoint->serverCertificate);
    	UA_ApplicationDescription_copy(&conf->applicationDescription, &endpoint->server);

    	return UA_STATUSCODE_GOOD;
}

/*
void CreateServerWebSockets(UA_Server* uaServer1, char* nginxProxy, UA_Int16 nsIdx_MKS)
{


}
*/

int GenerateSelfSignedSSLCert(UA_ByteString *privateKey, UA_ByteString *certificate)
{
			// generate self-signed SSL certs when both certificate and privatekey cannot be found
                        UA_String subject[5] = {UA_STRING_STATIC("C=SG"),
                                                UA_STRING_STATIC("O=M/S Virtual Skies"),
                                                UA_STRING_STATIC("OU=IT"),
						//UA_STRING_STATIC("LO=SG"),
						UA_STRING_STATIC("ST=Singapore"),
                                                UA_STRING_STATIC("CN=virtualskies.com.sg")};
                        UA_UInt32 lenSubject = 5;
                        char myString[255];
                        strcpy(myString, "URI:");
                        strcat(myString, APPLICATION_URI);
                        UA_String subjectAltName[3] = {
						UA_STRING_STATIC("DNS:192.168.1.115"),//UA_STRING_STATIC("DNS:localhost"),
						UA_STRING_STATIC("IP:192.168.1.115"),
                                                UA_STRING_STATIC(myString)
						};
                        UA_UInt32 lenSubjectAltName = 3;
                        UA_StatusCode statusCertGen =  UA_CreateCertificate(UA_Log_Stdout,
                                                subject, lenSubject,
                                                subjectAltName, lenSubjectAltName,
                                                4096, UA_CERTIFICATEFORMAT_PEM,
                                                privateKey, certificate);

                        if (statusCertGen != UA_STATUSCODE_GOOD)
                                UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                                                "StartOPCUAServer.c : Generating self signed certificate failed : %s",
                                                UA_StatusCode_name(statusCertGen) );

			else
				UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
						"StartOPCUAServer.c : Generating certificate and privateKey success");

printf("certificate generated is %s \n", certificate->data);
printf("privateKey generated is %s \n", privateKey->data);
//exit(0);
                        return statusCertGen;
}

//================================================================
//void* StartOPCUAServer(void* x_void_ptr, char* OPCipaddress, char* brokeripaddress)
void* StartOPCUAServer(void* x_void_ptr, char* argv[])
{

	//---------------------------------------------------------------------------------
	// refer to ~/open62541/plugins/ua_config_default.c on all the default settings for
	// 1. UA server
	// 2. UA client
	// --------------------------------------------------------------------------------

	int sockfd;
	char* OPCipaddress = argv[1];
	char* brokeripaddress = argv[3];

	signal(SIGINT, stopHandler);
	signal(SIGTERM, stopHandler);

	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,"In StartOPCUAServer thread: g_argc=%d\n", g_argc);

	//UA_Server* uaServer = (UA_Server*)x_void_ptr; -- caused it to crash when UA_newWithConfig(config);
	//UA_ServerConfig *config = UA_Server_getConfig(uaServer);

	UA_Server *uaServer1 = UA_Server_new(); //NULL;
	UA_ServerConfig *config1 = UA_Server_getConfig(uaServer1);
	//memset(&config1, 0, sizeof(UA_ServerConfig));

	if (g_argc==3 || g_argc==5 || g_argc==6)	// myNewServer 192.168.1.33 192.168.1.88 [192.168.1.11] [1883] pub
	{
		/*
			  #ifdef UA_ENABLE_PUBSUB
				// Details about the connection configuration and handling are located in
					//  the pubsub connection tutorial
				printf("\t----------------------Before setupUadpRange----------------- \n");
				//   setupUadpRange(server);
			  #endif
		*/
		//hostname or ip address available
			//copy the hostname from char * to an open62541 variable
		//printf("in g_argc segment %s %d\n", g_argv_ip, g_argv_port);

		//char* OPCUAServerIP = ipaddress;
		//UA_String hostname;
		//UA_String_init(&hostname);
		//hostname.length = strlen(g_argv[1]);
		//hostname.data = (UA_Byte *) g_argv[1];
		//hostname.length = strlen(g_argv_ip);
		//hostname.data = (UA_Byte *) g_argv_ip; // this should be the ipaddress of the OPCUA Server : 192.168.2.33

		//hostname.data = (UA_Byte*)OPCUAServerIP;

		//Change the configuration - deprecated in v1.1.3
		//UA_ServerConfig_setCustomHostname(config, hostname);
		//printf("hostname.data (ip) = %s\n", hostname.data);

		//#endif

                // start OPCUA Server
                UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "starting server...");

		UA_StatusCode retval;
        	UA_ByteString certificate = UA_BYTESTRING_NULL;
		certificate.length = 0;
        	UA_ByteString privateKey = UA_BYTESTRING_NULL;
		privateKey.length = 0;

                //----------------------------------------
                // configure port 4840 as the port for external communication - support encryption and userid/ password
                // ---------------------------------------
		// encryption routine
    		/* Load certificate and private key */
		#ifndef TESTSELFSIGNEDCERT	// i.e. CA cert generated using openssl
		// to test this segment, cannot have this statement: #define TESTSELFSIGNEDCERT
		certificate = loadFile(SSLCERTIFICATELOC);
	    	privateKey = loadFile(PRIVATEKEYLOC);
		//UA_ByteString certificate = loadFile("/etc/ssl/certs/62541ServerCert.pem");	// => symbolic link
   		//UA_ByteString certificate = loadFile("/usr/local/ssl/certs/62541ServerCert.pem");  // actual location
                printf("length of certificate is %d \n", certificate.length);
                printf("contents of certificate is %s \n", certificate.data);
		#endif

		if (certificate.length == 0 || privateKey.length == 0)
		{
			UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
					"StartOPCUAServer.c : cannot find SSL Certificate %s", SSLCERTIFICATELOC); // /usr/local/ssl/certs/62541ServerCert.pem");
			retval = GenerateSelfSignedSSLCert(&privateKey, &certificate);
                        if (retval != UA_STATUSCODE_GOOD)
                        {
                                UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER,"StartOPCUAServer.c : Generate Self Signed SSL Cert failed");
                                goto cleanup;
                        }
		}
		else if (privateKey.length == 0)
		{
    			//UA_ByteString privateKey = loadFile("/usr/local/ssl/private/private-key.pem");
			//printf("contents of privatekey is %d %s \n", privateKey.length, privateKey.data);
			//sleep(5);
			UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
					"StartOPCUAServer.c : cannot find private key %s", PRIVATEKEYLOC);  // /usr/local/ssl/private/private-key.pem");
			retval = GenerateSelfSignedSSLCert(&privateKey, &certificate);
                        if (retval != UA_STATUSCODE_GOOD)
                        {
                                UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER,"StartOPCUAServer.c : Generate Self Signed SSL Cert failed");
                                goto cleanup;
                        }
		}

    		/* Load the trustlist */
    		size_t trustListSize = 1;
    		//UA_STACKARRAY(UA_ByteString, trustList, trustListSize);
    		//for(size_t i = 0; i < trustListSize; i++)
        	//	trustList[i] = loadFile(TRUSTLISTLOC); //"/usr/local/ssl/trustlist/trustlist.crl");
                UA_ByteString *trustList = (UA_ByteString *) UA_Array_new(1, &UA_TYPES[UA_TYPES_BYTESTRING]);
                UA_ByteString_copy(&certificate,&trustList[0]);


    		/* Loading of a issuer list, not used in this application */
    		size_t issuerListSize = 0;
    		UA_ByteString *issuerList = NULL;

    		/* Loading of a revocation list currently unsupported */
    		UA_ByteString *revocationList = NULL;
    		size_t revocationListSize = 0;

		#ifdef __linux__
    		const char *trustlistFolder = NULL;
    		const char *issuerlistFolder = NULL;
    		const char *revocationlistFolder = NULL;

    		retval = UA_ServerConfig_setDefaultWithSecurityPolicies(config1, 4840,			// 26Feb2022 swap &config with config1
                                                       &certificate, &privateKey,
                                                       NULL, 0,
                                                       NULL, 0,
                                                       NULL, 0);
		if (retval != UA_STATUSCODE_GOOD)
		{
			UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,"StartOPCUAServer.c : error loading Server Configuration");
			goto cleanup;
		}
		config1->certificateVerification.clear(&config1->certificateVerification);
			#ifdef UA_ENABLE_CERT_REJECTED_DIR
			retval = UA_CertificateVerification_CertFolders(&config1->certificateVerification,
									trustlistFolder, issuerlistFolder,
									revocationlistFolder, NULL);
			#else
			retval = UA_CertificateVerification_CertFolders(&config1->certificateVerification,
									trustlistFolder, issuerlistFolder,
									revocationlistFolder);
			#endif
		if (retval != UA_STATUSCODE_GOOD)
		{
			UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,"StartOPCUAServer.c : error verifying Certificate folders");
                        goto cleanup;
		}
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,"StartOPCUAServer.c : success loading Server Config and verifying Certificate folders");

		#else
		retval = UA_ServerConfig_setDefaultWithSecurityPolicies(config1, 4840,                  // 26Feb2022 swap &config with config1
                                                       &certificate, &privateKey,
                                                       trustList, trustListSize,
                                                       issuerList, issuerListSize,
                                                       revocationList, revocationListSize);
                if (retval != UA_STATUSCODE_GOOD)
                {
                        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,"StartOPCUAServer.c : error loading Server Configuration");
                        goto cleanup;
                }
		#endif

                // refer to open62541.org->Server->Server Configuration & plugins/ua_config_default for the list of members in the UA_ServerConfig structure
		if (!config1)	//26Feb2022 !&config1
		{
			//return UA_STATUSCODE_BADINVALIDARGUMENT;
			UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,"StartOPCUAServer.c : error loading Server Configuration");
			goto cleanup;
		}
		if (config1->nodestore.context == NULL)
			UA_Nodestore_HashMap(&config1->nodestore);	//26Feb2022 &config1.nodestore

		//if (!config1.logger.log)
		//	config1.logger = UA_Log_Stdout;

                // Change the configuration
                char* OPCUAServerIP = OPCipaddress;   // 192.168.2.33
                UA_String hostname;
                UA_String_init(&hostname);
                //hostname.length = strlen(g_argv[1]);
                //hostname.data = (UA_Byte *) g_argv[1];
                hostname.length = strlen(OPCUAServerIP);
                hostname.data = (UA_Byte*)OPCUAServerIP;
                //UA_ServerConfig_setCustomHostname(&config1, hostname);	// deprecated
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,"StartOPCUAServer.c : hostname.data (ip) = %s", hostname.data);

		// Server Description
		//UA_BuildInfo_clear(&config1.buildInfo);
		config1->buildInfo.productUri = UA_STRING_ALLOC(PRODUCT_URI);
		config1->buildInfo.manufacturerName = UA_STRING_ALLOC(MANUFACTURER_NAME);
		config1->buildInfo.productName = UA_STRING_ALLOC(PRODUCT_NAME);
		config1->buildInfo.softwareVersion =
			UA_STRING_ALLOC(VERSION(UA_OPEN62541_VER_MAJOR, UA_OPEN62541_VER_MINOR,
						UA_OPEN62541_VER_PATCH, UA_OPEN62541_VER_LABEL));

		config1->buildInfo.buildDate = UA_DateTime_now();
		config1->buildInfo.buildNumber = UA_STRING_ALLOC(__DATE__ " " __TIME__);

		UA_ApplicationDescription_clear(&config1->applicationDescription);

		// somehow the following cannot overwrite the default config settings in open62541.c
		// have to manually update ::  #define APPLICATION_URI_SERVER "urn:virtualskies.com.sg"
		UA_String_clear(&config1->applicationDescription.applicationUri);
		config1->applicationDescription.applicationUri.length = strlen(APPLICATION_URI_SERVER);
    		config1->applicationDescription.applicationUri = UA_String_fromChars(APPLICATION_URI_SERVER); //UA_STRING_ALLOC(APPLICATION_URI_SERVER);
		config1->applicationDescription.productUri = UA_STRING_ALLOC(PRODUCT_URI);
		config1->applicationDescription.applicationName = UA_LOCALIZEDTEXT_ALLOC("en-US", APPLICATION_NAME);

 		config1->serverCertificate = certificate;
                config1->shutdownDelay = 0; //5000.0; // millisecond

		// Rule Handling
		// all attributes created in the OPCUA Address space must be time-stamped
		config1->verifyRequestTimestamp = UA_RULEHANDLING_WARN;       // required by Matrikon OPCUA Client
		config1->allowEmptyVariables = UA_RULEHANDLING_DEFAULT;


		// Custom DataTypes TODO
		/* config1.customDataTypesSize = 0; */
    		/* config1.customDataTypes = NULL; */

		// Networking TODO
	    	/* config1.networkLayersSize = 0; */
	    	/* config1.networkLayers = NULL; */
    		/* config1.customHostname = UA_STRING_NULL; */

		// PubSub => parameters are set in SV_PubSub.c : CreateServerPubSub()
		//config1.pubSubConfig.securityPolicies = (UA_PubSubSecurityPolicy *) UA_malloc(sizeof(UA_PubSubSecurityPolicy));
		//config1.pubSubConfig.securityPoliciesSize = 1;
		//UA_PubSubSecurityPolicy_Aes128Ctr(config1.pubSubConfig.securityPolicies, &config1.logger);
		//config1.pubSubConfig.securityPolicies[0].policyUri = UA_STRING("http://opcfoundation.org/UA/SecurityPolicy#Aes128_Sha256_RsaOaep");

		// Available opcua endpoints (to read)
		//config1.endpointsSize = 0;
		//config1.endpoints = {0, NULL}

		config1->securityPolicyNoneDiscoveryOnly = UA_FALSE;

		// Node Lifecycle callbacks TODO
    		/* config1.nodeLifecycle.constructor = NULL; */
    		/* config1.nodeLifecycle.destructor = NULL; */
    		/* config1.nodeLifecycle.createOptionalChild = NULL; */
    		/* config1.nodeLifecycle.generateChildNodeId = NULL; */
		config1->modellingRulesOnInstances = UA_TRUE;
 
		// Access Control
		// --add userid and password routine
  		// --to disable anonymous logins (2nd parameter (allowAnonymous) set to UA_FALSE)
		// -- enable 2 user/password logins
		config1->accessControl.clear(&config1->accessControl);

		// v1.2.2
		//retval = UA_AccessControl_default(&config1, UA_FALSE, &config1.securityPolicies[config1.securityPoliciesSize-1].policyUri, 2, logins);
		// new parameter added in v1.3 - *verifyX509 = NULL)
		retval = UA_AccessControl_default(config1, UA_FALSE, NULL, &config1->securityPolicies[config1->securityPoliciesSize-1].policyUri, 2, logins);
		// 26feb2022 : retval = UA_AccessControl_default(&config1, UA_FALSE, NULL, &config1.securityPolicies[config1.securityPoliciesSize-1].policyUri, 2, logins);
		if (retval != UA_STATUSCODE_GOOD)
			goto cleanup;

		// set accessControl functions for nodeManagement
		config1->accessControl.allowAddNode = allowAddNode;
		config1->accessControl.allowDeleteNode = allowDeleteNode;
		config1->accessControl.allowBrowseNode = allowBrowseNode;
		config1->accessControl.allowHistoryUpdateUpdateData = allowHistoryUpdateUpdateData;
		config1->accessControl.allowHistoryUpdateDeleteRawModified = allowHistoryUpdateDeleteRawModified;

		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_StartOPCUAServer.c : adding 2 user credentials to OPCUA server (without anonymous)...");

		// Async Operations TODO

		// Nodestore TODO

		// Certificate Verification
		// that accepts every certificate. Can be overwritten when the policy is specialized.
		UA_CertificateVerification_AcceptAll(&config1->certificateVerification);

		// Limits for SecureChannels
		config1->maxSecureChannels = 40;
		config1->maxSecurityTokenLifetime = 10 * 60 * 1000; // 10 minutes */

		// Limits for Sessions
		config1->maxSessions = 100;
		config1->maxSessionTimeout = 60 * 60 * 1000;	// 1 hour

		// Operation Limits
		/* kiv
		config1.maxNodesPerRead = 500;
		config1.maxNodesPerWrite = 500;
		config1.maxNodesPerMethodCall = 500;
		config1.maxNodesPerBrowse = 500;
		config1.maxNodesPerRegisterNodes = 500;
		config1.maxNodesPerTranslateBrowsePathsToNodeIds = 500;
		config1.maxNodesPerNodeManagement = 500;
		config1.maxMonitoredItemsPerCall = 500;
		*/

		// limits for Requests
		/* kiv
		config1.maxReferencesPerNode = 100;
		*/

		// Discovery

		// LDS ++ refer to https://opcfoundation.org/UA/schemas/1.04/ServerCapabilities.csv
		// NA, DA, HD, AC, HE, GDS, LDS, DI, ADI, FDI, FDIC, PLC, S95, RCP, PUB, AUTOID, MDIS, CNC, PLK, FDT, TMC, CSPP, 61850, PACKML, MTC
		// AUTOML, SERCOS, MIMOSA, WITSML, DEXPI, IOLINK, VROBOT, PNO
	// the following is only applicable if it is a DISCOVERY SERVER
	//config1.applicationDescription.applicationType = UA_APPLICATIONTYPE_DISCOVERYSERVER;	// acts as DISCOVERY SERVER ONLY
		config1->applicationDescription.applicationType = UA_APPLICATIONTYPE_SERVER;

		config1->mdnsEnabled = UA_TRUE;
		config1->mdnsConfig.mdnsServerName = UA_String_fromChars("Local Discovery Server (multi-cast) ");
		// set the capabilities
		config1->mdnsConfig.serverCapabilitiesSize = 6;
		UA_String *caps = (UA_String *)UA_Array_new(6, &UA_TYPES[UA_TYPES_STRING]);	// should be N but not sure why 1 does not crash the program
		caps[0] = UA_String_fromChars("DA");	// Provides current data
		caps[1] = UA_String_fromChars("HD");	// Provides historical data
		caps[2] = UA_String_fromChars("GDS");	// Supports global discovery Server information model
		//caps[3] = UA_String_fromChars("LDS");	// Only supports the Discovery service, cannot be used in combination with any other capability	==> use 192.168.1.44 as the LDS
		caps[3] = UA_String_fromChars("AC");	// Provides alarms and conditions that may require operator interaction
		caps[4] = UA_String_fromChars("HE");	// Provides historical alarms and events
		caps[5] = UA_String_fromChars("PUB");	// Supports the Publisher capabilities defined in Part 14
		config1->mdnsConfig.serverCapabilities = caps;
		// explictly set the interface
		config1->mdnsInterfaceIP = UA_String_fromChars("0.0.0.0");	// Multicast DNS: failed setting IP_MULTICAST_IF to 224.0.0.1: Cannot assign requested address

		config1->discoveryCleanupTimeout = 60*60; // 60 minutes elapsed before removing server

	    	/* conf->applicationDescription.gatewayServerUri = UA_STRING_NULL; */
    		/* conf->applicationDescription.discoveryProfileUri = UA_STRING_NULL; */
    		/* conf->applicationDescription.discoveryUrlsSize = 0; */
    		/* conf->applicationDescription.discoveryUrls = NULL; */

		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "StartOPCUAServer.c : after making changes to config1");

		/*
		// Limits for MonitoredItems
		config1.samplingIntervalLimits = UA_DURATIONRANGE(50.0, 24.0 * 3600.0 * 1000.0);	// see below - MonitoredItems
		config1.queueSizeLimits = UA_UINT32RANGE(1, 100);	// see below - MonitoredItems
 		// Limits for Discovery
		config1.discoveryCleanupTimeout = 60 * 60;		// see above
		*/

		UA_ByteString_clear(&certificate);
    		UA_ByteString_clear(&privateKey);
    		for(size_t i = 0; i < trustListSize; i++)
        		UA_ByteString_clear(&trustList[i]);
	// end encryption routine

	// Subscriptions
		config1->maxSubscriptions = 100;
		config1->maxSubscriptionsPerSession = 100;
		config1->publishingIntervalLimits = UA_DURATIONRANGE(100.0, 3600.0 * 1000.0);
			//config1.publishingIntervalLimits.min = 100.0;		// ms, must not be less than 5
			//config1.publishingIntervalLimits.max = 3600.0 * 1000.0;
		config1->lifeTimeCountLimits = UA_UINT32RANGE(3, 15000);
			//config1.lifeTimeCountLimits.min = 3;
			//config1.lifeTimeCountLimits.max = 15000;
		config1->keepAliveCountLimits = UA_UINT32RANGE(1, 100);
			//config1.keepAliveCountLimits.min = 1;
			//config1.keepAliveCountLimits.max = 100;
		config1->maxNotificationsPerPublish = 1000;
		config1->enableRetransmissionQueue = UA_TRUE;
		config1->maxRetransmissionQueueSize = 0;		// 0 -> unlimited size

	// MonitoredItems
		config1->maxMonitoredItems = 100;
		config1->maxMonitoredItemsPerSubscription = 100;
		config1->samplingIntervalLimits = UA_DURATIONRANGE(50.0, 24.0 * 3600.0 * 1000.0);
			//config1.samplingIntervalLimits.min = 50.0;		// ms, must not be less than 5
			//config1.samplingIntervalLimits.max = 24.0 * 3600.0 * 1000.0;		// ms, must not be less than 5
		config1->queueSizeLimits = UA_UINT32RANGE(1, 100);
			//config1.queueSizeLimits.min = 1;
			//config1.queueSizeLimits.max = 100;
		config1->maxPublishReqPerSession = 100;

	// Historizing
                config1->accessHistoryDataCapability = UA_TRUE;
                config1->maxReturnDataValues = 0 ;  // 0 means unlimited size
                config1->accessHistoryEventsCapability = UA_TRUE;
                config1->maxReturnEventValues = 0;  // 0 means unlimted size
                config1->insertDataCapability = UA_TRUE;
                config1->insertEventCapability = UA_TRUE;
                config1->insertAnnotationsCapability = UA_TRUE;
                config1->replaceDataCapability = UA_TRUE;
                config1->replaceEventCapability = UA_TRUE;
                config1->updateDataCapability = UA_TRUE;
                config1->updateEventCapability = UA_TRUE;
                config1->deleteRawCapability = UA_TRUE;
                config1->deleteEventCapability = UA_TRUE;
                config1->deleteAtTimeDataCapability = UA_TRUE;
	/*
		#ifdef UA_ENABLE_PUBSUB
					printf("\tJust about to start the OPCUA Server UADP thread\n");

					// start OPCUA Server
					UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "starting server...");

					retval = UA_Server_run(server, &running); // blocking call - to test United Automation (ok)
					if(retval != UA_STATUSCODE_GOOD)
							goto cleanup;
		#else
	*/
		// start OPCUA Server
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "starting OPCUA server ...");

		//uaServer1 = UA_Server_newWithConfig(&config1);
		// check the new configuration settings
		#ifdef DEBUG
		printf("config1.applicationDescription.applicationUri 	: %s \n", config1->applicationDescription.applicationUri.data);
		printf("config1.applicationDescription.productUri	: %s \n", config1->applicationDescription.productUri.data);
		printf("config1.applicationDescription.applicationName	: %s \n", config1->applicationDescription.applicationName.text.data);
		#endif

		// Register to LDSServer
		//RegisterOPCServerToLDS(uaServer1); => somehow OPC-Server connecting as a client to LDS server crashed

		#ifdef UA_ENABLE_WEBSOCKET_SERVER
			// example on enable websockets can be found here:
			// https://github.com/open62541/open62541/blob/master/examples/tutorial_server_variable.c
			// https://github.com/open62541/open62541/blob/master/examples/encryption/server_encryption.c
			UA_UInt32 sendBufferSize = 1024;	//1024 * 1024;	// 1 Mbyte
			UA_UInt32 recvBufferSize = 1024;	//1024 * 1024;	// 1 Mbyte
			UA_StatusCode retVal;
                        // open62541 websockets always use ws:// regardless whether certificate/ privatekey is null or not
                        // any port can be used e.g. 7681, 7682
			// make sure #4880 is implemented in github
                        // ---- somehow after addNetworkLayerWS(...), OPCClient and UAExpert cannot connect to OPCServer
                        // ---- error message in client is : AG_mainOPCUAClient.c : Cannot login securely to OPCUAServer : opc.tcp://192.168.1.33:4840
			//retval = UA_ServerConfig_addNetworkLayerWS(UA_Server_getConfig(uaServer1), 7681, sendBufferSize, recvBufferSize, NULL, NULL); // default settings
	    		// retval = UA_ServerConfig_addNetworkLayerWS(UA_Server_getConfig(uaServer1), 7681, sendBufferSize, recvBufferSize, &certificate, &privateKey);
			//UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "UA_ENABLE_WEBSOCKET_SERVER : retVal for ws:// is %s", UA_StatusCode_name(retVal) );
			// create a connection (websockets) from config1
			int index = config1->networkLayersSize - 1;
			UA_ServerNetworkLayer *wsconfig = &config1->networkLayers[index];//&config1.networkLayers[index];
			UA_Connection *wsconnection;


                        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "===============================================================");
                        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_StartOPCUAServer : after UA_ServerConfig_addNetworkLayerWS()");
                        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "===============================================================");


			UA_UInt64 websocketpublishingCallbackId;
			//UA_Server_addRepeatedCallback(uaServer1, (UA_ServerCallback)callback_opcua,
			//					wsconnection, PUBLISH_INTERVAL_WEBSOCKETS /2, &websocketpublishingCallbackId);

			//char* nginxProxy = "http://192.168.1.165:80");
			//CreateServerWebSockets(uaServer1, nginxProxy, nsIdx_MKS);
			//char* libwebsocketProxy = "http://192.168.1.165:80");
			//bind OPCServer to libwebsocketProxy
		#endif

		size_t namespaceIndex;
	        //Add a new namespace to the server => Returns the index of the new namespace i.e. namespaceIndex
        	UA_Int16 nsIdx_MKS = UA_Server_addNamespace(uaServer1, "virtualskies.com.sg/MKS/"); //"http://virtualskies.com.sg/MKS/"); UA_LOG_INFO(UA_Log_Stdout, 
		UA_Server_getNamespaceByName(uaServer1, UA_STRING((char*)"virtualskies.com.sg/MKS/"), &namespaceIndex);
	        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "New Namespace added: <virtualskies.com.sg/MKS/>, nsIdx_MKS assigned is %d", nsIdx_MKS); // create nodes

		// create nodes
		//UA_NodeId r2_airgard_method_Id = CreateOPCUANodes(uaServer1);	// TODO: pass in namespace index during Nodes creation
		UA_NodeId *NodeId, r2_airgard_method_Id, r2_airgard_event_Id;
		NodeId = CreateOPCUANodes(uaServer1);
		r2_airgard_method_Id = NodeId[0];
		r2_airgard_event_Id = NodeId[1];
		// create method items
		CreateServerMethodItems(uaServer1, r2_airgard_method_Id);
		// create events for remote control : methodcall -> event
		CreateServerEvents(uaServer1, r2_airgard_event_Id);
		// create Alarms & Conditions
		CreateServerAlarmsAndConditions(uaServer1);
		// connect to History database engine - mySQL
		GetHistoryDBConnection();
        	// create monitored items
	        CreateServerMonitoredItems(uaServer1);
		// create historizing - needs monitored items to be setup first
		CreateServerHistorizingItems(uaServer1);
		// enable pub sub uadp / mqtt / AMQP
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_StartOPCUAServer.c : g_argc = %d", g_argc);
		if (g_argc == 3)
		{
			// myNewServer <192.168.1.33> <192.168.1.99>	== using PubSub UADP
			CreateServerPubSub(uaServer1, NULL, 0, (char*)"nomode");	// namespaceIndex is needed for connectionProperties 
		}
		else if (g_argc == 5)
		{
			// myNewServer <192.168.1.33> <192.168.1.99> <192.168.1.22> <1883>	== using PubSub MQTT for PUB and SUB
			// myNewServer <192.168.1.33> <192.168.1.99> <192.168.1.22> <5672>      == using PubSub AMQP for PUB and SUB

			UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "mode not indicated, defaulting to 'pubsub'");
			CreateServerPubSub(uaServer1, brokeripaddress, atoi(argv[4]), (char*)"pubsub");
		}
		else if (g_argc == 6)	// argv[5] = pubsub, pub, sub
		{
			// myNewServer <192.168.1.33> <192.168.1.88> <192.168.1.22> <1883 /5672> <pubsub | pub | sub>	== using PubSub MQTT for PUB or SUB
			CreateServerPubSub(uaServer1, brokeripaddress, atoi(argv[4]), argv[5]);
		}

		/*
		#ifdef UA_ENABLE_MODBUSTCP
			modbus_t *mb=NULL;
			mb = initialiseModbusConnection("192.168.1.119")	// using a ModbusPoll Slave simulator installed in 192.168.1.119
			readModbusData(mb);

		#endif
		*/

		#ifdef DEFUNCT //UA_ENABLE_DISCOVERY
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "==========================================================");
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_StartOPCUAServer : Just before LDS registration process");
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_StartOPCUAServer : Begin LDS registration process");

		//-------------- register itself to the LDS Server <192.168.1.44>
		// code based on githug/open62541/62541/examples/discovery/server_register.c
		// acting as an OPCUA Client to LDS Server
		UA_ByteString LDScertificate = loadFile(DISCOVERY_SSLCERTIFICATELOC);  //loadFile("/etc/ssl/certs/62541LDSServerCert.pem"); //=> symbolic link
	        //UA_ByteString certificate = loadFile("/usr/local/ssl/certs/ldscert44.pem"); // actual location
		if (LDScertificate.length == 0)
	        {
	                UA_LOG_FATAL(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_StartOPCUAServer (LDS): Unable to load file : %s", DISCOVERY_SSLCERTIFICATELOC);
	                goto cleanup;
	        }

        	UA_ByteString LDSprivateKey = loadFile(DISCOVERY_PRIVATEKEYLOC);  //loadFile("/usr/local/ssl/private/62541LDSprivate-key.pem");
	        //UA_ByteString LDSprivateKey = loadFile("/usr/local/ssl/private/ldsprivate-key.pem");
	        if (LDSprivateKey.length == 0)
	        {
	                UA_LOG_FATAL(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_StartOPCUAServer (LDS): Unable to load file : %s", DISCOVERY_PRIVATEKEYLOC);
	                goto cleanup;
	        }
	        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_StartOPCUAServer (LDS): successfully loaded LDS certificate and privateKey");

	        // load the trustlist.  Load trustlist is not supported now
	        size_t LDStrustListSize = 0;
	        UA_STACKARRAY(UA_ByteString, LDStrustList, LDStrustListSize);
	        for(size_t LDStrustListCount=0; LDStrustListCount < LDStrustListSize; LDStrustListCount++)
	                LDStrustList[LDStrustListCount] = loadFile(DISCOVERY_TRUSTLISTLOC);  //loadFile("/usr/local/ssl/trustlist/trustlist.crl");
	        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_StartOPCUAServer (LDS) : successfully loaded LDS trustList");

	        /* Loading of a revocation list currently unsupported */
	        UA_ByteString *LDSrevocationList = NULL;
	        size_t LDSrevocationListSize = 0;

		UA_Client *LDSclient = UA_Client_new();
		UA_ClientConfig *LDSClient_config1 = UA_Client_getConfig(LDSclient);
		//UA_ClientConfig_setDefault(UA_Client_getConfig(LDSclient));
		//UA_ClientConfig_setDefault(LDSClient_config1);
		LDSClient_config1->localConnectionConfig = UA_ConnectionConfig_default;

	        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_StartOPCUAClient (LDS) : here 2");

		UA_ApplicationDescription_clear(&LDSClient_config1->clientDescription);
		LDSClient_config1->clientDescription.applicationUri = UA_STRING_ALLOC(DISCOVERY_SERVER_APPLICATION_URI);

		// Secure client connect
		LDSClient_config1->securityMode = UA_MESSAGESECURITYMODE_SIGNANDENCRYPT; //UA_MESSAGESECURITYMODE_SIGNANDENCRYPT;  // require encryption
		LDSClient_config1->securityPolicyUri = UA_STRING_ALLOC("");
	        LDSClient_config1->secureChannelLifeTime = 10 * 60 * 1000;        // 10 minutes
	        LDSClient_config1->securityPolicies = (UA_SecurityPolicy*)UA_malloc(sizeof(UA_SecurityPolicy));
	        if (!LDSClient_config1->securityPolicies)
		{
	               UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_StartOPCUAServer.c (LDS) : %s", UA_StatusCode_name(UA_STATUSCODE_BADOUTOFMEMORY));
			goto cleanup;
		}
		LDSClient_config1->initConnectionFunc = UA_ClientConnectionTCP_init; /* for async client */
		LDSClient_config1->pollConnectionFunc = UA_ClientConnectionTCP_poll; /* for async connection */
		LDSClient_config1->customDataTypes = NULL;
		LDSClient_config1->connectivityCheckInterval = 0;
                LDSClient_config1->requestedSessionTimeout = 1200000; /* requestedSessionTimeout */
                LDSClient_config1->inactivityCallback = NULL;
                LDSClient_config1->clientContext = NULL;
                LDSClient_config1->connectivityCheckInterval = 3000; // in milliseconds

	        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_StartOPCUAServer (LDS) : Calling setDefaultEncryption");
		UA_ClientConfig_setDefaultEncryption(LDSClient_config1, LDScertificate, LDSprivateKey,
	                                                        LDStrustList, LDStrustListSize,
	                                                        LDSrevocationList, LDSrevocationListSize);
	        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_StartOPCUAServer (LDS) : Finished calling setDefaultEncryption");

		UA_ByteString_clear(&LDScertificate);
		UA_ByteString_clear(&LDSprivateKey);
		for (size_t deleteCounter=0; deleteCounter < LDStrustListSize; deleteCounter++)
			UA_ByteString_clear(&LDStrustList[deleteCounter]);
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_StartOPCUAServer.c (LDS) : successfully setDefaultEncryption as LDS Client");

		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_StartOPCUAServer.c (LDS): Just about to call UA_Client_connectUsername() at line 564");
	        retval = UA_Client_connectUsername(LDSclient, DISCOVERY_SERVER_ENDPOINT, "admin", "defaultadminpassword24");
	        if (retval != UA_STATUSCODE_GOOD)
	        {
	                UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_StartOPCUAServer.c (LDS) : Cannot login securely to OPCUA LDS Server : %s", DISCOVERY_SERVER_ENDPOINT);
			UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_StartOPCUAServer.c (LDS): error is %s", UA_StatusCode_name(retval));
			goto cleanup;
	        }
	        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_StartOPCUAServer.c (LDS) : UA_LDS_connectUsername() : success");

		UA_UInt64 callbackId;
		// server registration starts after 0 (5=5 minutes); initial delay for 500ms
		UA_StatusCode LDS_retval = UA_Server_addPeriodicServerRegisterCallback(uaServer1,
									LDSclient,
									DISCOVERY_SERVER_ENDPOINT,		// opc.tcp://192.168.1.44:4840
									0 * 60 * 1000,				// delay after X milliseconds
									500,					// delay first register for 500 ms
									&callbackId);
		// the following check in UA_Server_addPeriodicServerRegisterCallback() failed
		//(client->connection.state != UA_CONNECTIONSTATE_CLOSED)

		if (LDS_retval != UA_STATUSCODE_GOOD)
		{
			UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER,
				"SV_StartOPCUAServer.c (LDS): registering to remote LDS server  : Fail at UA_Server_addPeriodicServerRegisterCallback. StatusCode %s", 
				UA_StatusCode_name(LDS_retval));
			UA_Client_disconnect(LDSclient);
			UA_Client_delete(LDSclient);
			goto cleanup;
		}
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_StartOPCUAServer (LDS) : End LDS registration process");
	    	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_StartPOCUAServer (LDS) : registered to LDS Server <192.168.1.44:4840>");
		#endif	// UA_ENABLE_DISCOVERY
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "=============================================================");


		retval = UA_Server_run(uaServer1, &running);
		if (retval != UA_STATUSCODE_GOOD)
			goto cleanup;
		else
			UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "OPCUA server started successfully ...");


		#ifdef UA_ENABLE_DISCOVERY
		//UA_Server_unregister_discovery(uaServer1, LDSclient);
		//UA_Server_removeCallback(uaServer1, callbackId);
		//UA_Client_disconnect(LDSclient);
	        //UA_Client_delete(LDSclient);
		#endif

	}

cleanup:
	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "OPCUA Server was unexpectedly shut down");
	if (uaServer1)	// (uaServer)
		UA_Server_delete(uaServer1); // UA_Server_delete(uaServer);
	else
		UA_ServerConfig_clean(config1); // UA_ServerConfig_clean(config);

	close(sockfd);
	CloseHistoryDBConnection();
	//return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
//#endif
}


