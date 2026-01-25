# Asciiquest

Asciiquest is an ASCII-based roguelike game. As the hero `@`, you're tasked with navigating through a randomly generated dungeon, filled with fearsome enemies and treacherous traps. Along the journey, you'll uncover pieces of a secret story, shaping the destiny of your character. The game can be played in a terminal and features AI enemies, real-time combat, dynamic environment interactions, multiplayer capability, and a compelling narrative.

## Screenshot

![screenshot](https://github.com/user-attachments/assets/23939c75-d47c-40dd-83b5-9686515d8a7e)

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
1. Build the game: `make`
1. Run the game: `make run`

Other useful commands:
- Install: `make install` (use `PREFIX=/path` to change the install location)
- Clean build artifacts: `make clean` or `make distclean`

## How to play

The game is controlled using the keyboard:
- **Movement**: Use the arrow keys or WASD to move the player.
- **Combat**: Walk into enemies to engage in melee combat.
- **Magic**: Press number keys 1-5 to cast spells in your facing direction:
  - **1**: Fireball (*) - A powerful offensive fire spell (30 damage, 20 mana)
  - **2**: Ice Shard (|) - An offensive ice spell (25 damage, 20 mana)
  - **3**: Heal (+) - Restores health (50 HP, 30 mana)
  - **4**: Lightning Bolt (-) - A powerful lightning attack (40 damage, 35 mana)
  - **5**: Shield (#) - Creates a protective barrier (0 damage, 25 mana)
- **Messages**: Press I/K to scroll through message history.
- **Pause**: Press P to pause the game.
- **Quit**: Press Q or ESC to quit the game.

### Tile Legend

| Symbol | Description | Color |
|--------|-------------|-------|
| `@` | Player (you!) | White (bold/reverse) |
| `.` | Floor | Gray (muted background) |
| `#` | Wall | Light Gray |
| `+` | Door | Green (bold) |
| `<` | Stairs up (entrance) | Cyan (bold) |
| `>` | Stairs down (exit) | Cyan (bold) |
| `$` | Treasure | Yellow (bold) |
| `!` | Potion | Cyan (bold) |
| `g` | Goblin | Red (bold) |
| `o` | Orc | Red (bold) |
| `T` | Troll | Magenta (bold) |
| `D` | Dragon | Red (bold) |
| `s` | Skeleton | Magenta (bold) |
| `,` | Grass | Green |
| `~` | Water | Blue |
| `^` | Spike Trap | Magenta |
| `/` | Blade Trap | Red |
| `=` | Arrow Trap | Red |

**Visual Effects:**
- **Fog-of-War**: Tiles beyond your vision radius appear dimmed
- **Player Halo**: Adjacent tiles are brightened for easy player location
- **Interactive Highlighting**: Items and enemies near you stand out

Encounter enemies and items as you explore the dungeon. The goal is to find the exit and advance to the next level. Manage your mana wisely and use spells strategically to defeat powerful enemies! Spells are cast in the direction you last moved, allowing for tactical positioning.

### Enemy AI Behavior

Each enemy type has unique AI behavior and detection range:

- **Goblin (g)**: Small and quick with limited vision. Follows player within **5 tiles** using simple direct movement. Moves randomly when player is out of range.
- **Skeleton (s)**: Undead creature with erratic behavior. Follows player within **8 tiles** with 70% reliability (30% chance of random movement even when tracking). Moves randomly when player is out of range.
- **Orc (o)**: Intelligent enemy with moderate tracking. Uses **A* pathfinding** to follow player within **10 tiles**, navigating around obstacles. Stands still when player is out of range.
- **Troll (T)**: Large creature with excellent vision. Follows player within **15 tiles** using simple direct movement. Moves randomly when player is out of range.
- **Dragon (D)**: Powerful boss that **never moves**. Guards its position and attacks only when the player approaches.

## Game design

Mysterious Dungeon combines elements of classic roguelike games with modern algorithms and AI techniques. The dungeon maze, generated with advanced algorithms, creates a unique experience for every game. The enemies, imbued with AI and pathfinding, provide a dynamic challenge. Each level introduces new gameplay elements and tougher enemies, ensuring an engaging experience throughout the game.

![game_design](https://user-images.githubusercontent.com/37275728/186153392-92685d9b-f267-4779-9157-ed41e56867f6.jpg)

Each AI entity independently requests a path and performs tasks while the path is being calculated. Once the path is ready, the entity is notified and can start following the path. This approach optimizes performance and avoids the main thread from being blocked.

## Contributing

Mysterious Dungeon is an open-source project. We welcome contributions from the community! Whether it's bug fixes, new features, or improvements to existing code, your contributions are appreciated. Please open an issue or submit a pull request with your proposed changes.

## License

This project is licensed under the [MIT license](https://github.com/djeada/Mysterious-dungeon/blob/master/LICENSE).
