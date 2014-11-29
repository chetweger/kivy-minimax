#include "metaboard.h"

MetaBoard::MetaBoard() {
}

void MetaBoard::printMe() {
    cout << "Printing out board:\n";
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

char MetaBoard::generateNextPlayer(char current_player) {
    if (current_player == MiniBoard::PLAYER_TWO) {
        return MiniBoard::PLAYER_ONE;
    } else {
        assert(current_player == MiniBoard::PLAYER_ONE);
        return MiniBoard::PLAYER_TWO;
    }
}

vector<MetaBoard> MetaBoard::generateChildrenAux() {
    vector<MetaBoard> children;
    if (!this->boards[next_mini_board].isOver()) {
        char childs_next_player = this->generateNextPlayer(this->next_player);
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

double MetaBoard::computeUtility() {
    double score_player_1 = 0.0;
    double score_player_2 = 0.0;
    for (int i = 0; i < 9; i++) {
        score_player_1 += this->getScore(MiniBoard::PLAYER_ONE);
        score_player_1 += this->getNumCenterPieces(MiniBoard::PLAYER_ONE);
        score_player_1 += this->getNumCornerPieces(MiniBoard::PLAYER_ONE);
        score_player_1 += this->getNumSidePieces(MiniBoard::PLAYER_ONE);
        score_player_1 += this->getPlayerOneBlocking(MiniBoard::PLAYER_ONE);
        score_player_1 += this->getPlayerOnePotential(MiniBoard::PLAYER_ONE);

        score_player_2 += this->getScore(MiniBoard::PLAYER_TWO);
        score_player_2 += this->getNumCenterPieces(MiniBoard::PLAYER_TWO);
        score_player_2 += this->getNumCornerPieces(MiniBoard::PLAYER_TWO);
        score_player_2 += this->getNumSidePieces(MiniBoard::PLAYER_TWO);
        score_player_2 += this->getPlayerOneBlocking(MiniBoard::PLAYER_TWO);
        score_player_2 += this->getPlayerOnePotential(MiniBoard::PLAYER_TWO);
    }

    if (MiniBoard::PLAYER_ONE == this->player_max) {

    }
}
