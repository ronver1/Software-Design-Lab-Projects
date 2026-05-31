# Build Instructions for RPG Lab

## Prerequisites

It is **highly recommended** to use the ECE linux servers for this project, whether you log into them directly or use them as a backend for your VSCode GUI running locally.  The project is NOT designed to run on Windows natively, and while it may be possible to get it working using WSL or MinGW, you may run into unexpected issues.

### Verify Installation

```bash
g++ --version       # Check compiler
make --version      # Check make
valgrind --version  # Check valgrind
```

---

## Project Structure

```
starter_code/
├── Documentation/
│   ├── README.md
│   ├── TODO.md
│   └── BUILD_INSTRUCTIONS.md
├── Makefile
├── bin/
│   └── dungeon_rpg.solution
├── include/          ← Header files (do NOT modify)
│   ├── Character.h
│   ├── Player.h
│   ├── Monster.h
│   ├── Item.h
│   ├── Room.h
│   └── Game.h
└── src/              ← Implementation files (your work goes here)
    ├── main.cpp      ← Complete, no changes needed
    ├── Character.cpp
    ├── Player.cpp
    ├── Monster.cpp
    ├── Item.cpp
    ├── Room.cpp
    └── Game.cpp
```

---

## Basic Build Commands

```bash
make              # Compile the project
./bin/rpg_game    # Run the game
make clean        # Remove compiled files
make rebuild      # Clean and rebuild from scratch
```

### Compiler Flags

- `-std=c++98` — C++98 standard
- `-Wall` — Enable all warnings
- `-g` — Include debugging symbols (for gdb)

---

## Debugging

### Using GDB

```bash
gdb ./bin/rpg_game
(gdb) run                    # Run program
(gdb) break Game.cpp:50      # Set breakpoint at line 50
(gdb) next                   # Execute next line
(gdb) step                   # Step into function
(gdb) print variable         # Print variable value
(gdb) backtrace              # Show call stack (use after a crash!)
(gdb) quit                   # Exit gdb
```

### Using Valgrind (Memory Leak Detection)

```bash
valgrind --leak-check=full --show-leak-kinds=all ./bin/rpg_game
```

**Good output** (what you want to see):
```
All heap blocks were freed -- no leaks are possible
```

**Bad output** (you have a leak):
```
LEAK SUMMARY:
   definitely lost: 240 bytes in 3 blocks
```

Valgrind shows *where* the leaked memory was allocated — check that location and make sure the corresponding destructor deletes it.

---

## Common Compilation Errors

### "undefined reference to vtable"
You declared a virtual function in a header but didn't implement it in the `.cpp`.
**Fix**: implement every function declared in the header.

### "no matching function for call to constructor"
Derived class constructor doesn't call base class constructor.
**Fix**: use initializer list:
```cpp
Player::Player(const std::string& name)
    : Character(name, 100, 10, 5) {  // Call base constructor
    level = 1;
}
```

### "'X' was not declared in this scope"
Missing `#include`.
**Fix**: add the appropriate include at the top of the file.

---

## Testing Strategy

### After Week 1
Compile the full project.  It may not be fully playable yet, but all classes should compile.  Test individual methods:

```bash
make
# If it compiles, try running — some features may crash until Week 2 is done
```

### After Week 2
Play through the full game and test all commands.  Then run valgrind:

```bash
valgrind --leak-check=full ./bin/rpg_game
# Play through, defeat dragon or quit, check for leaks
```

---

## Summary of Commands

```bash
make                           # Build
./bin/rpg_game                 # Run
gdb ./bin/rpg_game             # Debug
valgrind --leak-check=full ./bin/rpg_game  # Memory check
make clean && make             # Clean rebuild
```

**You're ready to build! Good luck!**
