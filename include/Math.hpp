#pragma once
#include <thread>
#include "../include/Board.hpp"

void playRandomSong(std::stop_token stopToken);

bool shouldSwitchPlayer(double probability);
void SwitchPlayer(Board &board);