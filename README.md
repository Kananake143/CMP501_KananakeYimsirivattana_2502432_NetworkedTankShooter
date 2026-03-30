# CMP501: Networked Tank Shooter (UDP Implementation)
Name : Kananake Yimsirivattana 2502432
A real-time multiplayer tank game developed using **C++** and **SFML**. This project focuses on high-performance networking, utilizing **UDP** and **Entity Interpolation** to handle network jitter and latency.

## 🛠 Features
- **Custom UDP Protocol:** Low-latency communication using `sf::UdpSocket`.
- **Server-Relay Architecture:** Centralized server to synchronize game state between clients.
- **Entity Interpolation:** 100ms jitter buffer for smooth remote player movement.
- **Local Authoritative Movement:** Instant responsiveness for the local player.

### Player 1 (Red Tank)
- **W / S:** Move Forward / Backward
- **A / D:** Rotate Tank Body (Left / Right)
- **Mouse Movement:** Aim Tank Barrel
- **Spacebar:** Fire Projectile

### Player 2 (Green Tank)
- **Arrow Keys (Up/Down):** Move Forward / Backward
- **Arrow Keys (Left/Right):** Rotate Tank Body
- **Mouse Movement:** Aim Tank Barrel
- **Enter:** Fire Projectile
*(Note: Controls are processed locally depending on the selected Role.)*

## 🚀 How to Build and Run
### Prerequisites
- Visual Studio 2022
- SFML 2.6 or 3.0 (ensure DLLs are in the output directory)

### Running the Game
1. **Start the Server:** Run the executable and select **Role 3**. (Default Port: 5000)
2. **Start Client 1:** Run a second instance and select **Role 1**.
3. **Start Client 2:** Run a third instance and select **Role 2**.

## 📁 Project Structure
- `main.cpp`: Entry point, handles the network loop and role selection.
- `Game.cpp / .h`: Manages game logic, local/remote tank updates, and rendering.
- `Tank.cpp / .h`: Contains the tank controller and **Interpolation logic**.
- `tank_message.h`: Defines the `TankMessage` struct for network packets.

## 📁 Technical Features
- **Custom UDP Protocol:** Low-latency communication via `sf::UdpSocket`.
- **Server-Relay Architecture:** Centralized server for world state synchronization.
- **Entity Interpolation:** 100ms jitter buffer for smooth remote movement.
- **Local Authoritative Movement:** Instant responsiveness for the active player.

## 🧪 Testing
The system has been stress-tested using **Clumsy 0.2** to simulate:
- Latency (100ms - 200ms)
- Packet Loss (5% - 10%)

## 📚 References
- Gaffer on Games (UDP vs TCP)
- Valve Developer Community (Multiplayer Networking)
