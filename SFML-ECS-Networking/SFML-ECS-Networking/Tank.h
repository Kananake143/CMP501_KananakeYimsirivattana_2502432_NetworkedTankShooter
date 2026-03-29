#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <deque>
#include <memory>
#include "tank_message.h"

class Tank {
public:
    Tank(std::string colour = "red", int id = 1); 

    void Update(float dt);
    void Render(sf::RenderWindow& window);
    void Fire();

    struct RemoteState {
        sf::Vector2f position;
        float bodyRot;
        float barrelRot;
        float timestamp;
    };

    std::deque<RemoteState> positionBuffer;
    void ApplyInterpolation(float renderTime);

    sf::Vector2f position;
    float bodyRotation = 0.f;
    float barrelRotation = 0.f;

    struct {
        bool up = false, down = false, left = false, right = false;
    } input;

private:
    sf::Texture bodyTexture;
    sf::Texture barrelTexture;
    sf::Texture bulletTexture;

    std::unique_ptr<sf::Sprite> body;
    std::unique_ptr<sf::Sprite> barrel;

    struct Bullet { sf::Vector2f pos; float angle; };
    std::vector<Bullet> bullets;
    float movementSpeed = 200.f;
    int tankId;
};