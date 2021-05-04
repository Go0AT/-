#include "Chess.h"
#include "ChessLogic.h"
#include "ChessBoard.h"
#include <SFML/Graphics.hpp>
#include <typeinfo>
#include <cstring>
#include <iostream>
using namespace std;
extern int CHESS_NUMBER;
extern int TIME_LIMIT_PER_MOVE;
///MoveAble
MoveAble::MoveAble(bool canmove, Chess *sp, int nx, int ny) : 
canmove(canmove), sp(sp) ,nx(nx), ny(ny) {}

bool MoveAble::getCanMove(){
    return canmove;
}

Chess *MoveAble::getChess(){
    return sp;
}

int MoveAble::getX(){
    return nx;
}

int MoveAble::getY(){
    return ny;
}

///OneMove
OneMove::OneMove(int sx,int sy,Chess *s,Chess *t) :
sx(sx),sy(sy),s(s),t(t){}

Chess *OneMove::getS(){
    return s;
}

Chess *OneMove::getT(){
    return t;
}

int OneMove::getSx(){
    return sx;
}

int OneMove::getSy(){
    return sy;
}

///MoveAction
MoveAction::MoveAction(){
    turn = -1;
    selected = -1;
    while(!withdrawstk.empty()) withdrawstk.pop();
}

int MoveAction::getSelected(){
    return selected;
}

int MoveAction::getTurn(){
    return turn;
}

MoveAble MoveAction::canMove(Chess **arr,ChessBoard &chessboard,sf::Vector2i &pos){
    int dex = -1;
    for (int i = 0; i < CHESS_NUMBER; i++){
        if (arr[i]->getSprite().getGlobalBounds().contains(pos.x, pos.y)){
            dex = i;
            break;
        }
    }
    // std::cout << "dex = " << dex << endl;
    // if(dex != -1) std::cout << "type = " << arr[dex]->getType() << endl;
    if (dex != -1){
        if (selected == -1){
            if (arr[dex]->getType() == turn) selected = dex;
        }else{
            if (arr[selected]->getType() == arr[dex]->getType()) selected = dex;
            else{
                // cout << "enter1" << endl;
                if (arr[selected]->legalMove(arr[dex]->getX(), arr[dex]->getY(), chessboard))
                    return MoveAble(true,arr[selected],arr[dex]->getX(),arr[dex]->getY());
                else{
                    selected = -1;
                    return MoveAble(false);
                }
            }
        }
    }
    else{
        // cout << "enter" << endl;
        if (selected == -1) return MoveAble(false);
        int nx = -1, ny;
        for (int i = 0; i < 9; i++){
            for (int j = 0; j < 10; j++){
                int ox = 90 + i * 89;
                int oy = 50 + j * 89;
                if ((ox - pos.x) * (ox - pos.x) + (oy - pos.y) * (oy - pos.y) <= 1936 /* 44^2 */){
                    nx = i;
                    ny = j;
                    break;
                }
            }
            if (nx != -1) break;
        }
        // std::cout << "hahha" << endl;
        if (nx == -1){
            selected = -1;
            return MoveAble(false);
        }
        // std::cout << "nx = " << nx << "ny = " << ny << endl;
        if (arr[selected]->legalMove(nx, ny, chessboard)) return MoveAble(true,arr[selected],nx,ny);
        else selected = -1;
    }
    return MoveAble(false);
}

bool MoveAction::checkMate(Chess **arr,ChessBoard &chessboard,int kingdex){
    for(int i = 0 ; i < CHESS_NUMBER ; i++ ){
        if(arr[i]->getX() == -2) continue;
        if(arr[i]->legalMove(arr[kingdex]->getX(),arr[kingdex]->getY(),chessboard)){
            return true;
        }
    }
    int kingdex2 = -1;
    for(int i = 0 ; i < CHESS_NUMBER ; i++ ){
        if(typeid(*arr[i]) == typeid(King) && i != kingdex){
            kingdex2 = i;
            break;
        }
    }
    if(arr[kingdex]->getX() != arr[kingdex2]->getX()) return false;
    bool facetoface = true;
    int y1 = arr[kingdex]->getY(),y2 = arr[kingdex2]->getY();
    for(int i = std::min(y1,y2) + 1 ; i < std::max(y1,y2) ; i++ ){
        if(chessboard.hasChess(arr[kingdex]->getX(),i)){
            facetoface = false;
        }
    }
    return facetoface;
}

bool MoveAction::move(Chess **arr,ChessBoard &chessboard,MoveAble moveable){
    Chess *itr = nullptr;
    for(int i = 0 ; i < CHESS_NUMBER ; i++ ){
        if(arr[i]->getX() == moveable.getX() && arr[i]->getY() == moveable.getY()){
            itr = arr[i];
            break;
        }
    }
    ChessBoard tmp = chessboard;
    int sx = moveable.getChess()->getX();
    int sy = moveable.getChess()->getY();
    tmp.setVal(sx,sy,0);
    tmp.setVal(moveable.getX(),moveable.getY(),turn);
    if(itr != nullptr){
        itr->setX(-2);
        itr->setY(-2);
    }
    moveable.getChess()->setX(moveable.getX());
    moveable.getChess()->setY(moveable.getY());
    int dex = -1;
    for(int i = 0 ; i < CHESS_NUMBER ; i++ ){
        if(arr[i]->getType() == turn && typeid(*arr[i]) == typeid(King)){
            dex = i;
            break;
        }
    }
    if(checkMate(arr,tmp,dex)){
        selected = -1;
        moveable.getChess()->setX(sx);
        moveable.getChess()->setY(sy);
        if(itr != nullptr){
            itr->setX(moveable.getX());
            itr->setY(moveable.getY());
        }
        return false;
    }
    selected = -1;
    turn = -turn;
    chessboard = tmp;
    withdrawstk.push(OneMove(sx,sy,moveable.getChess(),itr));
    return true;
}

bool MoveAction::win(Chess **arr,ChessBoard &chessboard){
    int dex = -1;
    for(int i = 0 ; i < CHESS_NUMBER ; i++ ){
        if(arr[i]->getType() == turn && typeid(*arr[i]) == typeid(King)){
            dex = i;
            break;
        }
    }
    // Maybe not checkmate at this moment, but will checkmate
    // if do any legal move
    // if(!checkMate(arr,chessboard,dex)) return false;
    bool escape = false;
    Chess *itr = nullptr;
    ChessBoard tmp = chessboard;
    for(int i = 0 ; i < CHESS_NUMBER && !escape; i++ ){
        if(arr[i]->getType() != turn) continue;
        int sx = arr[i]->getX(),sy = arr[i]->getY();
        for(int j = 0 ; j < 9 && !escape; j++ ){
            for(int k = 0 ; k < 10 && !escape; k++ ){
                if(arr[i]->legalMove(j,k,chessboard)){
                    for(int p = 0 ; p < CHESS_NUMBER ; p++ ){
                        if(arr[p]->getX() == j && arr[p]->getY() == k){
                            itr = arr[p];
                            break;
                        }
                    }
                    if(itr != nullptr){
                        itr->setX(-2);
                        itr->setY(-2);
                    }
                    arr[i]->setX(j);
                    arr[i]->setY(k);
                    tmp.setVal(j,k,turn);
                    tmp.setVal(sx,sy,0);
                    if(!checkMate(arr,tmp,dex)){
                        escape = true;
                    }
                    arr[i]->setX(sx);
                    arr[i]->setY(sy);
                    tmp.setVal(j,k,0);
                    if(itr != nullptr){
                        itr->setX(j);
                        itr->setY(k);
                        tmp.setVal(j,k,-turn);
                        itr = nullptr;
                    }
                    tmp.setVal(sx,sy,turn);
                }
            }
        }
    }
    return !escape;
}

bool MoveAction::repentance(ChessBoard &chessboard){
    if(withdrawstk.empty()) return false;
    OneMove onemove = withdrawstk.top();
    withdrawstk.pop();
    int tx = onemove.getS()->getX();
    int ty = onemove.getS()->getY();
    if(onemove.getT() != nullptr){
        onemove.getT()->setX(tx);
        onemove.getT()->setY(ty);
        chessboard.setVal(tx,ty,onemove.getT()->getType());
    }else chessboard.setVal(tx,ty,0);
    onemove.getS()->setX(onemove.getSx());
    onemove.getS()->setY(onemove.getSy());
    chessboard.setVal(onemove.getSx(),onemove.getSy(),onemove.getS()->getType());
    turn = -turn;
    selected = -1;
    return true;
}

///StopWatch
StopWatch::StopWatch(sf::Font &font){
    text.setFont(font);
    text.setFillColor(sf::Color::Red);
    text.setPosition(892 / 2 - 38, 900 / 2 - 15);
}

void StopWatch::setText(){
    int t = clock.getElapsedTime().asSeconds();
    t = TIME_LIMIT_PER_MOVE - t;
    int minutes = t / 60;
    int seconds = t % 60;
    std::string str;
    str.push_back('0' + minutes / 10);
    str.push_back('0' + minutes % 10);
    str.push_back(':');
    str.push_back('0' + seconds / 10);
    str.push_back('0' + seconds % 10);
    text.setString(str);
}

void StopWatch::reverseColor(){
    if(text.getFillColor() == sf::Color::Red){
        text.setFillColor(sf::Color::Black);
    }else text.setFillColor(sf::Color::Red);
}

void StopWatch::reStart(){
    clock.restart();
}

sf::Text &StopWatch::getText(){
    return text;
}

float StopWatch::getElapsedTime(){
    return clock.getElapsedTime().asSeconds();
}