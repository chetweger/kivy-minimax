#include <iostream>
#include <string>
#include <bitset>
//uncomment to disable assert()
//#define NDEBUG
#include <cassert>
using namespace std;

class MiniBoard {
    public:
        char board[9] = {0,0,0,0,0,0,0,0,0};
        int meta_position = -1;

        const static char PLAYER_ONE = 1;
        const static char PLAYER_TWO = 4;

        const static char PLAYER_ONE_BLOCKING = PLAYER_TWO * 2 + PLAYER_ONE;
        const static char PLAYER_TWO_BLOCKING = PLAYER_TWO + PLAYER_ONE * 2;

        const static char PLAYER_ONE_POTENTIAL = PLAYER_ONE * 2;
        const static char PLAYER_TWO_POTENTIAL = PLAYER_TWO * 2;

        MiniBoard(int a);

        void setMetaPosition(int input);

        int getMetaPosition();

        void setBoard(char input_board[]);

        void setBoard(int input_board[]);

        void printMe();

        void placePiece(char piece, int position);

        char isWin();

        bool isFull();

        bool isOver(); // is full or win

        int getScore(char player);

        int getNumCenterPieces(char player);

        int getNumCornerPieces(char player);

        int getNumSidePieces(char player);

        int getPlayerOneBlocking();

        int getPlayerTwoBlocking();

        int getPlayerOnePotential();

        int getPlayerTwoPotential();

        MiniBoard copyMe(MiniBoard mini);
};
