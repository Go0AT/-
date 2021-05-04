#ifndef CHESS_H__
#define CHESS_H__

#include <SFML/Graphics.hpp>
#include "ChessBoard.h"
class Chess{
protected:
    sf::Sprite sp;
    int x,y;
    int type;
    bool outOfBoard(int nx,int ny);
public:
    int getX();
    int getY();
    void setX(int x);
    void setY(int y);
    int getType();
    const sf::Sprite& getSprite();
    Chess(int x,int y,int type,sf::Texture & texture,sf::IntRect intrect);
    virtual bool legalMove(int nx,int ny,ChessBoard & chessboard) = 0;
};

class Root : public Chess{
public:
    virtual bool legalMove(int nx,int ny,ChessBoard & chessboard);
    Root(int x,int y,int type,sf::Texture & texture,sf::IntRect intrect);
};

class Cannon : public Chess{
public:
    virtual bool legalMove(int nx,int ny,ChessBoard & chessboard);
    Cannon(int x, int y, int type, sf::Texture &texture, sf::IntRect intrect);
};

class Knight : public Chess{
private:
    const static int dx[];
    const static int dy[];
    const static int bidx[];
    const static int bidy[];
public:
    virtual bool legalMove(int nx,int ny,ChessBoard & chessboard);
    Knight(int x, int y, int type, sf::Texture &texture, sf::IntRect intrect);
};

class Pawn : public Chess{
private:
    const static int dx[];
    const static int dy[];
    int bid;
public:
    virtual bool legalMove(int nx,int ny,ChessBoard & chessboard);
    Pawn(int x, int y, int type, sf::Texture &texture, sf::IntRect intrect);
};

class Bishop : public Chess{
private:
    const static int dx[];
    const static int dy[];
    const static int bidx[];
    const static int bidy[];
public:
    virtual bool legalMove(int nx,int ny,ChessBoard &chessboard);
    Bishop(int x,int y,int type,sf::Texture &texture,sf::IntRect intrect);
};

class Queen : public Chess{
private:
    const static int dx[];
    const static int dy[];
public:
    virtual bool legalMove(int nx,int ny,ChessBoard &chessboard);
    Queen(int x, int y, int type, sf::Texture &texture, sf::IntRect intrect);
};

class King : public Chess{
private:
    const static int dx[];
    const static int dy[];
public:
    virtual bool legalMove(int nx,int ny,ChessBoard &chessboard);
    King(int x, int y, int type, sf::Texture &texture, sf::IntRect intrect);
};
#endif