
# 🌍 Planetary Ball Physics Simulation (C++ & Raylib)

This project is a **2D physics-based ball simulation** written in **C++**, rendered in real time using **Raylib**, and numerically integrated with **Runge–Kutta 4th order (RK4)**.
It simulates falling, colliding balls under **different planetary environments**, including gravity, air drag, wind, wall collisions, and ball–ball interactions.

This project is a **direct sequel** to my previous **N-Body Simulation** project.  
While the earlier project focused on **gravitational interaction between bodies**, this one zooms in on **detailed rigid-body dynamics**, collisions, drag, and environmental effects — essentially going from *space-scale physics* to *near-surface physics*.

---

## 🎬 Demo
Demo is pretty bad quality because of 10mb upload limit


https://github.com/user-attachments/assets/eeb18fec-57a6-419c-910c-3cae1cb1fd3c


---

## 🚀 Features

* ⚙️ **RK4 (Runge–Kutta 4th Order) integration** for smooth, stable motion
* 🌍 **Multiple planetary environments** with different gravity & air density
* 🌬️ **Wind simulation** (static, dynamic sinusoidal, or disabled)
* 🧮 **Quadratic air drag** based on velocity, cross-sectional area, and density
* ⚽ **Elastic & inelastic ball collisions** with impulse-based resolution
* 🧱 **Wall and floor collision handling** with restitution
* 🖱️ **Interactive ball spawning** via mouse clicks
* 🧲 **Attraction / repulsion point** controlled with right mouse button
* 🎨 **Randomized ball properties** (mass, radius, color, velocity)
* 📊 **Live UI overlay** showing environment and simulation parameters

---

## 🧠 Skills Learned & Technologies Used

### 🧩 Physics & Numerical Methods

* Implemented **RK4 integration** for position & velocity updates
* Modeled **gravity, drag, wind, and impulses**
* Calculated **terminal velocity** analytically
* Implemented **ball–ball collision resolution** using impulse mechanics
* Handled **overlap correction** to prevent tunneling and sticking
* Worked with **vector projections and normals**

### 🎮 Graphics & Interaction

* Used **Raylib 2D rendering** and `Camera2D`
* Implemented **world-space mouse interaction**
* Built a simple **debug-style UI overlay**
* Learned practical use of **raymath** for vector operations

### 🧰 Tools & Libraries

* **C++17 STL** — vectors, strings, math utilities
* **Raylib** — rendering, input, timing
* **Raymath** — vector math
* **g++ / Raylib Quickstart** — build & run environment

---

## 🕹️ Controls

| Action                              | Key / Mouse |
|-----------------------------------|-------------|
| **Spawn ball**                    | Left-click  |
| **Attract / repel balls**         | Hold Right-click |
| **Invert attraction direction**   | Hold Shift + Right-click |
| **Change planet**                 | 1 – 0 keys  |
| **Toggle wind modes**             | W           |
| **Clear all balls**               | R           |
| **Exit simulation**               | ESC         |

---

## 🌠 Simulation Details

### 🟠 Each Ball Stores

* Position `(x, y)`
* Velocity `(vx, vy)`
* Mass
* Visual radius & real-world radius
* Restitution coefficient (bounciness)
* Drag properties (air density, drag coefficient)
* Color

### 🧮 Physics Model

* Gravity varies per planet
* Drag force follows:
  
```

F_drag = 0.5 * ρ * C_d * A * v²

```

* Motion is integrated using **RK4** each frame
* Collisions use **impulse-based resolution**:

```

J = -(1 + e) * v_rel / (1/m1 + 1/m2)

```

---

## 🎯 Default Ball Spawn Parameters

When a new ball is spawned (via **left-click**), its physical properties are **randomized within predefined ranges** to encourage varied and interesting interactions right away:

* ⚡ **Initial Velocity**
  * `vx`, `vy` are randomly chosen in the range **[-100, 100]**
  * This allows balls to spawn either stationary, drifting slowly, or with noticeable kinetic energy

* ⚖️ **Mass**
  * Randomized between **1 and 75**
  * Heavier balls respond more slowly to impulses, wind, and collisions

* 📏 **Radius**
  * Randomized between **1 and 75**
  * Affects collision size, drag area, and visual presence

* 🧱 **Restitution Coefficient (e)**
  * Randomized in the range **[0.8, 1.0]**
  * Most balls are **highly elastic**, resulting in energetic, bouncy collisions
  * Lower values introduce slight energy loss for realism

* 🎨 **Color**
  * Randomly selected from a wide predefined color palette
  * Ensures visual clarity and variety when many balls are present

These defaults are intentionally tuned toward **lively, chaotic motion**, making the simulation visually interesting even with a small number of balls.  
All values can be easily tweaked inside the `addOnClick()` function to experiment with different physical behaviors — from heavy, damped systems to near-perfect elastic chaos.

---


## 🌍 Planetary Environments

Switch planets using number keys:

| Key | Body     | Gravity (m/s² ×10) | Air Density (kg/m³) |
|----:|----------|--------------------|---------------------|
| 1   | Mercury  | 38                 | 0                   |
| 2   | Venus    | 90                 | 65                  |
| 3   | Earth    | 98.1               | 1.225               |
| 4   | Mars     | 37                 | 0.02                |
| 5   | Jupiter  | 240.8              | 0.16                |
| 6   | Saturn   | 104.4              | 0.19                |
| 7   | Uranus   | 87.3               | 0.42                |
| 8   | Neptune  | 111.8              | 0.45                |
| 9   | Moon     | 16.25              | 0                   |
| 0   | Sun      | 2740               | 0.0001              |

---

## 🧩 How to Run This Project

> **Same setup as my previous N-Body Simulation project**

To make testing quick and painless, use the official **Raylib Quickstart** template:

1. Download **Raylib Quickstart** from  
 https://github.com/raylib-extras/raylib-quickstart
2. Make sure the template builds and runs correctly first
3. Replace the template’s `main.cpp` with this project’s `main.cpp`
4. Build and run — the simulation should start immediately

> ⚠️ Due to local configuration differences, this is currently the most reliable way to run the project

---

## 🔁 Relationship to Previous Project (Sequel Notes)

This project is a **natural continuation** of my earlier **N-Body Simulation**:

**Similarities**
* RK4 numerical integration
* Physics-first architecture
* Raylib-based real-time visualization
* Interactive object spawning
* Strong focus on correctness & stability

**What’s New Here**
* Collision detection & resolution
* Drag, wind, and terminal velocity
* Planet-specific atmospheric effects
* Impulse-based mechanics
* Surface-level physics instead of orbital dynamics

Together, the two projects form a progression from **astronomical-scale simulations** to **detailed physical systems**.

---

## 🏁 Summary

This project demonstrates:

* Solid understanding of **physics simulation**
* Practical use of **numerical integration**
* Collision handling and force modeling
* Clean **object-oriented C++ design**
* Real-time interaction and visualization with Raylib

A hands-on exploration of **physics engines**, numerical methods, and interactive simulation design.

---

### 🧑‍🚀 Qclipsing — Me
Yes, I used AI to write this refine my original readme with colorful emojies, higlights and other improvments like grammar. Because I am pretty bad at writing readmes 

