#include "../include/Math.hpp"
#include <iostream>
#include <random>

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