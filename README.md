# 🍒 Ms. Pac-Man Arcade: Custom C++ Engine

<p align="center">
  <img src="path/to/gameplay.gif" width="600" alt="Ms. Pac-Man Gameplay">
  <br />
  <b>A faithful arcade recreation featuring a custom-built C++ engine.</b>
</p>

---

### 🕹️ Overview
This project is a full recreation of the arcade classic *Ms. Pac-Man*, developed for the **Programming 4** course. Built on a custom engine inspired by **Minigin**, the game expands on the original with modern features while maintaining the authentic feel of the 1982 classic.

### 🆕 Key Features
* **Expanded Game Modes:** Includes original Single Player, **Versus Mode** (play as the ghosts!), and **Co-op Mode**.
* **Authentic Ghost AI:** Recreated the specific behavioral logic for Blinky, Pinky, Inky, and Sue (based on the original arcade ROM logic).
* **Multiple Mazes:** Full support for the distinct level variations and fruit spawning patterns found in the original game.

---

### 🛠️ Engine Architecture & Design Patterns
The core of this project was the implementation of robust software architecture using principles from **Robert Nystrom’s *Game Programming Patterns***.

* **Component-Based System:** Developed a modular architecture where entities (Pac-Man, Ghosts, Pellets) are composed of independent components for rendering, physics, and AI.
* **State Pattern:** Utilized for Ghost behaviors (Scatter, Chase, Frightened) to ensure clean transitions and maintainable AI logic.
* **Command Pattern:** Decoupled input handling from actor logic, allowing for easy implementation of local multiplayer and AI controllers.
* **Observer Pattern:** Implemented for the scoring system and sound triggers to keep game systems decoupled and event-driven.
* **Singleton/Service Locator:** Used for global engine services like the Sound System and Asset Manager.

---

### 🚀 Tech Stack
![C++](https://img.shields.io/badge/C++-%2300599C.svg?style=flat-square&logo=c%2B%2B&logoColor=white) ![SDL2](https://img.shields.io/badge/SDL2-00599C?style=flat-square&logo=sdl&logoColor=white) ![Visual Studio](https://img.shields.io/badge/Visual%20Studio-5C2D91?style=flat-square&logo=visual-studio&logoColor=white)

# Minigin

Minigin is a very small project using [SDL2](https://www.libsdl.org/) and [glm](https://github.com/g-truc/glm) for 2D c++ game projects. It is in no way a game engine, only a barebone start project where everything sdl related has been set up. It contains glm for vector math, to aleviate the need to write custom vector and matrix classes.

[![Build Status](https://github.com/avadae/minigin/actions/workflows/msbuild.yml/badge.svg)](https://github.com/avadae/msbuild/actions)
[![GitHub Release](https://img.shields.io/github/v/release/avadae/minigin?logo=github&sort=semver)](https://github.com/avadae/minigin/releases/latest)

# Goal

Minigin can/may be used as a start project for the exam assignment in the course [Programming 4](https://youtu.be/j96Oh6vzhmg) at DAE. In that assignment students need to recreate a popular 80's arcade game with a game engine they need to program themselves. During the course we discuss several game programming patterns, using the book '[Game Programming Patterns](https://gameprogrammingpatterns.com/)' by [Robert Nystrom](https://github.com/munificent) as reading material. 

# Disclaimer

Minigin is, despite perhaps the suggestion in its name, **not** a game engine. It is just a very simple sdl2 ready project with some of the scaffolding in place to get started. None of the patterns discussed in the course are used yet (except singleton which use we challenge during the course). It is up to the students to implement their own vision for their engine, apply patterns as they see fit, create their game as efficient as possible.

# Use

Either download the latest release of this project and compile/run in visual studio or, since students need to have their work on github too, they can use this repository as a template (see the "Use this template" button at the top right corner). There is no point in forking this project.
