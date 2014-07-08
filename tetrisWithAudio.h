#include "ofxTetris.h"
#include "ofMain.h"

//ofxTetrisの継承クラス
class tetrisWithAudio : public ofxTetris {
protected:
    ofSoundPlayer putSound, deleteLineSound, bgm, gameOverSound, titleBgm;
    
public:
    virtual void setup(float x, float y, int _grid = 10, int _state = 0){
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
        
        putSound.loadSound("magic3.mp3");
        deleteLineSound.loadSound("magic4.mp3");
        bgm.loadSound("bgm.mp3");
        gameOverSound.loadSound("magic5.mp3");
        titleBgm.loadSound("title.mp3");
        
        
        //フォント読み込み
        font.loadFont("Krungthep.ttf", 32);
        
        setColor();
    };
    
    virtual void update(){
        inputProc();
        
        switch (state) {
            case 0:
                //Start Screen Function
                break;
            case 1:
                if(!isSet) { setField(); isSet = true; bgm.play();}
                basicProc();
                break;
            case 9:
                //GameOver update Function
                break;
            default:
                break;
        }
    };
    
    void basicProc() {
        if(ofGetFrameNum() % 5 == 0) {
            cout << "------------update log start--------------" << endl;
            deletePiece(current);
            current.y++;
            if(!putPiece(current)) {
                putSound.play();
                current.y--;
                putPiece(current);
                
                deleteLine();
                
                newPiece();
                if(!putPiece(current)) {
                    //bgm.stop();
                    gameOverSound.play();
                    state = 9;
                }
            }
            cout << "------------update log finish--------------" << endl;
        }
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
                deleteLineSound.play();
                y++;
            }
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
    
    
};
