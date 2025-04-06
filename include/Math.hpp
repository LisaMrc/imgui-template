#include <imgui.h>
#include <ctime>
#include <random>

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
