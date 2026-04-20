# LUMEN EXTERNAL - CS2 External Cheat

![Platform](https://img.shields.io/badge/Platform-Windows-blue)
![Language](https://img.shields.io/badge/Language-C++17-orange)
![Target](https://img.shields.io/badge/Target-CS2-yellow)

---

## Project Summary

**LUMEN EXTERNAL** is a production-ready external cheat for Counter-Strike 2 (CS2). It runs as a desktop overlay (not injected into the game process), making it highly stable and less prone to bans.

### Architecture

| Component | Description |
|-----------|-----------|
| **Type** | External Overlay Cheat |
| **Method** | ReadProcessMemory / Optional Kernel Driver |
| **Rendering** | DirectX 11 via ImGui |
| **Menu** | Custom dark-themed GUI |
| **Protection** | Optional kernel bypass via r6-driver |

### Features

#### Core Features
- **Aimbot** - Legit/Rage modes with weapon-specific configs
- **ESP** - 2D/3D boxes, skeleton, health bars, names, avatars
- **Skin Changer** - Custom paint kits with wear/StatTrak
- **Bomb Features** - Timer HUD, ESP, site detection
- **Misc** - Hitmarkers, hitsounds, spectator list, watermark
- **Grenade Prediction** - Trajectory draw for all grenades

#### Technical Features
- Auto-updating offsets from GitHub
- Kernel driver bypass support
- Async avatar fetching from Steam
- Multi-threaded architecture

---

## Kernel Driver Explanation

The project includes a kernel driver interface (`driver.h`) that can read/write process memory at ring 0, bypassing any user-mode anti-cheat. However:

> **WARNING**: Using kernel drivers is highly detectable and can lead to permanent bans. Use only with a proper kernel bypass driver loaded separately.

### How It Works

The `KernelDriver` class communicates with a kernel driver (r6-driver) via Windows Registry callback technique:

```
User Mode App → Registry Write → Kernel Driver →
  → Reads/Writes CS2 Process Memory → Returns Result
```

**Files involved:**
- `driver.h` - Driver interface class
- `memory.h` - Memory class that falls back to kernel if enabled

**To enable kernel bypass:**
1. Load the kernel driver separately
2. Enable "Kernel Bypass" in the menu (Misc tab)

---

## Building the Project

### Prerequisites

- **Visual Studio 2022** (or 2019)
- **Windows SDK** (10.0 or later)
- **DirectX 11 SDK**

### Build Steps

1. Open `External.vcxproj` in Visual Studio
2. Set configuration to **Release | x64**
3. Build → Build Solution (Ctrl+Shift+B)

**Output:** `Bin/x64/Release/External.exe`

### Dependencies

The project includes all required headers:
- `imgui/` - Dear ImGui (included)
- `SDK/` - Game structs and functions
- `Features/` - All cheat modules

---

## Running the Cheat

### Step by Step Tutorial

#### Step 1: Start the Game
```
1. Launch Steam
2. Open Counter-Strike 2
3. Wait for main menu to fully load
```

#### Step 2: Start the External
```
1. Run External.exe as Administrator
2. Wait for "SETUP COMPLETE" message
3. Press any key to start rendering
```

#### Step 3: Access the Menu
```
- Press INSERT to toggle menu visibility
- Use mouse to navigate
- Click checkboxes/sliders to configure
```

#### Step 4: Configure Features

**Aimbot Tab:**
- Enable Master Switch
- Set weapon profile (Rifles/SMGs/Pistols/Snipers)
- Configure FOV, smoothness, bone
- Set aim key (default: Right Mouse Button)

**Visuals Tab:**
- Enable ESP Master
- Toggle boxes, skeleton, health bar
- Configure RGB/rainbow modes

**Misc Tab:**
- Enable watermark
- Configure hitsounds
- Set up bomb timer HUD

---

## Updating Offsets

Offsets are critical for the cheat to work. CS2 updates frequently, so keeping offsets up to date is essential.

### Method 1: Auto-Update (Default)

The cheat automatically downloads offsets from GitHub on startup:
```
URL: https://raw.githubusercontent.com/a2x/cs2-dumper/main/output/offsets.json
```

If web update fails, it falls back to local file:
```
File: dumper offsets/output/offsets.json
```

### Method 2: Manual Update

If offsets break:

1. Download the latest dumper from:
   https://github.com/a2x/cs2-dumper

2. Run the dumper on your CS2 version

3. Export to JSON format

4. Place files in:
   ```
   dumper offsets/output/offsets.json
   dumper offsets/output/client_dll.json
   ```

5. Restart the external

### Current Tested Build

- **CS2 Build**: 14138+
- **Offsets Source**: a2x/cs2-dumper

---

## Troubleshooting

### "client.dll not found"
- CS2 is not running
- Run game first, then external

### "Offsets failed to load"
- No internet connection
- Use local offset files
- CS2 updated - dump new offsets

### Menu not showing
- Press INSERT key
- Check overlay compatibility

### Game crash
- Offsets outdated
- Run as Administrator
- Disable kernel bypass

---

## File Structure

```
External/
├── Main.cpp                 # Entry point, menu, settings
├── driver.h                 # Kernel driver interface
├── memory.h                # Process memory class
├── Offsets.h                # Game offsets + auto-update
├── Downloader.h             # Web request helper
├── External.vcxproj         # Visual Studio project
│
├── SDK/
│   ├── Game.h              # Game API functions
│   └── Structs.h            # Vector3, Matrix, etc
│
├── Features/
│   ├── ESP.h               # Visual rendering
│   ├── Aimbot.h            # Aim logic + RCS
│   ├── Bomb.h               # Bomb timer/ESP
│   └── AvatarManager.h       # Steam avatar fetch
│
├── imgui/                   # Dear ImGui rendering
└── Include/                 # Headers
```

---

## Credits

- **ImGui** - GUI framework
- **a2x/cs2-dumper** - Offset dumping
- **Catalyst** - Original code inspiration
- **UnknownCheats Community** - Testing

---

## Legal Warning

This code is for educational purposes only. 
Using external cheats in online games violates TOS and can result in permanent bans.
Use at your own risk.

---

## Git Push Commands

To push to your repository:

```bash
# Initialize (if not already)
git init

# Add remote
git remote add origin https://github.com/BlackWh1te/CS2-EXTERNAL-KERNAL-BASE-2026-C-.git

# Add all files
git add .

# Commit
git commit -m "Initial commit: LUMEN External CS2 Cheat"

# Push
git push -u origin main
```

---

## Changelog

### v3.0 (Current)
- Complete UI rewrite with dark theme
- Avatar ESP (Steam profile pictures)
- Weapon-specific aimbot profiles
- Grenade trajectory prediction
- Fake glow + Fake chams
- RGB global mode
- Spectator list
- Bomb timer HUD

### v2.x
- Initial release
- Basic ESP + Aimbot
- Skin changer
- Hitmarkers