#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <iostream>

void step();
int countAliveNeighbours(int x, int y);
void printBoard();
int checkOutOfBound(int x, int y);

#define HEIGHT 8
#define WIDTH 8

std::array<std::array<int, WIDTH>, HEIGHT> board;

class GameOfLife : public olc::PixelGameEngine
{
public:
    GameOfLife()
    {
        sAppName = "Game Of Life";
    }

private:
    float fTargetFrameTime = 1.0f / 2.0f;    
    float fAccumulatedTime = 0.0f;

public:
    bool OnUserCreate() override
    {
        return true;
    }
    bool OnUserUpdate(float fElapsedTime) override
    {
        if(GetMouse(olc::Mouse::LEFT).bPressed){
            olc::vi2d mousePos = GetMousePos();
            int mousePosX = mousePos.x/55;
            int mousePosY = mousePos.y/55;
            if(board[mousePosY][mousePosX] == 1){
                board[mousePosY][mousePosX] = 0;
            }
            else{
                board[mousePosY][mousePosX] = 1;
            }
            printBoard();   
        }
        fAccumulatedTime += fElapsedTime;
        if(fAccumulatedTime >= fTargetFrameTime){
            fAccumulatedTime -= fTargetFrameTime;
            fElapsedTime = fTargetFrameTime;
        }
        else{
            return true;
        }

        step();
        printBoard();
        return true;
    }
};

#define SQUARE_SIZE 55
GameOfLife gof;

void printBoard(){
    int step_size_width = gof.ScreenWidth()/WIDTH;
    int step_size_height = gof.ScreenHeight()/HEIGHT;
    int array_y = 0;
    for(int y = 0; y < gof.ScreenHeight(); y+=step_size_height){
        int array_x = 0;
        for(int x = 0; x < gof.ScreenWidth(); x+= step_size_width){
            olc::Pixel px;
            if(board[array_y][array_x] == 1){
                px = olc::Pixel(olc::BLACK);
            }
            else{
                px = olc::Pixel(olc::WHITE);
            }
            gof.FillRect(olc::vi2d(x,y),olc::vi2d(x+SQUARE_SIZE,y+SQUARE_SIZE), px);
            array_x++;
        }
        array_y++;
    }
    for(int y = 0; y <= HEIGHT; y ++){
        gof.DrawLine(olc::vi2d(0,y*SQUARE_SIZE), olc::vi2d(gof.ScreenWidth(),y*SQUARE_SIZE), olc::GREY);
        //gof.fillRect(0,y*SQUARE_SIZE, HEIGHT*SQUARE_SIZE, 5);
        for(int x = 0; x <= WIDTH; x++){
            gof.DrawLine(olc::vi2d(x*SQUARE_SIZE,0), olc::vi2d(x*SQUARE_SIZE,gof.ScreenHeight()), olc::GREY);
        }
    }
}

int checkOutOfBound(int x, int y){
    if(x < 0 || x >= WIDTH){
        return 0;
    }
    if(y < 0 || y >= HEIGHT){
        return 0;
    }
    else{
        return board[y][x];
    }
}

int countAliveNeighbours(int x, int y){

    int count = 0;
    count += checkOutOfBound(x-1, y-1);
    count += checkOutOfBound(x, y-1);
    count += checkOutOfBound(x-1, y);
    count += checkOutOfBound(x+1, y);
    count += checkOutOfBound(x, y+1);
    count += checkOutOfBound(x+1, y+1);
    count += checkOutOfBound(x+1, y-1);
    count += checkOutOfBound(x-1, y+1);
    return count;
}

void step(){

    std::array<std::array<int, WIDTH>, HEIGHT> temporaryBuffer;
    temporaryBuffer.fill({0});
    for(int y = 0; y < HEIGHT; ++y) {
        for(int x = 0; x < WIDTH; ++x){
            int aliveNeighbours = countAliveNeighbours(x,y);
            if(board[y][x] == 1){
                if(aliveNeighbours < 2){
                temporaryBuffer[y][x] = 0;
            }else if(aliveNeighbours == 2 || aliveNeighbours == 3){
                temporaryBuffer[y][x] = 1;
            }
            else if(aliveNeighbours > 3){
                temporaryBuffer[y][x] = 0;
            }
        }else{
                if(aliveNeighbours == 3){
                   temporaryBuffer[y][x] = 1;
                }
            }
        }
    }
    for(int y = 0; y < HEIGHT; ++y){
       for(int x = 0; x < WIDTH; ++x){
            board[y][x] = temporaryBuffer[y][x];
        }
    }
}

int main() {
    board.fill({0});
    board[4][2] = 1;
    board[4][3] = 1;
    board[4][4] = 1;
    board[3][4] = 1;
    if(gof.Construct(440,440,1,1))
        gof.Start();
    return 0;
}
