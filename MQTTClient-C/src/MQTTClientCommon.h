/*******************************************************************************
 * Copyright (c) 2023 Microsoft Corporation. All rights reserved.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 *******************************************************************************/

#if !defined(MQTT_CLIENT_COMMON_H_)
#define MQTT_CLIENT_COMMON_H_

#if defined(__cplusplus)
 extern "C" {
#endif

#if defined(WIN32_DLL) || defined(WIN64_DLL)
  #define DLLImport __declspec(dllimport)
  #define DLLExport __declspec(dllexport)
#elif defined(LINUX_SO)
  #define DLLImport extern
  #define DLLExport  __attribute__ ((visibility ("default")))
#else
  #define DLLImport
  #define DLLExport
#endif

#if defined(MQTTV5)
#include "MQTTV5Packet.h"
#else
#include "MQTTPacket.h"
#endif /* MQTTV5 */

#if defined(MQTTCLIENT_PLATFORM_HEADER)
/* The following sequence of macros converts the MQTTCLIENT_PLATFORM_HEADER value
 * into a string constant suitable for use with include.
 */
#define xstr(s) str(s)
#define str(s) #s
#include xstr(MQTTCLIENT_PLATFORM_HEADER)
#endif

#define MAX_PACKET_ID 65535 /* according to the MQTT specification - do not change! */

#if !defined(MAX_MESSAGE_HANDLERS)
/**
 * @brief The maximum number of message handlers. This can be redefined by the user and applies to all client objects.
 * 
 */
#define MAX_MESSAGE_HANDLERS 5 /* redefinable - how many subscriptions do you want? */
#endif

/// @brief MQTT Quality of Service levels.
enum MQTTQoS { 
  /// @brief QoS 0: At most once delivery
  MQTTQOS_0, 
  /// @brief QoS 1: At least once delivery
  MQTTQOS_1, 
  /// @brief QoS 2: Exactly once delivery
  MQTTQOS_2, 
  /// @brief SUBACK MQTTv3 failure return code
  MQTTQOS_SUBFAIL=0x80 };

/* all failure return codes must be negative */
enum MQTTClientReturnCode { 
  MQTTCLIENT_BUFFER_OVERFLOW = -2, 
  MQTTCLIENT_FAILURE = -1, 
  MQTTCLIENT_SUCCESS = 0 };

/* The Platform specific header must define the Network and Timer structures and functions
 * which operate on them.
 *
typedef struct Network
{
	int (*mqttread)(Network*, unsigned char* read_buffer, int, int);
	int (*mqttwrite)(Network*, unsigned char* send_buffer, int, int);
} Network;*/

/* The Timer structure must be defined in the platform specific header,
 * and have the following functions to operate on it.  */
extern void TimerInit(Timer*);
extern char TimerIsExpired(Timer*);
extern void TimerCountdownMS(Timer*, unsigned int);
extern void TimerCountdown(Timer*, unsigned int);
extern int TimerLeftMS(Timer*);

/**
 * @brief Data structure for holding information about a message.
 * 
 */
#if defined(MQTTV5)
typedef struct MQTTV5Message
#else
typedef struct MQTTMessage
#endif
{
#if defined(MQTTV5)
    /// @brief The MQTTv5 message properties.
    MQTTProperties* properties;
#endif /* MQTTV5 */
    /// @brief The MQTT message QoS.
    enum MQTTQoS qos;
    /// @brief The MQTT message retained flag.
    unsigned char retained;
    /// @brief The MQTT message dup flag.
    unsigned char dup;
    /// @brief The MQTT message identifier.
    unsigned short id;
    /// @brief The MQTT message payload.
    void *payload;
    /// @brief The MQTT message `payload` length.
    size_t payloadlen;
#if defined(MQTTV5)
} MQTTV5Message;
#else
} MQTTMessage;
#endif

/**
 * @brief Data structure for holding information about a received message.
 * 
 */
#if defined(MQTTV5)
typedef struct MessageV5Data
{
    /// @brief The MQTT message.
    MQTTV5Message* message;
    /// @brief The topic to which the message was published.
    MQTTString* topicName;
} MessageData;
#else
typedef struct MessageData
{
    /// @brief The MQTT message.
    MQTTMessage* message;
    /// @brief The topic to which the message was published.
    MQTTString* topicName;
} MessageData;
#endif

/**
 * @brief Data structure containing CONNACK information.
 * 
 */
typedef struct MQTTConnackData
{
#if defined(MQTTV5)
    /// @brief The MQTTv5 message properties.
    MQTTProperties* properties;
    /// @brief The MQTTv5 reason code.
    enum MQTTReasonCodes reasonCode;
#else
    /// @brief The MQTTv3 reason code.
    unsigned char rc;
#endif /* MQTTV5 */
    /// @brief The MQTT session present flag.
    unsigned char sessionPresent;
} MQTTConnackData;

/**
 * @brief Data structure containing SUBACK information.
 * 
 */
typedef struct MQTTSubackData
{
#if defined(MQTTV5)
    /// @brief The MQTTv5 message properties.
    MQTTProperties* properties;
    /// @brief The MQTT reason code.
    enum MQTTReasonCodes reasonCode;
#else
    /// @brief The MQTT granted QoS or `MQTTQOS_SUBFAIL` on failure.
    enum MQTTQoS grantedQoS;
#endif /* MQTTV5 */
} MQTTSubackData;

/**
 * @brief Data structure containing information about a published message.
 * @remark This structure is used for both QoS1 (PUBACK) and QoS2 (PUBCOMP) messages.
 * @remark The `id` field is omitted as it is already present in the `MQTTMessage` structure.
 */
typedef struct MQTTPubDoneData
{
#if defined(MQTTV5)
    /// @brief The MQTTv5 message properties.
    MQTTProperties* properties;
    /// @brief The MQTTv5 reason code.
    enum MQTTReasonCodes reasonCode;
#endif /* MQTTV5 */
    /// @brief The MQTT message dup flag.
    unsigned char dup;
} MQTTPubDoneData;

/**
 * @brief Callback type for handling incoming messages.
 * @remark Separate callbacks can be used for each subscription filter.
 * 
 * @param received The received message.
 */
typedef void (*messageHandler)(MessageData* received);

#if defined(MQTTV5)
/**
 * @brief Callback type used for asynchronous MQTTv5 DISCONNECT and AUTH.
 * @remark Separate callbacks should be used for each control message type.
 * 
 * @param properties The MQTTv5 message properties.
 * @param reasonCode The MQTTv5 reason code.
 * @param id The MQTT message identifier.
 * 
 */
typedef void (*controlHandler)(MQTTProperties* properties, enum MQTTReasonCodes reasonCode, unsigned short id);
#endif /* MQTTV5 */

/**
 * @brief The MQTTClient handle.
 * @note This structure should be treated as opaque by the user.
 * 
 */
typedef struct MQTTClient
{
    unsigned int next_packetid,
      command_timeout_ms;
    size_t buf_size,
      readbuf_size;
    unsigned char *buf,
      *readbuf;
    unsigned int keepAliveInterval;
    char ping_outstanding;
    int isconnected;

#if defined(MQTTV5)
    int cleanstart;
    MQTTProperties* recvProperties;
#else
    int cleansession;
#endif /* MQTTV5 */

    struct MessageHandlers
    {
        const char* topicFilter;
        void (*fp) (MessageData*);
    } messageHandlers[MAX_MESSAGE_HANDLERS];      /* Message handlers are indexed by subscription topic */

    void (*defaultMessageHandler) (MessageData*);

    Network* ipstack;
  Timer last_sent, last_received, pingresp_timer;
#if defined(MQTT_TASK)
    Mutex mutex;
    Thread thread;
#endif
} MQTTClient; // TODO Rename to MQTTV5Client for V5.

/**
 * @brief Default initializer for an MQTTClient structure.
 * 
 */
#define DefaultClient {0, 0, 0, 0, NULL, NULL, 0, 0, 0}


#if defined(__cplusplus)
     }
#endif

#endif /* MQTT_CLIENT_COMMON_H_ */
