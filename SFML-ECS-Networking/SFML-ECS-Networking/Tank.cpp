#include "Tank.h"
#include <cmath>
#include "utils.h"

Tank::Tank(std::string colour, int id) : tankId(id)
{
    if (!bodyTexture.loadFromFile("Assets/" + colour + "Tank.png")) {
        Utils::printMsg("Failed to load body", MessageType::error);
    }
    if (!barrelTexture.loadFromFile("Assets/" + colour + "Barrel.png")) {
        Utils::printMsg("Failed to load barrel", MessageType::error);
    }
    if (!bulletTexture.loadFromFile("Assets/bullet.png")) {
        Utils::printMsg("Failed to load bullet", MessageType::error);
    }

    body = std::make_unique<sf::Sprite>(bodyTexture);
    barrel = std::make_unique<sf::Sprite>(barrelTexture);

    sf::Vector2f bodySize = sf::Vector2f(bodyTexture.getSize());
    body->setOrigin(bodySize / 2.f);
    barrel->setOrigin({ 6.f, 2.f });

    if (id == 1) {
        barrelRotation = 270.f; 
    }
    else {
        barrelRotation = 90.f; 
    }
}

void Tank::Update(float dt) {
    if (input.up)    position.y -= movementSpeed * dt;
    if (input.down)  position.y += movementSpeed * dt;
    if (input.left)  position.x -= movementSpeed * dt;
    if (input.right) position.x += movementSpeed * dt;

    for (auto it = bullets.begin(); it != bullets.end();) {
        float rad = it->angle * 3.14159f / 180.f;
        it->pos.x += std::cos(rad) * 400.f * dt;
        it->pos.y += std::sin(rad) * 400.f * dt;

        if (it->pos.x < 0 || it->pos.x > 800 || it->pos.y < 0 || it->pos.y > 600) {
            it = bullets.erase(it);
        }
        else {
            ++it;
        }
    }
}

void Tank::Fire() {
    bullets.push_back({ position, barrelRotation });
}

void Tank::Render(sf::RenderWindow& window) {
    if (!body || !barrel) return;

    if (!positionBuffer.empty()) {
        sf::Vector2f latestPos = positionBuffer.back().position;
        float latestBodyRot = positionBuffer.back().bodyRot;
        float latestBarrelRot = positionBuffer.back().barrelRot;

        body->setPosition(latestPos);
        body->setRotation(sf::degrees(latestBodyRot));
        barrel->setPosition(latestPos);
        barrel->setRotation(sf::degrees(latestBarrelRot));
    }
    else {
        body->setPosition(position);
        body->setRotation(sf::degrees(bodyRotation));
        barrel->setPosition(position);
        barrel->setRotation(sf::degrees(barrelRotation));
    }

    window.draw(*body);
    window.draw(*barrel);

    for (auto& b : bullets) {
        sf::Sprite bSprite(bulletTexture);
        sf::Vector2f bSize = sf::Vector2f(bulletTexture.getSize());
        bSprite.setOrigin(bSize / 2.f);
        bSprite.setPosition(b.pos);
        bSprite.setRotation(sf::degrees(b.angle));
        window.draw(bSprite);
    }
}

void Tank::ApplyInterpolation(float renderTime) {
    if (positionBuffer.empty()) return;

    if (positionBuffer.size() > 8) renderTime += 0.05f;

    while (positionBuffer.size() >= 2 && positionBuffer[1].timestamp < renderTime) {
        positionBuffer.pop_front();
    }

    if (positionBuffer.size() >= 2) {
        const auto& p0 = positionBuffer[0];
        const auto& p1 = positionBuffer[1];
        float t = (renderTime - p0.timestamp) / (p1.timestamp - p0.timestamp);
        t = std::max(0.f, std::min(1.f, t));

        position = p0.position + t * (p1.position - p0.position);
        bodyRotation = p0.bodyRot + t * (p1.bodyRot - p0.bodyRot);
        barrelRotation = p0.barrelRot + t * (p1.barrelRot - p0.barrelRot);
    }
}