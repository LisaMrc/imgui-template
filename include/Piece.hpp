#pragma once
#include <cmath>
#include <iostream>

class Board;

class Piece {
public:
    int    row, col;
    bool   isWhite;
    bool   isCapture = true;
    bool   firstMove = true;
    Board* board;

    virtual char getSymbol() const { return '_'; }
    virtual char getType() const { return '_'; }
    virtual bool canMove(int row, int col) { return false; }
    Piece(int r, int c, bool isW)
        : row(r), col(c), isWhite(isW) {}
    virtual ~Piece() = default;
};

class Pawn : public Piece {
public:
    bool firstMove = true;

    Pawn(int r, int c, bool isW)
        : Piece(r, c, isW) {}
    char getSymbol() const override
    {
        return isWhite ? 'p' : 'o';
    }
    char getType() const override
    {
        return 'P';
    }
    bool canMove(int sRow, int sCol) override
    {
        if (isWhite)
        {
            if (sCol == col && (sRow == row - 1 || (firstMove && sRow == row - 2)))
            {
                if (!isCapture)
                {
                    firstMove = false;
                    return true;
                }
                return false;
            }
            if (isCapture && abs(sCol - col) == 1 && sRow == row - 1)
            {
                firstMove = false;
                return true;
            }
        }
        else
        {
            if (sCol == col && (sRow == row + 1 || (firstMove && sRow == row + 2)))
            {
                if (!isCapture)
                {
                    firstMove = false;
                    return true;
                }
                return false;
            }
            if (isCapture && abs(sCol - col) == 1 && sRow == row + 1)
            {
                firstMove = false;
                return true;
            }
        }

        return false;
    }
};

class Rook : public Piece {
public:
    Rook(int r, int c, bool isW)
        : Piece(r, c, isW) {}
    char getSymbol() const override { return isWhite ? 'r' : 't'; }
    char getType() const override
    {
        return 'R';
    }
    bool canMove(int sRow, int sCol) override { return row == sRow || col == sCol; }
};

class Knight : public Piece {
public:
    Knight(int r, int c, bool isW)
        : Piece(r, c, isW) {}
    char getSymbol() const override { return isWhite ? 'h' : 'j'; }
    char getType() const override
    {
        return 'N';
    }
    bool canMove(int sRow, int sCol) override { return (abs(row - sRow) == 2 && abs(col - sCol) == 1) || (abs(row - sRow) == 1 && abs(col - sCol) == 2); }
};

class Bishop : public Piece {
public:
    Bishop(int r, int c, bool isW)
        : Piece(r, c, isW) {}
    char getSymbol() const override { return isWhite ? 'b' : 'n'; }
    char getType() const override
    {
        return 'B';
    }
    bool canMove(int sRow, int sCol) override { return abs(row - sRow) == abs(col - sCol); }
};

class Queen : public Piece {
public:
    Queen(int r, int c, bool isW)
        : Piece(r, c, isW) {}
    char getSymbol() const override { return isWhite ? 'q' : 'w'; }
    char getType() const override
    {
        return 'Q';
    }
    bool canMove(int sRow, int sCol) override { return row == sRow || col == sCol || abs(row - sRow) == abs(col - sCol); }
};

class King : public Piece {
public:
    King(int r, int c, bool isW)
        : Piece(r, c, isW) {}
    char getSymbol() const override { return isWhite ? 'k' : 'l'; }
    char getType() const override
    {
        return 'K';
    }
    bool canMove(int sRow, int sCol) override { return abs(row - sRow) <= 1 && abs(col - sCol) <= 1; }
    bool canCastle(int destRow, int destCol);
};