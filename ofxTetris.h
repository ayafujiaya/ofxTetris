#include "ofMain.h"
#define FIELD_WIDTH 12
#define FIELD_HEIGHT 25
/*
 * テトリスの実装をクラス化して、縦横自在のテトリスを作る。
 * solved 二列同士に消そうとすると一列しか消えないバグ。
 * 次のテトリスを前の現在のテトリス作成時に決める
 * game over 画面を実装する
 * game start 画面を実装する
 */

struct POSITION {
    int x;
    int y;
};

struct PIECE {
    int rotate;
    POSITION p[4];
};

struct STATUS {
    int x;
    int y;
    int type;
    int rotate;
};

class ofxTetris {
    
public:
    
    /********** basic function *******/
    virtual void setup(float x, float y, int _grid = 10, int _state = 0);
    virtual void update();
    virtual void draw();
    virtual void keyInput(int key);
    
    /********** Tetris Logic *********/
    virtual void setField();
    virtual void basicProc();
    virtual bool putPiece(STATUS s, bool action = false);
    virtual void deleteLine();
    virtual void inputProc();
    virtual void deletePiece(STATUS s);
    virtual void newPiece();
    
    /************ Each block draw functions ************/
    virtual void drawBlockNum(int x, int y) = 0;
    virtual void drawPiece(int x, int y)  = 0;
    virtual void emptyBlock(int x, int y) = 0;
    virtual void wallBlock(int x, int y) = 0;
    virtual void setColor();
    
    /************ Scene Select *************/
    virtual void gameOver();
    
    int field[12][25];
    int grid;
    float tetrisX, tetrisY;
    
    STATUS current;
    bool inputLeft, inputRight, inputUp, inputDown, inputRotate, isContinue, isSet;
    ofTrueTypeFont font;
    int state;
    
    PIECE piece[8] = {
        {1, {{0,  0},{0, 0}, {0 ,0}}},
        {2, {{0, -1},{0, 1}, {0 ,2}}},
        {4, {{0, -1},{0, 1}, {1 ,1}}},
        {4, {{0, -1},{0, 1}, {-1,1}}},
        {2, {{0, -1},{1, 0}, {1 ,1}}},
        {2, {{0, -1},{-1,0}, {-1,1}}},
        {1, {{0,  1},{1, 0}, {1 ,1}}},
        {4, {{0, -1},{1, 0}, {-1 ,0}}},
    };
    vector<ofColor> colors;
};

