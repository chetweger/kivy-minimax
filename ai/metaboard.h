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
        int next_player = 1;
        int player_max = -1;

        int searchDepth = -1; // TODO: make sure clone works for ALL constants!!!!!!!

        static float constants[6];

        float myUtility;

        int killerHeuristics = 0;

        void printMe();

        MetaBoard clone();

        MetaBoard copyOver(MetaBoard *);

        static MetaBoard loadMetaBoard(char* args[], MetaBoard * metaBoard);

        bool operator>(const MetaBoard other) const;
        bool operator<(const MetaBoard other) const;
        bool operator>=(MetaBoard other);
        bool operator<=(MetaBoard other);

        char generateNextPlayer();

        vector<MetaBoard> generateChildrenAux();

        vector<MetaBoard> generateChildren();

        static void setConstants(float constants[]);

        float computeUtilityAndKillerHeuristics(int boardPlacement);

        MetaBoard minimaxSearch(int searchDepth, bool getNextMove);
        MetaBoard minSearch(int searchDepth, MetaBoard alpha, MetaBoard beta);
        MetaBoard maxSearch(int searchDepth, MetaBoard alpha, MetaBoard beta, bool getNextMove);
};
