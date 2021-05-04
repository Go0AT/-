#include "ChessBoard.h"
#include <cstring>
using namespace std;

ChessBoard::ChessBoard(){
    memset(arr,0,sizeof(arr));
    for(int i = 0 ; i <= 8 ; i++ ){
        arr[i][0] = 1;
    }
    arr[1][2] = arr[7][2] = 1;
    for(int i = 0 ; i <= 8 ; i += 2){
        arr[i][3] = 1;
    }
    for(int i = 0 ; i <= 8 ; i++ ){
        arr[i][9] = -1;
    }
    arr[1][7] = arr[7][7] = -1;
    for(int i = 0 ; i <= 8 ; i += 2){
        arr[i][6] = -1;
    }
    return ;
}

bool ChessBoard::hasChess(int x,int y){
    return arr[x][y] != 0;
}

bool ChessBoard::hasRedChess(int x,int y){
    return arr[x][y] == -1;
}

bool ChessBoard::hasBlackChess(int x,int y){
    return arr[x][y] == 1;
}

void ChessBoard::setVal(int x,int y,int val){
    arr[x][y] = val;
}

int ChessBoard::getVal(int x,int y){
    return arr[x][y];
}