#include "Chess.h"
#include "ChessBoard.h"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
///Chess
/// public
int Chess::getX(){
    return x;
}

int Chess::getY(){
    return y;
}

void Chess::setX(int x){
    this->x = x;
    sp.setPosition(90 + 89 * x,50 + 89 * y);
}

void Chess::setY(int y){
    this->y = y;
    sp.setPosition(90 + 89 * x,50 + 89 * y);
}

Chess::Chess(int x,int y,int type,sf::Texture & texture,sf::IntRect intrect){
    this->x = x;
    this->y = y;
    this->type = type;
    sp.setTexture(texture);
    sp.setOrigin(31,31);
    sp.setPosition(90 + 89 * x,50 + 89 * y);
    sp.setTextureRect(intrect);
    sp.setScale(1.435, 1.435);
}

const sf::Sprite& Chess::getSprite(){
    return sp;
}

int Chess::getType(){
    return type;
}

/// protected
bool Chess::outOfBoard(int nx,int ny){
    return nx < 0 || nx > 8 || ny < 0 || ny > 9;
}

///Root
Root::Root(int x, int y, int type, sf::Texture &texture, sf::IntRect intrect):
Chess(x,y,type,texture,intrect){}

bool Root::legalMove(int nx,int ny,ChessBoard & chessboard){
    int cnt = 0;
    if(nx == x) ++cnt;
    if(ny == y) ++cnt;
    if(cnt != 1) return false;
    if(outOfBoard(nx,ny)) return false;
    bool hasChess = false;
    for(int i = std::min(x,nx) + 1 ; i < std::max(x,nx) ; i++ ){
        if(chessboard.hasChess(i,y)) hasChess = true;
    }
    for(int i = std::min(y,ny) + 1 ; i < std::max(y,ny) ; i++ ){
        if(chessboard.hasChess(x,i)) hasChess = true;
    }
    if(hasChess) return false;
    if(type == -1 && chessboard.hasRedChess(nx,ny)) return false;
    if(type == 1 && chessboard.hasBlackChess(nx,ny)) return false;
    return true;
}

///Cannon
Cannon::Cannon(int x, int y, int type, sf::Texture &texture, sf::IntRect intrect):
Chess(x,y,type,texture,intrect){}

bool Cannon::legalMove(int nx,int ny,ChessBoard & chessboard){
    int cnt = 0;
    if(nx == x) ++cnt;
    if(ny == y) ++cnt;
    if(cnt != 1) return false;
    if(outOfBoard(nx,ny)) return false;
    cnt = 0;
    for(int i = std::min(x,nx) + 1 ; i < std::max(x,nx) ; i++ ){
        if(chessboard.hasChess(i,y)) ++cnt;
    }
    for(int i = std::min(ny,y) + 1 ; i < std::max(ny,y) ; i++ ){
        if(chessboard.hasChess(x,i)) ++cnt;
    }
    if(cnt == 0 && !chessboard.hasChess(nx,ny)) return true;
    cnt = 0;
    int midtype = 0;
    for (int i = std::min(x, nx) + 1; i < std::max(x, nx); i++)
    {
        if (chessboard.hasChess(i, y)){
            ++cnt;
            midtype = chessboard.hasBlackChess(i,y) ? 1 : -1;
        }
    }
    for (int i = std::min(ny, y) + 1; i < std::max(ny, y); i++)
    {
        if (chessboard.hasChess(x, i)){
            ++cnt;
            midtype = chessboard.hasBlackChess(x,i) ? 1 : -1;
        }
    }
    if(cnt != 1) return false;
    if(!chessboard.hasChess(nx,ny)) return false;
    midtype = chessboard.hasBlackChess(nx,ny) ? 1 : -1;
    return type != midtype;
}

///Knight
Knight::Knight(int x, int y, int type, sf::Texture &texture, sf::IntRect intrect):
Chess(x,y,type,texture,intrect){}
const int Knight::dx[] = {2,1,-1,-2,-2,-1,1,2};
const int Knight::dy[] = {-1,-2,-2,-1,1,2,2,1};
const int Knight::bidx[] = {1,0,0,-1,-1,0,0,1};
const int Knight::bidy[] = {0,-1,-1,0,0,1,1,0};
bool Knight::legalMove(int nx,int ny,ChessBoard & chessboard){
    if(outOfBoard(nx,ny)) return false;
    int dex = -1;
    for(int i = 0 ; i < 8 ; i++ ){
        if(nx == x + dx[i] && ny == y + dy[i]){
            dex = i;
            break;
        }
    }
    // std::cout << "Knight" << std::endl;
    // std::cout << "dex = " << dex << std::endl;
    // std::cout << "x = " << x << "y = " << y << std::endl;
    // for(int j = 0 ; j < 10 ; j++ ){
    //     for(int i = 0 ; i < 9 ; i++ ){
    //         std::cout << chessboard.getVal(i,j) << " ";
    //     }
    //     std::cout << std::endl;
    // }
    if(dex == -1) return false;
    if(chessboard.hasChess(x + bidx[dex],y + bidy[dex])) return false;
    if(!chessboard.hasChess(nx,ny)) return true;
    int ntype = chessboard.hasBlackChess(nx,ny) ? 1 : -1;
    return type != ntype;
}

///Pawn
const int Pawn::dx[] = {0,0,1,-1};
const int Pawn::dy[] = {1,-1,0,0};
Pawn::Pawn(int x, int y, int type, sf::Texture &texture, sf::IntRect intrect):
Chess(x,y,type,texture,intrect){
    bid = type == -1 ? 0 : 1;
}

bool Pawn::legalMove(int nx,int ny,ChessBoard &chessboard){
    // std::cout << "????" << std::endl;
    if(outOfBoard(nx,ny)) return false;
    int dex = -1;
    for(int i = 0 ; i < 4 ; i++ ){
        if(nx == x + dx[i] && ny == y + dy[i]){
            dex = i;
            break;
        }
    }
    if(dex == -1) return false;
    if(dex == bid) return false;
    bool overriver = false;
    if(type == -1 && y < 5) overriver = true;
    if(type == 1 && y >= 5) overriver = true;
    // std::cout << "overriver = " << overriver << std::endl;
    if(!overriver && dex >= 2) return false;
    if(!chessboard.hasChess(nx,ny)) return true;
    int ntype = chessboard.hasBlackChess(nx,ny) ? 1 : -1;
    return type != ntype;
}

///Bishop
const int Bishop::dx[] = {2,-2,-2,2};
const int Bishop::dy[] = {-2,-2,2,2};
const int Bishop::bidx[] = {1,-1,-1,1};
const int Bishop::bidy[] = {-1,-1,1,1};

Bishop::Bishop(int x, int y, int type, sf::Texture &texture, sf::IntRect intrect):
Chess(x,y,type,texture,intrect){}

bool Bishop::legalMove(int nx,int ny,ChessBoard &chessboard){
    if(outOfBoard(nx,ny)) return false;
    int ntype = (ny >= 5) ? -1 : 1;
    if(ntype != type) return false;
    int dex = -1;
    for(int i = 0 ; i < 4 ; i++ ){
        if(nx == x + dx[i] && ny == y + dy[i]){
            dex = i;
            break;
        }
    }
    if(dex == -1) return false;
    if(chessboard.hasChess(x + bidx[dex],y + bidy[dex])) return false;
    if(!chessboard.hasChess(nx,ny)) return true;
    ntype = chessboard.hasBlackChess(nx,ny) ? 1 : -1;
    return type != ntype;
}

///Queen
const int Queen::dx[] = {1,-1,-1,1};
const int Queen::dy[] = {-1,-1,1,1};
Queen::Queen(int x, int y, int type, sf::Texture &texture, sf::IntRect intrect):
Chess(x,y,type,texture,intrect){}

bool Queen::legalMove(int nx,int ny,ChessBoard &chessboard){
    if(outOfBoard(nx,ny)) return false;
    int dex = -1;
    for(int i = 0 ; i < 4 ; i++ ){
        if(nx == x + dx[i] && ny == y + dy[i]){
            dex = i;
            break;
        }
    }
    if(dex == -1) return false;
    std::pair<int,int> leftup,rightdown;
    if(type == -1){
        leftup = {3,7};
        rightdown = {5,9};
    }else{
        leftup = {3,0};
        rightdown = {5,2};
    }
    bool lim = leftup.first <= nx && nx <= rightdown.first 
    && leftup.second <= ny && ny <= rightdown.second;
    if(!lim) return false;
    if(!chessboard.hasChess(nx,ny)) return true;
    int ntype = chessboard.hasBlackChess(nx,ny) ? 1 : -1;
    return ntype != type;
}

///King
const int King::dx[] = {1,0,-1,0};
const int King::dy[] = {0,-1,0,1};
King::King(int x, int y, int type, sf::Texture &texture, sf::IntRect intrect):
Chess(x,y,type,texture,intrect){}

bool King::legalMove(int nx,int ny,ChessBoard &chessboard){
    if(outOfBoard(nx,ny)) return false;
    int dex = -1;
    for(int i = 0 ; i < 4 ; i++ ){
        if(nx == x + dx[i] && ny == y + dy[i]){
            dex = i;
            break;
        }
    }
    if(dex == -1) return false;
    std::pair<int, int> leftup, rightdown;
    if (type == -1){
        leftup = {3, 7};
        rightdown = {5, 9};
    }else{
        leftup = {3, 0};
        rightdown = {5, 2};
    }
    bool lim = leftup.first <= nx && nx <= rightdown.first 
    && leftup.second <= ny && ny <= rightdown.second;
    if (!lim) return false;
    if (!chessboard.hasChess(nx, ny)) return true;
    int ntype = chessboard.hasBlackChess(nx, ny) ? 1 : -1;
    return ntype != type;
}
