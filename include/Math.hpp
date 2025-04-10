#pragma once

#include <imgui.h>
#include <ctime>
#include <random>

class Tools {
public:
    std::random_device rd;
    std::mt19937       rng = std::mt19937(rd());
};

class Binomial {
public:
    std::default_random_engine      engine = std::default_random_engine(static_cast<unsigned int>(time(0)));
    std::binomial_distribution<int> dist   = std::binomial_distribution<int>(2, 0.33); // 2 trials, 33% chance each
    bool                            done   = false;
    float                           result = .0f;

    void printQuote(ImFont* defaultFont, ImFont* customFont)
    {
        ImGui::PopFont();

        ImGui::PushFont(defaultFont);

        static const std::vector<std::string> quotes = {
            "Believe in your move.",
            "Every square matters.",
            "Even the king started as a pawn.",
            "Patience is power.",
            "Think ahead. Always."
        };

        static int idx = rand() % quotes.size();
        ImGui::Text("%s", quotes[idx].c_str());

        ImGui::PopFont();

        ImGui::PushFont(customFont);
    }
};

class Exponential {
public:
    std::exponential_distribution<> dist = std::exponential_distribution<>(0.14); // 90% chance of a piece in the [0, 16] range being removed
    bool                            done = false;
};

class Gamma {
public:
    std::gamma_distribution<double> dist = std::gamma_distribution<double>(4, 7.5);
    bool                            done = false;
};

class Bernoulli {
public:
    std::default_random_engine engine;      // Random engine
    std::bernoulli_distribution dist;     // Bernoulli distribution

    bool done = false;
    bool result = false;

    Bernoulli() : engine(static_cast<unsigned int>(time(0))) {}

    // Method to perform the Bernoulli trial and return the result
    bool flip(double probability)
    {
        dist = std::bernoulli_distribution(probability);
        result = dist(engine);
        return result;
    }

    bool getResult() const {
        return result;
    }
};

class Multinomial {
public:
    std::default_random_engine engine;
    std::discrete_distribution<int> dist;
    int result = -1;
    bool done = false;

    Multinomial(const std::vector<double>& probabilities)
        : engine(static_cast<unsigned int>(time(0))),
          dist(probabilities.begin(), probabilities.end()) {}

    int roll() {
        result = dist(engine);
        done = true;
        return result;
    }

    int getResult() const {
        return result;
    }
};

class Poisson {
public:
    std::default_random_engine engine;
    std::poisson_distribution<int> dist;
    int result = -1;
    bool done = false;

    explicit Poisson(double lambda)
        : engine(static_cast<unsigned int>(time(0))),
          dist(lambda) {}

    int draw() {
        result = dist(engine);
        done = true;
        return result;
    }

    int getResult() const {
        return result;
    }
};