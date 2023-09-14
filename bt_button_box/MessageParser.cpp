#include "MessageParser.h"

String MessageParser::readStringFromSerial() {
    String input;

    // Read data until a newline character is received or until there's no more data to read
    while (Serial.available()) {
        char c = Serial.read();
        if (c == '\n') {
            break;
        }
        input += c;
    }

    return input;
}


MessageContainer MessageParser::parseString(const String& input) {
    MessageContainer::Message messagesArray[MAX_MESSAGES];
    size_t numMessages = 0;

    String token;
    int tokenStart = 0;
    for (size_t i = 0; i < input.length(); i++) {
        if (input.charAt(i) == ';') {
            
            token = input.substring(tokenStart, i);

            String messageTypeStr = token.substring(0, 2);
            float value = token.substring(2).toFloat();

            Serial.print("parsed message type: ");
            Serial.println(messageTypeStr);
            Serial.print("parsed value: ");
            Serial.println(value);

            if (messageTypeStr.length() == 2 && numMessages < MAX_MESSAGES) {
                messageTypeStr.toCharArray(messagesArray[numMessages].messageType, 3);
                messagesArray[numMessages].value = value;
                numMessages++;
            }

            tokenStart = i + 1;
        }
    }
    
    return MessageContainer(messagesArray, numMessages);
}
