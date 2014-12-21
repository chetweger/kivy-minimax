#include "metaboard.h"
#include "constants.h"

MetaBoard::MetaBoard() {
}

void MetaBoard::printMe() {
    cout << "Printing out board:\n";
    cout << "Current player is: " << (int) this->generateNextPlayer() << "\nPlayer MAX is: " << player_max << "\n";
    for (int i = 0; i < 9; i++) {
        this->boards[i].printMe();
    }
}

MetaBoard MetaBoard::clone() {
    MetaBoard cloned;
    for (int i = 0; i < 9; i++) {
        // Make identical copies of all boards.
        cloned.boards[i] = this->boards[i].copyMe(cloned.boards[i]);
        cloned.next_mini_board = this->next_mini_board;
        cloned.next_player = this->next_player;
        cloned.player_max = this->player_max;
    }
    return cloned;
}

MetaBoard MetaBoard::operator=(MetaBoard meta) {
    return meta.clone();
}

char MetaBoard::generateNextPlayer() {
    if ((int) next_player == (int) MiniBoard::PLAYER_TWO) {
        return MiniBoard::PLAYER_ONE;
    } else {
        assert((int) next_player == (int) MiniBoard::PLAYER_ONE);
        return MiniBoard::PLAYER_TWO;
    }
}

vector<MetaBoard> MetaBoard::generateChildrenAux() {
    vector<MetaBoard> children;
    if (!this->boards[next_mini_board].isOver()) {
        char childs_next_player = this->generateNextPlayer();
        for (int i = 0; i < 9; i++) {
            if (this->boards[next_mini_board].board[i] == 0) {
                MetaBoard child = *this;
                child.boards[next_mini_board].board[i] = this->next_player;
                child.next_mini_board = i;
                child.next_player = childs_next_player;
                children.push_back(child);
            }
        }
    }
    return children;
}

vector<MetaBoard> MetaBoard::generateChildren() {
    /*
     * Logic:
     * Case 1. next_mini_board !isOver
     *   - then call return generateChildrenAux on only that board and join vectors.
     * Case 2. next_mini_board isOver
     *   - then call generateChildrenAux on all boards mini boards
     */

    vector<MetaBoard> children;
    if (!this->boards[next_mini_board].isOver()) {
        children = this->generateChildrenAux();
    } else {
        for (int i = 0; i < 9; i++) {
            this->next_mini_board = i;
            vector<MetaBoard> board_i_children;
            board_i_children = this->generateChildrenAux();
            children.insert(children.end(), board_i_children.begin(), board_i_children.end());
        }
    }
    return children;
}

float MetaBoard::getScore(char player) {
    float score = 0;
    for (int i = 0; i < 9; i++) {
        score += this->boards[i].getScore(player);
    }
    return score;
}

float MetaBoard::getNumCenterPieces(char player) {
    float pieces = 0;
    for (int i = 0; i < 9; i++) {
        pieces += this->boards[i].getNumCenterPieces(player);
    }
    return pieces;
}

float MetaBoard::getNumCornerPieces(char player) {
    float pieces = 0;
    for (int i = 0; i < 9; i++) {
        pieces += this->boards[i].getNumCornerPieces(player);
    }
    return pieces;
}

float MetaBoard::getNumSidePieces(char player) {
    float pieces = 0;
    for (int i = 0; i < 9; i++) {
        pieces += this->boards[i].getNumSidePieces(player);
    }
    return pieces;
}

float MetaBoard::getPlayerOneBlocking() {
    float positions = 0;
    for (int i = 0; i < 9; i++) {
        positions += this->boards[i].getPlayerOneBlocking();
    }
    return positions;
}

float MetaBoard::getPlayerOnePotential() {
    float positions = 0;
    for (int i = 0; i < 9; i++) {
        positions += this->boards[i].getPlayerOnePotential();
    }
    return positions;
}

float MetaBoard::getPlayerTwoBlocking() {
    float positions = 0;
    for (int i = 0; i < 9; i++) {
        positions += this->boards[i].getPlayerTwoBlocking();
    }
    return positions;
}

float MetaBoard::getPlayerTwoPotential() {
    float positions = 0;
    for (int i = 0; i < 9; i++) {
        positions += this->boards[i].getPlayerTwoPotential();
    }
    return positions;
}

float MetaBoard::computeUtility() {
    float score_player_1 = 0.0;
    float score_player_2 = 0.0;

    score_player_1 += td_constants.c1 * this->getScore(MiniBoard::PLAYER_ONE);
    score_player_1 += td_constants.c2 * this->getNumCenterPieces(MiniBoard::PLAYER_ONE);
    score_player_1 += td_constants.c3 * this->getNumCornerPieces(MiniBoard::PLAYER_ONE);
    score_player_1 += td_constants.c4 * this->getNumSidePieces(MiniBoard::PLAYER_ONE);
    score_player_1 += td_constants.c5 * this->getPlayerOneBlocking();
    score_player_1 += td_constants.c6 * this->getPlayerOnePotential();

    score_player_2 += td_constants.c1 * this->getScore(MiniBoard::PLAYER_TWO);
    score_player_2 += td_constants.c2 * this->getNumCenterPieces(MiniBoard::PLAYER_TWO);
    score_player_2 += td_constants.c3 * this->getNumCornerPieces(MiniBoard::PLAYER_TWO);
    score_player_2 += td_constants.c4 * this->getNumSidePieces(MiniBoard::PLAYER_TWO);
    score_player_2 += td_constants.c5 * this->getPlayerOneBlocking();
    score_player_2 += td_constants.c6 * this->getPlayerOnePotential();

    if (MiniBoard::PLAYER_ONE == this->player_max) {
        return score_player_2 - score_player_1;
    } else {
        return score_player_1 - score_player_2;
    }
}
