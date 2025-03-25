#include "../include/Math.hpp"
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <iostream>
#include <random>
#include <thread>
#include <vector>
#include "miniaudio.h"
namespace fs = std::filesystem;

// TODO(🚀) : make audio manager class and optimize loading
void playRandomSong(std::stop_token stopToken)
{
    const std::string directory = "../../Assets/Music";

    std::vector<std::string> songs;

    if (!fs::exists(directory) || !fs::is_directory(directory))
    {
        std::cout << "Wrong folder" << '\n';
        return;
    }

    for (const auto& entry : fs::directory_iterator(directory))
    {
        if (entry.path().extension() == ".mp3")
        {
            songs.push_back(entry.path().string());
        }
    }

    if (songs.empty())
    {
        std::cout << "No songs in the folder" << '\n';
        return;
    }

    std::srand(std::time(nullptr));
    int               songIndex    = std::rand() % songs.size();
    const std::string selectedSong = songs[songIndex];

    ma_engine engine;
    if (ma_engine_init(nullptr, &engine) != MA_SUCCESS)
    {
        std::cout << "Miniaudio has an error during its initialisation" << '\n';
        return;
    }

    while (!stopToken.stop_requested())
    { // Check if the thread should stop
        std::srand(std::time(nullptr));
        int               songIndex    = std::rand() % songs.size();
        const std::string selectedSong = songs[songIndex];

        std::cout << "Playing: " << selectedSong << '\n';

        ma_sound sound;
        if (ma_sound_init_from_file(&engine, selectedSong.c_str(), 0, NULL, NULL, &sound) != MA_SUCCESS)
        {
            std::cout << "Error playing file." << '\n';
            continue;
        }

        ma_sound_start(&sound);

        // Wait for song to finish OR stop request
        while (!stopToken.stop_requested() && ma_sound_is_playing(&sound))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        ma_sound_uninit(&sound);
    }

    ma_engine_uninit(&engine);
    std::cout << "Music stopped." << '\n';
}

bool shouldSwitchPlayer(double probability)
{
    static std::random_device   rd;
    static std::mt19937         gen(rd());
    std::bernoulli_distribution d(probability);

    return d(gen);
}

void SwitchPlayer(Board& board)
{
    double switchProbability = 0.1; // 10% chance to switch

    if (shouldSwitchPlayer(switchProbability))
    {
        board.activePlayer = (board.activePlayer->getColor()) ? &board.black : &board.white;

        std::cout << "⚡ Player SWITCHED! Now it's "
                  << (board.activePlayer->getColor() ? "White" : "Black")
                  << "'s turn!" << '\n';
    }
}