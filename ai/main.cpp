#include "metaboard.cpp"
#include <iostream> 
#include <string> 
#include <bitset> 
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
}

int main() {
    cout << (int) MiniBoard::PLAYER_ONE << "\n";

    run_tests();

    MetaBoard state; // declare state without parenthesis to avoid confustion with function call
    state.boards[3].board[3] = 4;
    state.boards[4].board[0] = 1;
    state.boards[4].board[1] = 1;
    state.boards[4].board[2] = 1;

    state.player_max = 1;
    state.printMe();
    cout << "\nUtility: " << state.computeUtility() << "\n"; 

    return 0;
}
