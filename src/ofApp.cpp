#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup() {
	ofSetWindowTitle("Tic Tac Toe");
	
    restart();

	cam.setup(RES_WIDTH, RES_HEIGHT);
	contourFinderSetup();
	buf += "\nGame starts in 3 seconds";
	pauseForSecs(3);
	buf = "";
}

void ofApp::contourFinderSetup() {
	contourFinder.setMinAreaRadius(10);
	contourFinder.setMaxAreaRadius(150);
	gui.setup();
	gui.add(threshold.set("Threshold", threshold, 0, 255));
	gui.add(trackHs.set("Track Hue/Saturation", false));
}

//--------------------------------------------------------------
void ofApp::update() {

	cam.update();
	if(cam.isFrameNew()) {
        if(!gameOver){
            
            if(checkHowLongInField()) {
                if(setSymbolToField()){
                    changePlayer();
                }
            }
            setPlayerColor();
            callContourFinder();
            setCenterCoordinatesOfFoundContour();
        }else{
            pauseForSecs(WAIT_TIMER);
            restart();
        }
        checkGameOver();
	}
}

void ofApp::restart(){
    buf = "";
    randomPlayerStart();
    gameOver = false;
    startTime = 0;
    currentField = -1;
    fullFieldCounter = 0;
    resetGameFieldsArray();
}

void ofApp::resetGameFieldsArray() {
    for(int i= 0; i < 9; i++){
        gameTiles[i] = ' ';
    }
}

void ofApp::randomPlayerStart() {
	zufall = rand() % 2;
	if (zufall == 0) {
		selectedPlayer = 1;
	}
	else {
		selectedPlayer = 2;
	}
}

void ofApp::pauseForSecs(int limit){
    int start = (int)time(NULL);
    while(true){
        int end = (int)time(NULL);
        if(end - start >= limit){
            break;
        }
    }
}

void ofApp::setPlayerColor()
{
	if (selectedPlayer == 1) {
		colorizeTarget(190, 35, 54);
		threshold = 53;
	}
	else if(selectedPlayer == 2) {
		colorizeTarget(117, 252, 85);
		threshold = 70;
	}
}

bool ofApp::setSymbolToField() {
    
    int field = findOutFieldPosition();
    
    if(gameTiles[field] == ' '){
        if(gameTiles[field] != 'f'){
            if (selectedPlayer == 1) {
                gameTiles[field] = 'X';
                playSound("loggedInField.mp3", 0.5);
				fullFieldCounter++;
                return true;
            }
            else if(selectedPlayer == 2) {
                gameTiles[field] = 'O';
                playSound("loggedInField.mp3", 0.5);
				fullFieldCounter++;
                return true;
            }
        }
        buf = field;
    }
    return false;
}

int ofApp::findOutFieldPosition(){
    int field = 0;
    int x = RES_WIDTH - centerOfTrackedX;
    int y = centerOfTrackedY;
    
    float offsetX = 0;
    float offsetY = 0;

    for (int i = 0; i < 3; i++) {
        offsetY = i * TILE_HEIGHT;

        for (int j = 0; j < 3; j++) {
            offsetX = j * TILE_WIDTH;
            
            bool a = x > offsetX;
            bool b = x < (offsetX + TILE_WIDTH);
            bool c = y > offsetY;
            bool d = y < (offsetY + TILE_WIDTH);
            
            if((a && b) && (c && d)){
                return field;
            }
            field++;
        }
    }
    return 'f';
}

void ofApp::changePlayer() {
	if (selectedPlayer == 1) {
		selectedPlayer = 2;
	}
	else {
		selectedPlayer = 1;
	}
}

bool ofApp::checkHowLongInField() {
	
    endTime = (int)time(NULL);
    int tempField = findOutFieldPosition();
    
    if(currentField != tempField){
        currentField = tempField;
        startTime = 0;
    }
    
    if(startTime == 0){
        startTime = (int)time(NULL);
    }else{
        int counter = endTime - startTime;
        pointerCountdown = to_string(WAIT_TIMER-counter);
        
        if(counter >= WAIT_TIMER){
            startTime = 0;
            return true;
        }
    }
	return false;
}

void ofApp::checkGameOver()
{
	if (fullFieldCounter == 9) {
		buf = "Unentschieden";
		gameOver = true;
	}
	if (checkWin('O')){
        buf = "O gewinnt";
        gameOver = true;
	}else if(checkWin('X')){
        buf = "X gewinnt";
        gameOver = true;
	}
    if(gameOver == true){
        playSound("win.mp3", 0.5);
        buf += "\nRestart in " + to_string(WAIT_TIMER) + " Sekunden";
    }
}

void ofApp::playSound(string soundFile, double volume) {
    win.load(soundFile);
    win.setVolume(volume);
    win.play();
}

void ofApp::colorizeTarget(int r, int g, int b) {
	targetColor.r = r;
	targetColor.g = g;
	targetColor.b = b;
}

void ofApp::callContourFinder() {
	ofSetColor(targetColor);
	contourFinder.setTargetColor(targetColor, trackHs ? TRACK_COLOR_HS : TRACK_COLOR_RGB);
	contourFinder.setThreshold(threshold);
	contourFinder.findContours(cam);
}

void ofApp::setCenterCoordinatesOfFoundContour() {
	for (int i = 0; i < contourFinder.size(); i++) {
		ofVec2f center = toOf(contourFinder.getCenter(i));

		centerOfTrackedX = (int)center.x;
		centerOfTrackedY = (int)center.y;
	}
}

bool ofApp::checkWin(char playerSymbol)
{
	if (gameTiles[0] == playerSymbol && gameTiles[1] == playerSymbol && gameTiles[2] == playerSymbol) return true;
	if (gameTiles[3] == playerSymbol && gameTiles[4] == playerSymbol && gameTiles[5] == playerSymbol) return true;
	if (gameTiles[6] == playerSymbol && gameTiles[7] == playerSymbol && gameTiles[8] == playerSymbol) return true;
	if (gameTiles[0] == playerSymbol && gameTiles[3] == playerSymbol && gameTiles[6] == playerSymbol) return true;
	if (gameTiles[1] == playerSymbol && gameTiles[4] == playerSymbol && gameTiles[7] == playerSymbol) return true;
	if (gameTiles[2] == playerSymbol && gameTiles[5] == playerSymbol && gameTiles[8] == playerSymbol) return true;
	if (gameTiles[0] == playerSymbol && gameTiles[4] == playerSymbol && gameTiles[8] == playerSymbol) return true;
	if (gameTiles[2] == playerSymbol && gameTiles[4] == playerSymbol && gameTiles[6] == playerSymbol) return true;

	return false;
}

//--------------------------------------------------------------
void ofApp::draw() {
	//camImage();
	gameGui();
	//showContourControlls();
	contourFinder.draw();
	drawCursor();
	showMessages();
}

void ofApp::showMessages()
{
    if(buf != ""){
        ofDrawBitmapStringHighlight(buf, 10, 20);
    }
}

void ofApp::showContourControlls() {
	gui.draw();
	ofTranslate(8, 75);
	ofFill();
	ofSetColor(0);
	ofDrawRectangle(-3, -3, 64 + 6, 64 + 6);
	ofSetColor(targetColor);
	ofDrawRectangle(0, 0, 64, 64);
}

void ofApp::camImage() {
	ofSetColor(255);
	cam.draw(0, 0);
}

void ofApp::gameGui() {
	ofSetLineWidth(2);
	ofBackgroundGradient(255, 255);
	drawGameTable();
}

void ofApp::drawGameTable() {
	float offsetX = 0;
	float offsetY = 0;
	int squareIndex = 0;

	for (int i = 0; i < 3; i++) {
		offsetY = i * TILE_HEIGHT;

		for (int j = 0; j < 3; j++) {
			offsetX = j * TILE_WIDTH;
			drawSingleTile(offsetX, offsetY);
			drawTileSymbol(squareIndex, offsetX, offsetY);
			squareIndex++;
		}
	}
}

void ofApp::drawSingleTile(float offsetX, float offsetY) {
	ofNoFill();
	ofSetColor(0);
	ofDrawRectangle(offsetX, offsetY, TILE_WIDTH, TILE_HEIGHT);
}

void ofApp::drawCursor() {
	glm::vec3 mouseIn(RES_WIDTH - centerOfTrackedX, centerOfTrackedY, 0);
	ofFill();
	if (selectedPlayer == 1) {
		ofSetColor(190, 35, 54);
	}
	else if (selectedPlayer == 2) {
		ofSetColor(117, 252, 85);
	}
	ofDrawCircle(mouseIn, 20);
    if(pointerCountdown != ""){
        ofSetColor(255);
        ofDrawBitmapString(pointerCountdown, mouseIn);
    }
}

void ofApp::drawTileSymbol(int squareIndex, float offsetX, float offsetY) {
	if (gameTiles[squareIndex] == 'X') {
        drawCross(offsetX, offsetY);
	}
	else if (gameTiles[squareIndex] == 'O') {
        drawCircle(offsetX, offsetY);
	}
}

void ofApp::drawCircle(float offsetX, float offsetY) {
    ofFill();
    ofSetColor(117, 252, 85);
    ofDrawCircle(offsetX+100, offsetY+80, 60);
    ofSetColor(255,255,255);
    ofDrawCircle(offsetX+100, offsetY+80, 50);
}

void ofApp::drawCross(float offsetX, float offsetY) {
    ofFill();
    ofSetColor(190, 35, 54);
    ofDrawRectangle(offsetX + 60, offsetY + 40, 100, 100);
    ofSetColor(255,255,255);
    ofDrawTriangle(offsetX + 65, offsetY + 140, offsetX + 155, offsetY + 140, offsetX + 110, offsetY + 97.5);
    ofDrawTriangle(offsetX + 65, offsetY+40, offsetX + 155, offsetY + 40, offsetX + 110, offsetY+77.5 );
    ofDrawTriangle(offsetX + 60, offsetY+ 45, offsetX +60, offsetY + 135, offsetX + 105, offsetY+87.5 );
    ofDrawTriangle(offsetX + 160, offsetY+ 45, offsetX +160, offsetY + 135, offsetX + 115, offsetY+87.5 );
}
