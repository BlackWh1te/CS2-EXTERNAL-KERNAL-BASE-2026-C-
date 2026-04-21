# LUMEN EXTERNAL - Counter-Strike 2 External Cheat

<p align="center">
  <img src="https://img.shields.io/badge/Platform-Windows-blue" alt="Platform">
  <img src="https://img.shields.io/badge/Language-C++17-orange" alt="Language">
  <img src="https://img.shields.io/badge/Rendering-DirectX_11-green" alt="Rendering">
  <img src="https://img.shields.io/badge/Target-CS2-yellow" alt="Target">
  <img src="https://img.shields.io/badge/Version-3.0-red" alt="Version">
</p>

---

## Table of Contents

1. [Overview](#overview)
2. [Features](#features)
3. [Technical Architecture](#technical-architecture)
4. [Kernel Driver](#kernel-driver)
5. [Requirements](#requirements)
6. [Building](#building)
7. [Installation](#installation)
8. [How to Use](#how-to-use)
9. [Menu Controls](#menu-controls)
10. [Updating Offsets](#updating-offsets)
11. [Troubleshooting](#troubleshooting)
12. [File Structure](#file-structure)
13. [FAQ](#faq)
14. [Credits](#credits)
15. [Legal](#legal)

---

## Overview

**LUMEN EXTERNAL** is a production-ready external cheat for Counter-Strike 2 (CS2). Unlike internal cheats that inject into the game process, LUMEN runs as a separate desktop overlay application, making it significantly more stable and less prone to detection by Valve's anti-cheat systems.

### What is an External Cheat?

An external cheat operates outside the game process by:
- Reading game memory via `ReadProcessMemory` API
- Rendering visuals on an overlay window (not in-game)
- Communicating through Windows APIs rather than game functions

This approach provides:
- **Stability** - Cannot crash the game process
- **Safety** - Not detected by game introspection
- **Simplicity** - No need for complex injection techniques

---

## Features

### Aimbot

| Feature | Description |
|---------|-------------|
| **Legit Mode** | Uses mouse events for human-like movement |
| **Rage Mode** | Direct memory writes for instant snaps |
| **FOV** | Configurable field of view (1-30 degrees) |
| **Smoothing** | Human-like aim smoothing |
| **Bone Selection** | Head, Neck, Chest, Stomach |
| **Weapon Profiles** | Separate configs for Rifles, SMGs, Pistols, Snipers |
| **RCS (Recoil Control)** | Automatic recoil compensation |
| **Visibility Check** | Only aim at visible enemies |
| **Target Lock** | Persistent target until dead |
| **Humanization** | Random jitter for natural movement |

### ESP / Visuals

| Feature | Description |
|---------|-------------|
| **2D Boxes** | Regular and Corner styles |
| **3D Boxes** | World-space 3D bounding boxes |
| **Skeleton** | Player bone structure rendering |
| **Health Bar** | Enemy health indication |
| **Armor Bar** | Enemy armor indication |
| **Names** | Player usernames |
| **Distance** | Distance in meters |
| **Snaplines** | Line from center to player |
| **Head Dot** | Small head indicator |
| **Avatar ESP** | Steam profile pictures |
| **Fake Glow** | Rainbow skeleton effect |
| **Fake Chams** | White polygon fill rendering |
| **RGB Mode** | Rainbow color cycling |

### Skin Changer

| Feature | Description |
|---------|-------------|
| **Custom Skins** | AK-47, AWP, M4A4, Glock, USP, Deagle |
| **Wear** | Float value control (0.001 - 1.0) |
| **StatTrak** | StatTrak modification |
| **Weapon ID** | Item definition index |

### Bomb Features

| Feature | Description |
|---------|-------------|
| **Bomb Timer HUD** | Glassmorphism countdown display |
| **Bomb ESP** | 3D box at bomb location |
| **Site Detection** | Shows Site A or B |
| **Defuse Info** | Defuse timer and status |

### Misc Features

| Feature | Description |
|---------|-------------|
| **Hitmarkers** | Damage numbers on hit |
| **Hitsounds** | Audio feedback on hit |
| **Spectator List** | Shows who's watching you |
| **Watermark** | Entity count and FPS display |
| **Grenade Prediction** | Trajectory lines for grenades |

---

## Technical Architecture

### Threading Model

```
┌─────────────────────────────────────────────────────────┐
│                   Main Thread                          │
│         (DX11 Rendering + Menu + Input)                 │
└─────────────────────────┬───────────────────────────────┘
                          │
        ┌────────────────┼────────────────┐
        │                │                │        │
        ▼                ▼                ▼        ▼
┌─────────────┐  ┌─────────────┐  ┌──────────┐  ┌──────────┐
│   Skin      │  │  Entity    │  │  Aimbot  │  │  Avatar  │
│  Changer   │  │  Reader    │  │  Thread  │  │  Manager │
│  Thread    │  │  Thread    │  │          │  │  Thread  │
└─────────────┘  └─────────────┘  └──────────┘  └──────────┘
```

### Memory System

The project uses two methods for reading game memory:

1. **User Mode** (Default)
   - Uses `ReadProcessMemory` API
   - Standard Windows process access
   - Safe and stable

2. **Kernel Mode** (Optional)
   - Uses kernel driver
   - Bypasses all user-mode protections
   - HIGHLY DETECTABLE - USE WITH CAUTION

### View Matrix

The project uses CS2's view matrix for World-to-Screen transforms:

```
ScreenX = (Pos.x * Matrix[0][0] + Pos.y * Matrix[0][1] + Pos.z * Matrix[0][2] + Matrix[0][3]) / w
ScreenY = (Pos.x * Matrix[1][0] + Pos.y * Matrix[1][1] + Pos.z * Matrix[1][2] + Matrix[1][3]) / w
```

### Offsets

Offsets are stored in `Offsets.h` and include:
- Entity list pointer
- Local player controllers/pawns
- View matrix and angles
- Player health, team, armor
- Bone matrices
- Weapon services
- Bomb information

Offsets update automatically from:
- Primary: https://raw.githubusercontent.com/a2x/cs2-dumper/main/output/offsets.json
- Fallback: Local `dumper offsets/output/offsets.json`

---

## Kernel Driver

### What is a Kernel Driver?

A kernel driver runs at ring 0 (kernel mode), which has full system access. This allows reading/writing any process memory without going through standard Windows APIs.

### How LUMEN's Driver Works

```
┌────────────────────────────────────────────────────────────────┐
│                    USER MODE                                 │
│  ┌───────────────────────────────────────────────────────┐  │
│  │  KernelDriver class (driver.h)                       │  │
│  │  - Creates registry request                          │  │
│  │  - Writes to: HKLM\System\CurrentControlSet\Services │  │
│  └──────────────────────────┬──────────────────────────────┘  │
└─────────────��───────────────┼─────────────────────────────────┘
                             │ Registry Callback
                             ▼
┌────────────────────────────────────────────────────────────────┐
│                    KERNEL MODE                               │
│  ┌───────────────────────────────────────────────────────┐  │
│  │  r6-driver kernel driver                              │  │
│  │  - Intercepts registry callback                      │  │
│  │  - Performs memory read/write                       │  │
│  │  - Returns data to user mode                        │  │
│  └───────────────────────────────────────────────────────┘  │
└────────────────────────────────────────────────────────────────┘
```

### Supported Operations

| Operation | Code | Description |
|-----------|------|-------------|
| Read | 0 | Read memory from target process |
| Write | 1 | Write memory to target process |
| Get Module Base | 2 | Get DLL base address |

### Warning

> **⚠️ EXTREME RISK**: Using kernel drivers is highly detectable by anti-cheat systems. Valve's VAC can detect kernel drivers. Use only with a separate, properly signed driver. This feature is for educational purposes only.

---

## Requirements

### Minimum Requirements

| Requirement | Version |
|-------------|---------|
| **OS** | Windows 10/11 (64-bit) |
| **Visual Studio** | 2022 or 2019 |
| **Windows SDK** | 10.0 or later |
| **DirectX** | DirectX 11 |

### Optional Requirements

| Requirement | Purpose |
|-------------|---------|
| **Kernel Driver** | Memory bypass (risky) |
| **Internet** | Auto-offset updates |

---

## Building

### Step 1: Open Project

Open `External.vcxproj` in Visual Studio 2022.

### Step 2: Select Configuration

Set configuration to **Release | x64**

### Step 3: Build

```
Ctrl + Shift + B
```

### Step 4: Locate Executable

Output will be at:
```
Bin/x64/Release/External.exe
```

### Build Settings

| Setting | Value |
|---------|-------|
| Platform | x64 |
| Configuration | Release |
| Runtime Library | Multi-threaded (/MT) |
| Language Standard | C++20 |

---

## Installation

### Step 1: Extract Files

Extract all project files to a folder.

### Step 2: Install Dependencies

All dependencies are included in the project:
- ImGui (imgui folder)
- SDK headers (SDK folder)
- Feature modules (Features folder)

### Step 3: Run as Administrator

Right-click External.exe → Run as Administrator

> **Note**: Administrator privileges are required for:
> - Reading process memory
> - Creating overlay windows
> - Optional kernel driver communication

---

## How to Use

### Step 1: Start Counter-Strike 2

1. Launch Steam
2. Open Counter-Strike 2
3. Wait for main menu to fully load

### Step 2: Start LUMEN

1. Run External.exe as Administrator
2. Wait for console output:
   ```
   =========================================
   Starting cs2 Lumen External...
   =========================================
   [*] Searching for cs2.exe...
   [*] Finding modules...
   [*] Loading offsets...
   [*] Starting worker threads...
   [*] Creating overlay...
   [*] Initializing D3D11...
   =========================================
   [+] SETUP COMPLETE. Press any key to start rendering...
   =========================================
   ```
3. Press any key

### Step 3: Toggle Menu

Press **INSERT** key to show/hide the menu.

### Step 4: Configure

Navigate with mouse. Click checkboxes and sliders to adjust settings.

### Step 5: Play

Exit menu (INSERT) and play CS2 normally.

---

## Menu Controls

### Navigation

| Input | Action |
|-------|--------|
| Mouse | Move cursor |
| Left Click | Select/Toggle |
| INSERT | Toggle Menu |

### Aimbot Settings

| Setting | Default | Description |
|---------|---------|-------------|
| Master Switch | OFF | Enable/disable aimbot |
| Legit Mode | ON | Mouse-based aiming |
| Safety Lock | OFF | Block memory writes |
| Target Lock | OFF | Keep target until dead |
| Humanized | ON | Add random jitter |
| Vis Check | ON | Only visible targets |
| Auto Wall | OFF | Through walls |
| FOV | 5.0 | Field of view |
| Smooth | 10.0 | Aim smoothness |
| Aim Key | RMB | Binding key |

### Visual Settings

| Setting | Default | Description |
|---------|---------|-------------|
| ESP Master | OFF | Enable/disable ESP |
| Avatar ESP | ON | Show Steam avatars |
| Health Bar | ON | Health indicator |
| Names | ON | Player names |
| 2D Box | ON | 2D bounding box |
| Box Style | Regular | Box rendering style |
| Skeleton | OFF | Bone structure |
| RGB Mode | OFF | Rainbow colors |

---

## Updating Offsets

### Why Offsets Change

Counter-Strike 2 updates regularly, which changes memory locations. When CS2 updates, offsets must be updated for the cheat to work.

### Auto-Update (Recommended)

On startup, LUMEN attempts to download offsets from:
```
https://raw.githubusercontent.com/a2x/cs2-dumper/main/output/offsets.json
```

If this fails, it loads from local files:
```
dumper offsets/output/offsets.json
```

### Manual Update

If auto-update fails:

1. Download cs2-dumper:
   ```
   https://github.com/a2x/cs2-dumper
   ```

2. Run dumper while cs2.exe is running

3. Export JSON files:
   - `offsets.json`
   - `client_dll.json`

4. Place in:
   ```
   dumper offsets/output/
   ```

5. Restart LUMEN

### Current Build

| Item | Version |
|------|---------|
| CS2 Build | 14138+ |
| Dumper | a2x/cs2-dumper |

---

## Troubleshooting

### Error: "client.dll not found"

**Cause**: CS2 is not running

**Solution**:
1. Start Counter-Strike 2 first
2. Wait for main menu to load
3. Run External.exe

---

### Error: "Offsets failed to load"

**Cause**: No internet connection or CS2 updated

**Solution**:
1. Check internet connection
2. Or dump new offsets manually
3. Or use local offset files

---

### Menu Not Showing

**Cause**: Overlay issue or key press failed

**Solution**:
1. Press INSERT multiple times
2. Check if overlay window exists (taskbar)
3. Run as Administrator

---

### Game Crashes

**Cause**: Outdated offsets or kernel bypass

**Solution**:
1. Update offsets
2. Disable kernel bypass
3. Check for CS2 updates

---

### Aimbot Not Working

**Cause**: Configuration issue

**Solution**:
1. Enable aimbot master switch
2. Set weapon profile enabled
3. Check aim key binding
4. Ensure FOV > 0

---

### ESP Not Showing

**Cause**: ESP disabled or team filter

**Solution**:
1. Enable ESP Master
2. Disable "Enemies Only" if playing offline
3. Check visibility settings

---

## File Structure

```
External/
│
├── Main.cpp                     # Main entry point
├── Main.h                      # (if exists)
├── driver.h                    # Kernel driver interface
├── memory.h                    # Memory read/write class
├── Offsets.h                   # Game offset definitions
├── Downloader.h                # Web request class
├── External.vcxproj            # Visual Studio project
├── External.vcxproj.filters   # Project filters
│
├── SDK/
│   ├── Game.h                 # Game API functions
│   │                           # - GetEntityList()
│   │                           # - GetLocalPlayerPawn()
│   │                           # - GetViewMatrix()
│   │                           # etc.
│   └── Structs.h               # Data structures
│                               # - Vector3
│                               # - view_matrix_t
│                               # - GlobalVars
│                               # etc.
│
├── Features/
│   ├── ESP.h                  # ESP rendering
│   │                           # - EntityReaderThread()
│   │                           # - DrawESP()
│   │                           # - DrawFakeGlow()
│   ├── Aimbot.h               # Aimbot logic
│   │                           # - AimbotThread()
│   │                           # - RCS logic
│   ├── Bomb.h                 # Bomb features
│   │                           # - DrawBombInfo()
│   │                           # - DrawBombESP()
│   │                           # - DrawSpectatorList()
│   └── AvatarManager.h        # Steam avatars
│                               # - Async avatar fetching
│                               # - Texture creation
│
├── imgui/                      # Dear ImGui
│   ├── imgui.h                # Main header
│   ├── imgui.cpp              # Implementation
│   ├── imgui_draw.cpp         # Drawing
│   ├── imgui_tables.cpp       # Tables
│   ├── imgui_widgets.cpp     # Widgets
│   ├── imgui_impl_dx11.cpp  # DX11 renderer
│   ├── imgui_impl_win32.cpp  # Win32 input
│   └── [Other ImGui files]
│
└── Include/
    └── External/
        └── Include.h         # External includes
```

---

## FAQ

### Q: Is this undetected?

A: External cheats are significantly harder to detect than internal cheats because they don't modify game memory. However, no cheat is 100% safe. Use at your own risk.

---

### Q: Can I get banned?

A: Yes, using cheats can result in permanent VAC bans. This code is for educational purposes only.

---

### Q: Why does it need Administrator?

A: To read process memory and create overlay windows. This is standard for external cheats.

---

### Q: How often should I update offsets?

A: After every CS2 update. Check regularly for game updates.

---

### Q: Can I use this on VAC servers?

A: External cheats can sometimes work on VAC servers, but Valve actively works to detect them. Use at your own risk.

---

### Q: How does the overlay work?

A: LUMEN creates a transparent, topmost window that renders directly on screen. It's not rendered in the game.

---

## Credits

| Credit | Description |
|--------|-------------|
| **ImGui** | Dear ImGui GUI library |
| **a2x** | cs2-dumper offset project |
| **Catalyst** | Original code inspiration |
| **UnknownCheats** | Community testing and feedback |
| **Valve** | Creating CS2 (just kidding) |

---

## Legal

### Disclaimer

This code is for **educational purposes only**. 

By using this code, you agree to the following:

1. **No Commercial Use** - This code is free and open source
2. **At Your Own Risk** - You accept all responsibility
3. **Educational Only** - Learning game memory structures
4. **No Warranty** - Provided as-is

### Terms Violated

Using this cheat violates:
- Valve Terms of Service
- Steam Subscriber Agreement
- VAC Terms of Service

###Ban Risks

| Action | Risk Level |
|--------|------------|
| Running external | LOW-MEDIUM |
| Using kernel driver | HIGH |
| Using internal | VERY HIGH |

---

## Changelog

### v3.0 (Current Release)

**New Features:**
- Complete UI rewrite with custom dark theme
- Avatar ESP (Steam profile pictures)
- Weapon-specific aimbot profiles
- Grenade trajectory prediction
- Fake Glow effect
- Fake Chams rendering
- RGB global mode
- Spectator list
- Bomb timer HUD (glassmorphism)
- Enhanced hitmarkers

**Improvements:**
- Multi-threaded entity reading
- Async offset updates
- Better menu controls
- Custom slider/checkbox widgets

---

### v2.x

**Features:**
- Basic Aimbot
- ESP
- Skin Changer
- Hitmarkers

---

## Contact & Support

For educational discussions:
- GitHub Issues: Report bugs
- No support for cheating questions

---

## Final Warning

> **⚠️ WARNING**: This code is for learning game memory structures and reverse engineering. Using cheats in online games violates Terms of Service and can result in permanent account bans. The author is not responsible for any bans or consequences.

---

<p align="center">
Made with ⚓ by LUMEN Team
</p>