#define GUSI_SOURCE
#include <GUSIConfig.h>
#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Declarations of Socket Factories */

void GUSIwithInetSockets();
void GUSIwithLocalSockets();
void GUSIwithMTInetSockets();
void GUSIwithMTTcpSockets();
void GUSIwithMTUdpSockets();
void GUSIwithOTInetSockets();
void GUSIwithOTTcpSockets();
void GUSIwithOTUdpSockets();
void GUSIwithPPCSockets();
void GUSISetupFactories();

/* Configure Socket Factories */

void GUSISetupFactories()
{
#ifdef GUSISetupFactories_BeginHook
    GUSISetupFactories_BeginHook
#endif
    GUSIwithInetSockets();
#ifdef GUSISetupFactories_EndHook
    GUSISetupFactories_EndHook
#endif
}

/* Declarations of File Devices */

void GUSIwithDConSockets();
void GUSIwithNullSockets();
void GUSISetupDevices();

/* Configure File Devices */

void GUSISetupDevices()
{
#ifdef GUSISetupDevices_BeginHook
    GUSISetupDevices_BeginHook
#endif
    GUSIwithNullSockets();
#ifdef GUSISetupDevices_EndHook
    GUSISetupDevices_EndHook
#endif
}

#ifdef __cplusplus
}
#endif
