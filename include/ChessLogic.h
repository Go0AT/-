#ifndef CHESS_LOGIC_H__
#define CHESS_LOGIC_H__

#include "Chess.h"
#include <SFML/Graphics.hpp>
#include "ChessBoard.h"
#include <stack>

class MoveAble{
private:
    bool canmove;
    Chess *sp;
    int nx,ny;
public:
    MoveAble(bool canmove, Chess *sp = nullptr,int nx = -1, int ny = -1);
    bool getCanMove();
    Chess *getChess();
    int getX();
    int getY();
};

class OneMove{
private:
    Chess *s,*t;
    int sx,sy;
public:
    OneMove(int sx,int sy,Chess *s,Chess *t = nullptr);
    Chess *getS();
    Chess *getT();
    int getSx();
    int getSy();
};

class MoveAction{
private:
    int turn,selected;
    std::stack<OneMove> withdrawstk;
public:
    MoveAction();
    int getSelected();
    int getTurn();
    MoveAble canMove(Chess **arr, ChessBoard &chessboard,sf::Vector2i &pos);
    bool checkMate(Chess **arr,ChessBoard &chessboard,int kingdex);
    bool move(Chess **arr,ChessBoard &chessboard,MoveAble moveable);
    bool win(Chess **arr,ChessBoard &chessboard);
    bool repentance(ChessBoard &chessboard);
};

class StopWatch{
private:
    sf::Clock clock;
    sf::Text text;
public:
    StopWatch(sf::Font &font);
    void setText();
    void reverseColor();
    void reStart();
    sf::Text &getText();
    float getElapsedTime();
};

#endif