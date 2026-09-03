# Marked Treasure SSE (NG)

**Marked Treasure SSE (NG)** is an SKSE plugin for *The Elder Scrolls V: Skyrim Special Edition* that adds quest markers to treasure map locations.

When a treasure map is read, Marked Treasure creates a journal objective pointing to the corresponding treasure location, making it much easier to find the chest without having to manually interpret the map.

The NG version is the native-code continuation of Marked Treasure SSE, rebuilt using modern Skyrim modding technologies and **CommonLibSSE-NG**.

---

## Features

### Treasure Map Quest Markers

When a supported treasure map is read, Marked Treasure automatically displays a corresponding objective in the player's journal.

The objective provides a quest marker pointing towards the treasure location.

This removes the need to repeatedly refer back to the treasure map while searching for the chest.

### Treasure Completion Tracking

When the correct treasure chest is activated, the corresponding objective is automatically completed.

Completed treasure maps are tracked so that objectives are not incorrectly completed multiple times.

### Vanilla Treasure Maps

Marked Treasure supports the vanilla Skyrim treasure maps.

The plugin dynamically installs the appropriate map and treasure data when the game loads.

### Legacy of the Dragonborn Support

Marked Treasure provides support for **Legacy of the Dragonborn**, including its treasure maps.

The plugin detects whether Legacy of the Dragonborn is installed and dynamically loads the appropriate treasure map data.

This allows the same plugin to support different load orders without requiring separate patches for every installation.

### New Treasure Hunt Support

Marked Treasure supports treasure maps added by **New Treasure Hunt**.

### Treasure Hunter Support

Marked Treasure also supports treasure maps added by **Treasure Hunter for SSE**.

### Dynamic Mod Detection

Optional integrations are detected automatically.

If a supported mod is not installed, its treasure map data is simply not loaded.

This means users do not need separate versions of Marked Treasure for different combinations of supported mods.

### Scriptless Implementation

The current NG implementation uses native C++ functionality for its event handling and objective manipulation.

The plugin listens for:

* Treasure map read events
* Object activation events
* Skyrim/SKSE game lifecycle events

When a supported map is read, the appropriate treasure reference is assigned to the corresponding quest objective. When the treasure is collected, the objective is completed.

---

## Supported Content

Marked Treasure currently supports treasure maps from:

* **Skyrim Special Edition**

  * Vanilla treasure maps
* **Legacy of the Dragonborn**

  * Version 6.7.1+
* **New Treasure Hunt**
* **Treasure Hunter for SSE**

Optional integrations are detected automatically at runtime.

---

## Requirements

The following are required:

* Skyrim Special Edition / Anniversary Edition
* **SKSE64**
* **Address Library for SKSE Plugins**

Optional dependencies:

* **Legacy of the Dragonborn**
* **New Treasure Hunt**
* **Treasure Hunter for SSE**

See the [Nexus Mods page](https://www.nexusmods.com/skyrimspecialedition/mods/46011) for the current supported game versions and installation requirements.

---

## Installation

Install Marked Treasure SSE (NG) using your preferred mod manager, such as **Mod Organizer 2** or **Vortex**.

Ensure that:

1. SKSE64 is installed for your version of Skyrim.
2. Address Library for SKSE Plugins is installed.
3. Marked Treasure SSE (NG) is installed through your mod manager.
4. Skyrim is launched through SKSE64.

Once installed, no additional configuration is required.

When a supported treasure map is read, the corresponding quest marker will automatically appear.

---

## How It Works

Marked Treasure maintains a collection of supported treasure map definitions containing information about:

* Treasure map references
* Treasure chest references
* Associated quest objectives
* Plugin names
* Reference IDs
* Special-case treasure locations

When the plugin initialises, it registers the required Skyrim event handlers and determines which optional supported mods are present.

For each supported treasure map, the plugin then registers its map data.

When the player reads a supported treasure map:

1. Marked Treasure detects the map being read.
2. The corresponding treasure reference is assigned to the quest objective.
3. The objective is displayed in the journal.
4. Skyrim provides a quest marker pointing towards the treasure.

When the player activates the correct treasure chest:

1. Marked Treasure identifies the associated treasure location.
2. The treasure map is recorded as completed.
3. The corresponding quest objective is completed.

The native implementation also handles treasure locations that require special reference or position handling.

---

## Technical Details

Marked Treasure SSE (NG) is a native **C++ SKSE plugin**.

The project uses:

* **C++23**
* **CommonLibSSE-NG**
* **DKUtil**
* **SKSE**
* **Address Library**
* **CMake**
* **vcpkg**
* **spdlog**

The plugin uses Skyrim's native runtime interfaces to register event handlers and manipulate quest objectives.

The project is configured as a shared library and uses CommonLibSSE-NG and DKUtil as build dependencies.

---

## Building

### Prerequisites

A development environment capable of building Skyrim SKSE plugins is required.

The project expects the following environment variables:

* `VCPKG_ROOT`
* `CommonLibSSEPath`
* `DKUtilPath`

The project uses CMake 3.21 or newer and requires C++23 support.

### Clone the Repository

```bash
git clone https://github.com/Faen668/Marked-Treasure-NG.git
cd Marked-Treasure-NG
```

### Configure and Build

Configure and build the project using the supplied CMake configuration.

CommonLibSSE-NG and DKUtil are included as build dependencies through their configured paths.

The project also uses vcpkg for package management.

---

## Project Structure

```text
Marked-Treasure-NG/
├── cmake/
│   ├── Plugin.h.in
│   └── version.rc.in
├── src/
│   ├── Events.cpp
│   ├── Events.hpp
│   ├── PCH.h
│   └── main.cpp
├── !Update.ps1
├── .clang-format
├── .gitattributes
├── CMakeLists.txt
├── CMakePresets.json
├── EXCEPTIONS.md
├── LICENSE
└── vcpkg.json
```

### `src/main.cpp`

Initialises the SKSE plugin, configures logging and registers the SKSE messaging listener.

### `src/Events.cpp`

Contains the main Marked Treasure implementation, including:

* Event registration
* Treasure map detection
* Treasure map data installation
* Book read handling
* Treasure chest activation handling
* Objective display/completion
* Quest alias manipulation
* Optional mod detection

---

## Compatibility

Marked Treasure SSE (NG) is designed for modern Skyrim Special Edition / Anniversary Edition installations using SKSE64 and Address Library.

The plugin dynamically detects supported optional mods rather than requiring separate compatibility versions.

For the latest compatibility information, see the [Nexus Mods page](https://www.nexusmods.com/skyrimspecialedition/mods/46011).

---

## Credits

### Skyrim Script Extender

Marked Treasure requires **SKSE64**.

SKSE is developed by the SKSE Team.

### CommonLibSSE-NG

The NG version of Marked Treasure is built using **CommonLibSSE-NG**, providing modern C++ interfaces to Skyrim's runtime.

### DKUtil

Used for plugin utilities and logging functionality.

### Supported Mods

Thanks to the authors of:

* Legacy of the Dragonborn
* New Treasure Hunt
* Treasure Hunter for SSE

for creating the content that Marked Treasure provides integration for.

---

## License

Marked Treasure SSE (NG) is released under the **GNU General Public License v3.0** with the additional exceptions described in [`EXCEPTIONS.md`](EXCEPTIONS.md).

See [`LICENSE`](LICENSE) for the full license text.

See [`EXCEPTIONS.md`](EXCEPTIONS.md) for the project's modding and linking exceptions.

---

## Author

**Faen90**

* Nexus Mods: https://www.nexusmods.com/users/1383079
* GitHub: https://github.com/Faen668

---

## Links

* [Nexus Mods — Marked Treasure SSE (NG)](https://www.nexusmods.com/skyrimspecialedition/mods/46011)
* [GitHub — Marked Treasure SSE (NG)](https://github.com/Faen668/Marked-Treasure-NG)
* [GitHub — Completionist NG](https://github.com/Faen668/Completionist-NG)

---

## History

Marked Treasure originally released in 2021 and has since been substantially rewritten.

Version **2.0.0** introduced the CommonLibSSE-NG implementation and dynamic support for Legacy of the Dragonborn V6, New Treasure Hunt and Treasure Hunter.

Version **2.0.3** moved the mod to a scriptless implementation and introduced native quest-objective manipulation.

Version **2.0.5** was subsequently rewritten to resolve existing issues and provide full compatibility with the latest Legacy of the Dragonborn.

The current repository contains the native NG implementation of the plugin.
