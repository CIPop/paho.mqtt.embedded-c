/*******************************************************************************
 * Copyright (c) 2014, 2023 IBM Corp., Ian Craggs and others
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
 * Contributors:
 *    Ian Craggs - initial API and implementation and/or initial documentation
 *    Xiang Rong - 442039 Add makefile to Embedded C client
 *******************************************************************************/

#ifndef MQTTPACKETCOMMON_H_
#define MQTTPACKETCOMMON_H_

#include <stdint.h>

#if defined(__cplusplus) /* If this is a C++ compiler, use C linkage */
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

enum errors
{
	MQTTPACKET_BUFFER_TOO_SHORT = -2,
	MQTTPACKET_READ_ERROR = -1,
	MQTTPACKET_READ_COMPLETE
};

enum msgTypes
{
	CONNECT = 1, CONNACK, PUBLISH, PUBACK, PUBREC, PUBREL,
	PUBCOMP, SUBSCRIBE, SUBACK, UNSUBSCRIBE, UNSUBACK,
	PINGREQ, PINGRESP, DISCONNECT
#if defined(MQTTV5)
  , AUTH
#endif
};

/**
 * Bitfields for the MQTT header byte.
 */
typedef union
{
	unsigned char byte;	                /**< the whole byte */
#if defined(REVERSED)
	struct
	{
		unsigned int type : 4;			/**< message type nibble */
		unsigned int dup : 1;				/**< DUP flag bit */
		unsigned int qos : 2;				/**< QoS value, 0, 1 or 2 */
		unsigned int retain : 1;		/**< retained flag bit */
	} bits;
#else
	struct
	{
		unsigned int retain : 1;		/**< retained flag bit */
		unsigned int qos : 2;				/**< QoS value, 0, 1 or 2 */
		unsigned int dup : 1;				/**< DUP flag bit */
		unsigned int type : 4;			/**< message type nibble */
	} bits;
#endif
} MQTTHeader;

typedef struct
{
	int32_t len;
	char* data;
} MQTTLenString;

typedef struct
{
	char* cstring;
	MQTTLenString lenstring;
} MQTTString;

#define MQTTString_initializer {NULL, {0, NULL}}

int MQTTstrlen(MQTTString mqttstring);

#include "V3/MQTTConnect.h"
#include "V3/MQTTPublish.h"
#include "V3/MQTTSubscribe.h"
#include "V3/MQTTUnsubscribe.h"
#include "V3/MQTTFormat.h"

DLLExport int32_t MQTTSerialize_ack(unsigned char* buf, int32_t buflen, unsigned char type, unsigned char dup, unsigned short packetid);
DLLExport int32_t MQTTDeserialize_ack(unsigned char* packettype, unsigned char* dup, unsigned short* packetid, unsigned char* buf, int32_t buflen);

int32_t MQTTPacket_VBIlen(int32_t rem_len);
int32_t MQTTPacket_len(int32_t rem_len);
DLLExport int MQTTPacket_equals(MQTTString* a, char* b);

DLLExport int32_t MQTTPacket_encode(unsigned char* buf, int32_t length);
int32_t MQTTPacket_decode(int (*getcharfn)(unsigned char*, int), int32_t* value);
int32_t MQTTPacket_decodeBuf(unsigned char* buf, int32_t* value);

int readInt(unsigned char** pptr);
char readChar(unsigned char** pptr);
void writeChar(unsigned char** pptr, char c);
void writeInt(unsigned char** pptr, int anInt);
int readMQTTLenString(MQTTString* mqttstring, unsigned char** pptr, unsigned char* enddata);
void writeCString(unsigned char** pptr, const char* string);
void writeMQTTString(unsigned char** pptr, MQTTString mqttstring);

DLLExport int MQTTPacket_read(unsigned char* buf, int32_t buflen, int (*getfn)(unsigned char*, int));

typedef struct {
	int (*getfn)(void *, unsigned char*, int); /* must return -1 for error, 0 for call again, or the number of bytes read */
	void *sck;	/* pointer to whatever the system may use to identify the transport */
	int multiplier;
	int rem_len;
	int32_t len;
	char state;
}MQTTTransport;

int MQTTPacket_readnb(unsigned char* buf, int32_t buflen, MQTTTransport *trp);

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
}
#endif


#endif /* MQTTPACKETCOMMON_H_ */
