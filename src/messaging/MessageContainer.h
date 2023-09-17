#ifndef MESSAGE_CONTAINER_H
#define MESSAGE_CONTAINER_H

#include <Arduino.h>

const size_t MAX_MESSAGES = 10; 

class MessageContainer {
public:
    struct Message {
        char messageType[3]; 
        float value;
    };
    
    MessageContainer();

    MessageContainer(const Message messages[], size_t numMessages);

    bool getValueByMessageType(const char messageType[], float* value);

private:
    Message messages_[MAX_MESSAGES]; 
    size_t numMessages_; 
};

#endif 
