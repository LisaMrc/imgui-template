#include "../include/Board.hpp"
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "../include/Math.hpp"
#include "../include/Piece.hpp"
#include "imgui.h"

void Board::init()
{
    board_size = 800.0f;
    tile_size  = board_size / 8.0f;

    lightSquare     = IM_COL32(255, 206, 158, 255);
    darkSquare      = IM_COL32(209, 139, 71, 255);
    highlight_color = IM_COL32(0, 255, 0, 255);
    check_color     = IM_COL32(255, 0, 0, 255);

    pieces.emplace_back(std::make_unique<Pawn>(6, 0, true));
    pieces.emplace_back(std::make_unique<Pawn>(6, 1, true));
    pieces.emplace_back(std::make_unique<Pawn>(6, 2, true));
    pieces.emplace_back(std::make_unique<Pawn>(6, 3, true));
    pieces.emplace_back(std::make_unique<Pawn>(6, 4, true));
    pieces.emplace_back(std::make_unique<Pawn>(6, 5, true));
    pieces.emplace_back(std::make_unique<Pawn>(6, 6, true));
    pieces.emplace_back(std::make_unique<Pawn>(6, 7, true));
    pieces.emplace_back(std::make_unique<Pawn>(1, 0, false));
    pieces.emplace_back(std::make_unique<Pawn>(1, 1, false));
    pieces.emplace_back(std::make_unique<Pawn>(1, 2, false));
    pieces.emplace_back(std::make_unique<Pawn>(1, 3, false));
    pieces.emplace_back(std::make_unique<Pawn>(1, 4, false));
    pieces.emplace_back(std::make_unique<Pawn>(1, 5, false));
    pieces.emplace_back(std::make_unique<Pawn>(1, 6, false));
    pieces.emplace_back(std::make_unique<Pawn>(1, 7, false));

    pieces.emplace_back(std::make_unique<Rook>(7, 0, true));
    pieces.emplace_back(std::make_unique<Rook>(7, 7, true));
    pieces.emplace_back(std::make_unique<Rook>(0, 0, false));
    pieces.emplace_back(std::make_unique<Rook>(0, 7, false));

    pieces.emplace_back(std::make_unique<Knight>(7, 1, true));
    pieces.emplace_back(std::make_unique<Knight>(7, 6, true));
    pieces.emplace_back(std::make_unique<Knight>(0, 1, false));
    pieces.emplace_back(std::make_unique<Knight>(0, 6, false));

    pieces.emplace_back(std::make_unique<Bishop>(7, 2, true));
    pieces.emplace_back(std::make_unique<Bishop>(7, 5, true));
    pieces.emplace_back(std::make_unique<Bishop>(0, 2, false));
    pieces.emplace_back(std::make_unique<Bishop>(0, 5, false));

    pieces.emplace_back(std::make_unique<Queen>(7, 3, true));
    pieces.emplace_back(std::make_unique<Queen>(0, 3, false));

    pieces.emplace_back(std::make_unique<King>(7, 4, true));
    pieces.emplace_back(std::make_unique<King>(0, 4, false));

    for (auto& piece : pieces)
    {
        piece->board = this;
    }

    if (kings.size() < 2)
    {
        kings.emplace_back(pieces[pieces.size() - 1].get());
        kings.emplace_back(pieces[pieces.size() - 2].get());
    }

    ImGuiIO& io = ImGui::GetIO();
    defaultFont = io.Fonts->AddFontFromFileTTF("../../Assets/Fonts/ROBOTO.ttf", 20.0f);
}

bool Board::IsValidMove(Piece* piece, int row, int col)
{
    if (!piece)
        return false;

    if (isBlocked(row, col))
    {
        return false;
    }

    Piece* targetPiece = getPieceAt(row, col);

    if (!isPathClear(piece, row, col) && piece->getType() != 'N')
    {
        return false;
    }

    if (piece->getType() == 'P')
        piece->isCapture = (targetPiece && targetPiece->isWhite != piece->isWhite);

    // En passant capture condition
    if (piece->getType() == 'P')
    {
        if (enPassantTarget.has_value() && row == enPassantTarget->first && col == enPassantTarget->second && abs(piece->col - col) == 1 && ((piece->isWhite && row == piece->row - 1) || (!piece->isWhite && row == piece->row + 1)))
        {
            return true;
        }
    }

    if (piece->getType() == 'K')
    {
        King* king = dynamic_cast<King*>(piece);
        if (king && king->canCastle(row, col))
        {
            isCastle = true;
            performCastle(king, row, col);
            return true;
        }
    }

    return piece->canMove(row, col);
}

bool Board::isPathClear(Piece* piece, int destRow, int destCol)
{
    int startRow = piece->row;
    int startCol = piece->col;

    int rowDirection = (destRow > startRow) ? 1 : (destRow < startRow) ? -1
                                                                       : 0;
    int colDirection = (destCol > startCol) ? 1 : (destCol < startCol) ? -1
                                                                       : 0;

    int r = startRow + rowDirection;
    int c = startCol + colDirection;

    while (r != destRow || c != destCol)
    {
        // If the square is blocked, return false
        if (isBlocked(r, c))
        {
            std::cout << "Path is blocked by a leaf!" << std::endl;
            return false;
        }

        if (getPieceAt(r, c) != nullptr)
        {
            return false;
        }

        r += rowDirection;
        c += colDirection;

        if (r < 0 || r >= 8 || c < 0 || c >= 8)
            return false;
    }

    return true;
}

Piece* Board::getPieceAt(int row, int col)
{
    for (const auto& piece : pieces)
    {
        if (piece->row == row && piece->col == col)
        {
            return piece.get();
        }
    }
    return nullptr;
}

void Board::removePiece(Piece* piece)
{
    if (!piece)
        return;

    if (piece->getType() == 'K')
    {
        kings.erase(!piece->isWhite ? kings.begin() : kings.begin() + 1);
        gameOver = true;
    }

    auto it = std::find_if(pieces.begin(), pieces.end(), [piece](const std::unique_ptr<Piece>& p) {
        return p.get() == piece;
    });

    obj3D myObj;

    if (it != pieces.end())
    {
        myObj = renderEngine.gameObjects[std::distance(pieces.begin(), it)+1];
    }

    if(!myObj.piece)
        return;

    renderEngine.gameObjects.erase(std::remove_if(renderEngine.gameObjects.begin()+1, renderEngine.gameObjects.end(), [myObj](const obj3D& obj) {
        std::cout << obj.piece->getType() << "\n";
        return obj == myObj;
    }),
    renderEngine.gameObjects.end());

    pieces.erase(std::remove_if(pieces.begin(), pieces.end(), [piece](const std::unique_ptr<Piece>& p) {
                     return p.get() == piece;
                 }),
                 pieces.end());
}

void Board::performCastle(King* king, int destRow, int destCol)
{
    isCastle = false;

    std::cout << toChessNotation(selectedPiece->row, selectedPiece->col) + toChessNotation(destRow, destCol) + " \n";
    movesPlayed.push_back(toChessNotation(selectedPiece->row, selectedPiece->col) + toChessNotation(destRow, destCol) + " ");

    moveCount += .5;

    stateStartTime = currentTime;
    whitePlayed    = true;

    activePlayer = activePlayer == &white ? &black : &white;

    int    direction = (destCol > king->col) ? 1 : -1;
    int    rookCol   = (direction == 1) ? 7 : 0;
    Piece* rook      = getPieceAt(king->row, rookCol);

    if (!rook)
        return;

    king->col = destCol;

    rook->col = king->col - direction;

    king->firstMove = false;
    rook->firstMove = false;

    std::cout << "Castled\n";
}

bool Board::isKingInCheck(King* king)
{
    if (!king)
        return false;

    bool isKingAttacked = false;

    for (auto& piece : pieces)
    {
        if (piece->isWhite != king->isWhite)
        {
            if (IsValidMove(piece.get(), king->row, king->col))
            {
                isKingAttacked = true;
            }
        }
    }

    return isKingAttacked;
}

bool Board::canHighlightSquares(Piece* selectedPiece, bool isWhite, int row, int col)
{
    if (selectedPiece->getType() == 'P')
    {
        int direction = selectedPiece->isWhite ? -1 : 1;

        // Forward movement
        if (selectedPiece->firstMove)
        {
            if (row == selectedPiece->row + 2 * direction && col == selectedPiece->col)
            {
                return true;
            }
        }

        if (row == selectedPiece->row + direction && col == selectedPiece->col)
        {
            return true;
        }

        // Diagonal captures
        if (row == selectedPiece->row + direction && (col == selectedPiece->col + 1 || col == selectedPiece->col - 1))
        {
            Piece* target = getPieceAt(row, col);
            if (target && target->isWhite != selectedPiece->isWhite)
            {
                return true;
            }
        }
    }
    if (selectedPiece->getType() == 'R')
    {
        if ((selectedPiece->row == row || selectedPiece->col == col) && isPathClear(selectedPiece, row, col))
        {
            return true;
        }
    }
    if (selectedPiece->getType() == 'N')
    {
        int dx = abs(selectedPiece->row - row);
        int dy = abs(selectedPiece->col - col);
        if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2))
        {
            return true;
        }
    }
    if (selectedPiece->getType() == 'B')
    {
        if (abs(selectedPiece->row - row) == abs(selectedPiece->col - col) && isPathClear(selectedPiece, row, col))
        {
            return true;
        }
    }
    if (selectedPiece->getType() == 'Q')
    {
        if (((selectedPiece->row == row || selectedPiece->col == col) || // Rook move
             (abs(selectedPiece->row - row) == abs(selectedPiece->col - col)))
            && // Bishop move
            isPathClear(selectedPiece, row, col))
        {
            return true;
        }
    }
    if (selectedPiece->getType() == 'K')
    {
        int dx = abs(selectedPiece->row - row);
        int dy = abs(selectedPiece->col - col);
        if (dx <= 1 && dy <= 1)
        {
            return true;
        }
    }
    return false;
}

void Board::highlightSquares(ImVec2 min, ImVec2 max, int row, int col)
{
    Piece* targetPiece = getPieceAt(row, col);

    if (!targetPiece || targetPiece->isWhite != selectedPiece->isWhite)
    {
        if (selectedPiece->isWhite)
        {
            if (canHighlightSquares(selectedPiece, true, row, col))
            {
                draw_list->AddRect(min, max, highlight_color, 0.0f, 0, 3.0f);
            }
        }
        else
        {
            if (canHighlightSquares(selectedPiece, false, row, col))
            {
                draw_list->AddRect(min, max, highlight_color, 0.0f, 0, 3.0f);
            }
        }
    }
    if (selectedPiece->row == row && selectedPiece->col == col)
    {
        draw_list->AddRect(min, max, highlight_color, 0.0f, 0, 3.0f);
    }
}

void Board::promotePawn(char newSymbol)
{
    if (!promotedPawn)
        return;

    // Add the new piece
    switch (newSymbol)
    {
    case 'Q':
        pieces.emplace_back(std::make_unique<Queen>(promotionTargetRow, promotionTargetCol, promotedPawn->isWhite));
        break;
    case 'R':
        pieces.emplace_back(std::make_unique<Rook>(promotionTargetRow, promotionTargetCol, promotedPawn->isWhite));
        break;
    case 'B':
        pieces.emplace_back(std::make_unique<Bishop>(promotionTargetRow, promotionTargetCol, promotedPawn->isWhite));
        break;
    case 'N':
        pieces.emplace_back(std::make_unique<Knight>(promotionTargetRow, promotionTargetCol, promotedPawn->isWhite));
        break;
    }

    for (auto& piece : pieces)
    {
        piece->board = this;
    }

    // Reset promotion state
    removePiece(promotedPawn);

    promotedPawn       = nullptr;
    isPromotionPending = false;
}

void Board::showPromotionWindow()
{
    if (ImGui::BeginPopupModal(".", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        if (binomial.result > 0)
            binomial.printQuote(defaultFont, customFont);

        if (ImGui::Button("q"))
        {
            promotePawn('Q');
            binomial.done = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("r"))
        {
            promotePawn('R');
            binomial.done = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("b"))
        {
            promotePawn('B');
            binomial.done = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("h"))
        {
            promotePawn('N');
            binomial.done = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

bool Board::checkPromotion()
{
    for (auto& piece : pieces)
    {
        if (piece.get()->getType() == 'P' && ((piece->isWhite && piece->row == 0) || (!piece->isWhite && piece->row == 7)))
        {
            promotedPawn = piece.get();
            return true;
        }
    }
    return false;
}

ImFont* Board::getFont(int what)
{
    return what == 1 ? customFont : defaultFont;
}

void Board::setFont(ImFont* font)
{
    customFont = font;
}

void Board::playAI()
{
    stockfish.WriteToEngine("uci\n");
    Sleep(500); // Let Stockfish respond

    std::string allMoves = "";

    for (auto& move : movesPlayed)
    {
        allMoves += move;
    }

    std::string command = "position startpos moves " + allMoves + "\n";

    std::cout << command << "\n";

    stockfish.WriteToEngine(command);
    stockfish.WriteToEngine("go depth 1\n");

    Sleep(1000); // Let Stockfish calculate
    std::string bestMove = stockfish.getBestMove();

    std::cout << bestMove << "\n";

    std::string from = bestMove.substr(0, 2);
    std::string to   = bestMove.substr(2);

    std::cout << chessNotationToIndices(from).row << " " << chessNotationToIndices(from).col << "\n";

    Piece* pieceToMove = getPieceAt(chessNotationToIndices(from).row, chessNotationToIndices(from).col);

    int row = chessNotationToIndices(to).row;
    int col = chessNotationToIndices(to).col;

    Piece* targetPiece = getPieceAt(row, col);
    if (targetPiece && targetPiece->isWhite != pieceToMove->isWhite)
    {
        removePiece(targetPiece);
    }

    std::cout << abs(pieceToMove->col - col) << " amount\n";

    if (pieceToMove->getType() == 'K' && abs(pieceToMove->col - col) > 1)
    {
        int    direction = (col > pieceToMove->col) ? -1 : 1;
        int    rookCol   = (direction == -1) ? 7 : 0;
        Piece* rook      = getPieceAt(pieceToMove->row, rookCol);

        if (!rook)
            return;

        rook->col       = pieceToMove->col - direction;
        rook->firstMove = false;
    }

    if (enPassantTarget.has_value())
    {
        std::cout << enPassantTarget->first << " "
                  << enPassantTarget->second << "\n";

        std::cout << row << " "
                  << col << "\n\n";
    }

    if (pieceToMove->getType() == 'P' && enPassantTarget.has_value() && row == enPassantTarget->first && col == enPassantTarget->second)
    {
        int capturedRow = row - 1;
        removePiece(getPieceAt(capturedRow, col));
    }

    if (pieceToMove->getType() == 'P' && abs(row - pieceToMove->row) == 2)
    {
        enPassantTarget = {(pieceToMove->row + row) / 2, pieceToMove->col};
    }
    else
    {
        enPassantTarget.reset(); // Reset if not a double step
    }

    if (pieceToMove)
    {
        pieceToMove->row = row;
        pieceToMove->col = col;
    }

    movesPlayed.push_back(bestMove + " ");
    activePlayer = &white;
}

std::string Board::toChessNotation(int row, int col)
{
    char file = 'a' + col;
    int  rank = 8 - row;

    return std::string(1, file) + std::to_string(rank);
}

Position Board::chessNotationToIndices(const std::string& notation)
{
    if (notation.size() != 2)
    {
        throw std::invalid_argument("Invalid chess notation.");
    }

    char col = notation[0]; // Column ('a' to 'h')
    char row = notation[1]; // Row ('1' to '8')

    int colIndex = col - 'a';
    if (colIndex < 0 || colIndex > 7)
    {
        throw std::invalid_argument("Invalid column in chess notation.");
    }

    int rowIndex = '8' - row;
    if (rowIndex < 0 || rowIndex > 7)
    {
        throw std::invalid_argument("Invalid row in chess notation.");
    }

    return {rowIndex, colIndex};
}

void Board::move(int row, int col)
{
    if (!AImode)
    {
        if (selectedPiece && IsValidMove(selectedPiece, row, col) && selectedPiece->isWhite == activePlayer->getColor())
        {
            Piece* targetPiece = getPieceAt(row, col);
            if (targetPiece && targetPiece->isWhite != selectedPiece->isWhite)
            {
                removePiece(targetPiece);
            }
            if (!targetPiece || targetPiece->isWhite != selectedPiece->isWhite)
            {
                movesPlayed.push_back(toChessNotation(selectedPiece->row, selectedPiece->col) + toChessNotation(row, col) + " ");

                if (selectedPiece->getType() == 'P' && enPassantTarget.has_value() && row == enPassantTarget->first && col == enPassantTarget->second)
                {
                    int capturedRow = selectedPiece->isWhite ? row + 1 : row - 1;
                    removePiece(getPieceAt(capturedRow, col));
                }

                if (selectedPiece->getType() == 'P' && abs(row - selectedPiece->row) == 2)
                {
                    enPassantTarget = {(selectedPiece->row + row) / 2, selectedPiece->col};
                }
                else
                {
                    enPassantTarget.reset(); // Reset if not a double step
                }

                selectedPiece->row = row;
                selectedPiece->col = col;

                if (selectedPiece->getType() == 'P')
                    selectedPiece->firstMove = false;

                moveCount += .5;
                activePlayer  = activePlayer == &white ? &black : &white;
                selectedPiece = nullptr;
            }
        }
        else
        {
            for (auto& piece : Board::pieces)
            {
                if (piece->row == row && piece->col == col)
                {
                    selectedPiece = piece.get();
                    break;
                }
            }
        }
    }
    else
    {
        if (activePlayer == &white)
        {
            if (selectedPiece && IsValidMove(selectedPiece, row, col) && selectedPiece->isWhite == activePlayer->getColor())
            {
                Piece* targetPiece = getPieceAt(row, col);
                if (targetPiece && targetPiece->isWhite != selectedPiece->isWhite)
                {
                    removePiece(targetPiece);
                }
                if (!targetPiece || targetPiece->isWhite != selectedPiece->isWhite)
                {
                    movesPlayed.push_back(toChessNotation(selectedPiece->row, selectedPiece->col) + toChessNotation(row, col) + " ");

                    if (selectedPiece->getType() == 'P' && enPassantTarget.has_value() && row == enPassantTarget->first && col == enPassantTarget->second)
                    {
                        int capturedRow = selectedPiece->isWhite ? row + 1 : row - 1;
                        removePiece(getPieceAt(capturedRow, col));
                    }

                    if (selectedPiece->getType() == 'P' && abs(row - selectedPiece->row) == 2)
                    {
                        enPassantTarget = {(selectedPiece->row + row) / 2, selectedPiece->col};
                    }
                    else
                    {
                        enPassantTarget.reset(); // Reset if not a double step
                    }

                    selectedPiece->row = row;
                    selectedPiece->col = col;

                    if (selectedPiece->getType() == 'P')
                        selectedPiece->firstMove = false;
                    moveCount += .5;
                    selectedPiece = nullptr;

                    std::cout << "Here\n";

                    stateStartTime = currentTime;
                    whitePlayed    = true;
                }
            }
            else
            {
                for (auto& piece : Board::pieces)
                {
                    if (piece->row == row && piece->col == col)
                    {
                        selectedPiece = piece.get();
                        break;
                    }
                }
            }
        }
    }
}

void Board::update(int row, int col)
{
    if (ImGui::Button(("##tile" + std::to_string(row) + std::to_string(col)).c_str(), ImVec2(tile_size, tile_size)))
    {
        move(row, col);
    }

    currentTime = glfwGetTime();

    if (whitePlayed)
    {
        if (currentTime - stateStartTime > waitDuration)
        {
            std::cout << "Here\n";
            whitePlayed  = false;
            activePlayer = &black;
            playAI();
        }
    }

    // Exponential distribution to simulate an event happening after a set amount of time
    if (!AImode)
    {
        if (moveCount > 1 && static_cast<int>(moveCount) % 15 == 0)
        {
            if (!exp.done)
            {
                Piece* toRemove = kings[0];
                int    idx      = 0;

                while (toRemove->getType() == 'K')
                {
                    idx = std::round(exp.dist(tools.rng));
                    idx = std::clamp(idx, 0, static_cast<int>(pieces.size() - 1));

                    toRemove = pieces[idx].get();
                }

                removePiece(toRemove);
                exp.done = true;
            }
        }
        else
        {
            exp.done = false;
        }
    }

    int randomRow{};
    int randomCol{};

    if (moveCount > 1 && static_cast<int>(moveCount) % 2 == 0 && !squareBlocked)
    {
        Poisson leafFall(1.0); // for on average 1 leaf falling per event
        int     leavesFallen = leafFall.draw();

        squareBlocked = true;

        for (int i = 0; i < leavesFallen; ++i)
        {
            randomRow = std::rand() % 8;
            randomCol = std::rand() % 8;

            std::cout << "Leaf falls on tile: (" << randomRow << ", " << randomCol << ")\n";

            blockSquare(randomRow, randomCol);
        }
    }

    if (moveCount > 1 && static_cast<int>(moveCount) % 10 == 0)
    {
        squareBlocked = false;
        unblockSquare(randomRow, randomCol);
    }
}

void Board::SwitchPlayer(double probability)
{
    if (bernoulli.flip(probability))
    {
        activePlayer = (activePlayer->getColor()) ? &black : &white;

        std::cout << "⚡ Player SWITCHED! Now it's "
                  << (activePlayer->getColor() ? "White" : "Black")
                  << "'s turn!" << '\n';
    }
}

void Board::draw()
{
    draw_list   = ImGui::GetWindowDrawList();
    window_pos  = ImGui::GetWindowPos();
    window_size = ImGui::GetWindowSize();
    p           = {window_pos.x + (window_size.x - board_size) * 0.5f, window_pos.y + (window_size.y - board_size) * 0.5f};

    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            ImVec2 minimum(p.x + col * tile_size, p.y + row * tile_size);
            ImVec2 maximum(minimum.x + tile_size, minimum.y + tile_size);
            draw_list->AddRectFilled(minimum, maximum, (row + col) % 2 == 0 ? lightSquare : darkSquare);

            ImGui::SetCursorScreenPos(minimum);
            update(row, col);

            if (selectedPiece)
                // Deselect piece with right mouse button
                if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
                {
                    selectedPiece = nullptr;
                }

            if (selectedPiece)
            {
                highlightSquares(minimum, maximum, row, col);
            }

            for (auto& king : kings)
            {
                if (isKingInCheck(dynamic_cast<King*>(king)) && king && king->row == row && king->col == col)
                {
                    draw_list->AddRect(minimum, maximum, check_color, 0.0f, 0, 3.0f);
                }
            }
        }
    }
    if (checkPromotion())
    {
        isPromotionPending = true;
        promotionTargetRow = promotedPawn->row;
        promotionTargetCol = promotedPawn->col;
        ImGui::OpenPopup(".");
        if (!binomial.done)
        {
            binomial.done   = true;
            binomial.result = binomial.dist(binomial.engine);
        }
        showPromotionWindow();
    }

    drawPieces();
}

void Board::drawPieces()
{
    for (auto& piece : Board::pieces)
    {
        ImVec2 piece_pos(p.x + piece->col * tile_size + tile_size * 0.28f, p.y + piece->row * tile_size + tile_size * 0.28f);
        char   symbol = piece->getSymbol();

        draw_list->AddText(piece_pos, IM_COL32(0, 0, 0, 255), std::string(1, symbol).c_str());
    }
}