#include "miniboard.cpp"
#include <vector>
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
        int next_player = 2;
        int player_max = -1;

        void printMe();

        MetaBoard clone();

        MetaBoard operator=(MetaBoard mini);

        char generateNextPlayer(char current_player);

        vector<MetaBoard> generateChildrenAux();

        vector<MetaBoard> generateChildren();

        double computeUtility();
};
