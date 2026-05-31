# Dungeon Crawler RPG — C++ Lab Project

## Overview

In this project you will build a text-based dungeon crawler RPG that exercises the core object-oriented programming concepts covered in lecture.  You will work with class hierarchies, polymorphism, STL containers, and dynamic memory management to create a playable game.

This is a **2-week** lab.  The starter code provides substantial scaffolding — constructors, helper functions, and detailed TODO comments — so you can focus on understanding the C++ concepts rather than software architecture.

## Schedule

| Milestone | Date | What to complete |
|-----------|------|-----------------|
| Lab released | Monday, April 13 | Read README, TODO, and BUILD_INSTRUCTIONS |
| **Week 1 checkpoint** | Monday, April 20 | Character, Item, Player, and Monster classes compiling|
| **Final submission** | Monday, April 27 | Complete playable game, valgrind clean |

## Getting Started

### Compilation

```bash
make          # Build the project
make clean    # Remove build files
make rebuild  # Clean and rebuild from scratch
```

### Running the Game

```bash
./bin/rpg_game
```

A fully compiled reference solution is also provided at `bin/dungeon_rpg.solution` (runs on mario.ece.utexas.edu) so you can see the expected behavior.

## Project Structure

```
starter_code/
├── Documentation/
│   ├── README.md              ← You are here
│   ├── TODO.md                ← Step-by-step implementation guide
│   └── BUILD_INSTRUCTIONS.md  ← Compilation & debugging help
├── Makefile
├── bin/
│   └── dungeon_rpg.solution   ← Reference executable
├── include/
│   ├── Character.h            ← Base character class
│   ├── Player.h               ← Player (inherits Character)
│   ├── Monster.h              ← Monster hierarchy
│   ├── Item.h                 ← Item hierarchy
│   ├── Room.h                 ← Dungeon rooms
│   └── Game.h                 ← Game controller
└── src/
    ├── main.cpp               ← COMPLETE — no changes needed
    ├── Character.cpp           ← Has TODOs
    ├── Player.cpp              ← Has TODOs
    ├── Monster.cpp             ← Has TODOs
    ├── Item.cpp                ← Has TODOs
    ├── Room.cpp                ← Has TODOs
    └── Game.cpp                ← Has TODOs (many functions provided)
```

## Your Tasks — Summary

Look for `TODO` comments throughout the `.cpp` files.  The TODOs include detailed hints and, in many cases, the answer in commented-out form.

### Week 1: Classes and Inheritance (April 13 – 19)

1. **Character.cpp** — `calculateDamage`, `takeDamage`, `heal`, `displayStats`, `displayStatus`
2. **Item.cpp** — `displayInfo` overrides for Weapon / Armor / Consumable, `use()` for Consumable
3. **Player.cpp** — destructor, `calculateDamage`, `equipWeapon`, `equipArmor`, `useItem`, `gainExperience`, `levelUp`
4. **Monster.cpp** — destructor, `displayStats`, `dropLoot`, `getAttackMessage` overrides, Dragon's `calculateDamage`

### Week 2: Rooms and Game Integration (April 20 – 27)

1. **Room.cpp** — destructor, `displayExits`, `getExit`, `hasExit`, `clearMonster`
2. **Game.cpp** — destructor, `addRoom`, `connectRooms`, `run` (game loop), `move`, `combat` (victory check), `pickupItem`, `equip`

## Game Commands

Once the game is running:

- `go <direction>` — Move north / south / east / west
- `look` — Examine current room
- `attack` — Fight the monster in the room
- `pickup <item>` — Take an item from the room
- `inventory` — View your items
- `equip <item>` — Equip a weapon or armor
- `use <item>` — Use a consumable item
- `stats` — View character statistics
- `help` — Show all commands
- `quit` — Exit game

## C++ Concepts Demonstrated

### Object-Oriented Programming
- **Inheritance**: Character → Player / Monster; Item → Weapon / Armor / Consumable
- **Polymorphism**: virtual `calculateDamage`, `displayInfo`, `getAttackMessage`
- **Encapsulation**: private / protected / public members

### STL Containers
- `std::vector` — dynamic arrays for inventory and loot
- `std::map` — room connections keyed by direction strings
- `std::string` — string manipulation and comparison

### Memory Management
- Dynamic allocation with `new` / `delete`
- Destructor chains for cleanup
- Ownership semantics (who deletes what)

## Grading (100 points)

| Category | Points | Description |
|----------|--------|-------------|
| Compiles without errors | 10 | `make` succeeds with no warnings |
| Correct functionality | 45 | All game features work as specified |
| Memory management | 25 | `valgrind --leak-check=full` shows no leaks |
| Code quality | 20 | Clean code, good comments, proper style |


Please upload all of your files to Gradescope and test there to ensure your submission is complete.  You can submit multiple times before the deadline, and only your last submission will be graded. We will also be conducting a code review and plagiarism check, so make sure your code is your own work and properly cited if you used any references.



## Testing

### Memory Leak Check

```bash
valgrind --leak-check=full ./bin/rpg_game
```

Play through and quit normally — should show "no leaks are possible".

## Common Issues

**Segmentation fault when picking up items?**
→ Check for NULL pointers before dereferencing.

*Memory leaks in valgrind?**
→ Every `new` must have a matching `delete`.  Check your destructors!

**Items not found in inventory?**
→ String comparison is case-sensitive.  The helper `toLower()` is provided.

**Combat never ends?**
→ Make sure you check `isAlive()` after each attack.

## Academic Integrity

This is an individual project.  You may:
- ✅ Discuss general concepts with classmates
- ✅ Use C++ reference documentation
- ✅ Ask TAs / instructor for help

You may NOT:
- ❌ Share code with classmates
- ❌ Copy code from the internet
- ❌ Submit someone else's work

Good luck, adventurer!
