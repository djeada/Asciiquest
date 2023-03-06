# Mysterious Dungeon

Mysterious Dungeon is a roguelike dungeon game in which you play as a hero trying to find the exit of a randomly generated dungeon. The game features enemy AI, pathfinding algorithms, multithreading, and the ability to play multiplayer. As you progress through the levels, the enemies will become more difficult to defeat. You can use items found in the dungeon to help you on your quest. The game is designed to be played in a terminal, but there are plans to make it work in a browser as well.

## Screenshot

![new_screenshot](https://user-images.githubusercontent.com/37275728/186153359-84890588-1989-40dd-8da3-a0665ca130d4.png)

## Features

* Maze generation using advanced algorithms
* Multiplayer support for cooperative play
* Multithreaded design for faster gameplay
* Pathfinding and AI algorithms for intelligent enemy behavior
* Multilevel gameplay with progressively challenging enemies

## System requirements

To play Mysterious Dungeon, you will need to have C++14 and Ncursed installed, as well as CMake 3.9 or higher.

## Building and running the game

To build and run Mysterious Dungeon, follow these steps:

1. Clone the repository: `git clone https://github.com/djeada/Mysterious-dungeon.git`
1. Navigate to the project directory: `cd Mysterious-dungeon`
1. Create a build directory: `mkdir build`
1. Navigate to the build directory: `cd build`
1. Run CMake: `cmake ..`
1. Build the game: `make`
1. Run the game: `./mysterious-dungeon`

## How to play

The game is controlled using the keyboard. Use the arrow keys or WASD to move the player. Press the spacebar to attack enemies. Press q or ESC to quit the game. As you explore the dungeon, you will encounter randomly generated enemies and items that can help you on your quest. The goal is to find the exit and advance to the next level.

## Game design

Mysterious Dungeon features a unique game design that combines elements of classic roguelike games with advanced algorithms and AI techniques. The maze is generated using state-of-the-art algorithms, and the enemies use pathfinding and AI to navigate the dungeon and engage in combat with the player. The game is designed to be challenging and engaging, with progressively more difficult enemies and new gameplay elements introduced as you advance through the levels.

![game_design](https://user-images.githubusercontent.com/37275728/186153392-92685d9b-f267-4779-9157-ed41e56867f6.jpg)

The AI entity sends a request for a path and then continues to perform other tasks while the pathfinding algorithm calculates the path in the background. Once the path is found, the AI entity is notified and can start following the calculated path. This approach helps to improve performance and avoid freezing the main thread while the path is being calculated.

## Contributing
Mysterious Dungeon is an open source project, and we welcome contributions from the community! If you are interested in making a contribution, please open an issue or submit a pull request with your proposed changes. Contributions could include bug fixes, new features, or improvements to existing code.

## License
This project is licensed under the <a href="https://github.com/djeada/Mysterious-dungeon/blob/master/LICENSE">MIT license</a>.
