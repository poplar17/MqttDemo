#include "MQTT_CONN.h"

#include <utility>
#include "mqtt/async_client.h"
#include "user_callback.h"
#include "include/mqtt/client.h"
#include "LOG_SYS.h"

using namespace std;

Logger logger(Logger::file_and_terminal, Logger::info, "log.log");

const int QOS = 1;

class Callback : public virtual mqtt::callback
{
    void (*func)(std::string, std::string);
public:
    void message_arrived(mqtt::const_message_ptr msg) override {
        std::cout << "Message arrived" << std::endl;
        std::cout << "\ttopic: '" << msg->get_topic() << "'" << std::endl;
        // 收到消息后
        typedef void (*fp)(string topicName, string msg);
        fp f = func;
        if (f == nullptr) {
            return;
        } else {
            f(msg->get_topic(), msg->to_string());
        }
    }

    void delivery_complete(mqtt::delivery_token_ptr token) override {
        std::cout << "Message send success." << std::endl;
    }
public:
    explicit Callback(void (*func_)(std::string, std::string))
    : func(func_) {}
};

class action_listener : public virtual mqtt::iaction_listener
{
protected:
    void on_failure(const mqtt::token& tok) override {
        cout << "\tListener failure for token111: "
             << tok.get_message_id() << endl;
    }

    void on_success(const mqtt::token& tok) override {
        cout << "\tListener success for token: "
             << tok.get_message_id() << endl;
    }
};

/**
 * A derived action listener for publish events.
 */
class delivery_action_listener1 : public action_listener
{
    atomic<bool> done_;

    void on_failure(const mqtt::token& tok) override {
        action_listener::on_failure(tok);
        done_ = true;
    }

    void on_success(const mqtt::token& tok) override {
        action_listener::on_success(tok);
        logger.INFO("send message success.");
        done_ = true;
    }

public:
    delivery_action_listener1() : done_(false) {}
    bool is_done() const { return done_; }
};

/**
 * A subscribe action listener for subscribe events.
 */
class subscribe_action_listener : public virtual mqtt::iaction_listener
{
public:

    string topicName;

    void on_failure(const mqtt::token& tok) override {
        // action_listener::on_failure(tok);
        // cout << "subscribe error: "<< tok.get_topics().get() << endl;
    }

    void on_success(const mqtt::token& tok) override {
        // action_listener::on_success(tok);
        cout << topicName << endl;
        // func("hello");
        logger.INFO("receive message success.");
    }
public:
    subscribe_action_listener(string topicName_) : topicName(std::move(topicName_)) {}
};

mqtt::async_client* MqqClientConn(std::string serverAddress, std::string port, string clientId, std::string keyPath [], void (*func)(std::string, std::string)) {
    if (serverAddress.empty() || port.empty() || keyPath->length() < 3) {
        logger.ERROR("serverAddress or port is empty.");
        return nullptr;
    }
    string connAddress = "ssl://" + serverAddress + ":" + port;
    auto* client = new mqtt::async_client(connAddress, clientId);


    auto cb = new Callback(func);
    client->set_callback(*cb);

    auto sslopts = mqtt::ssl_options_builder()
            .trust_store(keyPath[0])
            .key_store(keyPath[1])
            .private_key(keyPath[2])
            // .enabled_cipher_suites()
            //.enable_server_cert_auth(true)
            .verify()
            //.enable_server_cert_auth()
            .error_handler([](const std::string& msg) {
                logger.ERROR(msg);
                std::cerr << "SSL Error: " << msg << std::endl;
            })
            .finalize();

    auto connopts = mqtt::connect_options_builder()
            .ssl(std::move(sslopts))
            .clean_session(true)
            .finalize();
    try {
        mqtt::token_ptr conntok = client->connect(connopts);
        conntok->wait();
    } catch (const mqtt::exception& exc) {
        logger.ERROR(exc.what());
    }
    return client;
}

int MqttClientSendMsg(mqtt::async_client* client, const std::string& topic, const std::string& msg) {
    if (client == nullptr || !client->is_connected()) {
        logger.ERROR("client is null or disconnect.");
        return 1;
    }
    try {
        mqtt::message_ptr pubmsg;
        mqtt::delivery_token_ptr pubtok;

        delivery_action_listener1 deliveryListener;
        pubmsg = mqtt::make_message(topic, msg);
        pubmsg->set_qos(1);
        pubtok = client->publish(pubmsg, nullptr, deliveryListener);
        pubtok->wait();
    } catch (const mqtt::exception& exc) {
        logger.ERROR(exc.what());
        return 1;
    }
    return 0;
}

/**
 * 订阅消息
 *
 * @param client mqtt客户端
 * @param topic  订阅的主题
 * @return       是否成功
 */
int MqttClientSubscribeMsg(mqtt::async_client* client, const std::string& topic) {
    if (client == nullptr || !client->is_connected()) {
        logger.ERROR("client is null");
        return 1;
    }
    try {
        // 订阅topic
        static subscribe_action_listener listener(topic);
        // auto listener = new subscribe_action_listener(topic);
        // static subscribe_action_listener listener;
        client->subscribe(topic, QOS, nullptr, listener)->wait();
    } catch (const mqtt::exception& exc) {
        logger.ERROR("subscribe to topic: " + topic + " failed.");
        logger.ERROR( exc.what());
        return 1;
    }
    return 0;
}

int MqttClientUnSubscribeMsg(mqtt::async_client* client, const std::string& topic) {
    if (client == nullptr || !client->is_connected()) {
        logger.ERROR("client is null");
        return 1;
    }
    try {
        client->unsubscribe(topic)->wait();
    } catch (const mqtt::exception& exc) {
        logger.ERROR(exc.what());
        return 1;
    }
    return 0;
}

int MqttClientDisconnect(mqtt::async_client * client) {
    if (client == nullptr || !client->is_connected()) {
        logger.ERROR("client is null or disconnect");
        return 0;
    }
    try {
        client->disconnect()->wait();
    } catch (const mqtt::exception& exc) {
        logger.ERROR("disconnect error: ");
        logger.ERROR(exc.what());
        return 1;
    }
    logger.INFO("disconnect client success");
    return 0;
}

