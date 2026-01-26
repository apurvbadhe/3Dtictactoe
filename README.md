# 4×4×4 3D Tic Tac Toe (Raylib + C)

A fully interactive **3D Tic Tac Toe game** built in **C using Raylib**, featuring a 4×4×4 grid, first-person camera controls, multiple game modes, and AI bots with adjustable difficulty.

This project explores **3D interaction, ray-based selection, game state management, and AI logic** in a low-level graphics environment.

---

## 🎮 Features

### Core Gameplay
- 4×4×4 3D Tic Tac Toe board
- Translucent cubes with wireframe edges
- Ray-based cube selection (only **one cube selectable at a time**)
- Crosshair-based interaction
- Real-time win detection

### Winning Rules
A player wins if **4 identical pieces** align:
- Along any straight line (X, Y, Z)
- Along any 2D diagonal (XY, XZ, YZ planes)
- Along any 3D diagonal (space diagonals)
- Works for **all interior and border cases**

### Game Modes
- **Player vs Player**
- **Player vs Bot**
- **Bot vs Bot** (visible, slowed gameplay)

### Bot AI
Three difficulty levels:
- **Easy**: Random valid moves
- **Medium**: Win → Block → Random
- **Hard**: Win → Block → Strategic fallback

Bot vs Bot mode is intentionally slowed for visibility.

### Camera & Controls
- First-person camera (FPS / Roblox-style)
- Mouse look
- WASD movement
- Locked cursor during gameplay

---

## ⌨️ Controls

| Action | Key |
|------|----|
| Move | W A S D |
| Look Around | Mouse |
| Place Piece | Left Mouse Button |
| Back to Menu (Rules / Win) | Q |
| Exit Game | ESC |

---

## 🧭 Game States
- Main Menu
- Rules Page
- Gameplay
- Win Screen

---

## 🛠️ Built With
- **Language**: C
- **Graphics Library**: Raylib
- **Compiler**: GCC (via MSYS2 MinGW64)

---

## ⚙️ Build Instructions (Windows)

### Prerequisites
- MSYS2 installed
- Raylib installed via MSYS2
- MinGW64 toolchain

### Compile
```bash
gcc main.c -o game -lraylib -lopengl32 -lgdi32 -lwinmm
