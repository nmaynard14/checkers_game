# Checkers Game

A classic 8x8 checkers game built with C++ and SDL2, featuring an AI opponent, sound effects, and a modern graphical interface.

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

- C++17 compatible compiler (g++ recommended)
- SDL2 development libraries
- SDL2_ttf development libraries
- SDL2_mixer development libraries

On Ubuntu/Debian:
```bash
sudo apt install libsdl2-dev libsdl2-ttf-dev libsdl2-mixer-dev
```

### Compilation

```bash
make
```

The executable will be created in `bin/checkers_sdl`.

### Running

```bash
./bin/checkers_sdl
```

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

