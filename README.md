# Checkers Game

A classic 8x8 checkers game built with C++ and Raylib, featuring 3D graphics, an AI opponent, sound effects, and a modern graphical interface.

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
- Raylib development library (version 4.0 or later)

### Linux

#### Install Dependencies

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install build-essential libraylib-dev
```

If `libraylib-dev` is not available in your repositories, you can build Raylib from source:
```bash
git clone https://github.com/raysan5/raylib.git
cd raylib/src
make PLATFORM=PLATFORM_DESKTOP
sudo make install
```

**Fedora/RHEL:**
```bash
sudo dnf install gcc-c++ raylib-devel
```

If not available, build from source (see Ubuntu instructions above).

**Arch Linux:**
```bash
sudo pacman -S base-devel raylib
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
brew install raylib
```

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

- **Raylib** - Simple and easy-to-use library for game development with built-in support for 3D graphics, audio, input handling, and text rendering. Raylib provides a clean, modern API that simplifies game development compared to lower-level libraries like SDL2 + OpenGL.

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

