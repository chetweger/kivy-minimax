#include "miniboard.cpp"
#include <vector>
#include "constants.h"

class MetaBoard {
    public:
        MetaBoard(); // constructor
        MiniBoard boards[9] = {
            MiniBoard(0),
            MiniBoard(1),
            MiniBoard(2),
            MiniBoard(3),
            MiniBoard(4),
            MiniBoard(5),
            MiniBoard(6),
            MiniBoard(7),
            MiniBoard(8)
        };

        int next_mini_board = 4;
        int next_player = 1;
        int player_max = -1;

        int searchDepth = -1; // TODO: make sure clone works for ALL constants!!!!!!!

        float myUtility;

        void printMe();

        MetaBoard clone();

        MetaBoard operator=(MetaBoard mini);

        bool operator>(MetaBoard other);
        bool operator<(MetaBoard other);
        bool operator>=(MetaBoard other);
        bool operator<=(MetaBoard other);

        char generateNextPlayer();

        vector<MetaBoard> generateChildrenAux();

        vector<MetaBoard> generateChildren();

        float getScore(char player);
        float getNumCenterPieces(char player);
        float getNumCornerPieces(char player);
        float getNumSidePieces(char player);
        
        float getPlayerOneBlocking();
        float getPlayerOnePotential();
        float getPlayerTwoBlocking();
        float getPlayerTwoPotential();

        float computeUtility();

        MetaBoard minimaxSearch(MetaBoard state, int searchDepth);
        MetaBoard minSearch(int searchDepth, MetaBoard alpha, MetaBoard beta);
        MetaBoard maxSearch(int searchDepth, MetaBoard alpha, MetaBoard beta);
};
