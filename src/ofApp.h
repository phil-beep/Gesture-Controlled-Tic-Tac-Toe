#pragma once
#include <iostream>
#include <time.h>
#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include <stdlib.h>

using namespace ofxCv;
using namespace cv;
using namespace std;


#define NUM_MSG_STRINGS 20

const int RES_WIDTH = 640;
const int RES_HEIGHT = 480;
const float TILE_WIDTH = RES_WIDTH / 3;
const float TILE_HEIGHT = RES_HEIGHT / 3;
const int WAIT_TIMER = 4;

class ofApp : public ofBaseApp {
public:
    
    void setup();
    void resetGameFieldsArray();
	void contourFinderSetup();
	void oscSetup();

	void update();
	void setPlayerColor();
	void randomPlayerStart();
    void extracted();
    
    bool setSymbolToField();
    int findOutFieldPosition();
	void changePlayer();
	bool checkHowLongInField();
    void playSound(string fileName, double volume);
    
    void checkGameOver();
	void oscSendReceive();
	void callContourFinder();
	void setCenterCoordinatesOfFoundContour();
	void sendOscInformation();
	void colorizeTarget(int r, int g, int b);
	void hideOldMessages();
	bool checkWin(char c);
    void pauseForSecs(int limit);

	void draw();
	void showMessages();
	void showContourControlls();
	void camImage();
	void gameGui();
	void drawSingleTile(float offsetX, float offsetY);
    void drawCircle(float offsetX, float offsetY);
    void drawCross(float offsetX, float offsetY);
    void drawTileSymbol(int squareIndex, float offsetX, float offsetY);
	void drawGameTable();
	void drawCursor();
    void restart();


private:
	int fullFieldCounter;
	int zufall;
	int selectedPlayer;
    bool gameOver;
	bool myTurn;
	int centerOfTrackedX;
	int centerOfTrackedY;
    
    int currentField;
	int startTime;
	int endTime;
	
	char gameTiles[9];

	ofSoundPlayer placeSound;
	ofSoundPlayer win;

	ofVideoGrabber cam;
	ofxCv::ContourFinder contourFinder;
	ofColor targetColor;

	ofxPanel gui;
	ofParameter<float> threshold;
	ofParameter<bool> trackHs;

	ofTrueTypeFont font;
    string buf;
    string pointerCountdown;
};
