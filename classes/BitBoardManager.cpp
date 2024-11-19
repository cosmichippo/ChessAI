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
bitboard BitBoardManager::kingAttackBitBoard(bitboard kingBitboard){
    const bitboard leftOff = 0x7F7F7F7F7F7F7F7F;  // 01111111
    const bitboard rightOff = 0xFEFEFEFEFEFEFEFE; // 11111110

    bitboard pawnAttackEast = ((kingBitboard << 9) | (kingBitboard >> 7) | (kingBitboard << 1)) & rightOff;
    bitboard pawnAttackWest = ((kingBitboard  << 7) | (kingBitboard>> 9) | (kingBitboard >> 1))& leftOff;
    bitboard upDown = (kingBitboard << 8) | (kingBitboard >> 8) | kingBitboard; 
    return  pawnAttackEast | pawnAttackWest | upDown;
}
bitboard BitBoardManager::pawnAttackBitBoard(bitboard pawnBitboard, bitboard opponent, bool color) {
    const bitboard leftOff = 0x7F7F7F7F7F7F7F7F;  // 01111111
    const bitboard rightOff = 0xFEFEFEFEFEFEFEFE; // 11111110
    bitboard pawnAttackEast = (!color) ? (pawnBitboard << 9 & rightOff) : (pawnBitboard >> 7 & rightOff);
    bitboard pawnAttackWest = (!color) ? (pawnBitboard << 7 & leftOff) : (pawnBitboard >> 9 & leftOff);
    bitboard attacks = (pawnAttackWest | pawnAttackEast) & opponent; // you can move to the side if there is a thingy there;
    bitboard moveUp;
    if(color){
        moveUp = ((pawnBitboard >> 8) & ~opponent);
    } else {
        moveUp = (pawnBitboard << 8) & ~opponent;
    }
    if((pawnBitboard & 0x000000000000FF00) && !color && moveUp){
        moveUp = moveUp | (pawnBitboard << 16) & ~opponent;
        // if pawn is white, and pawn at start pos
    }
    else if((pawnBitboard & 0x00FF000000000000) && color && moveUp){
        moveUp = moveUp | (pawnBitboard >> 16) & ~opponent;
        // if pawn is white, and pawn at start pos
    }
    return attacks | moveUp | pawnBitboard;
}
bitboard BitBoardManager::knightAttackBitBoard(bitboard knightBitboard, bitboard opponent, bool color){
    const bitboard leftOff =  0x7F7F7F7F7F7F7F7F; // 01111111
    const bitboard rightOff = 0xFEFEFEFEFEFEFEFE; // 11111110
    const bitboard ABoff =    0x3F3F3F3F3F3F3F3F; // 00111111
    const bitboard GHoff =    0xFCFCFCFCFCFCFCFC; // 11111100
    // vals to left -> 17, 10
    bitboard leftOne = knightBitboard << 17;
    leftOne = leftOne | (knightBitboard >> 15);
    leftOne = leftOne & rightOff;
    
    bitboard leftTwo = knightBitboard << 10;
    leftTwo = leftTwo | (knightBitboard >> 6);
    leftTwo = leftTwo & GHoff;
    
    bitboard rightOne = knightBitboard << 15; 
    rightOne = rightOne | (knightBitboard >> 17);
    rightOne = rightOne & leftOff;

    bitboard rightTwo = knightBitboard << 6; 
    rightTwo = rightTwo | (knightBitboard >> 10);
    rightTwo = rightTwo & ABoff;

    return (rightOne | rightTwo) | (leftOne | leftTwo);
    }
bitboard BitBoardManager::generatePawnAttackPos(std::string state, int srcCol, int srcRow, int dstCol, int dstRow, bool color){
        bitboard opponent = (color) ? generateWhiteBitboard(state) : generateBlackBitboard(state);
        bitboard myBit = bitboardFromPosition(srcCol, srcRow);
        bitboard destBit = bitboardFromPosition(dstCol, dstRow);
        bitboard myAttack = pawnAttackBitBoard(myBit, opponent, color);
        return (myAttack & destBit);

}

bitboard BitBoardManager::generateKnightAttackPos(std::string state, int srcCol, int srcRow, int dstCol, int dstRow){
        bitboard opponent = generateBlackBitboard(state);
        bitboard myBit = bitboardFromPosition(srcCol, srcRow);
        std::cout << myBit << "myBit" <<std::endl;
        bitboard destBit = bitboardFromPosition(dstCol, dstRow);
        bitboard myAttack = knightAttackBitBoard(myBit, opponent, 0);
        return (myAttack & destBit);

}

bitboard BitBoardManager::rookAttackBitBoard(int rowIndex, int colIndex, bitboard opponent){
    bitboard block = opponent;
    int rk = rowIndex;
    int fl = colIndex;
    bitboard result = 0ULL;
    int r, f;

    // reformat to include like rowindex, colindex of source, and like shoot off from there
    for (r = rk + 1; r <= 7; r++) {
        result |= (1ULL << (fl + r * 8));
        if (block & (1ULL << (fl + r * 8))) break;
    }
    // South
    for (r = rk - 1; r >= 0; r--) {
        result |= (1ULL << (fl + r * 8));
        if (block & (1ULL << (fl + r * 8))) break;
    }
    // East
    for (f = fl + 1; f <= 7; f++) {
        result |= (1ULL << (f + rk * 8));
        if (block & (1ULL << (f + rk * 8))) break;
    }
    // West
    for (f = fl - 1; f >= 0; f--) {
        result |= (1ULL << (f + rk * 8));
        if (block & (1ULL << (f + rk * 8))) break;
    }
    return result;
}
bitboard BitBoardManager::bishopAttackBitBoard(int rowIndex, int colIndex, bitboard opponent){
    bitboard block = opponent;
    int rk = rowIndex;
    int fl = colIndex;
    bitboard result = 0ULL;
    int r, f;
    // Northeast
    for (r = rk + 1, f = fl + 1; r <= 7 && f <= 7; r++, f++) {
        result |= (1ULL << (f + r * 8));
        if (block & (1ULL << (f + r * 8))) break;
    }
    // Southeast
    for (r = rk - 1, f = fl + 1; r >= 0 && f <= 7; r--, f++) {
        result |= (1ULL << (f + r * 8));
        if (block & (1ULL << (f + r * 8))) break;
    }
    // Southwest
    for (r = rk - 1, f = fl - 1; r >= 0 && f >= 0; r--, f--) {
        result |= (1ULL << (f + r * 8));
        if (block & (1ULL << (f + r * 8))) break;
    }
    // Northwest
    for (r = rk + 1, f = fl - 1; r <= 7 && f >= 0; r++, f--) {
        result |= (1ULL << (f + r * 8));
        if (block & (1ULL << (f + r * 8))) break;
    }
    return result;
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