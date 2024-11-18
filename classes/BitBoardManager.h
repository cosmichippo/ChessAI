#include <cstdint>
#include <vector>
// #include "Chess.h"



typedef uint64_t bitboard;

class BitBoardManager
{
public:        
    BitBoardManager();
    ~BitBoardManager();

    //std::vector<bitboard> generateAttackBitBoards(ChessPiece myPiece, char color, bitboard myBoard);
    bitboard generateBlackBitboard(std::string stateString);
    bitboard generateWhiteBitboard(std::string stateString);
    bitboard pawnAttackBitBoard(bitboard pawnBitboard, bitboard opponent, bool color);
    bitboard generatePawnAttackPos(std::string state, int srcCol, int srcRow, int dstCol, int dstRow, bool color);
    bitboard generateKnightAttackPos(std::string state, int srcCol, int srcRow, int dstCol, int dstRow);
    bitboard rookAttackBitBoard(bitboard pawnBitboard, bitboard opponent, bool color);
    bitboard knightAttackBitBoard(bitboard pawnBitboard, bitboard opponent, bool color);
    bitboard bishopAttackBitBoard(bitboard pawnBitboard, bitboard opponent, bool color);
    bitboard kingAttackBitBoard(bitboard pawnBitboard, bitboard opponent, bool color);
    bitboard queenAttackBitBoard(bitboard pawnBitboard, bitboard opponent, bool color);
    bitboard bitboardFromPosition(const int column, const int row);

};