// base code written by https://github.com/zaphodgjd

#pragma once

#include <assert.h>
#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <vector>

const char WHITE = 'W';
const char BLACK = 'B';
// Define a constant for the maximum depth of your AI.
const int MAX_DEPTH = 24;
/*
enum ChessPiece {
    NoPiece = 0,
    Pawn = 1,
    Knight,
    Bishop,
    Rook,
    Queen,
    King
};*/

enum MoveFlags {
    EnPassant = 0x01, // 0000 0001
    IsCapture = 0x02, // 0000 0010
    KingSideCastle = 0x04, // 0000 0100
    QueenSideCastle = 0x08, // 0000 1000
    IsPromotion = 0x10 // 0001 0000
};

#pragma pack(push, 1)
struct BitMove {
    char from;          // byte position 0;
    char to;            // byte position 1;
    unsigned char piece; // Changed from ChessPiece to unsigned char
    unsigned char flags;
    BitMove()
        : from(0)
        , to(0)
        , piece(NoPiece)
        , flags(0) { }
    bool equals(const BitMove& other) const {
        return from == other.from && to == other.to && flags == other.flags && piece == other.piece;
    }
};
#pragma pack(pop)

class GameState {
  public:
    char state[64];
    int castleStatus;
    int flags;
    char color;
    std::vector<GameState> stateStack;
    int stackPtr = 0;

    GameState()
        : stackPtr(0)
        , flags(0)
        , color(WHITE)
        , castleStatus(0) {
        std::memset(state, '0', sizeof(state));
    }

    GameState& operator=(const GameState& other) {
        if (this != &other) {
            std::memcpy(state, other.state, sizeof(state));
            castleStatus = other.castleStatus;
            color = other.color;
            flags = other.flags;
        }
        return *this;
    }

    inline void pushMove(const BitMove& move) {
        pushState();
        unsigned char fromPiece = state[move.from];
        state[move.from] = '0';
        unsigned char toPiece = state[move.to];
        state[move.to] = fromPiece;
        if (move.flags & KingSideCastle) {
            state[move.to - 1] = state[move.to + 1];
            state[move.to + 1] = '0';
        } else if (move.flags & QueenSideCastle) {
            state[move.to + 1] = state[move.to - 2];
            state[move.to - 2] = '0';
        } else if (move.flags & EnPassant) {
            // check for color to determine which direction to capture
            if (fromPiece == 'P') {
                state[move.to - 8] = '0';
            } else {
                state[move.to + 8] = '0';
            }
        } else if (move.flags & IsPromotion) {
            state[move.to] = color == WHITE ? 'Q' : 'q';
        }
        // flip the color bit as it now becomes the other player's turn
        color = (color == WHITE) ? BLACK : WHITE;
        flags = 0; // invalidate all the flags
    }

    inline void pushState() {
        if (stackPtr < MAX_DEPTH) {
            if (stateStack.size() == 0) {
                stateStack.resize(MAX_DEPTH);
            }
            stateStack[stackPtr] = *this;
            ++stackPtr;
        } else {
            // Handle error: stack overflow
            assert(false);
        }
    }
    inline void popState() {
        if (stackPtr > 0) {
            --stackPtr;
            *this = stateStack[stackPtr];
        } else {
            // Handle error: stack underflow
            assert(false);
        }
    }
  private:
};
