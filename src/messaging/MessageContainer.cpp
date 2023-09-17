#include "MessageContainer.h"

// Constructor that takes a sequence of Message structs
MessageContainer::MessageContainer() {
    numMessages_ = 0;
}

// Constructor that takes a sequence of Message structs
MessageContainer::MessageContainer(const Message messages[], size_t numMessages) {
    numMessages_ = min(numMessages, MAX_MESSAGES); // Limit the number of messages to the maximum allowed
    for (size_t i = 0; i < numMessages_; i++) {
        messages_[i] = messages[i];
    }
}

// Method to get the value of a message based on its messageType
// Returns true if messageType is found, false otherwise
bool MessageContainer::getValueByMessageType(const char messageType[], float* value) {
    for (size_t i = 0; i < numMessages_; i++) {
        if (strcmp(messages_[i].messageType, messageType) == 0) {
            *value = messages_[i].value;
            return true;
        }
    }
    return false; // messageType not found
}
