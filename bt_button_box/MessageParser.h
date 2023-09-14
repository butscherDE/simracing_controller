#ifndef MESSAGE_PARSER_H
#define MESSAGE_PARSER_H

#include <Arduino.h>
#include "MessageContainer.h"

class MessageParser {
public:
    static String readStringFromSerial();

    static MessageContainer parseString(const String& input);
};

#endif
