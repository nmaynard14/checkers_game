# Checkers Game

A classic 8x8 checkers game built with C++ and SDL2, featuring 3D graphics rendered with OpenGL, an AI opponent, sound effects, and a modern graphical interface.

## Description

This is a fully-featured checkers game where you play as the Teal player against a Purple AI opponent. The game includes:

- **Standard 8x8 checkers board** with traditional rules
- **AI opponent** that makes strategic moves approximately 50% of the time
- **King pieces** that can move in all four diagonal directions
- **Sound effects** for moves, captures, victories, and defeats
- **Visual feedback** with piece selection highlighting and crown graphics for kings
- **Game state tracking** with piece count display
- **Victory/Defeat popup** with options to start a new game or exit

## Building

### Prerequisites

- C++17 compatible compiler
- SDL2 development libraries
- SDL2_ttf development libraries
- SDL2_mixer development libraries
- OpenGL development libraries (libGL, libGLU)

### Linux

#### Install Dependencies

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install build-essential libsdl2-dev libsdl2-ttf-dev libsdl2-mixer-dev libgl1-mesa-dev libglu1-mesa-dev
```

**Fedora/RHEL:**
```bash
sudo dnf install gcc-c++ SDL2-devel SDL2_ttf-devel SDL2_mixer-devel mesa-libGL-devel mesa-libGLU-devel
```

**Arch Linux:**
```bash
sudo pacman -S base-devel sdl2 sdl2_ttf sdl2_mixer mesa glu
```

#### Compilation

```bash
make
```

The executable will be created in `bin/checkers_sdl`.

#### Running

```bash
./bin/checkers_sdl
```

### macOS

#### Install Dependencies

Using Homebrew (recommended):
```bash
brew install sdl2 sdl2_ttf sdl2_mixer
```
Note: OpenGL is included with macOS by default.

#### Compilation

The Makefile should work on macOS, but you may need to adjust the compiler flags. Alternatively, compile manually:

```bash
g++ -std=c++17 -Wall -Wextra -O2 \
    -I/opt/homebrew/include/SDL2 -D_REENTRANT \
    -Iinclude \
    -c src/main.cpp -o build/main.o
g++ -std=c++17 -Wall -Wextra -O2 \
    -I/opt/homebrew/include/SDL2 -D_REENTRANT \
    -Iinclude \
    -c src/GameLogic.cpp -o build/GameLogic.o
g++ -std=c++17 -Wall -Wextra -O2 \
    -I/opt/homebrew/include/SDL2 -D_REENTRANT \
    -Iinclude \
    -c src/CheckersAI.cpp -o build/CheckersAI.o
g++ -std=c++17 -Wall -Wextra -O2 \
    -I/opt/homebrew/include/SDL2 -D_REENTRANT \
    -Iinclude \
    -c src/SoundManager.cpp -o build/SoundManager.o
g++ -std=c++17 -Wall -Wextra -O2 \
    -I/opt/homebrew/include/SDL2 -D_REENTRANT \
    -Iinclude \
    -c src/Renderer.cpp -o build/Renderer.o
g++ -std=c++17 -Wall -Wextra -O2 \
    -o bin/checkers_sdl \
    build/main.o build/GameLogic.o build/CheckersAI.o \
    build/SoundManager.o build/Renderer.o \
    -L/opt/homebrew/lib -lSDL2 -lSDL2_ttf -lSDL2_mixer
```

Or use the Makefile after adjusting paths:
```bash
make
```

#### Running

```bash
./bin/checkers_sdl
```

### Windows

#### Install Dependencies

**Using vcpkg (recommended):**

1. Install vcpkg: https://github.com/Microsoft/vcpkg
2. Install SDL2 packages:
```cmd
vcpkg install sdl2:x64-windows
vcpkg install sdl2-ttf:x64-windows
vcpkg install sdl2-mixer:x64-windows
```
Note: OpenGL is typically included with Windows SDK or can be obtained via vcpkg.

**Using MSYS2/MinGW:**

```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-SDL2 \
          mingw-w64-x86_64-SDL2_ttf mingw-w64-x86_64-SDL2_mixer \
          mingw-w64-x86_64-mesa
```

#### Compilation

**Using Visual Studio:**

1. Create a new C++ project
2. Add all source files from `src/` and headers from `include/`
3. Configure include directories:
   - `include/`
   - SDL2 include path (e.g., `C:\vcpkg\installed\x64-windows\include`)
4. Configure library directories:
   - SDL2 library path (e.g., `C:\vcpkg\installed\x64-windows\lib`)
5. Link against: `SDL2.lib`, `SDL2_ttf.lib`, `SDL2_mixer.lib`
6. Copy SDL2 DLLs to the output directory

**Using MinGW/MSYS2:**

```bash
g++ -std=c++17 -Wall -Wextra -O2 \
    -Iinclude -ISDL2_include_path \
    -c src/main.cpp -o build/main.o
# Repeat for all .cpp files
g++ -std=c++17 -Wall -Wextra -O2 \
    -o bin/checkers_sdl.exe \
    build/*.o -lSDL2 -lSDL2_ttf -lSDL2_mixer
```

#### Running

```cmd
bin\checkers_sdl.exe
```

**Note:** Ensure SDL2 DLLs are in the same directory as the executable or in your PATH.

## Project Structure

```
.
├── assets/          # Sound effect files (MP3 format)
├── bin/            # Compiled executable (generated)
├── build/          # Object files (generated)
├── include/        # Header files
│   ├── CheckersAI.h
│   ├── GameLogic.h
│   ├── Renderer.h
│   └── SoundManager.h
└── src/            # Source files
    ├── CheckersAI.cpp
    ├── GameLogic.cpp
    ├── main.cpp
    ├── Renderer.cpp
    └── SoundManager.cpp
```

## Sound Effects

The game supports MP3 sound effects. Place the following files in the `assets/` directory:

- `move.mp3` - Sound for regular moves
- `capture.mp3` - Sound for piece captures
- `win.mp3` - Victory sound
- `lose.mp3` - Defeat sound

Alternatively, place `demo.mp3` in the `assets/` directory to use a single sound file for all effects.

## Resources and Credits

### Libraries

- **SDL2** - Cross-platform development library for low-level access to audio, keyboard, mouse, joystick, and graphics hardware
- **SDL2_ttf** - TrueType font rendering library for SDL2
- **SDL2_mixer** - Multi-channel audio mixer library for SDL2
- **OpenGL** - 3D graphics rendering API used for rendering the game board and pieces in 3D
- **GLU** - OpenGL Utility Library for additional 3D rendering functions

### Sound Effects

Sound effects use royalty-free chiptune audio files. Sources include:
- Pixabay (https://pixabay.com/sound-effects/)
- Orange Free Sounds (https://orangefreesounds.com)
- Other royalty-free chiptune sound effect libraries

## Development

This project was created using [Cursor](https://cursor.sh), an AI-powered code editor.

## Developer

**Nick Maynard** (nmaynard14)

## License

This project is provided as-is for educational and personal use.

## Keeping the README Updated

This README should be updated whenever significant changes are made to:
- **Project structure** (new files, directories, or reorganization)
- **Build system** (Makefile changes, new dependencies, compilation flags)
- **Dependencies** (new libraries, version requirements, platform-specific needs)
- **Features** (new functionality, removed features, gameplay changes)
- **Platform support** (new platforms, platform-specific fixes)

A pre-commit hook is configured to remind you to update the README when source files are modified.

