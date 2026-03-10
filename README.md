# 🐍 Snake Game (C + Raylib)

A simple **Snake Game remake written in C using the Raylib graphics library**.

The player controls a snake that grows longer by eating food. The goal is to survive as long as possible while avoiding collisions with the walls or the snake's own body.

---

# 🎮 Gameplay

* Move the snake around the grid.
* Eat food to increase your **score**.
* Each food item makes the snake **grow longer**.
* The game ends if the snake:

  * Hits the **wall**
  * Hits **itself**

Press **R** after losing to restart the game.

---

# ⌨ Controls

| Key | Action       |
| --- | ------------ |
| W   | Move Up      |
| S   | Move Down    |
| A   | Move Left    |
| D   | Move Right   |
| R   | Restart Game |

---

# ✨ Features

* Grid-based snake movement
* Random food spawning
* Score tracking
* Sound effects
* Texture-based food rendering
* Collision detection
* Restart system

---

# 📂 Project Structure

```
snake-game
│
├─ src
│   ├─ main.c
│   └─ run.bat
│
├─ assets
│   ├─ food.png
│   ├─ foodpopup.wav
│   ├─ eat.ogg
│   └─ die.ogg
│
└─ README.md
```

---

# 🛠 Build and Run (Windows)

The project includes a **run.bat** script inside the `src` folder to compile and run the game.

### Step 1 — Navigate to the src folder

```
cd src
```

### Step 2 — Run the script

```
run.bat
```

The batch script will:

* Compile the game using **gcc**
* Link the **raylib libraries**
* Generate the executable
* Launch the game

This allows the project to be **built and executed with one command**.


---
---

# ⚙ Requirements

* C Compiler (GCC / Clang)
* Raylib Library
* Windows / Linux / macOS

Download Raylib:
https://www.raylib.com/

---


# 🚀 Future Improvements

Possible improvements for the project:

* Game menu system
* Pause feature
* High score saving
* Snake animation
* Better UI

---

# 👨‍💻 Author

**Zahin Bin Hasan**
*(C Programming Practice Project)
