# RPG Lab — Implementation Guide (TODO)

This document provides step-by-step tasks for implementing the RPG game. Complete Week 1 before starting Week 2.

---

## Week 1: Classes and Inheritance (April 13 – 19)

**Time Estimate**: 8–10 hours
**Concepts**: Classes, constructors/destructors, inheritance, virtual functions,
`std::vector`, `new`/`delete`

---

### Task 1.1: Character Base Class (1.5 hours)

**File**: `src/Character.cpp`

The constructor and destructor are already provided.  You need to implement 5 small methods.  Each one has a detailed hint in the source file.

| TODO | Method | What to do |
|------|--------|------------|
| 1 | `calculateDamage()` | Return `attack + (rand() % 5)` |
| 2 | `takeDamage(int)` | Apply damage minus defense; clamp HP at 0; set `alive` |
| 3 | `heal(int)` | Add HP, cap at max |
| 4 | `displayStats()` | Print name and HP |
| 5 | `displayStatus()` | Same but no newline |

**Quick test** (compile just Character):
```bash
g++ -std=c++98 -Wall -g -Iinclude -c src/Character.cpp -o src/Character.o
```
If it compiles, you are on track.

---

### Task 1.2: Item Hierarchy (1 hour)

**File**: `src/Item.cpp`

Constructors and destructors are provided.  You need to fill in display methods and the Consumable `use()` function.

| TODO | Method | What to do |
|------|--------|------------|
| 1 | `Item::displayInfo()` | Print name, description, value |
| 2 | `Item::displayBrief()` | Print "Name (Type)" |
| 3 | `Weapon::displayInfo()` | Print damage bonus line |
| 4 | `Armor::displayInfo()` | Print defense bonus line |
| 5 | `Consumable::displayInfo()` | Print healing amount |
| 6 | `Consumable::use()` | Mark as used, print message |

---

### Task 1.3: Monster Hierarchy (1.5 hours)

**File**: `src/Monster.cpp`

Constructors are provided.  You need the destructor, display, loot, and attack messages.

| TODO | Method | What to do |
|------|--------|------------|
| 1 | `Monster::~Monster()` | Delete all loot items |
| 2 | `Monster::displayStats()` | Print "Name [HP: cur/max]" |
| 3 | `Monster::dropLoot()` | Copy vector, clear original, return copy |
| 4 | `Monster::getAttackMessage()` | Return "Name attacks!" |
| 5 | `Goblin()` constructor body | Add a Small Potion to loot |
| 6 | `Goblin::getAttackMessage()` | Return goblin flavor text |
| 7 | `Skeleton()` constructor body | Add an Old Sword to loot |
| 8 | `Skeleton::getAttackMessage()` | Return skeleton flavor text |
| 9 | `Dragon()` constructor body | Add 3 legendary items to loot |
| 10 | `Dragon::getAttackMessage()` | Return dragon flavor text |
| 11 | `Dragon::calculateDamage()` | Base damage + 5 fire bonus |

---

### Task 1.4: Player Class (4 hours)

**File**: `src/Player.cpp`

This is the largest file.  Many helper functions (`addItem`, `displayInventory`, `hasItem`, `getItem`, `displayStats`, `unequipWeapon`, `unequipArmor`) are already provided or nearly complete.  Focus on the TODOs listed below.

| TODO | Method | What to do |
|------|--------|------------|
| 1 | `~Player()` | Delete all inventory items |
| 3 | `calculateDamage()` | Base damage + weapon bonus |
| 5 | `removeItem()` | Fill in the equipped-pointer NULL check |
| 9 | `equipWeapon()` | Set `equipped_weapon` and print |
| 10 | `equipArmor()` | Set `equipped_armor` and print |
| 11 | `useItem()` | Cast, heal, use, remove |
| 12 | `gainExperience()` | Check for level-up threshold |
| 13 | `levelUp()` | Increment stats and print |

**Testing**: At this point you should be able to compile the full project (`make`) even though the game won't be fully playable yet.

---

### Week 1 Checkpoint (April 19)

Before moving to Week 2, verify:

- [ ] `make` compiles without errors
- [ ] Character takes damage and heals correctly
- [ ] All Item types display their info
- [ ] Consumable `use()` works
- [ ] Player destructor frees inventory (test with valgrind on a small program)
- [ ] Monster destructor frees loot
- [ ] Dragon deals bonus fire damage
- [ ] All `getAttackMessage()` overrides return correct strings

---

## Week 2: Rooms and Game Integration (April 20 – 27)

**Time Estimate**: 8–10 hours
**Concepts**: `std::map`, object composition, ownership, game loop, command
parsing

---

### Task 2.1: Room Class (2 hours)

**File**: `src/Room.cpp`

The constructor, `addExit`, `addItem`, `removeItem`, `displayItems`, `getItem`, and `display` are already provided or nearly complete.

| TODO | Method | What to do |
|------|--------|------------|
| 1 | `~Room()` | Delete monster and all items |
| 3 | `displayExits()` | Iterate exits map, print directions |
| 4 | `getExit()` | Map lookup, return Room* or NULL |
| 5 | `hasExit()` | Map lookup, return bool |
| 6 | `clearMonster()` | Delete and NULL |

---

### Task 2.2: Game Class (6 hours)

**File**: `src/Game.cpp`

This file has the most provided code.  `initializeWorld()`, `createStartingInventory()`, `processCommand()`, `look()`, `inventory()`, `useItem()`, `help()`, and most of `combat()` are already written.

| TODO | Method | What to do |
|------|--------|------------|
| 1 | `~Game()` | Delete player + all rooms in map |
| 2 | `addRoom()` | One-liner: add to world map |
| 3 | `connectRooms()` | Add the reverse exit (one line) |
| 4 | `run()` | Write the main game loop (hint provided) |
| 5 | `move()` | Move to next room if exit exists |
| 7 | `combat()` | Set `victory = true` when Dragon dies |
| 8 | `pickupItem()` | Add to player, remove from room |
| 9 | `equip()` | Check type and call equip method |

---

### Task 2.3: Final Testing (2 hours)

1. **Compile**: `make clean && make`
2. **Play through** the full game — navigate, fight, pick up items, equip, use potions, defeat the dragon.
3. **Memory check**: `valgrind --leak-check=full ./bin/rpg_game` — play through and quit normally.  Should show **no leaks**.
4. **Edge cases**: try invalid commands, going wrong directions, picking up nonexistent items, fleeing from combat.

---

### Week 2 / Final Checkpoint (April 27)

- [ ] Game compiles without errors or warnings
- [ ] Can navigate between all 5 rooms
- [ ] Can fight and defeat all monsters
- [ ] Inventory, equip, and use all work
- [ ] Defeating the dragon triggers victory
- [ ] Player death triggers game over
- [ ] `valgrind` shows no memory leaks
- [ ] Code is well-commented

---

## Submission Checklist

Before submitting, verify:

- [ ] All `.cpp` files compile without errors (`make`)
- [ ] Game runs and is playable start to finish
- [ ] Can defeat dragon and win
- [ ] No memory leaks (valgrind clean)
- [ ] No segmentation faults
- [ ] All commands work
- [ ] Code is commented where you added implementation

---

## Debugging Tips

### Segmentation fault?
Run under gdb:
```bash
gdb ./bin/rpg_game
(gdb) run
(gdb) bt        # shows where the crash happened
```
Most common cause: dereferencing a NULL pointer.

### Memory leaks?
```bash
valgrind --leak-check=full ./bin/rpg_game
```
Valgrind tells you *where* the leaked memory was allocated.

### Virtual function not being called?
Make sure you are calling through a base-class pointer or reference.
`Monster*` calling `calculateDamage()` will use Dragon's override.

### "undefined reference to vtable"?
You declared a virtual function in a `.h` but didn't implement it in the `.cpp`.

---

## Getting Unstuck

1. Read the hints in the TODO comments — most answers are there in commented form
2. Review the header files to understand the interface
3. Test incrementally: compile after each TODO
4. Use `std::cout` to print variable values while debugging
5. Ask in office hours or on the course forum

**Start early! Don't wait until the last minute!**
