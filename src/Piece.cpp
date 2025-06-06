#include "../include/Piece.hpp"
#include "../include/Board.hpp"

bool King::canCastle(int destRow, int destCol)
{
    if (!firstMove || row != destRow)
        return false;

    bool isKingside  = (destCol == 6);
    bool isQueenside = (destCol == 2);

    if (!isKingside && !isQueenside)
        return false;

    int rookCol = isKingside ? 7 : 0;

    Piece* rook = board->getPieceAt(row, rookCol);
    if (!rook || rook->getType() != 'R' || !rook->firstMove)
        return false;

    int direction = (isKingside) ? 1 : -1;
    int checkCol  = col + direction;
    while (checkCol != rookCol)
    {
        if (board->getPieceAt(row, checkCol))
            return false;
        checkCol += direction;
    }

    return true;
}