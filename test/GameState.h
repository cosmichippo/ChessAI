#include <cstdint>
#include <vector>

enum ChessPiece {
    NoPiece = 0,
    Pawn = 1,
    Knight,
    Bishop,
    Rook,
    Queen,
    King
};


typedef uint64_t bitboard;
//rename uint64_t to 'bitboard'

class BitBoardManager
{
public:        
    BitBoardManager();
    ~BitBoardManager();

    std::vector<bitboard> generateAttackBitBoards(ChessPiece myPiece, char color, bitboard myBoard);
    bitboard pawnAttackBitBoard(bitboard pawnBitboard, bitboard opponent, bool color);
    bitboard rookAttackBitBoard(bitboard pawnBitboard, bitboard opponent, bool color);
    bitboard knightAttackBitBoard(bitboard pawnBitboard, bitboard opponent, bool color);
    bitboard bishopAttackBitBoard(bitboard pawnBitboard, bitboard opponent, bool color);
    bitboard kingAttackBitBoard(bitboard pawnBitboard, bitboard opponent, bool color);
    bitboard queenAttackBitBoard(bitboard pawnBitboard, bitboard opponent, bool color);
    bitboard bitboardFromPosition(const int column, const int row);


};