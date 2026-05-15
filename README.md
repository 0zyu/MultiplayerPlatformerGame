# Multiplayer Platformer Game

A 2D multiplayer platformer built in C++ using SDL3 and ENet.  
The project focuses on real-time networking, game architecture, and responsive platforming mechanics rather than complex graphics.

---

## Features

- Real-time multiplayer gameplay
- Server-authoritative networking architecture
- Player movement and jumping physics
- Rolling mechanic
- Enemy collision system
- Moving platforms
- Collectible coins
- Animated sprites
- Parallax scrolling background
- SDL3 rendering pipeline
- Delta time based movement

---



## Tech Stack

- C++
- SDL3
- SDL3_image
- SDL3_ttf
- ENet

---

## Project Structure

```text
Platformer/
│
├── assets/
├── src/
├── include/
├── main.cpp
└── README.md
```

---

## Networking Architecture

The game is being developed using a server-authoritative multiplayer model.

### Planned Networking Features

- Dedicated game server
- Real-time player position synchronization
- Client-side prediction
- Interpolation/smoothing
- Packet handling with ENet
- Multiplayer race system

The main goal of the project is to develop strong understanding of multiplayer game networking and scalable game architecture.

---

## Controls

| Key | Action |
|------|---------|
| A / D | Move |
| Space | Jump |
| W | Roll |

---

## Installation

### Clone the repository

```bash
git clone https://github.com/0zyu/MultiplayerPlatformerGame.git
```

### Requirements

- Visual Studio 2022
- SDL3
- SDL3_image
- SDL3_ttf
- ENet

### Build

1. Open the solution in Visual Studio
2. Configure SDL3 include/lib directories
3. Build and run

---

## Future Plans

- Online multiplayer support
- Matchmaking/lobbies
- Improved enemy AI
- Sound effects and music
- Additional levels
- Better UI and menus
- Steam release
- Custom animations and effects

---

## Assets & Licensing

Some assets used during development are temporary placeholders.

Current sprite assets from Brackeys are licensed under CC0 and are allowed for commercial use.

Any non-commercial placeholder assets will be replaced before release.

---

## Goals

This project is primarily being developed to:

- Improve C++ programming skills
- Learn multiplayer networking
- Build scalable game systems
- Develop a strong software engineering portfolio project

---

## Author

Ben McIlroy

GitHub: https://github.com/0zyu
