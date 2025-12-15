#include "SoundManager.h"

#include <iostream>

/**
 * @brief Constructs a new SoundManager and initializes the audio system.
 * Attempts to initialize Raylib audio and load sound effect files.
 * If demo.mp3 exists, it will be used for all sound effects.
 */
SoundManager::SoundManager()
    : initialized(false), demo{}, move{}, capture{}, win{}, lose{}
{
    // Initialize Raylib audio
    InitAudioDevice();
    
    if (!IsAudioDeviceReady()) {
        std::cerr << "Failed to initialize audio device\n";
        return;
    }

    initialized = true;
    loadSounds();
}

/**
 * @brief Destructor that cleans up all loaded sounds and closes the audio system.
 */
SoundManager::~SoundManager() {
    // Unload sounds (check if frameCount > 0 to see if sound was loaded)
    if (demo.frameCount > 0) {
        UnloadSound(demo);
    } else {
        if (move.frameCount > 0)    UnloadSound(move);
        if (capture.frameCount > 0) UnloadSound(capture);
        if (win.frameCount > 0)     UnloadSound(win);
        if (lose.frameCount > 0)    UnloadSound(lose);
    }

    if (initialized) {
        CloseAudioDevice();
    }
}

/**
 * @brief Loads sound effect files from the assets directory.
 * If demo.mp3 exists, it will be used for all sound effects.
 * Otherwise, loads individual MP3 files for move, capture, win, and lose.
 */
void SoundManager::loadSounds() {
    // If demo.mp3 exists and loads, use it for all sound effects.
    if (FileExists("assets/demo.mp3")) {
        demo = LoadSound("assets/demo.mp3");
        if (demo.frameCount > 0) {
            std::cout << "Using demo.mp3 for all sound effects\n";
            return;
        }
    }

    // Otherwise, load individual sound files
    if (FileExists("assets/move.mp3")) {
        move = LoadSound("assets/move.mp3");
    }
    if (FileExists("assets/capture.mp3")) {
        capture = LoadSound("assets/capture.mp3");
    }
    if (FileExists("assets/win.mp3")) {
        win = LoadSound("assets/win.mp3");
    }
    if (FileExists("assets/lose.mp3")) {
        lose = LoadSound("assets/lose.mp3");
    }
}

/**
 * @brief Plays a sound effect, stopping any currently playing sound first.
 * @param sound The sound to play
 */
void SoundManager::playSound(Sound sound) {
    if (sound.frameCount == 0) return;  // Sound not loaded
    
    // Stop any currently playing sound and play the new one
    StopSound(sound);  // Stop this specific sound if playing
    PlaySound(sound);
}

/**
 * @brief Plays the move sound effect.
 * Stops any currently playing sound before playing the new one.
 */
void SoundManager::playMove() {
    if (demo.frameCount > 0) {
        playSound(demo);
    } else if (move.frameCount > 0) {
        playSound(move);
    }
}

/**
 * @brief Plays the capture sound effect.
 * Stops any currently playing sound before playing the new one.
 */
void SoundManager::playCapture() {
    if (demo.frameCount > 0) {
        playSound(demo);
    } else if (capture.frameCount > 0) {
        playSound(capture);
    }
}

/**
 * @brief Plays the victory sound effect.
 * Stops any currently playing sound before playing the new one.
 */
void SoundManager::playWin() {
    if (demo.frameCount > 0) {
        playSound(demo);
    } else if (win.frameCount > 0) {
        playSound(win);
    }
}

/**
 * @brief Plays the defeat sound effect.
 * Stops any currently playing sound before playing the new one.
 */
void SoundManager::playLose() {
    if (demo.frameCount > 0) {
        playSound(demo);
    } else if (lose.frameCount > 0) {
        playSound(lose);
    }
}
