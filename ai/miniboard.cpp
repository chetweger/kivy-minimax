#include <iostream>
#include <string>
#include <bitset>
//uncomment to disable assert()
//#define NDEBUG
#include <cassert>
#include "miniboard.h"
using namespace std;

MiniBoard::MiniBoard(int meta) {
    meta_position = meta;
}

void MiniBoard::setMetaPosition(int input) {
    meta_position = input;
}

int MiniBoard::getMetaPosition() {
    return meta_position;
}

void MiniBoard::setBoard(char input_board[]) {
    for (int i = 0; i <= 8; i++) {
        board[i] = input_board[i];
    }
}

void MiniBoard::setBoard(int input_board[]) {
    for (int i = 0; i <= 8; i++) {
        board[i] = input_board[i];
    }
}

void MiniBoard::printMe() {
    //cout << board;
    cout << "MiniBoard:\t";
    cout << (int) board[0] << "  ";
    cout << (int) board[1] << "  ";
    cout << (int) board[2] << "  ";

    cout << (int) board[3] << "  ";
    cout << (int) board[4] << "  ";
    cout << (int) board[5] << "  ";

    cout << (int) board[6] << "  ";
    cout << (int) board[7] << "  ";
    cout << (int) board[8] << "  ";
    cout << "\tmeta_position is:" << meta_position << "\n";
}

void MiniBoard::placePiece(char piece, int position) {
    MiniBoard::board[position] = piece;
}

char MiniBoard::isWin() {
    return ((board[0] & board[1] & board[2]) |
            (board[3] & board[4] & board[5]) |
            (board[6] & board[7] & board[8]) |
            (board[0] & board[3] & board[6]) |
            (board[1] & board[4] & board[7]) |
            (board[2] & board[5] & board[8]) |
            (board[0] & board[4] & board[8]) |
            (board[2] & board[4] & board[6]));
}

bool MiniBoard::isFull() {
    return (((bool) board[0]) && ((bool) board[1]) && ((bool) board[2]) && ((bool) board[3]) && ((bool) board[4]) && ((bool) board[5]) && ((bool) board[6]) && ((bool) board[7]) && ((bool) board[8]));
}

bool MiniBoard::isOver() {
    return this->isFull() || this->isWin();
}

int MiniBoard::getScore(char player) {
    return (int) (this->isWin() == player);
}

int MiniBoard::getNumCenterPieces(char player) {
    return (int) ((bool) (board[4] & player));
}

int MiniBoard::getNumCornerPieces(char player) {
    return (int) ((bool) (board[0] & player)) +
           (int) ((bool) (board[2] & player)) +
           (int) ((bool) (board[6] & player)) +
           (int) ((bool) (board[8] & player));
}

int MiniBoard::getNumSidePieces(char player) {
    return (int) ((bool) (board[1] & player)) +
           (int) ((bool) (board[3] & player)) +
           (int) ((bool) (board[5] & player)) +
           (int) ((bool) (board[7] & player));
}

int MiniBoard::getPlayerOneBlocking() {
    return ((board[0] + board[1] + board[2]) == MiniBoard::PLAYER_ONE_BLOCKING) ||
           ((board[3] + board[4] + board[5]) == MiniBoard::PLAYER_ONE_BLOCKING) ||
           ((board[6] + board[7] + board[8]) == MiniBoard::PLAYER_ONE_BLOCKING) ||
           ((board[0] + board[3] + board[6]) == MiniBoard::PLAYER_ONE_BLOCKING) ||
           ((board[1] + board[4] + board[7]) == MiniBoard::PLAYER_ONE_BLOCKING) ||
           ((board[2] + board[5] + board[8]) == MiniBoard::PLAYER_ONE_BLOCKING) ||
           ((board[0] + board[4] + board[8]) == MiniBoard::PLAYER_ONE_BLOCKING) ||
           ((board[2] + board[4] + board[6]) == MiniBoard::PLAYER_ONE_BLOCKING);
}

int MiniBoard::getPlayerTwoBlocking() {
    return ((board[0] + board[1] + board[2]) == MiniBoard::PLAYER_TWO_BLOCKING) ||
           ((board[3] + board[4] + board[5]) == MiniBoard::PLAYER_TWO_BLOCKING) ||
           ((board[6] + board[7] + board[8]) == MiniBoard::PLAYER_TWO_BLOCKING) ||
           ((board[0] + board[3] + board[6]) == MiniBoard::PLAYER_TWO_BLOCKING) ||
           ((board[1] + board[4] + board[7]) == MiniBoard::PLAYER_TWO_BLOCKING) ||
           ((board[2] + board[5] + board[8]) == MiniBoard::PLAYER_TWO_BLOCKING) ||
           ((board[0] + board[4] + board[8]) == MiniBoard::PLAYER_TWO_BLOCKING) ||
           ((board[2] + board[4] + board[6]) == MiniBoard::PLAYER_TWO_BLOCKING);
}

int MiniBoard::getPlayerOnePotential() {
    return ((board[0] + board[1] + board[2]) == MiniBoard::PLAYER_ONE_POTENTIAL) ||
           ((board[3] + board[4] + board[5]) == MiniBoard::PLAYER_ONE_POTENTIAL) ||
           ((board[6] + board[7] + board[8]) == MiniBoard::PLAYER_ONE_POTENTIAL) ||
           ((board[0] + board[3] + board[6]) == MiniBoard::PLAYER_ONE_POTENTIAL) ||
           ((board[1] + board[4] + board[7]) == MiniBoard::PLAYER_ONE_POTENTIAL) ||
           ((board[2] + board[5] + board[8]) == MiniBoard::PLAYER_ONE_POTENTIAL) ||
           ((board[0] + board[4] + board[8]) == MiniBoard::PLAYER_ONE_POTENTIAL) ||
           ((board[2] + board[4] + board[6]) == MiniBoard::PLAYER_ONE_POTENTIAL);
}

int MiniBoard::getPlayerTwoPotential() {
    return ((board[0] + board[1] + board[2]) == MiniBoard::PLAYER_TWO_POTENTIAL) ||
           ((board[3] + board[4] + board[5]) == MiniBoard::PLAYER_TWO_POTENTIAL) ||
           ((board[6] + board[7] + board[8]) == MiniBoard::PLAYER_TWO_POTENTIAL) ||
           ((board[0] + board[3] + board[6]) == MiniBoard::PLAYER_TWO_POTENTIAL) ||
           ((board[1] + board[4] + board[7]) == MiniBoard::PLAYER_TWO_POTENTIAL) ||
           ((board[2] + board[5] + board[8]) == MiniBoard::PLAYER_TWO_POTENTIAL) ||
           ((board[0] + board[4] + board[8]) == MiniBoard::PLAYER_TWO_POTENTIAL) ||
           ((board[2] + board[4] + board[6]) == MiniBoard::PLAYER_TWO_POTENTIAL);
}

MiniBoard MiniBoard::copyMe(MiniBoard copy) {
    copy.setMetaPosition(meta_position);
    copy.setBoard(board);
    return copy;
}
