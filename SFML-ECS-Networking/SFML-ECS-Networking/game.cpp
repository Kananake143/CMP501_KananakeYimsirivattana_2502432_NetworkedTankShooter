#include "game.h"
#include "utils.h"

Game::Game() {

    backgroundTexture = std::make_unique<sf::Texture>();
    if (backgroundTexture->loadFromFile("Assets/tileSand1.png")) {
        backgroundTexture->setRepeated(true);
        background = std::make_unique<sf::Sprite>(*backgroundTexture);
        background->setTextureRect(sf::IntRect({ 0, 0 }, { 800, 600 }));
    }
    tank = std::make_unique<Tank>("red", 1);
}

void Game::InitLocalTank(int role) {
    this->role = role;
    if (role == 1) {
        tank = std::make_unique<Tank>("red", 1);
        tank->position = { 400.f, 500.f };
        tank->barrelRotation = 270.f; 
    }
    else if (role == 2) {
        tank = std::make_unique<Tank>("green", 2);
        tank->position = { 400.f, 100.f };
        tank->barrelRotation = 90.f;  
    }
}

void Game::HandleEvents(const std::optional<sf::Event>& event) {
    if (!event || !tank) return;

    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        if (role == 1) {
            if (keyPressed->scancode == sf::Keyboard::Scancode::W) tank->input.up = true;
            if (keyPressed->scancode == sf::Keyboard::Scancode::S) tank->input.down = true;
            if (keyPressed->scancode == sf::Keyboard::Scancode::A) tank->input.left = true;
            if (keyPressed->scancode == sf::Keyboard::Scancode::D) tank->input.right = true;
        }
        else if (role == 2) {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Up) tank->input.up = true;
            if (keyPressed->scancode == sf::Keyboard::Scancode::Down) tank->input.down = true;
            if (keyPressed->scancode == sf::Keyboard::Scancode::Left) tank->input.left = true;
            if (keyPressed->scancode == sf::Keyboard::Scancode::Right) tank->input.right = true;
        }
    }

    if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {
        if (role == 1) {
            if (keyReleased->scancode == sf::Keyboard::Scancode::W) tank->input.up = false;
            if (keyReleased->scancode == sf::Keyboard::Scancode::S) tank->input.down = false;
            if (keyReleased->scancode == sf::Keyboard::Scancode::A) tank->input.left = false;
            if (keyReleased->scancode == sf::Keyboard::Scancode::D) tank->input.right = false;
        }
        else if (role == 2) {
            if (keyReleased->scancode == sf::Keyboard::Scancode::Up) tank->input.up = false;
            if (keyReleased->scancode == sf::Keyboard::Scancode::Down) tank->input.down = false;
            if (keyReleased->scancode == sf::Keyboard::Scancode::Left) tank->input.left = false;
            if (keyReleased->scancode == sf::Keyboard::Scancode::Right) tank->input.right = false;
        }
    }
}

void Game::UpdateRemoteTank(TankMessage data) {
    // If the tank ID doesn't exist in our map, instantiate a new remote tank.
    if (remoteTanks.find(data.id) == remoteTanks.end()) {
        std::string color = (data.id == 1) ? "red" : "green";
        remoteTanks[data.id] = std::make_unique<Tank>(color, data.id);
        Utils::printMsg("Added remote tank ID " + std::to_string(data.id), MessageType::success);
    }

    auto& rTank = remoteTanks[data.id];
    // Push the latest network state into the buffer for interpolation.
    rTank->positionBuffer.push_back({ {data.x, data.y}, data.bodyRotation, data.barrelRotation, data.timestamp });
    // Handle firing events immediately as they are discrete actions.
    if (data.isFiring) {
        rTank->position = { data.x, data.y }; 
        rTank->barrelRotation = data.barrelRotation; 
        rTank->Fire();
    }
}

void Game::Update(float dt) {
    if (role != 3 && tank) tank->Update(dt);
    for (auto& [id, rTank] : remoteTanks) {
        if (rTank) rTank->Update(dt);
    }
}

void Game::Render(sf::RenderWindow& window, float currentNetTime) {
    // Determine the specific point in the past to render remote entities.
    float renderTime = currentNetTime - interpolationDelay; 

    if (background) window.draw(*background);
    // Local tank is rendered using immediate local state (Authoritative).
    if (role != 3 && tank) {
        tank->Render(window);
    }
    // Remote tanks are rendered using interpolated states to mask network jitter.
    for (auto& [id, rTank] : remoteTanks) {
        if (rTank) {
            rTank->ApplyInterpolation(renderTime);
            rTank->Render(window);
        }
    }
}
TankMessage Game::GetNetworkUpdate() {
    TankMessage m;
    if (tank) { 
        m.id = this->role;
        m.x = tank->position.x;      
        m.y = tank->position.y;     
        m.bodyRotation = tank->bodyRotation;
        m.barrelRotation = tank->barrelRotation;
    }
    else {
        Utils::printMsg("Local tank is NULL in GetNetworkUpdate", MessageType::error);
    }
    return m;
}