#pragma once
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <iostream>
#include <random>
#include <thread>
#include <vector>
#include "../include/Board.hpp"
#include "miniaudio.h"

class AudioEngine {
public:
    void playRandomSong(std::stop_token stopToken);
    void playRandomSound(std::stop_token stopToken, Board&);
};