#ifndef CHESSBOARD_H__
#define CHESSBOARD_H__

class ChessBoard{
private:
    int arr[9][10];
public:
    ChessBoard();
    bool hasChess(int x,int y);
    bool hasBlackChess(int x,int y);
    bool hasRedChess(int x,int y);
    void setVal(int x,int y,int val);
    int getVal(int x,int y);
};

#endif