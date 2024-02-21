# Asciiquest

Asciiquest is an ASCII-based roguelike game. As the hero `@`, you're tasked with navigating through a randomly generated dungeon, filled with fearsome enemies and treacherous traps. Along the journey, you'll uncover pieces of a secret story, shaping the destiny of your character. The game can be played in a terminal and features AI enemies, real-time combat, dynamic environment interactions, multiplayer capability, and a compelling narrative.

## Screenshot

![Peek 2023-07-27 21-58](https://github.com/djeada/Mysterious-Dungeon/assets/37275728/14089291-aaea-4272-a368-27fb6dfd8908)

## Features

* Procedural Dungeon Generation: Each game is a new challenge with levels generated on-the-fly.
* Real-Time Gameplay: Keep moving, thinking, and fighting your way out of dangerous situations.
* AI Enemies: Face off against a variety of enemies each with unique characteristics and behaviours.
* Environment Interactions: Use your surroundings to your advantage, break walls, shoot arrows, set traps, and cast magic spells.
* Multiplayer Support: Enjoy cooperative gameplay over LAN or Internet.
* Hidden Story: Uncover a secret narrative as you progress through the levels, adding another layer of depth to your quest.

## System requirements

To play Mysterious Dungeon, you need:

* C++17 compiler
* NCurses library
* CMake 3.15 or higher
* Knowledge of networking for multiplayer (LAN / Internet)

## Building and running the game

To build and run Mysterious Dungeon, follow these steps:

1. Clone the repository: `git clone https://github.com/djeada/Asciiquest.git`
1. Navigate to the project directory: `cd Asciiquest`
1. Create a build directory: `mkdir build`
1. Navigate to the build directory: `cd build`
1. Run CMake: `cmake ..`
1. Build the game: `make`
1. Run the game: `./exe`

## How to play

The game is controlled using the keyboard. Use the arrow keys or WASD to move the player. Press the spacebar to attack enemies. Press q or ESC to quit the game. Encounter enemies and items as you explore the dungeon. The goal is to find the exit and advance to the next level.

## Game design

Mysterious Dungeon combines elements of classic roguelike games with modern algorithms and AI techniques. The dungeon maze, generated with advanced algorithms, creates a unique experience for every game. The enemies, imbued with AI and pathfinding, provide a dynamic challenge. Each level introduces new gameplay elements and tougher enemies, ensuring an engaging experience throughout the game.

![game_design](https://user-images.githubusercontent.com/37275728/186153392-92685d9b-f267-4779-9157-ed41e56867f6.jpg)

Each AI entity independently requests a path and performs tasks while the path is being calculated. Once the path is ready, the entity is notified and can start following the path. This approach optimizes performance and avoids the main thread from being blocked.

## Contributing

Mysterious Dungeon is an open-source project. We welcome contributions from the community! Whether it's bug fixes, new features, or improvements to existing code, your contributions are appreciated. Please open an issue or submit a pull request with your proposed changes.

## License

This project is licensed under the [MIT license](https://github.com/djeada/Mysterious-dungeon/blob/master/LICENSE).
