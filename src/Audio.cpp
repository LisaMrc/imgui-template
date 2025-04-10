#include "../include/Audio.hpp"

namespace fs = std::filesystem;

void AudioEngine::playRandomSong(std::stop_token stopToken)
{
    const std::string        directory = "../../Assets/Music";
    std::vector<std::string> songs;

    if (!fs::exists(directory) || !fs::is_directory(directory))
    {
        std::cout << "Wrong folder\n";
        return;
    }

    for (const auto& entry : fs::directory_iterator(directory))
        if (entry.path().extension() == ".mp3")
            songs.push_back(entry.path().string());

    if (songs.empty())
    {
        std::cout << "No songs in the folder\n";
        return;
    }

    ma_engine engine;
    if (ma_engine_init(nullptr, &engine) != MA_SUCCESS)
    {
        std::cout << "Miniaudio failed to initialize\n";
        return;
    }

    std::srand(static_cast<unsigned>(std::time(nullptr)));
    int lastIndex = -1;

    while (!stopToken.stop_requested())
    {
        int songIndex = 0;
        do
        {
            songIndex = std::rand() % songs.size();
        } while (songs.size() > 1 && songIndex == lastIndex); // avoid repeat

        lastIndex                       = songIndex;
        const std::string& selectedSong = songs[songIndex];

        std::cout << "Playing: " << selectedSong << '\n';

        ma_sound sound;
        if (ma_sound_init_from_file(&engine, selectedSong.c_str(), 0, NULL, NULL, &sound) != MA_SUCCESS)
        {
            std::cout << "Error playing file.\n";
            continue;
        }

        ma_sound_start(&sound);

        while (!stopToken.stop_requested() && ma_sound_is_playing(&sound))
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

        ma_sound_uninit(&sound);
    }

    ma_engine_uninit(&engine);
    std::cout << "Music stopped.\n";
}

void AudioEngine::playRandomSound(std::stop_token stopToken, Board& board)
{
    ma_engine engine;
    if (ma_engine_init(nullptr, &engine) != MA_SUCCESS)
    {
        std::cerr << "Failed to initialize audio engine for sounds.\n";
        return;
    }

    std::vector<std::string> soundFiles;
    const std::string        directory = "../../Assets/Sounds";

    for (const auto& entry : std::filesystem::directory_iterator(directory))
    {
        if (entry.path().extension() == ".mp3")
            soundFiles.push_back(entry.path().string());
    }

    if (soundFiles.empty())
    {
        std::cerr << "No sounds found.\n";
        return;
    }

    while (!stopToken.stop_requested())
    {
        double delay = board.gamma.dist(board.tools.rng);
        std::this_thread::sleep_for(std::chrono::seconds(static_cast<int>(delay)));

        int                soundIndex    = std::rand() % soundFiles.size();
        const std::string& selectedSound = soundFiles[soundIndex];

        std::cout << "🔊 Random sound: " << selectedSound << " (after " << delay << "s)\n";

        ma_engine_play_sound(&engine, selectedSound.c_str(), nullptr);
    }

    ma_engine_uninit(&engine);
}
