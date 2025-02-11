// // #pragma once

// // #include <cmath>
// // #include <iostream>

// // enum class PieceType {
// //     pawn,
// //     rook,
// //     knight,
// //     bishop,
// //     king,
// //     queen,
// // };

// // struct Position {
// //     // TODO(lisam): create something to block values from 1 to 8
// //     int x{};
// //     int y{};
// // };

// <<<<<<< HEAD
// // class Piece {
// // private:
// //     Position position{};
// //     bool     isOnBoard = true;
// == == == =
//     public : Piece()              = default;
// virtual PieceType getType() const = 0;

// explicit Piece(Position const& position);
// Position getPosition() const
// {
//     return position;
// }
// void setPosition(const Position& newPosition)
// {
//     position = newPosition;
// }

// bool getStatus() const
// {
//     return isOnBoard;
// }
// void steStatus(const bool& newStatus)
// {
//     isOnBoard = newStatus;
// }
// >>>>>>> a197c62bdd3f28eab569a26ce91bfc8c1247a3a5

// // public:
// //     virtual PieceType getType() const = 0;
// //     Position          getPosition() const { return position; }
// //     void              setPosition(const Position& newPosition) { position = newPosition; }

// //     //   virtual void whereTo();
// //     virtual void move(Position newPosition) = 0;

// <<<<<<< HEAD
// //     virtual ~Piece() = default;
// //     //   TODO (lisam) : copy constructor etc = default
// // };
// == == == =
//     class King : public Piece {
// public:
//     explicit King(Position const& position)
//     {
//         setPosition(position);
//     }

//     PieceType getType() const override { return PieceType::king; }
// >>>>>>> a197c62bdd3f28eab569a26ce91bfc8c1247a3a5

//     // class King : public Piece {
//     // public:
//     //     PieceType getType() const override { return PieceType::king; }

//     //     void move(Position newPosition) override
//     //     {
//     //         int dx = std::abs(newPosition.x - getPosition().x);
//     //         int dy = std::abs(newPosition.y - getPosition().y);

// <<<<<<< HEAD
//     //         if (dx <= 1 && dy <= 1)
//     //         {
//     //             getPosition() = newPosition;
//     //             std::cout << "King moved to (" << getPosition().x << ", " << getPosition().y << ")"
//     //                       << '\n';
//     //         }
//     //         else
//     //         {
//     //             std::cout << "Invalid move for King" << '\n';
//     //         }
//     //     }
//     // };
//     == == == =
//         class Queen : public Piece {
//     public:
//         explicit Queen(Position const& position)
//         {
//             setPosition(position);
//         }

//         PieceType getType() const override { return PieceType::queen; }
// >>>>>>> a197c62bdd3f28eab569a26ce91bfc8c1247a3a5

//         // class Queen : public Piece {
//         // public:
//         //     PieceType getType() const override { return PieceType::queen; }

//         //     void move(Position newPosition) override
//         //     {
//         //         int dx = std::abs(newPosition.x - getPosition().x);
//         //         int dy = std::abs(newPosition.y - getPosition().y);

// <<<<<<< HEAD
//         //         if (dx == dy || dx == 0 || dy == 0)
//         //         {
//         //             getPosition() = newPosition;
//         //             std::cout << "Queen moved to (" << getPosition().x << ", " << getPosition().y << ")" << '\n';
//         //         }
//         //         else
//         //         {
//         //             std::cout << "Invalid move for Queen" << '\n';
//         //         }
//         //     }
//         // };
//         == == == =
//             class Rook : public Piece {
//         public:
//             explicit Rook(Position const& position)
//             {
//                 setPosition(position);
//             }

//             PieceType getType() const override { return PieceType::rook; }
// >>>>>>> a197c62bdd3f28eab569a26ce91bfc8c1247a3a5

//             // class Rook : public Piece {
//             // public:
//             //     PieceType getType() const override { return PieceType::rook; }

// <<<<<<< HEAD
//             //     void move(Position newPosition) override
//             //     {
//             //         if (newPosition.x == getPosition().x || newPosition.y == getPosition().y)
//             //         {
//             //             getPosition() = newPosition;
//             //             std::cout << "Rook moved to (" << getPosition().x << ", " << getPosition().y << ")" << '\n';
//             //         }
//             //         else
//             //         {
//             //             std::cout << "Invalid move for Rook" << '\n';
//             //         }
//             //     }
//             // };
//             == == == =
//                 class Bishop : public Piece {
//             public:
//                 explicit Bishop(Position const& position)
//                 {
//                     setPosition(position);
//                 }

//                 PieceType getType() const override { return PieceType::bishop; }
// >>>>>>> a197c62bdd3f28eab569a26ce91bfc8c1247a3a5

//                 // class Bishop : public Piece {
//                 // public:
//                 //     PieceType getType() const override { return PieceType::bishop; }

//                 //     void move(Position newPosition) override
//                 //     {
//                 //         int dx = std::abs(newPosition.x - getPosition().x);
//                 //         int dy = std::abs(newPosition.y - getPosition().y);

// <<<<<<< HEAD
//                 //         if (dx == dy)
//                 //         {
//                 //             getPosition() = newPosition;
//                 //             std::cout << "Bishop moved to (" << getPosition().x << ", " << getPosition().y << ")" << '\n';
//                 //         }
//                 //         else
//                 //         {
//                 //             std::cout << "Invalid move for Bishop" << '\n';
//                 //         }
//                 //     }
//                 // };
//                 == == == =
//                     class Knight : public Piece {
//                 public:
//                     explicit Knight(Position const& position)
//                     {
//                         setPosition(position);
//                     }

//                     PieceType getType() const override { return PieceType::knight; }
// >>>>>>> a197c62bdd3f28eab569a26ce91bfc8c1247a3a5

//                     // class Knight : public Piece {
//                     // public:
//                     //     PieceType getType() const override { return PieceType::knight; }

//                     //     void move(Position newPosition) override
//                     //     {
//                     //         int dx = std::abs(newPosition.x - getPosition().x);
//                     //         int dy = std::abs(newPosition.y - getPosition().y);

// <<<<<<< HEAD
//                     //         if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2))
//                     //         {
//                     //             getPosition() = newPosition;
//                     //             std::cout << "Knight moved to (" << getPosition().x << ", " << getPosition().y << ")" << '\n';
//                     //         }
//                     //         else
//                     //         {
//                     //             std::cout << "Invalid move for Knight" << '\n';
//                     //         }
//                     //     }
//                     // };
//                     == == == =
//                         class Pawn : public Piece {
//                     public:
//                         explicit Pawn(Position const& position)
//                         {
//                             setPosition(position);
//                         }

//                         PieceType getType() const override { return PieceType::pawn; }
// >>>>>>> a197c62bdd3f28eab569a26ce91bfc8c1247a3a5

//                         // class Pawn : public Piece {
//                         // public:
//                         //     PieceType getType() const override { return PieceType::pawn; }

//                         //     void move(Position newPosition) override
//                         //     {
//                         //         int dy = newPosition.y - getPosition().y;
//                         //         int dx = std::abs(newPosition.x - getPosition().x);

//                         //         // Pawn moves forward 1 square or 2 squares from the initial position
//                         //         if ((dy == 1 && dx == 0) || (dy == 2 && getPosition().y == 1 && dx == 0))
//                         //         {
//                         //             getPosition() = newPosition;
//                         //             std::cout << "Pawn moved to (" << getPosition().x << ", " << getPosition().y << ")" << '\n';
//                         //         }
//                         //         else
//                         //         {
//                         //             std::cout << "Invalid move for Pawn" << '\n';
//                         //         }
//                         //     }
//                         // };