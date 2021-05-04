#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <Chess.h>
#include <ChessBoard.h>
#include <ChessLogic.h>
using namespace std;
extern const int CHESS_NUMBER = 32;
extern const int TIME_LIMIT_PER_MOVE = 120;
sf::Texture chess,board;
sf::Font font;
Chess *arr[CHESS_NUMBER];
void init();
void recover();
int main(){
    init();
    std::ios::sync_with_stdio(false);
    sf::RenderWindow window(sf::VideoMode(892,900),"Chess!",sf::Style::Close | sf::Style::Titlebar);
    sf::Event event;
    sf::Sprite background(board);
    ChessBoard chessboard;
    MoveAction moveaction;
    StopWatch stopwatch(font);
    bool win = false;
    while(window.isOpen()){
        sf::Vector2i pos = sf::Mouse::getPosition(window);
        if(!win && stopwatch.getElapsedTime() > TIME_LIMIT_PER_MOVE){
            win = true;
            if(moveaction.getTurn() == -1) std::cout << "Black Win!" << endl;
            else std::cout << "Red Win!" << endl;
        }
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            }
            if(!win && event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left){
                MoveAble moveable = moveaction.canMove(arr,chessboard,pos);
                // std::cout << "selected = " << moveaction.getSelected() << endl;
                // std::cout << "canMove = " << moveable.getCanMove() << endl;
                if(!moveable.getCanMove()) continue;
                if(moveaction.move(arr,chessboard,moveable)){
                    stopwatch.reverseColor();
                    stopwatch.reStart();
                }
                if(moveaction.win(arr,chessboard)){
                    if(moveaction.getTurn() == -1) std::cout << "Black Win!" << endl;
                    else std::cout << "Red Win!" << endl;
                    win = true;
                }
                // std::cout << win << endl;
            }
            if(event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Right){
                win = false;
                if(!moveaction.repentance(chessboard)){
                    std::cout << "Can't repent at first" << endl;
                }else{
                    stopwatch.reverseColor();
                    stopwatch.reStart();
                }
            }
        }
        // std::cout << pos.x << " " << pos.y << endl;
        window.clear();
        window.draw(background);
        for(int i = 0 ; i < CHESS_NUMBER ; i++ ){
            window.draw(arr[i]->getSprite());
        }
        if(!win){
            stopwatch.setText();
            window.draw(stopwatch.getText());
        }
        window.display();
    }
    recover();
    std::system("pause");
    return 0;
}

void init(){
    chess.loadFromFile("../res/chess.png");
    board.loadFromFile("../res/background.png");
    font.loadFromFile("../res/arial.ttf");
    int cnt = 0;
    arr[cnt++] = new King(4,0,1,chess,sf::IntRect(62,0,62,62));
    arr[cnt++] = new Queen(3,0,1,chess,sf::IntRect(62,62,62,62));
    arr[cnt++] = new Queen(5,0,1,chess,sf::IntRect(62, 62, 62, 62));
    arr[cnt++] = new Bishop(2,0,1,chess,sf::IntRect(62,2 * 62,62,62));
    arr[cnt++] = new Bishop(6, 0, 1, chess, sf::IntRect(62, 2 * 62, 62, 62));
    arr[cnt++] = new Knight(1,0,1,chess,sf::IntRect(62,62 * 4,62,62));
    arr[cnt++] = new Knight(7, 0, 1, chess, sf::IntRect(62, 62 * 4, 62, 62));
    arr[cnt++] = new Root(0,0,1,chess,sf::IntRect(62,3 * 62,62,62));
    arr[cnt++] = new Root(8, 0, 1, chess, sf::IntRect(62, 3 * 62, 62, 62));
    arr[cnt++] = new Cannon(1,2,1,chess,sf::IntRect(62,5 * 62,62,62));
    arr[cnt++] = new Cannon(7, 2, 1, chess, sf::IntRect(62, 5 * 62, 62, 62));
    for(int i = 0 ; i <= 8 ; i += 2){
        arr[cnt++] = new Pawn(i,3,1,chess,sf::IntRect(62,6 * 62,62,62));
    }
    arr[cnt++] = new King(4, 9, -1, chess, sf::IntRect(0, 0, 62, 62));
    arr[cnt++] = new Queen(3, 9, -1, chess, sf::IntRect(0, 62, 62, 62));
    arr[cnt++] = new Queen(5, 9, -1, chess, sf::IntRect(0, 62, 62, 62));
    arr[cnt++] = new Bishop(2, 9, -1, chess, sf::IntRect(0, 2 * 62, 62, 62));
    arr[cnt++] = new Bishop(6, 9, -1, chess, sf::IntRect(0, 2 * 62, 62, 62));
    arr[cnt++] = new Knight(1, 9, -1, chess, sf::IntRect(0, 62 * 4, 62, 62));
    arr[cnt++] = new Knight(7, 9, -1, chess, sf::IntRect(0, 62 * 4, 62, 62));
    arr[cnt++] = new Root(0, 9, -1, chess, sf::IntRect(0, 3 * 62, 62, 62));
    arr[cnt++] = new Root(8, 9, -1, chess, sf::IntRect(0, 3 * 62, 62, 62));
    arr[cnt++] = new Cannon(1, 7, -1, chess, sf::IntRect(0, 5 * 62, 62, 62));
    arr[cnt++] = new Cannon(7, 7, -1, chess, sf::IntRect(0, 5 * 62, 62, 62));
    for (int i = 0; i <= 8; i += 2){
        arr[cnt++] = new Pawn(i, 6, -1, chess, sf::IntRect(0, 6 * 62, 62, 62));
    }
}

void recover(){
    for(int i = 0 ; i < 32 ; i++ ){
        delete arr[i];
    }
}