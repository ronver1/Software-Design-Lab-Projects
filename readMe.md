# Software Design Lab Projects

This repository contains my programming assignments and projects from **ECE 312H: Introduction to Software Design and Implementation** at The University of Texas at Austin. The course develops strong foundations in:

- C/C++ programming and memory management
- Pointers, arrays, and dynamic data structures
- Modular software design and abstraction
- Debugging, testing, and program correctness
- Algorithmic thinking and computational problem solving

These projects emphasize clean code, efficiency, and low-level understanding that directly support embedded systems, firmware, and systems programming.

---
## Overview
This repository contains my programming assignments and projects from ECE 312H: Introduction to Software Design and Implementation at The University of Texas at Austin. The course develops strong foundations in:

C/C++ programming and memory management
Pointers, arrays, and dynamic data structures
Modular software design and abstraction
Debugging, testing, and program correctness
Algorithmic thinking and computational problem solving

These projects emphasize clean code, efficiency, and low-level understanding that directly support embedded systems, firmware, and systems programming.
---

## Academic Integrity Notice
- This repository is shared for educational and portfolio purposes only.
- If you are currently enrolled in ECE 319H or a similar course, please do not copy this code directly.
---

## Labs

### Lab 0 — Getting Started: Basic C Functions
**Language:** C  
An introductory lab to establish the development and submission workflow. Implements three foundational C functions: `isPrime` (primality test using square root optimization), `calculateHypotenuse` (Pythagorean theorem with edge case handling), and `gcd` (greatest common divisor with support for negative inputs). Graded via Gradescope autograder.

**Key concepts:** C functions, edge case handling, `math.h`, compilation with `gcc`

---

### Lab 1 — Dynamic Array
**Language:** C  
Implements a **dynamic array** data structure in C — the conceptual foundation of C++'s `std::vector`. The array automatically doubles its capacity when full using `realloc`. Exercises heap memory allocation, pointer arithmetic, output parameters, and safe reallocation patterns.

**Key concepts:** `malloc`, `realloc`, `free`, pointers, stack vs. heap, output parameters, memory leaks

**Functions implemented:** `createArray`, `destroyArray`, `addElement`, `getElement`, `setElement`, `getSize`, `getCapacity`, `removeElement`

---

### Lab 2 — Linked List
**Language:** C  
Implements a **singly linked list** with a full suite of operations. Covers dynamic node allocation, pointer-based list manipulation, and proper memory cleanup including traversal-based deallocation.

**Key concepts:** linked list design, `malloc`/`free`, pointer chaining, struct layout, NULL handling

**Functions implemented:** `createList`, `destroyList`, `insertAtHead`, and additional list operations

---

### Lab 3 — Memory Pool Allocator
**Language:** C  
Implements a custom **memory pool allocator** that manages a fixed 4096-byte pool — modeling how `malloc` and `free` work under the hood. Features a **doubly-linked free list** sorted by address, first-fit allocation, block splitting, and coalescing of adjacent free blocks to combat fragmentation.

**Key concepts:** free list, first-fit allocation, block splitting, coalescing, memory fragmentation, doubly-linked list, alignment (`ALIGN` macro)

**Functions implemented:** `pool_init`, `pool_cleanup`, `pool_malloc`, `pool_free`, `find_free_block`, `add_to_free_list`, `remove_from_free_list`, `split_block`, `coalesce_blocks`

**Test scripts:** `test1.txt` – `test7.txt` covering basic alloc/free, fragmentation, coalescing chains, edge cases, and pool exhaustion

---

### Lab 4 — Tech Support Diagnosis Tool
**Language:** C  
Builds an interactive **tech support decision tree** that grows through use. The program navigates yes/no questions to suggest fixes, learns new problems at runtime, and permanently grafts new branches into the tree. Features a full ncurses TUI with color, undo/redo, binary file persistence, BFS tree visualization, integrity checking, and LCA-based shortest path finding between solutions.

**Key concepts:** binary tree, dynamic array stack, linked-list queue, hash table, BFS traversal, LCA algorithm, binary serialization, undo/redo with two stacks, ncurses

**Data structures implemented (in `ds.c`):**
- Binary decision/solution tree
- `FrameStack` — dynamic array stack for iterative traversal
- `EditStack` — dynamic array stack for undo/redo
- `Queue` — linked-list queue for BFS
- `Hash` — hash table mapping symptom keywords to solution IDs

**Logic implemented:** `run_diagnosis`, `undo_last_edit`, `redo_last_edit` (`game.c`), `save_tree`/`load_tree` BFS binary serialization (`persist.c`), `check_integrity`, `find_shortest_path` (`utils.c`)

---

### Lab 5 — Dungeon Crawler RPG
**Language:** C++  
Builds a text-based **dungeon crawler RPG** using object-oriented C++. Implements a full class hierarchy with inheritance, polymorphism, STL containers, and RAII-based memory management. Players explore rooms, fight monsters, collect items, level up, and equip gear — all driven by a command-line game loop.

**Key concepts:** inheritance, polymorphism (`virtual` functions), encapsulation, `std::vector`, `std::map`, `std::string`, `new`/`delete`, destructors, RAII

**Class hierarchy:**
- `Character` (base) → `Player`, `Monster`
- `Monster` subtypes: Goblin, Orc, Dragon (overrides `calculateDamage`, `getAttackMessage`, `dropLoot`)
- `Item` (base) → `Weapon`, `Armor`, `Consumable`
- `Room` — dungeon rooms with directional exits via `std::map`
- `Game` — top-level controller managing the game loop, combat, movement, and inventory

**Game commands:** `go <direction>`, `look`, `attack`, `pickup`, `inventory`, `equip`, `use`, `stats`, `help`, `quit`

---

## Building & Running

Each lab is self-contained. General build pattern:

```bash
# C labs (0–4)
gcc -Wall -Wextra -o <output> <files>.c -lm

# Lab 4
cd Lab4/src && make

# Lab 5
cd Lab5/starter_code && make
./bin/rpg_game
```

Memory leak checking (available on ECE Linux: `linux.ece.utexas.edu`):
```bash
valgrind --leak-check=full ./<binary>
```

---

## Course Info

Course: ECE 312H — Software Design and Implementation (Honors)  
University: The University of Texas at Austin  
Semester: Spring 2026  
Instructor: Evan Speight  
Student: Ronit Verma

--- 
## Author
### Ronit Verma

The University of Texas at Austin

Electrical & Computer Engineering Honors + Business Honors

LinkedIn: https://www.linkedin.com/in/ronit-verma-b0a80a383/
