#include "GameState.h"
#include <iostream>

BitBoardManager::BitBoardManager() {};

BitBoardManager::~BitBoardManager() {};

bitboard BitBoardManager::bitboardFromPosition(const int y, const int x){
    bitboard b = 0x00000001;
    b = b << ((y * 8) + x);
    return b;
}


bitboard BitBoardManager::pawnAttackBitBoard(bitboard pawnBitboard, bitboard opponent, bool color) {
    const bitboard leftOff = 0x7F7F7F7F7F7F7F7F; // 01111111
    const bitboard rightOff = 0xFEFEFEFEFEFEFEFE; // 11111110
    bitboard pawnAttackEast = pawnBitboard << 9 & leftOff; 
    bitboard pawnAttackWest = pawnBitboard << 7 & rightOff;
    return (pawnAttackWest | pawnAttackEast) & opponent;
}

int main(int argc, char* argv[]){
    BitBoardManager myManager = BitBoardManager();
    bitboard myBit = myManager.bitboardFromPosition(1,1);
    bitboard myopponent = myManager.bitboardFromPosition(2,2);
    bitboard out = myManager.pawnAttackBitBoard(myBit, myopponent, 0);
}