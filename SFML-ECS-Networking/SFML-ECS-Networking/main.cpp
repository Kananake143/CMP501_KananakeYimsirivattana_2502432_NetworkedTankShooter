#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <map>
#include <optional> 
#include "tank_message.h"
#include "game.h"
#include "utils.h"

struct ClientAddr {
    sf::IpAddress ip;
    unsigned short port;

    ClientAddr(sf::IpAddress _ip, unsigned short _port) : ip(_ip), port(_port) {}
};

int main() {
    std::cout << "Select Role: 1.P1 (Red), 2.P2 (Green), 3.Server: ";
    int role;
    if (!(std::cin >> role)) return -1;

    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "CMP501 Tank - Role: " + std::to_string(role));

    Game game;
    game.InitLocalTank(role); 

    sf::Clock clock;

    sf::UdpSocket socket;
    unsigned short localPort = (role == 3) ? 5000 : (5000 + role);

    if (socket.bind(localPort) != sf::Socket::Status::Done) {
        Utils::printMsg("Failed to bind port " + std::to_string(localPort), MessageType::error);
        return -1;
    }
    socket.setBlocking(false);

    auto resolvedIp = sf::IpAddress::resolve("127.0.0.1");
    if (!resolvedIp) {
        Utils::printMsg("Could not resolve server IP", MessageType::error);
        return -1;
    }
    sf::IpAddress serverIp = resolvedIp.value();
    unsigned short serverPort = 5000;

    std::map<int, ClientAddr> clients;
    sf::Clock netClock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        float currentNetTime = clock.getElapsedTime().asSeconds();

        // --- Handle Events ---
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            game.HandleEvents(event);
        }
       
        // --- 1. Send Data ---
        if (role == 1 || role == 2) {
            TankMessage m = game.GetNetworkUpdate();
            m.id = role;
            m.timestamp = currentNetTime;

            if (m.x == 0.0f && m.y == 0.0f) {
                Utils::printMsg("Warning: Sending Zero Position! Check GetNetworkUpdate", MessageType::warning);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up)) {
                std::string debugStr = "Sending Pos: " + std::to_string(m.x) + ", " + std::to_string(m.y);
                Utils::printMsg(debugStr, MessageType::debug);
            }
            if (role == 1)
                m.isFiring = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space);
            else if (role == 2)
                m.isFiring = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Enter);

            if (m.isFiring) game.GetLocalTank().Fire();

            sf::Packet p;
            p << m;
            // Asynchronous send via UDP - efficiency over reliability
            socket.send(p, serverIp, serverPort);
        }

       

        // --- 2. Receive Data---
        sf::Packet rp;
        std::optional<sf::IpAddress> senderIp;
        unsigned short senderPort;

        while (socket.receive(rp, senderIp, senderPort) == sf::Socket::Status::Done) {
            TankMessage d;

            if (rp >> d && senderIp) {
                if (role == 3) { // Logic  SERVER
                    // Register new client address upon first packet arrival.
                    if (clients.find(d.id) == clients.end()) {
                        clients.emplace(d.id, ClientAddr(senderIp.value(), senderPort));
                        Utils::printMsg("Client " + std::to_string(d.id) + " connected.", MessageType::success);
                    }

                    game.UpdateRemoteTank(d);
                    // Relay the received packet to all clients except the original sender.
                    for (auto const& [id, addr] : clients) {
                        if (id != d.id) {
                            socket.send(rp, addr.ip, addr.port);
                        }
                    }
                }
                else if (d.id != role) { 
                    // CLIENT LOGIC: Update the local representation of remote entities.
                    game.UpdateRemoteTank(d);
                }
            }
        }

        game.Update(dt);

        window.clear(sf::Color(50, 50, 50)); 
        game.Render(window, currentNetTime);
        window.display();
    }

    return 0;
}