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
    virtual void setup(float x, float y, int _grid = 10){
        grid = _grid;
        current.x = 6;
        current.y = 16;
        current.type = ofRandom(1, 7);
        current.rotate = 1;
        
        //キー入力関連
        inputLeft = false;
        inputRight = false;
        inputUp = false;
        inputDown = false;
        inputRotate = false;
        
        isNew = false;
        
        setField();
        setColor();
    };
    
    virtual void update(){
        
        inputProc();
        
        if(ofGetFrameNum() % 5 == 0) {
        cout << "------------update log start--------------" << endl;
        deletePiece(current);
        current.y++;
        if(!putPiece(current)) {
            current.y--;
            putPiece(current);
            
            deleteLine();
            
            newPiece();
            if(!putPiece(current)) {
                cout << "game over" << endl;
            }
        }
        cout << "------------update log finish--------------" << endl;
        }
    };
    
    virtual void draw(){
        for(int x=0; x < FIELD_WIDTH; x++) {
            for(int y=0; y < FIELD_HEIGHT; y++) {
                if(field[x][y] != 0) {
                    ofSetColor(colors[field[x][y]]);
                    drawPiece(x, y);
                    if(field[x][y] == 9) {
                        wallBlock(x, y);
                    }
                }else {
                    //emptyBlock(x, y);
                }
                //drawBlockNum(x, y);
            }
        }
    };
    
    //他のupdate処理とクロックをあわせるために、イベントハンドラ内で直接処理をしない
    void keyInput(int key) {
        switch(key) {
            case OF_KEY_LEFT:
                inputLeft = true;
                break;
            case OF_KEY_RIGHT:
                inputRight = true;
                break;
            case OF_KEY_UP:
                inputUp = true;
                break;
            case OF_KEY_DOWN:
                inputDown = true;
                break;
            case ' ':
                inputRotate = true;
                break;
            default:
                break;
        }
    }
    
    /************ Each block draw functions ************/
    virtual void drawBlockNum(int x, int y) {
        ofSetColor(100, 100, 100);
        ofDrawBitmapString(ofToString(field[x][y]), x * grid +5 , y * grid + 15);
    }
    
    virtual void drawPiece(int x, int y) {
        ofFill();
        ofRect(x * grid, y * grid, grid, grid);
    }
    
    virtual void emptyBlock(int x, int y) {
        ofNoFill();
        float xColor = ofMap(x, 0, FIELD_WIDTH, 0, 255);
        float yColor = ofMap(y, 0, FIELD_HEIGHT, 0, 255);
        ofSetColor(xColor, yColor, 255);
        ofRect(x * grid, y * grid, grid, grid);
    }
    
    virtual void wallBlock(int x, int y) {
        ofFill();
        float xColor = ofMap(x, 0, FIELD_WIDTH, 0, 255);
        float yColor = ofMap(y, 0, FIELD_HEIGHT, 0, 255);
        ofSetColor(xColor, yColor, 255);
        ofRect(x * grid, y * grid, grid, grid);
    }

protected:
    /*
     * solved 壁に当たったとき、消えないように処理をする。
     * solvde ピースの横方向へのあたり判定を見直す 原因：横方向へのあたり判定を確認せずに横方向へピースが置かれてしまう問題
     */
    void inputProc() {
        STATUS n = current;
        
        if(inputLeft) {
            n.x--;
            inputLeft = false;
            cout << "------------push LEFT KEY--------------" << endl;
        }
        
        if(inputRight) {
            n.x++;
            inputRight = false;
            cout << "------------push RIGHT KEY--------------" << endl;
        }
        if(inputUp) {
            inputUp = false;
        }
        
        if(inputDown) {
            inputDown = false;
        }
        
        if(inputRotate) {
            n.rotate++;
            inputRotate = false;
            cout << "-----pushed rotated----" << endl;
        }
        
        if(n.x != current.x || n.y != current.y || n.rotate != current.rotate) {
            deletePiece(current);
            if(putPiece(n)) {
                current = n;
            } else {
                putPiece(current);
            }
        }
        
    }
    
    void deletePiece(STATUS s) {
        
        field[s.x][s.y] = 0;
        
        for(int i = 0; i < 3; i++) {
            int dx = piece[s.type].p[i].x;
            int dy = piece[s.type].p[i].y;
            int r = s.rotate % piece[s.type].rotate;
            for(int j = 0; j < r; j++) {
                int nx = dx, ny = dy;
                dx = ny; dy = -nx;
            }
            field[s.x + dx][s.y + dy] = 0;
            
            cout << "deleted : " << ofToString(s.x + dx) << " : " << ofToString(s.y + dy) << endl;
        }
        
        return true;
        cout << "------deleted------" << endl;
    }
    
    /* TODO
     * solved 中途半端に for分がまわってしまって、中途半端にブロックが置かれてしまう問題を解決する
     */
    bool putPiece(STATUS s, bool action = false) {
        
        if(field[s.x][s.y] != 0) {
            return false;
        }
        
        //意味不明
        if(action) {
            field[s.x][s.y] = s.type;
        }
        
        for(int i = 0; i < 3; i++) {
            //各ブロックのx, yの位置をdx, dy に代入
            int dx = piece[s.type].p[i].x;
            int dy = piece[s.type].p[i].y;
            
            //回転数をr に代入
            int r = s.rotate % piece[s.type].rotate;
            
            //回転数分のブロックの位置を計算
            for(int j = 0; j < r; j++) {
                int nx = dx, ny = dy;
                dx = ny; dy = -nx;
            }
            
            if(field[s.x + dx][s.y + dy] != 0) {
                return false;
            }
            
            if(action) {
                field[s.x + dx][s.y + dy] = s.type;
                cout << "puted : " << ofToString(s.x + dx) << " : " << ofToString(s.y + dy) << endl;
            }
        }
        if(!action) {
            putPiece(s, true);
        }
        return true;
        
        cout << "--------puted--------" << endl;

    }
    
    void newPiece() {
        pCurrent = current;
        current.x = 6;
        current.y = 5;
        current.type = ofRandom(1, 7);
        current.rotate = 1;
        cout << "------created------" << endl;
    }
    
    void deleteLine() {
        //キャンパス画面を走査する
        for(int y = 23; y > 5; y--) {
            bool flag = true;
            for(int x = 1;x <= 10; x++) {
                if(field[x][y] == 0) {
                    flag = false;
                }
            }
            
            if(flag) {
                for(int j = y; j > 5; j--) {
                    for(int i = 1; i <= 10; i++) {
                        field[i][j] = field[i][j - 1];
                    }
                }
                y++;
            }
        }
    }
    
    void setField(){
        ofNoFill();
        for(int x=0; x < FIELD_WIDTH; x++) {
            for(int y=0; y < FIELD_HEIGHT; y++) {
                if(x == 0 || x == 11 || y == 24 || y < 4) {
                    field[x][y] = 9;
                } else {
                    field[x][y] = 0;
                }
            }
        }
    };
    
    void setColor() {
        ofColor black(0, 0, 0), red(255,0,0), orange(255,128,0), yellow(255,255,0), green(0,255,0), cyan(0,255,255), blue(0,0,255), white(255,255,255);
        colors.push_back(black);
        colors.push_back(red);
        colors.push_back(orange);
        colors.push_back(yellow);
        colors.push_back(green);
        colors.push_back(cyan);
        colors.push_back(blue);
        colors.push_back(white);
    }
protected:
    int field[12][25]; //実際のステージの幅は、10x20
    int grid;
    STATUS current, pCurrent;
    bool isNew;
    bool inputLeft, inputRight, inputUp, inputDown, inputRotate;;
    
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

