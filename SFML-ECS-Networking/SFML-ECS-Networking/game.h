#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <optional>
#include <memory>
#include "Tank.h"
#include "tank_message.h"

class Game {
public:
    Game();

    void InitLocalTank(int role);
    void HandleEvents(const std::optional<sf::Event>& event);
    void Update(float dt);
    void UpdateRemoteTank(TankMessage data);
    void Render(sf::RenderWindow& window, float currentNetTime);

    TankMessage GetNetworkUpdate();

    Tank& GetLocalTank() { return *tank; }

private:
    std::unique_ptr<Tank> tank; 
    int role = 0;

    std::unique_ptr<sf::Texture> backgroundTexture;
    std::unique_ptr<sf::Sprite> background;

    std::map<int, std::unique_ptr<Tank>> remoteTanks;

    const float interpolationDelay = 0.1f;
};