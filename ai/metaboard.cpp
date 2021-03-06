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
    cout << "constants are: " << this->constants[0] << "\t" << this->constants[1] << "\t"  << this->constants[2] << "\t"  << this->constants[3] << "\t"  << this->constants[4] << "\t"  << this->constants[5] << "\n";
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
    cloned.killerHeuristics = this->killerHeuristics;
    return cloned;
}

MetaBoard MetaBoard::loadMetaBoard(char* args[], MetaBoard * metaBoard) {
    for (int i = 0; i < 9; i++) {
        metaBoard->boards[i].setBoard(args[i]);
    }
    metaBoard->next_mini_board = (int) args[9][0];
    metaBoard->next_player = (int) args[9][1];
    metaBoard->player_max = (int) args[9][2];
    metaBoard->searchDepth = (int) args[9][3];
    metaBoard->myUtility = (int) args[9][4];
    return * metaBoard;
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
    cloned->killerHeuristics = this->killerHeuristics;
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
        //assert((int) next_player == (int) MiniBoard::PLAYER_ONE);
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
                child.computeUtilityAndKillerHeuristics(next_mini_board);
                children.push_back(child);
            }
        }
    }
    return children;
}

struct greater_than_key
{
    inline bool operator() (const MetaBoard& struct1, const MetaBoard& struct2)
    {
        return (struct1.killerHeuristics > struct2.killerHeuristics);
    }
};

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
    sort(children.begin(), children.end(), greater_than_key());
    return children;
}

float MetaBoard::constants[6] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};

void MetaBoard::setConstants(float inputConstants[]) {
    for (int i = 0; i < 6; i++) {
        MetaBoard::constants[i] = inputConstants[i];
    }
}

float MetaBoard::computeUtilityAndKillerHeuristics(int boardPlacement) {
    float score_player_1 = 0.0f;
    float score_player_2 = 0.0f;

    int player_one_scored = 0;
    int player_two_scored = 0;

    this->killerHeuristics = 0;

    int scorePlayer1 = 0;
    int centerPlayer1 = 0;
    int cornerPlayer1 = 0;
    int sidePlayer1 = 0;
    int blockingPlayer1 = 0;
    int potentialPlayer1 = 0;

    int scorePlayer2 = 0;
    int centerPlayer2 = 0;
    int cornerPlayer2 = 0;
    int sidePlayer2 = 0;
    int blockingPlayer2 = 0;
    int potentialPlayer2 = 0;

    for (int i = 0; i < 9; i++) {
        player_one_scored = this->boards[i].getScore(MiniBoard::PLAYER_ONE);
        player_two_scored = this->boards[i].getScore(MiniBoard::PLAYER_TWO);
        scorePlayer1 += player_one_scored;
        scorePlayer2 +=  player_two_scored;
        if (!(player_one_scored || player_two_scored)) {
            centerPlayer1 += this->boards[i].getNumCenterPieces(MiniBoard::PLAYER_ONE);
            cornerPlayer1 += this->boards[i].getNumCornerPieces(MiniBoard::PLAYER_ONE);
            sidePlayer1 += this->boards[i].getNumSidePieces(MiniBoard::PLAYER_ONE);
            blockingPlayer1 += this->boards[i].getPlayerOneBlocking();
            potentialPlayer1 += this->boards[i].getPlayerOnePotential();

            centerPlayer2 += this->boards[i].getNumCenterPieces(MiniBoard::PLAYER_TWO);
            cornerPlayer2 += this->boards[i].getNumCornerPieces(MiniBoard::PLAYER_TWO);
            sidePlayer2 += this->boards[i].getNumSidePieces(MiniBoard::PLAYER_TWO);
            blockingPlayer2 += this->boards[i].getPlayerTwoBlocking();
            potentialPlayer2 += this->boards[i].getPlayerTwoPotential();

            if (i == boardPlacement) {
                // compute "blocking" killer heuristic
                if (this->boards[i].getPlayerOneBlocking() || this->boards[i].getPlayerTwoBlocking()) {
                    this->killerHeuristics += 2;
                }

                // compute "potential" killer heuristic
                if (this->boards[i].getPlayerOneBlocking() || this->boards[i].getPlayerTwoBlocking()) {
                    this->killerHeuristics += 1;
                }
            }
        }
        
        // compute "board win" killer heuristic
        if (i == boardPlacement && (player_one_scored || player_two_scored)) {
            this->killerHeuristics += 3;
        }
    }
    score_player_1 += (float) MetaBoard::constants[0] * scorePlayer1;
    score_player_1 += (float) MetaBoard::constants[1] * centerPlayer1;
    score_player_1 += (float) MetaBoard::constants[2] * cornerPlayer1;
    score_player_1 += (float) MetaBoard::constants[3] * sidePlayer1;
    score_player_1 += (float) MetaBoard::constants[4] * blockingPlayer1;
    score_player_1 += (float) MetaBoard::constants[5] * potentialPlayer1;

    score_player_2 += (float) MetaBoard::constants[0] * scorePlayer2;
    score_player_2 += (float) MetaBoard::constants[1] * centerPlayer2;
    score_player_2 += (float) MetaBoard::constants[2] * cornerPlayer2;
    score_player_2 += (float) MetaBoard::constants[3] * sidePlayer2;
    score_player_2 += (float) MetaBoard::constants[4] * blockingPlayer2;
    score_player_2 += (float) MetaBoard::constants[5] * potentialPlayer2;

    if (MiniBoard::PLAYER_ONE == this->player_max) {
        this->myUtility = score_player_1 - score_player_2;
    } else {
        this->myUtility = score_player_2 - score_player_1;
    }
    return this->myUtility;
}

MetaBoard MetaBoard::minimaxSearch(int searchDepth, bool getNextMove) {
    this->searchDepth = searchDepth;
    MetaBoard alpha = *this;
    alpha.myUtility = -90000005;
    MetaBoard beta = *this;
    beta.myUtility = 90000005;
    cout << "Inside minimaxSearch, searchDepth is: " << searchDepth << "\n";
    return this->maxSearch(searchDepth, alpha, beta, getNextMove);
}

MetaBoard MetaBoard::maxSearch(int searchDepth, MetaBoard alpha, MetaBoard beta, bool getNextMove) {
    MetaBoard parent = *this;
    vector<MetaBoard> children = parent.generateChildren();
    // base cases ...
    if (children.size() <= 0 || searchDepth <= 0) {
        return *this;
    }
    int depthDelta = 1;
    if (children.size() > 27) {
        depthDelta = 2;
    }
    MetaBoard highest = children[0].minSearch(searchDepth - depthDelta, alpha, beta);
    MetaBoard possibleHighest;
    int highestChildIndex = 0;
    for (int i = 1; i < children.size(); i++) {
        children[i].minSearch(searchDepth - depthDelta, highest, beta).copyOver(&possibleHighest);
        if (getNextMove) {
            cout << "possibleHighest: " << i << "\n";
            //possibleHighest.printMe();
            cout << "alpha: " << alpha.myUtility << "\n";
            cout << "beta: " << beta.myUtility << "\n";
            cout << "possibleHighest: " << possibleHighest.myUtility << "\n";
            cout << "beta: " <<  (possibleHighest > highest) << "\n";
        }
        if (possibleHighest > highest) {
            highestChildIndex = i;
            possibleHighest.copyOver(&highest);
        }
        if (possibleHighest >= beta) {
            highest.myUtility = 90000005;
            break;
        }
    }
    if (getNextMove) {
        return children[highestChildIndex];
    } else {
        return highest;
    }
}

MetaBoard MetaBoard::minSearch(int searchDepth, MetaBoard alpha, MetaBoard beta) {
    MetaBoard parent = *this;
    vector<MetaBoard> children = parent.generateChildren();
    // base cases ...
    if (children.size() <= 0 || searchDepth <= 0) {
        return *this;
    }
    int depthDelta = 1;
    if (children.size() > 27) {
        depthDelta = 2;
    }
    MetaBoard lowest = children[0].maxSearch(searchDepth - depthDelta, alpha, beta, false);
    MetaBoard possibleLowest;
    for (int i = 1; i < children.size(); i++) {
        children[i].maxSearch(searchDepth - depthDelta, alpha, lowest, false).copyOver(&possibleLowest);
        if (possibleLowest < lowest) {
            possibleLowest.copyOver(&lowest); 
        }
        if (possibleLowest <= alpha) {
            lowest.myUtility = -90000005;
            break;
        }
    }
    return lowest;
}
