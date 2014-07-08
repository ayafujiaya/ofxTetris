#include "ofxTetris.h"

void ofxTetris::setup(float x, float y, int _grid, int _state) {
    grid = _grid;
    tetrisX = x;
    tetrisY = y;
    newPiece();
    
    //キー入力関連
    inputLeft = false;
    inputRight = false;
    inputUp = false;
    inputDown = false;
    inputRotate = false;
    isSet = false;
    
    state = _state;
    
    //フォント読み込み
    font.loadFont("Krungthep.ttf", 32);
    
    setColor();
    
}

void ofxTetris::update() {
    inputProc();
    
    switch (state) {
        case 0:
            //Start Screen update Function
            break;
        case 1:
            if(!isSet) { setField(); isSet = true; }
            basicProc();
            break;
        case 9:
            //GameOver update Function
            break;
        default:
            break;
    }
}

void ofxTetris::draw(){
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
    
    if (state == 9) {
        ofSetColor(0, 0, 0, 180);
        ofRect(0, 0, FIELD_WIDTH * grid, FIELD_HEIGHT * grid);
        ofSetColor(255, 0, 0);
        font.drawString("GAME OVER", 50, 420);
        
    }
    
    if(state == 0) {
        ofSetColor(0, 0, 0);
        ofRect(0, 0, FIELD_WIDTH * grid, FIELD_HEIGHT * grid);
        ofSetColor(255, 128, 128);
        font.drawString("TETRIS", 110, 380);
        //cout << ofToString(ofGetMouseX()) << " : " << ofToString(ofGetMouseY()) << endl;
    }
};

void ofxTetris::setField(){
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

void ofxTetris::basicProc() {
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
                state = 9;
            }
        }
        cout << "------------update log finish--------------" << endl;
    }
}

bool ofxTetris::putPiece(STATUS s, bool action) {
    
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

void ofxTetris::deleteLine() {
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

void ofxTetris::newPiece() {
    current.x = 6;
    current.y = 5;
    current.type = ofRandom(1, 7);
    current.rotate = 1;
    cout << "------created------" << endl;
}

void ofxTetris::deletePiece(STATUS s) {
    
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

void ofxTetris::setColor() {
    ofColor black(0, 0, 0), red(255,0,0), orange(255,128,0), yellow(255,255,0), green(0,255,0), cyan(0,255,255), blue(0,0,255), white(255,255,255);
    colors.push_back(red);
    colors.push_back(black);
    colors.push_back(white);
    colors.push_back(orange);
    colors.push_back(yellow);
    colors.push_back(green);
    colors.push_back(cyan);
    colors.push_back(blue);
    
}

void ofxTetris::gameOver() {
    cout << "Game Over" << endl;
}

void ofxTetris::inputProc() {
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


void ofxTetris::keyInput(int key) {
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
        case 'r':
            state = 1;
            setup(0, 0, grid, 1);
            break;
        case OF_KEY_RETURN:
            state = 1;
            setup(0, 0, grid, 1);
            break;
        default:
            break;
    }
}