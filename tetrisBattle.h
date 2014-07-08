#include "ofxTetris.h"

class tetrisBattle : ofxTetris {
public:
    void setup(float x, float y, int _grid = 20, int _state = 0) {
        grid = _grid;
        tetrisX = x;
        tetrisY = y;
        currents.clear();
        newPieces();
        
        //キー入力関連
        inputLeft = false;
        inputRight = false;
        inputUp = false;
        inputDown = false;
        inputRotate = false;
        isSet = false;
        
        state = 1; //_state;
        
        //フォント読み込み
        font.loadFont("Krungthep.ttf", 32);
        
        setColor();
    }
    
    void update() {
        inputProc();
        
        switch (state) {
            case 0:
                //Start Screen update Function
                break;
            case 1:
                if(!isSet) { setField(); isSet = true; }
                if(!debugStop) basicProc();
                break;
            case 9:
                //GameOver update Function
                break;
            default:
                break;
        }
    }
    
    void draw(){
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
        
        int debuglogSize = debuglog.size() - 50;
        if(debuglogSize < 0) debuglogSize = 0;
        for(int i = debuglogSize; i < debuglog.size(); i++) {
            float debugheight = (i - debuglogSize) * 15;
            ofSetColor(255, 255, 255);
            ofDrawBitmapString(debuglog[i], ofGetWidth() - 580, debugheight);
        }
    };
    
    void basicProc() {
        if(ofGetFrameNum() % 5 == 0) {
            cout << "------------update for line STARTED!!--------------" << endl;
            debuglog.push_back("------------update for line STARTED!!--------------");
            
            for (int i = 0; i < currents.size(); i++) {
                
                cout << "--------- Player : " << ofToString(i) << " start-------------" << endl;
                debuglog.push_back("Player : " + ofToString(i) + " start");
                deletePiece(currents[i]);
                currents[i].y++;
                //cout << "---------!putPiece(c) is " << ofToString(!putPiece(c)) << "--------------" << endl;
                cout << "---------first !putpiece---------" <<endl;
                debuglog.push_back("first !putpiece");
                //
                if(!putPiece(currents[i], i)) {
                    
                    currents[i].y--;
                    cout << "---------second !putpiece---------" <<endl;
                    debuglog.push_back("second !putpiece");
                    putPiece(currents[i], i);
                    cout << "------------Processed second putPiece--------------" << endl;
                    debuglog.push_back("Processed second putPiece");
                    deleteLine();
                    
                    newPiece(i);
                    puted(i);
                    if(!putPiece(currents[i], i)) {
                        state = 9;
                    }
                }
                cout << "--------- Player : " << ofToString(i) << " finish-------------" << endl;
                debuglog.push_back("Player : " + ofToString(i) + " finish");
                
            }
            cout << "------------update for line Finished!!--------------" << endl;
            debuglog.push_back("update for line Finished!!");
        }
    }
    
    bool putPiece(STATUS s, int i, bool action = false) {
        
        int type;
        int del;
        if(i == 0) { type = 1; del = 2; }
        if(i == 1) { type = 2; del = 1; }
        
        if(field[s.x][s.y] == 6 || field[s.x][s.y] == 7 || field[s.x][s.y] == 9) {
            return false;
            string debug = "first rerutn false(cant puted here) : " + ofToString(s.x) + ", " + ofToString(s.y) + "i : " + ofToString(i) + ", num :" + ofToString(field[s.x][s.y]);
            debuglog.push_back(debug);
            cout << debug  << endl;
        } else {
            if(action) {
                field[s.x][s.y] = type;
                string debug = "first ex puted : " + ofToString(s.x) + ", " + ofToString(s.y);
                debuglog.push_back(debug);
            }
        }

        if(action) {
            field[s.x][s.y] = type;
            string debug = "first action puted : " + ofToString(s.x) + ", " + ofToString(s.y);
            debuglog.push_back(debug);
            cout << "first action" << endl;
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
            
            if(field[s.x + dx][s.y + dy] == 6 || field[s.x + dx][s.y + dy] == 7 || field[s.x + dx][s.y + dy] == 9) {
                string debug = "second rerutn false(cant puted here) : " + ofToString(s.x + dx) + ", " + ofToString(s.y + dy) + ", i : " + ofToString(i) + ", num : " + ofToString(field[s.x + dx][s.y + dx]);
                debuglog.push_back(debug);
                cout << debug << endl;
                return false;
                cout << debug  << endl;
            } else {
                if(action) {
                    field[s.x + dx][s.y + dy] = type;
                    string debug = "second ex puted : " + ofToString(s.x + dx) + ", " + ofToString(s.y + dy);
                    debuglog.push_back(debug);
                }
            }
            
            if(action) {
                field[s.x + dx][s.y + dy] = type;
                cout << "second action puted : " << ofToString(s.x + dx) << " : " << ofToString(s.y + dy) << endl;
                string debug = "second action puted : " + ofToString(s.x + dx) + ", " + ofToString(s.y + dy);
                debuglog.push_back(debug);
            }
        }
        if(!action) {
            cout << "------- return to myself --------" << endl;
            putPiece(s, i, true);
        }
        return true;
        
        cout << "--------puted--------" << endl;
    }
    
    void puted(int i) {
        //キャンパス画面を走査する
        int pGrid;
        int grid;
        if(i == 0) { pGrid = 1; grid = 6; }
        if(i == 1) { pGrid = 2; grid = 7; }
        
        for(int y = 23; y > 5; y--) {
            bool flag = true;
            for(int x = 1;x <= 10; x++) {
                if(field[x][y] == pGrid) {
                    field[x][y] = grid;
                }
            }
        }
    }
    
    void newPieces() {
        for (int i=0; i<2; i++) {
            STATUS c;
            if(i == 0) {c.x = 3;} else {c.x = 7;}
            c.y = 5;
            c.type = ofRandom(1, 7);
            c.rotate = 1;
            currents.push_back(c);
        }
        cout << "------created------" << endl;
    }
    
    void newPiece(int i) {
        STATUS c;
        if(i == 0) {c.x = 3;} else {c.x = 7;}
        c.y = 5;
        c.type = ofRandom(1, 7);
        c.rotate = 1;
        currents[i] = c;
        cout << "------generate single pieace------" << endl;
    }
    
    void inputProc() {
        
        STATUS n2 = currents[0];
        if(inputLeft) {
            n2.x--;
            inputLeft = false;
            cout << "------------push LEFT KEY--------------" << endl;
        }
        
        if(inputRight) {
            n2.x++;
            inputRight = false;
            cout << "------------push RIGHT KEY--------------" << endl;
        }
        if(inputRotate) {
            n2.rotate++;
            inputRotate = false;
            cout << "-----pushed rotated----" << endl;
        }
        
        if(n2.x != currents[0].x || n2.y != currents[0].y || n2.rotate != currents[0].rotate) {
            deletePiece(currents[0]);
            if(putPiece(n2, 0)) {
                currents[0] = n2;
            } else {
                putPiece(currents[0], 0);
            }
        }
        
        
        STATUS n = currents[1];
        //2P
        if(input2pLeft) {
            n.x--;
            input2pLeft = false;
            cout << "------------push LEFT KEY--------------" << endl;
        }
        if(input2pRight) {
            n.x++;
            input2pRight = false;
            cout << "------------push RIGHT KEY--------------" << endl;
        }
        if(input2pRotate) {
            n.rotate++;
            input2pRotate = false;
            cout << "-----pushed rotated----" << endl;
        }
        
        if(n.x != currents[1].x || n.y != currents[1].y || n.rotate != currents[1].rotate) {
            deletePiece(currents[1]);
            if(putPiece(n, 1)) {
                currents[1] = n;
            } else {
                putPiece(currents[1], 1);
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
            string debug = "deleted : " + ofToString(s.x + dx) + " : " + ofToString(s.y + dy);
            debuglog.push_back(debug);
        }
        cout << "------deleted------" << endl;
        return true;
    }
    
    /************ Each block draw functions ************/
    virtual void drawBlockNum(int x, int y) {
        ofSetColor(100, 100, 100);
        if(gridDisp) {
            string gridNum = ofToString(x) + "," + ofToString(y);
            ofDrawBitmapString(gridNum, x * grid +3 , y * grid + 15);
        } else {
            ofDrawBitmapString(ofToString(field[x][y]), x * grid +5 , y * grid + 15);
        }
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
    
    void keyInput(int key) {
        switch(key) {
            case 'z':
                inputLeft = true;
                break;
            case 'c':
                inputRight = true;
                break;
            case 'x':
                inputRotate = true;
                break;
                
                //2P
            case '.':
                input2pLeft = true;
                break;
            case '_':
                input2pRight = true;
                break;
            case '/':
                input2pRotate = true;
                break;
                
                //Scine Change
            case 'r':
                state = 1;
                setup(0, 0, grid, 1);
                break;
            case OF_KEY_RETURN:
                state = 1;
                setup(0, 0, grid, 1);
                break;
            case 'd':
                if(debugStop) {
                    debugStop = false;
                } else {
                    debugStop = true;
                }
            case 'g':
                if(gridDisp) {
                    gridDisp = false;
                } else {
                    gridDisp = true;
                }
                
            default:
                break;
        }
    }
    
protected:
    vector<STATUS> currents;
    vector<string> debuglog;
    bool input2pLeft, input2pRight, input2pUp, input2pDown, input2pRotate;
    bool debugStop, gridDisp;
};
