// #include "../include/Stockfish.hpp"

// FILE* stockfish = nullptr;

// void Stockfish::start()
// {
//     stockfish = _popen("../../AI/stockfish", "r+");
//     if (!stockfish)
//     {
//         std::cerr << "Failed to start Stockfish" << std::endl;
//         return;
//     }

//     fprintf(stockfish, "uci\n");
//     fflush(stockfish);
// }

// void Stockfish::sendPositionAndGetMove(const std::string& fen)
// {
//     fprintf(stockfish, "position fen %s\n", fen.c_str());
//     fprintf(stockfish, "go depth 15\n");
//     fflush(stockfish);

//     char buffer[256];
//     while (fgets(buffer, sizeof(buffer), stockfish))
//     {
//         std::string line(buffer);
//         if (line.find("bestmove") != std::string::npos)
//         {
//             std::cout << "Best move: " << line;
//             break;
//         }
//     }
// }

// void Stockfish::stop()
// {
//     if (stockfish)
//     {
//         fprintf(stockfish, "quit\n");
//         _pclose(stockfish);
//     }
// }