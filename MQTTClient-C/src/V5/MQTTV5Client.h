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

#if !defined(MQTTV5_CLIENT_H)
#define MQTTV5_CLIENT_H

#if defined(__cplusplus)
 extern "C" {
#endif

#include "../MQTTClient.h"
#include "V5/MQTTV5Packet.h"


/**
 * @brief Data structure containing MQTTv5 UNSUBACK information.
 * 
 */
typedef struct MQTTV5UnsubackData
{
    /// @brief The MQTTv5 message properties.
    MQTTProperties* properties;
    /// @brief The MQTT reason code.
    enum MQTTReasonCodes reasonCode;
} MQTTV5UnsubackData;

/**
 * @brief Create an `MQTTClient` for an MQTTv5 connection.
 * 
 * @param client The `MQTTClient` object to initialize.
 * @param network The `Network` object to use. 
 * @param command_timeout_ms The command timeout value in milliseconds. 
 * @param sendbuf The send buffer.
 * @param sendbuf_size The size of the `sendbuf` buffer. 
 * @param readbuf The read buffer.
 * @param readbuf_size The size of the `readbuf` buffer.
 * @param recvProperties The MQTTv5 receive properties. This allocation will be used for all MQTTv5 packets.
 * @param truncateRecvProperties If true, the MQTTv5 properties will be truncated if they do not fit in the `recvProperties` buffer.
 * 
 * @note If `recvProperties` does not contain sufficient space for the received properties and `truncateRecvProperties` is false, 
 *       `MQTTYield` will fail with `MQTTCLIENT_BUFFER_OVERFLOW`.
 */
DLLExport void MQTTV5ClientInit(MQTTClient* client, Network* network, unsigned int command_timeout_ms,
		unsigned char* sendbuf, size_t sendbuf_size, unsigned char* readbuf, size_t readbuf_size, 
    MQTTProperties* recvProperties, bool truncateRecvProperties);

/**
 * @brief MQTT Connect - send an MQTT connect packet down the network and wait for a Connack.
 * @note The network object must be connected to the network endpoint before calling this.
 * 
 * @param client The `MQTTClient` object to use.
 * @param options The connect options.
 * @param connack CONNACK response information.
 * @return An `MQTTClientReturnCode` indicating success or failure.
 */
DLLExport int MQTTV5ConnectWithResults(MQTTClient* client, MQTTPacket_connectData* options,
    MQTTProperties* connectProperties, MQTTProperties* willProperties, MQTTConnackData* data);

/** MQTT Connect - send an MQTT connect packet down the network and wait for a Connack
 *  The nework object must be connected to the network endpoint before calling this
 *  @param options - connect options
 *  @return success code
 */
DLLExport int MQTTV5Connect(MQTTClient* client, MQTTPacket_connectData* options, 
  MQTTProperties* connectProperties, MQTTProperties* willProperties);

/** MQTT Publish - send an MQTT publish packet and wait for all acks to complete for all QoSs
 *  @param client - the client object to use
 *  @param topic - the topic to publish to
 *  @param message - the message to send
 *  @return success code
 */
DLLExport int MQTTV5Publish(MQTTClient* client, const char* topic, MQTTMessage* message, 
  MQTTProperties* properties);


DLLExport int MQTTV5PublishWithResults(MQTTClient* client, const char* topic, MQTTMessage* message, 
  MQTTProperties* properties, MQTTPubDoneData* ack);

/** MQTT SetMessageHandler - set or remove a per topic message handler
 *  @param client - the client object to use
 *  @param topicFilter - the topic filter set the message handler for
 *  @param messageHandler - pointer to the message handler function or NULL to remove
 *  @return success code
 */
DLLExport int MQTTV5SetMessageHandler(MQTTClient* c, const char* topicFilter, messageHandler messageHandler);

/**
 * @brief MQTT Auth - send an MQTT AUTH packet
 * 
 * @param client - the client object to use
 * @param reasonCode - the reason code to send
 * @param properties - the properties to send
 * @return success code
 */
DLLExport int MQTTV5Auth(MQTTClient* client, unsigned char reasonCode, MQTTProperties* properties);

DLLExport int MQTTV5SetAuthHandler(MQTTClient* c, controlHandler authHandler);

/** MQTT Subscribe - send an MQTT subscribe packet and wait for suback before returning.
 *  @param client - the client object to use
 *  @param topicFilter - the topic filter to subscribe to
 *  @param message - the message to send
 *  @return success code
 */
DLLExport int MQTTV5Subscribe(MQTTClient* client, const char* topicFilter, enum MQTTQoS qos, 
  messageHandler messageHandler, MQTTProperties* properties, MQTTSubscribe_options options);

/** MQTT Subscribe - send an MQTT subscribe packet and wait for suback before returning.
 *  @param client - the client object to use
 *  @param topicFilter - the topic filter to subscribe to
 *  @param message - the message to send
 *  @param data - suback granted QoS returned
 *  @return success code
 */
DLLExport int MQTTV5SubscribeWithResults(MQTTClient* client, const char* topicFilter, 
  enum MQTTQoS qos, messageHandler messageHandler, MQTTSubackData* data);

/** MQTT Unsubscribe - send an MQTT unsubscribe packet and wait for unsuback before returning.
 *  @param client - the client object to use
 *  @param topicFilter - the topic filter to unsubscribe from
 *  @return success code
 */
DLLExport int MQTTV5Unsubscribe(MQTTClient* client, const char* topicFilter, MQTTProperties* properties);


DLLExport int MQTTV5UnsubscribeWithResults(MQTTClient* client, const char* topicFilter, MQTTProperties* properties, MQTTV5UnsubackData* data);

/** MQTT Disconnect - send an MQTT disconnect packet and close the connection
 *  @param client - the client object to use
 *  @return success code
 */
DLLExport int MQTTV5Disconnect(MQTTClient* client, unsigned char reasonCode, MQTTProperties* properties);

DLLExport int MQTTV5SetDisconnectHandler(MQTTClient* c, controlHandler disconnectHandler);

#if defined(__cplusplus)
     }
#endif

#endif // MQTTV5_CLIENT_H
