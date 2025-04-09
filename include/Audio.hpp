#pragma once
#include <thread>

class AudioEngine 
{
    public:
        void playRandomSong(std::stop_token stopToken);
        void playRandomSound(std::stop_token stopToken);
};