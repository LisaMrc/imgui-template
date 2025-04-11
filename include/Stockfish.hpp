#include <windows.h>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

class StockfishEngine {
public:
    StockfishEngine(const std::string& path = "../../AI/stockfish.exe")
    {
        SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};

        // Create pipes for STDOUT
        CreatePipe(&hReadFromEngine, &hWriteToMe, &sa, 0);
        SetHandleInformation(hReadFromEngine, HANDLE_FLAG_INHERIT, 0);

        // Create pipes for STDIN
        CreatePipe(&hReadFromMe, &hWriteToEngine, &sa, 0);
        SetHandleInformation(hWriteToEngine, HANDLE_FLAG_INHERIT, 0);

        // Set up the startup info
        STARTUPINFOA si = {};
        si.cb           = sizeof(si);
        si.dwFlags |= STARTF_USESTDHANDLES;
        si.hStdInput  = hReadFromMe;
        si.hStdOutput = hWriteToMe;
        si.hStdError  = hWriteToMe;

        PROCESS_INFORMATION pi = {};

        BOOL success = CreateProcessA(
            path.c_str(), nullptr, nullptr, nullptr, TRUE,
            0, nullptr, nullptr, &si, &pi
        );

        if (!success)
        {
            std::cerr << "Failed to launch Stockfish.\n";
            exit(1);
        }

        hProcess = pi.hProcess;
        hThread  = pi.hThread;

        // Launch a background thread to read from Stockfish
        readingThread = std::thread([&]() {
            char        buffer[128];
            DWORD       bytesRead;
            std::string output;

            while (true)
            {
                if (ReadFile(hReadFromEngine, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0)
                {
                    buffer[bytesRead] = '\0';
                    output += buffer;

                    size_t movePos = output.find("bestmove");

                    // If we received a full response like "bestmove", print it
                    if (movePos != std::string::npos)
                    {
                        bestMove = output.substr(movePos + 9, 4);
                        std::lock_guard<std::mutex> lock(outputMutex);
                        lastOutput = output;
                        output.clear();
                    }
                }
            }
        });
    }

    ~StockfishEngine()
    {
        std::cout << "Bye\n";
        WriteToEngine("quit\n");
        CloseHandle(hReadFromEngine);
        CloseHandle(hWriteToMe);
        CloseHandle(hReadFromMe);
        CloseHandle(hWriteToEngine);
        TerminateProcess(hProcess, 0);
        CloseHandle(hProcess);
        CloseHandle(hThread);
        readingThread.detach();
    }

    void WriteToEngine(const std::string& command)
    {
        DWORD written;
        WriteFile(hWriteToEngine, command.c_str(), command.size(), &written, NULL);
    }

    std::string GetLastOutput()
    {
        std::lock_guard<std::mutex> lock(outputMutex);
        return lastOutput;
    }

    std::string getBestMove()
    {
        return bestMove;
    }

private:
    HANDLE      hReadFromEngine = NULL;
    HANDLE      hWriteToMe      = NULL;
    HANDLE      hReadFromMe     = NULL;
    HANDLE      hWriteToEngine  = NULL;
    HANDLE      hProcess        = NULL;
    HANDLE      hThread         = NULL;
    std::thread readingThread;

    std::string lastOutput;
    std::mutex  outputMutex;

    std::string bestMove;
};
