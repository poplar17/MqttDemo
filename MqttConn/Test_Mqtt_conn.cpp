//
// Created by h00587496 on 2022/9/29.
//
#include "MQTT_CONN.h"
#include <chrono>
#include <thread>

using namespace std;

void TestFunc(string topic, string msg) {
    if (topic == "client/test1/131456789") {
        cout << "topic1: " <<  msg << endl;
    } else if (topic == "client/test2/131456789") {
        cout << "topic2: " <<  msg << endl;
    }
    // cout << topic <<  msg << endl;
}

void TestFunc2(string topic, string msg) {
    cout << "\nclient2 is string..." << endl;
    cout << topic <<  msg << endl;
}

int main() {
    std::string keyPath [3] = {"D:\\SDK\\myca\\ca.crt", "D:\\SDK\\myca\\iov_hzb_047.crt","D:\\SDK\\myca\\iov_hzb_047.pk8"};

    mqtt::async_client* client = MqqClientConn("xxx.xxx.xxx.xxx", "xxx", "8cb4a68ca8544f4d87446f66cdb4bd4d", keyPath, TestFunc);
    if (client == nullptr) {
        cout << "\nclient is null..." << endl;
    }
    MqttClientSendMsg(client, "client/test1/131456791", "hello world");
    // MqttClientSubscribeMsg(client, "client/test1/131456789");
    // MqttClientSendMsg(client, "client/test1/131456789", "hello world9");
    // MqttClientSubscribeMsg(client, "client/test2/131456789");
    while (std::tolower(std::cin.get()) != 'q')
        ;
    MqttClientUnSubscribeMsg(client, "client/test1/131456791");
    MqttClientDisconnect(client);
}