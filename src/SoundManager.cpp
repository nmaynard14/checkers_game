#include "SoundManager.h"

#include <iostream>

/**
 * @brief Constructs a new SoundManager and initializes the audio system.
 * Attempts to open the SDL_mixer audio device and load sound effect files.
 * If demo.mp3 exists, it will be used for all sound effects.
 */
SoundManager::SoundManager()
    : initialized(false), demo(nullptr), move(nullptr),
      capture(nullptr), win(nullptr), lose(nullptr)
{
    // Initialize audio (SDL_mixer)
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Mix_OpenAudio Error: " << Mix_GetError() << "\n";
        return;
    }

    initialized = true;
    loadSounds();
}

/**
 * @brief Destructor that cleans up all loaded sound chunks and closes the audio system.
 */
SoundManager::~SoundManager() {
    // Free sound chunks
    if (demo) {
        Mix_FreeChunk(demo);
    } else {
        if (move)    Mix_FreeChunk(move);
        if (capture) Mix_FreeChunk(capture);
        if (win)     Mix_FreeChunk(win);
        if (lose)    Mix_FreeChunk(lose);
    }

    if (initialized) {
        Mix_CloseAudio();
    }
}

/**
 * @brief Loads sound effect files from the assets directory.
 * If demo.mp3 exists, it will be used for all sound effects.
 * Otherwise, loads individual MP3 files for move, capture, win, and lose.
 */
void SoundManager::loadSounds() {
    // If demo.mp3 exists and loads, use it for all sound effects.
    demo = Mix_LoadWAV("assets/demo.mp3");
    if (demo) {
        move    = demo;
        capture = demo;
        win     = demo;
        lose    = demo;
    } else {
        // Otherwise, load individual MP3 sound effects.
        move    = Mix_LoadWAV("assets/move.mp3");
        capture = Mix_LoadWAV("assets/capture.mp3");
        win     = Mix_LoadWAV("assets/win.mp3");
        lose    = Mix_LoadWAV("assets/lose.mp3");
    }
}

/**
 * @brief Internal helper function to play a sound chunk.
 * Stops any currently playing sound before playing the new one.
 * @param chunk The sound chunk to play, or nullptr to do nothing
 */
void SoundManager::playSound(Mix_Chunk *chunk) {
    if (!chunk) return;
    // Stop any currently playing sound effect
    Mix_HaltChannel(-1);
    Mix_PlayChannel(-1, chunk, 0);
}

/**
 * @brief Plays the move sound effect.
 * Stops any currently playing sound before playing the new one.
 */
void SoundManager::playMove() {
    playSound(move);
}

/**
 * @brief Plays the capture sound effect.
 * Stops any currently playing sound before playing the new one.
 */
void SoundManager::playCapture() {
    playSound(capture);
}

/**
 * @brief Plays the victory sound effect.
 * Stops any currently playing sound before playing the new one.
 */
void SoundManager::playWin() {
    playSound(win);
}

/**
 * @brief Plays the defeat sound effect.
 * Stops any currently playing sound before playing the new one.
 */
void SoundManager::playLose() {
    playSound(lose);
}

