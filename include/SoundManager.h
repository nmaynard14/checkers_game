#pragma once

#include <raylib.h>

/**
 * @brief Manages sound effects for the checkers game.
 * Handles loading and playing sound files using Raylib audio.
 * Supports a demo.mp3 override that will be used for all sound effects if present.
 */
class SoundManager {
public:
    /**
     * @brief Constructs a new SoundManager and initializes the audio system.
     * Attempts to initialize Raylib audio and load sound effect files.
     * If demo.mp3 exists, it will be used for all sound effects.
     */
    SoundManager();

    /**
     * @brief Destructor that cleans up all loaded sounds and closes the audio system.
     */
    ~SoundManager();

    /**
     * @brief Plays the move sound effect.
     * Stops any currently playing sound before playing the new one.
     */
    void playMove();

    /**
     * @brief Plays the capture sound effect.
     * Stops any currently playing sound before playing the new one.
     */
    void playCapture();

    /**
     * @brief Plays the victory sound effect.
     * Stops any currently playing sound before playing the new one.
     */
    void playWin();

    /**
     * @brief Plays the defeat sound effect.
     * Stops any currently playing sound before playing the new one.
     */
    void playLose();

    /**
     * @brief Checks if the sound system was successfully initialized.
     * @return true if audio initialization succeeded, false otherwise
     */
    bool isInitialized() const { return initialized; }

private:
    bool initialized;
    Sound demo;      // optional shared demo sound
    Sound move;
    Sound capture;
    Sound win;
    Sound lose;

    void loadSounds();
    void playSound(Sound sound);
};
