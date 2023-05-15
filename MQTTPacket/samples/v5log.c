/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
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
 *    Cristian Pop - initial implementation and/or initial documentation
 *******************************************************************************/

#include "V5/MQTTV5Packet.h"

const char* v5property_identifier_to_string(int identifier)
{
    switch (identifier) 
    {
        case PAYLOAD_FORMAT_INDICATOR:
            return "PAYLOAD_FORMAT_INDICATOR";
        case MESSAGE_EXPIRY_INTERVAL:
            return "MESSAGE_EXPIRY_INTERVAL";
        case CONTENT_TYPE:
            return "CONTENT_TYPE";
        case RESPONSE_TOPIC:
            return "RESPONSE_TOPIC";
        case CORRELATION_DATA:
                return "CORRELATION_DATA";
        case SUBSCRIPTION_IDENTIFIER :
                return "SUBSCRIPTION_IDENTIFIER";
        case SESSION_EXPIRY_INTERVAL :
                return "SESSION_EXPIRY_INTERVAL";
        case ASSIGNED_CLIENT_IDENTIFER :
                return "ASSIGNED_CLIENT_IDENTIFER";
        case SERVER_KEEP_ALIVE :
                return "SERVER_KEEP_ALIVE";
        case AUTHENTICATION_METHOD :
                return "AUTHENTICATION_METHOD";
        case AUTHENTICATION_DATA :
                return "AUTHENTICATION_DATA";
        case REQUEST_PROBLEM_INFORMATION :
                return "REQUEST_PROBLEM_INFORMATION";
        case WILL_DELAY_INTERVAL :
                return "WILL_DELAY_INTERVAL";
        case REQUEST_RESPONSE_INFORMATION :
                return "REQUEST_RESPONSE_INFORMATION";
        case RESPONSE_INFORMATION :
                return "RESPONSE_INFORMATION";
        case SERVER_REFERENCE :
                return "SERVER_REFERENCE";
        case REASON_STRING :
                return "REASON_STRING";
        case RECEIVE_MAXIMUM :
                return "RECEIVE_MAXIMUM";
        case TOPIC_ALIAS_MAXIMUM :
                return "TOPIC_ALIAS_MAXIMUM";
        case TOPIC_ALIAS :
                return "TOPIC_ALIAS";
        case MAXIMUM_QOS :
                return "MAXIMUM_QOS";
        case RETAIN_AVAILABLE :
                return "RETAIN_AVAILABLE";
        case USER_PROPERTY :
                return "USER_PROPERTY";
        case MAXIMUM_PACKET_SIZE :
                return "MAXIMUM_PACKET_SIZE";
        case WILDCARD_SUBSCRIPTION_AVAILABLE :
                return "WILDCARD_SUBSCRIPTION_AVAILABLE";
        case SUBSCRIPTION_IDENTIFIER_AVAILABLE :
                return "SUBSCRIPTION_IDENTIFIER_AVAILABLE";
        case SHARED_SUBSCRIPTION_AVAILABLE:
                return "SHARED_SUBSCRIPTION_AVAILABLE";
        default:
            return "UNKNOWN";
    }
}

const char* v5property_type_to_string(int type)
{
    switch (type)
    {
        case BYTE:
            return "BYTE";
        case TWO_BYTE_INTEGER:
            return "TWO_BYTE_INTEGER";
        case FOUR_BYTE_INTEGER:
            return "FOUR_BYTE_INTEGER";
        case VARIABLE_BYTE_INTEGER:
            return "VARIABLE_BYTE_INTEGER";
        case BINARY_DATA:
            return "BINARY_DATA";
        case UTF_8_ENCODED_STRING:
            return "UTF_8_ENCODED_STRING";
        case UTF_8_STRING_PAIR:
            return "UTF_8_STRING_PAIR";
        default:
            return "UNKNOWN";
    }
}

const char* v5property_print(MQTTProperty property)
{
    const char* identifier_str = v5property_identifier_to_string(property.identifier);
    int type = MQTTProperty_getType(property.identifier);
    const char* type_str = v5property_type_to_string(type);

    switch(type)
    {
        case BYTE:
            printf("\t%s (%s) = %d\n", identifier_str, type_str, (int)property.value.byte);
            break;
        case TWO_BYTE_INTEGER:
            printf("\t%s (%s) = %d\n", identifier_str, type_str, (int)property.value.integer2);
            break;
        case FOUR_BYTE_INTEGER:
            printf("\t%s (%s) = %d\n", identifier_str, type_str, (int)property.value.integer4);
            break;
        case VARIABLE_BYTE_INTEGER:
            printf("\t%s (%s) = %d\n", identifier_str, type_str, (int)property.value.integer4);
            break;
        case BINARY_DATA:
            printf("\t%s (%s) = <BINARY>\n", identifier_str, type_str);
            break;
        case UTF_8_ENCODED_STRING:
            printf("\t%s (%s) = [%.*s]\n", identifier_str, type_str, property.value.data.len, property.value.data.data);
            break;
        case UTF_8_STRING_PAIR:
            printf("\t%s: (%s) = [%.*s], [%.*s]\n", identifier_str, type_str, 
                property.value.string_pair.key.len, property.value.string_pair.key.data,
                property.value.string_pair.val.len, property.value.string_pair.val.data);
            break;
        default:
            return "UNKNOWN";
    }
}