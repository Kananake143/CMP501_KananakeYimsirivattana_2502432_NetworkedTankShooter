#include "tank_message.h"

sf::Packet& operator <<(sf::Packet& packet, const TankMessage& m) {
    return packet << m.id << m.x << m.y << m.bodyRotation << m.barrelRotation
        << m.isFiring << m.sequence_number << m.timestamp;
}

sf::Packet& operator >>(sf::Packet& packet, TankMessage& m) {
    return packet >> m.id >> m.x >> m.y >> m.bodyRotation >> m.barrelRotation
        >> m.isFiring >> m.sequence_number >> m.timestamp;
}