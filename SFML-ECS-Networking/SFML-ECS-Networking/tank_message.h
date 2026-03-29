#pragma once
#include <SFML/Network.hpp>

struct TankMessage {
    int id = 0;
    float x = 0.f;
    float y = 0.f;
    float bodyRotation = 0.f;    
    float barrelRotation = 0.f;
    bool isFiring = false;
    int sequence_number = 0;
    float timestamp = 0.f;
};

sf::Packet& operator <<(sf::Packet& packet, const TankMessage& m);
sf::Packet& operator >>(sf::Packet& packet, TankMessage& m);