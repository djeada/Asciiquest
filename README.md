# Mysterious-dungeon

Mysterious-dungeon is a roguelike dungeon game, where you play as a hero, trying to find the exit. The enemies are randomly generated, and you can fight them, or try to sneak past them. With each level, the enemies are getting more difficult to defeat. You can also use the items in the dungeon to help you. The game is designed to be played in a terminal, but there are plans to make it work in a browser as well.

## Screenshot

![new_screenshot](https://user-images.githubusercontent.com/37275728/186153359-84890588-1989-40dd-8da3-a0665ca130d4.png)

## Features

* Maze generation
* Multiplayer
* Multithreaded 
* Pathfinding graph algorithms
* Enemy AI
* Multilevel

## Installation

In order to play the game, you need to install the following dependencies:

    * C++14
    * Ncurses
    * CMake 3.9 or higher
    
To compile the game using CMake, run the following commands:

    git clone https://github.com/djeada/Mysterious-dungeon.git
    cd Mysterious-dungeon
    mkdir build && cd build
    cmake ..
    make

## Usage

After the game is compiled, you can play it by running the following command:

    build/bin/mysterious-dungeon

The game is controlled only by the keyboard. The following keys are available:

    * To move, use the arrow keys or WASD
    * To attack, press the spacebar 
    * To quit, press q or ESC

## Game design

![game_design](https://user-images.githubusercontent.com/37275728/186153392-92685d9b-f267-4779-9157-ed41e56867f6.jpg)

## Contributing
It is an open source project, so feel free to contribute!

## License
This project is licensed under the <a href="https://github.com/djeada/Mysterious-dungeon/blob/master/LICENSE">MIT license</a>.
