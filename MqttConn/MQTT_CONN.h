//
// Created by h00587496 on 2022/9/29.
//

#ifndef TEST_MQTT_CONN_H
#define TEST_MQTT_CONN_H

#define __FILE__ "MQTT_CONN.cpp"

#include <string>
#include "include/mqtt/client.h"

enum qos_enum {
    MQTT_QOS_ZERO = 0, /* QoS 等级0:最多只发送一次 */
    MQTT_QOS_ONE,      /* QoS 等级1:至少发送一次 */
    MQTT_QOS_TWO       /* QoS 等级2:只发送一次 */
};

mqtt::async_client* MqqClientConn(std::string serverAddress, std::string port, std::string clientId, std::string keyPath [], void (*func)(std::string, std::string));

// topic是否暴露
int MqttClientSendMsg(mqtt::async_client* client, const std::string& topic, const std::string& msg);

int MqttClientSubscribeMsg(mqtt::async_client* client, const std::string& topic);

int MqttClientUnSubscribeMsg(mqtt::async_client* client, const std::string& topic);

int MqttClientDisconnect(mqtt::async_client * client);
#endif //TEST_MQTT_CONN_H
