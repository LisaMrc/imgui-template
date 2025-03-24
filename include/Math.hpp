#pragma once
#include <thread>

void playRandomSong(std::stop_token stopToken);

bool shouldSwitchPlayer(double probability);