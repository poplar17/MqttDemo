//
// Created by l00579557 on 2022/9/26.
//

#ifndef TEST_USER_CALLBACK_H
#define TEST_USER_CALLBACK_H


#include <string>
#include <iostream>
#include "mqtt/callback.h"

class user_callback : public virtual mqtt::callback {
    void connection_lost(const std::string &cause) override {
        std::cout << "\nConnection lost....." << std::endl;
        if (!cause.empty())
            std::cout << "\ncause: " << cause << std::endl;
    }

    void delivery_complete(mqtt::delivery_token_ptr tok) override {
        std::cout << "\n\t[Delivery complete for token: "
                  << (tok ? tok->get_message_id() : -1) << "]" << std::endl;
    }

public:
};


#endif //TEST_USER_CALLBACK_H
