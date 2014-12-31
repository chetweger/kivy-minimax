#include "metaboard.h"
#include <algorithm>

MetaBoard::MetaBoard() {
}

void MetaBoard::printMe() {
    cout << "Printing out board:\n";
    cout << "Current player is: " << (int) this->generateNextPlayer() << "\nPlayer MAX is: " << player_max << "\n";
    for (int i = 0; i < 9; i++) {
        this->boards[i].printMe();
    }
    cout << "Utility is: " << this->myUtility << "\n";
}

MetaBoard MetaBoard::clone() {
    MetaBoard cloned;
    for (int i = 0; i < 9; i++) {
        // Make identical copies of all boards.
        cloned.boards[i] = this->boards[i].copyMe(cloned.boards[i]);
    }
    cloned.next_mini_board = this->next_mini_board;
    cloned.next_player = this->next_player;
    cloned.player_max = this->player_max;
    cloned.searchDepth = this->searchDepth;
    cloned.myUtility = this->myUtility;
    return cloned;
}

MetaBoard MetaBoard::copyOver(MetaBoard * cloned) {
    for (int i = 0; i < 9; i++) {
        // Make identical copies of all boards.
        cloned->boards[i] = this->boards[i].copyMe(cloned->boards[i]);
    }
    cloned->next_mini_board = this->next_mini_board;
    cloned->next_player = this->next_player;
    cloned->player_max = this->player_max;
    cloned->searchDepth = this->searchDepth;
    cloned->myUtility = this->myUtility;
    return * cloned;
}

bool MetaBoard::operator > (const MetaBoard other) const {
    return this->myUtility > other.myUtility;
}

bool MetaBoard::operator < (const MetaBoard other) const {
    return this->myUtility < other.myUtility;
}

bool MetaBoard::operator >= (MetaBoard other) {
    return this->myUtility >= other.myUtility;
}

bool MetaBoard::operator <= (MetaBoard other) {
    return this->myUtility <= other.myUtility;
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
                child.computeUtility();
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
    sort(children.begin(), children.end(), greater<MetaBoard>());
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

    int player_one_scored = -1;
    int player_two_scored = -1;
    for (int i = 0; i < 9; i++) {
        player_one_scored = this->boards[i].getScore(MiniBoard::PLAYER_ONE);
        player_two_scored = this->boards[i].getScore(MiniBoard::PLAYER_TWO);
        score_player_1 += td_constants.c1 * player_one_scored;
        score_player_2 += td_constants.c1 * player_two_scored;
        if ( ! (player_one_scored || player_two_scored)){
            score_player_1 += td_constants.c2 * this->boards[i].getNumCenterPieces(MiniBoard::PLAYER_ONE);
            score_player_1 += td_constants.c3 * this->boards[i].getNumCornerPieces(MiniBoard::PLAYER_ONE);
            score_player_1 += td_constants.c4 * this->boards[i].getNumSidePieces(MiniBoard::PLAYER_ONE);
            score_player_1 += td_constants.c5 * this->boards[i].getPlayerOneBlocking();
            score_player_1 += td_constants.c6 * this->boards[i].getPlayerOnePotential();

            score_player_2 += td_constants.c2 * this->boards[i].getNumCenterPieces(MiniBoard::PLAYER_TWO);
            score_player_2 += td_constants.c3 * this->boards[i].getNumCornerPieces(MiniBoard::PLAYER_TWO);
            score_player_2 += td_constants.c4 * this->boards[i].getNumSidePieces(MiniBoard::PLAYER_TWO);
            score_player_2 += td_constants.c5 * this->boards[i].getPlayerTwoBlocking();
            score_player_2 += td_constants.c6 * this->boards[i].getPlayerTwoPotential();
        }
    }

    if (MiniBoard::PLAYER_ONE == this->player_max) {
        this->myUtility = score_player_2 - score_player_1;
    } else {
        this->myUtility = score_player_1 - score_player_2;
    }
    return this->myUtility;
}

MetaBoard MetaBoard::minimaxSearch(int searchDepth, bool getNextMove) {
    this->searchDepth = searchDepth;
    MetaBoard alpha = *this;
    alpha.myUtility = -9005.;
    MetaBoard beta = *this;
    beta.myUtility = 9005.;
    return this->maxSearch(searchDepth, alpha, beta, getNextMove);
}

MetaBoard MetaBoard::maxSearch(int searchDepth, MetaBoard alpha, MetaBoard beta, bool getNextMove) {
    MetaBoard parent = *this;
    vector<MetaBoard> children = parent.generateChildren();
    // base cases ...
    if (children.size() <= 0 || searchDepth <= 0) {
        return *this;
    }
    MetaBoard highest;
    if (getNextMove) {
        children[0].copyOver(&highest);;
    } else {
        children[0].minSearch(searchDepth - 1, alpha, beta).copyOver(&highest);
    }
    for (int i = 1; i < children.size(); i++) {
        MetaBoard possibleHighest = children[i].minSearch(searchDepth - 1, highest, beta);
        if (possibleHighest > highest) {
            if (getNextMove) {
                children[i].copyOver(&highest);
            } else {
                possibleHighest.copyOver(&highest);
            }
        }
        if (possibleHighest >= beta) {
            break;
        }
    }
    return highest;
}

MetaBoard MetaBoard::minSearch(int searchDepth, MetaBoard alpha, MetaBoard beta) {
    MetaBoard parent = *this;
    vector<MetaBoard> children = parent.generateChildren();
    // base cases ...
    if (children.size() <= 0 || searchDepth <= 0) {
        return *this;
    }
    MetaBoard lowest = children[0].maxSearch(searchDepth - 1, alpha, beta, false);
    for (int i = 1; i < children.size(); i++) {
        MetaBoard possibleLowest = children[i].maxSearch(searchDepth - 1, alpha, lowest, false);
        if (possibleLowest < lowest) {
            possibleLowest.copyOver(&lowest); 
        }
        if (possibleLowest <= alpha) {
            break;
        }
    }
    return lowest;
}
