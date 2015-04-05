#include <Python.h>
#include "metaboard.cpp"

// 16 seconds for depth of 39
// with optimization of -O0

static PyObject* minimaxSearch(PyObject* self, PyObject *metaBoardStateArgs)
{
    MetaBoard metaBoard;
    int board[9][9];
    float constants[6];
    int getNextMove;

    if (!PyArg_ParseTuple(metaBoardStateArgs,
        "(iiiiiiiii)(iiiiiiiii)(iiiiiiiii)(iiiiiiiii)(iiiiiiiii)(iiiiiiiii)(iiiiiiiii)(iiiiiiiii)(iiiiiiiii)(ffffff)(iiiii)",
        &board[0][0], &board[0][1], &board[0][2], &board[0][3], &board[0][4], &board[0][5], &board[0][6], &board[0][7], &board[0][8],
        &board[1][0], &board[1][1], &board[1][2], &board[1][3], &board[1][4], &board[1][5], &board[1][6], &board[1][7], &board[1][8],
        &board[2][0], &board[2][1], &board[2][2], &board[2][3], &board[2][4], &board[2][5], &board[2][6], &board[2][7], &board[2][8],
        &board[3][0], &board[3][1], &board[3][2], &board[3][3], &board[3][4], &board[3][5], &board[3][6], &board[3][7], &board[3][8],
        &board[4][0], &board[4][1], &board[4][2], &board[4][3], &board[4][4], &board[4][5], &board[4][6], &board[4][7], &board[4][8],
        &board[5][0], &board[5][1], &board[5][2], &board[5][3], &board[5][4], &board[5][5], &board[5][6], &board[5][7], &board[5][8],
        &board[6][0], &board[6][1], &board[6][2], &board[6][3], &board[6][4], &board[6][5], &board[6][6], &board[6][7], &board[6][8],
        &board[7][0], &board[7][1], &board[7][2], &board[7][3], &board[7][4], &board[7][5], &board[7][6], &board[7][7], &board[7][8],
        &board[8][0], &board[8][1], &board[8][2], &board[8][3], &board[8][4], &board[8][5], &board[8][6], &board[8][7], &board[8][8],
        &constants[0], &constants[1], &constants[2], &constants[3], &constants[4], &constants[5],
        &metaBoard.next_mini_board, &metaBoard.next_player, &metaBoard.player_max, &metaBoard.searchDepth, &getNextMove
    )) {
        return NULL;
    }

    for (int i = 0; i < 9; i++) {
        metaBoard.boards[i].setBoard(board[i]);
    }
    metaBoard.setConstants(constants);

    MetaBoard next = metaBoard.minimaxSearch(metaBoard.searchDepth, (bool) getNextMove);

    cout << "next mini board is " << next.next_mini_board << "\n";

    return Py_BuildValue( "(iiiiiiiii)(iiiiiiiii)(iiiiiiiii)(iiiiiiiii)(iiiiiiiii)(iiiiiiiii)(iiiiiiiii)(iiiiiiiii)(iiiiiiiii)(ffffff)(iiiii)",
            next.boards[0].board[0], next.boards[0].board[1], next.boards[0].board[2], next.boards[0].board[3], next.boards[0].board[4], next.boards[0].board[5], next.boards[0].board[6], next.boards[0].board[7], next.boards[0].board[8], 
            next.boards[1].board[0], next.boards[1].board[1], next.boards[1].board[2], next.boards[1].board[3], next.boards[1].board[4], next.boards[1].board[5], next.boards[1].board[6], next.boards[1].board[7], next.boards[1].board[8], 
            next.boards[2].board[0], next.boards[2].board[1], next.boards[2].board[2], next.boards[2].board[3], next.boards[2].board[4], next.boards[2].board[5], next.boards[2].board[6], next.boards[2].board[7], next.boards[2].board[8], 
            next.boards[3].board[0], next.boards[3].board[1], next.boards[3].board[2], next.boards[3].board[3], next.boards[3].board[4], next.boards[3].board[5], next.boards[3].board[6], next.boards[3].board[7], next.boards[3].board[8], 
            next.boards[4].board[0], next.boards[4].board[1], next.boards[4].board[2], next.boards[4].board[3], next.boards[4].board[4], next.boards[4].board[5], next.boards[4].board[6], next.boards[4].board[7], next.boards[4].board[8], 
            next.boards[5].board[0], next.boards[5].board[1], next.boards[5].board[2], next.boards[5].board[3], next.boards[5].board[4], next.boards[5].board[5], next.boards[5].board[6], next.boards[5].board[7], next.boards[5].board[8], 
            next.boards[6].board[0], next.boards[6].board[1], next.boards[6].board[2], next.boards[6].board[3], next.boards[6].board[4], next.boards[6].board[5], next.boards[6].board[6], next.boards[6].board[7], next.boards[6].board[8], 
            next.boards[7].board[0], next.boards[7].board[1], next.boards[7].board[2], next.boards[7].board[3], next.boards[7].board[4], next.boards[7].board[5], next.boards[7].board[6], next.boards[7].board[7], next.boards[7].board[8], 
            next.boards[8].board[0], next.boards[8].board[1], next.boards[8].board[2], next.boards[8].board[3], next.boards[8].board[4], next.boards[8].board[5], next.boards[8].board[6], next.boards[8].board[7], next.boards[8].board[8], 
            constants[0], constants[1], constants[2], constants[3], constants[4], constants[5],
            next.next_mini_board, next.next_player, next.player_max, next.searchDepth, getNextMove
        );
}

static char minimaxSearch_docs[] = "minimaxSearch docs ...";

static PyMethodDef ai_funcs[] = {
    {"minimaxSearch", (PyCFunction)minimaxSearch,
     METH_VARARGS, minimaxSearch_docs},
    {NULL}
};

extern "C" {
    void initmeta_ttt_ai(void)
    {
        Py_InitModule3("meta_ttt_ai", ai_funcs,
                       "Extension module example!");
    }
}
