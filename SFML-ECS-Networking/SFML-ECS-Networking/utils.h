#pragma once
#include <string>
#include <iostream>

enum class MessageType { info, debug, warning, error, success };

class Utils
{
public:
    static void printMsg(std::string msg, MessageType type = MessageType::info) {
        switch (type)
        {
        case MessageType::debug:   std::cout << "[DEBUG] " << msg << std::endl; break;
        case MessageType::error:   std::cout << "[ERROR] " << msg << std::endl; break;
        case MessageType::success: std::cout << "[SUCCESS] " << msg << std::endl; break;
        case MessageType::warning: std::cout << "[WARNING] " << msg << std::endl; break;
        default:                   std::cout << "[INFO] " << msg << std::endl; break;
        }
    }
};