#include "BitBoardManager.h"
#include <iostream>

BitBoardManager::BitBoardManager() {};

BitBoardManager::~BitBoardManager() {};

bitboard BitBoardManager::bitboardFromPosition(const int y, const int x){
    bitboard b = 0x00000001;
    b = b << ((y * 8) + x);
    return b;
}

bitboard BitBoardManager::generateBlackBitboard(std::string stateString) {
    bitboard initialState = 0;
    char checkVal = 32;
    // note - empty space is represented as a 0, not a space, so that needs to be corrected
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            int index = 63 - ((i * 8) + j); // subtract from 63 so that ways we put the 'closest' thing on the state string to be 
            // the 'furthest' away on the bitboard, in doing so correctly place the associated locations of the board onto the bitboard.
            if ((checkVal & stateString[index]) && stateString[index] != '0') {
                // if this bit is flipped, then it is either a lowercase letter, or a zero
                // if value isn't a space (32) and it is a uppercase letter
                initialState |= bitboardFromPosition(i, j);
                // std::cout << initialState << std::endl; 
            }
        }
    }
    return initialState; 
}
bitboard BitBoardManager::generateWhiteBitboard(std::string stateString) {
    bitboard initialState = 0;
    char checkVal = 32;

    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            int index = 63 - ((i * 8) + j);
            if ('0' != stateString[index] && !(checkVal & stateString[index])) {
                // std::cout << stateString[index] << std::endl;
                // if value isn't a space (32) and it is a uppercase letter
                initialState |= bitboardFromPosition(i, j);
                // std::cout << initialState << std::endl; 
            }
        }
    }

    return initialState; 
}

bitboard BitBoardManager::pawnAttackBitBoard(bitboard pawnBitboard, bitboard opponent, bool color) {
    const bitboard leftOff = 0x7F7F7F7F7F7F7F7F; // 01111111
    const bitboard rightOff = 0xFEFEFEFEFEFEFEFE; // 11111110
    bitboard pawnAttackEast = pawnBitboard << 9 & leftOff; 
    bitboard pawnAttackWest = pawnBitboard << 7 & rightOff;
    bitboard attacks = (pawnAttackWest | pawnAttackEast) & opponent; // you can move to the side if there is a thingy there;
    bitboard moveUp = (pawnBitboard << 8) ^ opponent; // if pawn moves up and there is not an opponent there
    return attacks | moveUp;
}

/*
int main(int argc, char* argv[]){
    std::string startState = 
    "RNBKQBNRPPPPPPPP                                pppppppprnbqkbnr";
    BitBoardManager myManager = BitBoardManager();
    bitboard myBit = myManager.generateWhiteBitboard(startState);
    std::cout << myBit << std::endl;
    std::cout << startState.length() << std::endl;
}
*/