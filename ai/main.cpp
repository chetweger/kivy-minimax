#include "metaboard.cpp"
#include <iostream> 
#include <string> 
#include <bitset> 
#include <ctime>
//uncomment to disable assert() 
////#define NDEBUG 
//#include <cassert> 

// {'c3': 0.767944, 'c2': 1.049451, 'c1': 3.074038, 'c6': 0.220823, 'c5': 0.281883, 'c4': 0.605861}

void run_tests() {
    MiniBoard a(0);
    char input1[9] = {1, 2, 3, 4, 8, 16, 32, 64, 70};
    a.setBoard(input1);
    assert(a.isFull());
    assert(!a.isWin());
    assert(a.isOver());

    MiniBoard b(0);
    char input2[9] = {0, 0, 0, 1, 1, 1, 0, 0, 0};
    b.setBoard(input2);
    assert(b.isWin());
    assert(!b.isFull());
    assert(b.isOver());

    MiniBoard c(0);
    char input3[9] = {0, 2, 3, 4, 8, 16, 32, 64, 70};
    c.setBoard(input3);
    assert(!c.isWin());
    assert(!c.isFull());
    assert(!c.isOver());
    
    cout << "Tests Passed!\n";

    MiniBoard test_potential(0);
    char test_potential_board[9] = {0, 4, 4, 0, 0, 0, 1, 0, 1};
    test_potential.setBoard(test_potential_board);
    assert(test_potential.getPlayerOnePotential());
    assert(test_potential.getPlayerTwoPotential());

    MiniBoard test_blocking(0);
    char test_blocking_board[9] = {4, 0, 0, 0, 1, 0, 0, 0, 4};
    test_blocking.setBoard(test_blocking_board);
    assert(test_blocking.getPlayerOneBlocking());

    MetaBoard utilityTest;
    utilityTest.boards[0].board[2] = 4;
    utilityTest.boards[0].board[4] = 4;
    utilityTest.boards[0].board[6] = 4;

    utilityTest.boards[1].board[2] = 4;
    utilityTest.boards[1].board[5] = 4;
    utilityTest.boards[1].board[8] = 1;

    utilityTest.boards[2].board[4] = 4;
    utilityTest.boards[2].board[6] = 4;

    utilityTest.player_max = 1;
    utilityTest.next_player = 1;

    utilityTest.computeUtility();

    cout << "myUtility is: " << utilityTest.myUtility;
    cout << "Expected utility is: " << (td_constants.c1 + 
                                     td_constants.c2 + 
                                     td_constants.c3 + 
                                     td_constants.c4 - 
                                     td_constants.c5 + 
                                     td_constants.c6);

    assert(utilityTest.myUtility == (td_constants.c1 + 
                                     td_constants.c2 + 
                                     td_constants.c3 + 
                                     td_constants.c4 - 
                                     td_constants.c5 + 
                                     td_constants.c6));
}

int main(int numArgs, char* args[]) {
    cout << "Num args:\n";

    cout << numArgs;

    if (numArgs > 1) {

        cout << "\n";

        cout << "args is: " << (int) args[0][0] << "\n";
        cout << "args is: " << (int) args[2][2] << "\n";
        args[2][2] = 'd';
        args[3][3] = 't';
        cout << "args is: " << (int) args[2][2] << "\n";
        cout << "args is: " << (int) args[3][3] << "\n";

        MetaBoard metaBoard;
        MetaBoard::loadMetaBoard(args, &metaBoard);
        metaBoard.printMe();

        cout << (int) MiniBoard::PLAYER_ONE << "\n";

    }

    run_tests();

    MetaBoard state; // declare state without parenthesis to avoid confustion with function call

    cout << "\nUtility: " << state.computeUtility() << "\n"; 

    int start = time(0);
    MetaBoard bestChild = state.minimaxSearch(11, false);
    int end = time(0);

    bestChild.printMe();

    cout << "Time elapsed is: " << end - start << "\n";

    return 0;
}
