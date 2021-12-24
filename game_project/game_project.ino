#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "LedControl.h"
#include "game_constants.h"
#include "buzzer_notes.h"
#include "player.h"
int xValue = 0;
int yValue = 0;
byte xPosSettings = 0;
byte xPos = 0;
byte yPos = 0;
byte xLastPos = 0;
byte yLastPos = 0;
int matrixBrightness = 2;
int maxLevels = 1;
int buzzerTone = 123;
unsigned long lastMoved = 0;
int moveInterval = 180;
unsigned long  currentMillis = 0;
bool matrixUpdate = true;
bool menuState = true;
bool joyMovedX = false;
bool joyMovedY = false;
bool isVisited = false;
bool joyMovedXSettings = false;
bool greetingMessage = true;
bool inSettings = false;
bool playerTurn = false;
bool isAlive = true;
bool wasChecked = false;
bool isInSetName = false;
bool beginGame = false;
bool nameWasSet = false;
bool hasLost = false;
bool hasWin = false;
bool clearLevelDisplay = false;
volatile bool lockedIn = false;
volatile bool lastSwState = false;
volatile long long lastDebounce = 0;
static int currentLevel = 1;
static int currentScore = 0;
int previousScore = -1;
int previousLevel = 0;
int livesNumber = 5;
int subMenu = 1;
int newScore =  0;
int potValue;
int offset = 3;
int contrastValue = 0;
int brightnessValue = 0;
int matrixBrightnessValue = 0;
int indexAbout = 0;
byte previousLives = 5;
byte indexOfHighscore = 0;
String name[NAMESIZE] = {"", "", "", ""};
char * aboutInfo = "Brainstorming Game.Created by Larisuk.Find me at https://github.com/Larisuk/Matrix-Game-Arduino.";
unsigned long currentElapsedTime = 0;
unsigned long currentGameOverTime = 0;
unsigned long currentTimeHighscore = 0;
unsigned long currentAboutMillis = 0;
unsigned long introMillis = 0;
LedControl lc = LedControl(DINPIN, CLOCKPIN, LOADPIN, 1);
LiquidCrystal lcd(RS, ENABLE, D4, D5, D6, D7);
bool matrix[MATRIXSIZE][MATRIXSIZE] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};
bool playerMatrix[MATRIXSIZE][MATRIXSIZE] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};
byte lettersArray[ALPHABETSIZE] = {
  B1000001, //A
  B1000010, //B
  B1000011, //C
  B1000100, //D
  B1000101, //E
  B1000110, //F
  B1000111, //G
  B1001000, //H
  B1001001, //I
  B1001010, //J
  B1001011, //K
  B1001100, //L
  B1001101, //M
  B1001110, //N
  B1001111, //O
  B1010000, //P
  B1010001, //Q
  B1010010, //R
  B1010011, //S
  B1010100, //T
  B1010101, //U
  B1010110, //V
  B1010111, //W
  B1011000, //X
  B1011001, //Y
  B1011010, //Z
};
byte figures[FIGURESNUMBER][NUMBEROFCOORD][2] = {
  {{4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 4}, {4, 5}, {4, 6}, {4, 7}, {5, 0}, {5, 1}, {5, 2}, {5, 3}},
  {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {1, 6}, {2, 6}, {3, 6}, {4, 6}, {5, 6}, {6, 6}},
  {{0, 1}, {1, 1}, {2, 1}, {3, 1}, {0, 6}, {1, 6}, {2, 6}, {3, 6}, {5, 2}, {5, 3}, {5, 4}, {5, 5}},
  {{0, 1}, {1, 1}, {2, 1}, {3, 1}, {1, 0}, {1, 2}, {1, 3}, {2, 0}, {3, 0}, {0, 0}, {0, 2}, {1, 2}},
  {{7, 3}, {7, 4}, {7, 5}, {6, 2}, {6, 1}, {6, 4}, {6, 3}, {5, 0}, {6, 0}, {7, 0}, {4, 2}, {5, 2}},
  {{7, 3}, {7, 7}, {7, 5}, {6, 6}, {6, 1}, {6, 4}, {6, 3}, {5, 0}, {6, 0}, {7, 0}, {4, 2}, {5, 2}},
  {{4, 2}, {4, 3}, {1, 1}, {1, 0}, {1, 2}, {3, 1}, {3, 2}, {3, 3}, {3, 5}, {4, 5}, {5, 5}, {5, 6}},
  {{3, 1}, {5, 0}, {5, 3}, {5, 7}, {7, 3}, {7, 7}, {0, 0}, {7, 0}, {3, 2}, {3, 5}, {3, 6}, {6, 2}},
  {{2, 2}, {2, 0}, {7, 2}, {7, 5}, {6, 7}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {1, 1}, {1, 2}, {1, 3}},
  {{3, 3}, {3, 2}, {3, 7}, {0, 3}, {0, 4}, {0, 5}, {5, 4}, {5, 2}, {1, 0}, {1, 7}, {1, 6}, {4, 1}},
  {{1, 5}, {1, 3}, {1, 7}, {6, 3}, {6, 4}, {6, 5}, {5, 4}, {5, 2}, {1, 1}, {3, 3}, {3, 6}, {4, 0}},
  {{3, 3}, {1, 4}, {0, 5}, {0, 2}, {6, 1}, {7, 4}, {7, 3}, {2, 0}, {1, 0}, {2, 2}, {4, 2}, {5, 2}}
};

char* menu[MENUSIZE] = {
  "Start game",
  "Highscore",
  "Settings",
  "About"
};
char* settings[SETTINGSSIZE] = {
  "Starting level",
  "Contrast",
  "Matrix light",
  "Brightness",
  "Back"
};
int melody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3,0,NOTE_B3, NOTE_C4};
byte noteDurations[] = {4,8,8,4,4,4,4,4};
Player* highScore[HIGHSCORESIZE] = {
  new Player("", 0), new Player("", 0), new Player("", 0)
};

void setup() {
  lc.shutdown(0, false);
  lc.setIntensity(0, matrixBrightness);
  lcd.begin(16, 2);
  lcd.print(F(">>Welcome to a"));
  lcd.setCursor(0, 1);
  lcd.print(F("nerdy world!<<"));
  lc.clearDisplay(0);
  pinMode(POTPIN, INPUT);
  pinMode(SWPIN, INPUT_PULLUP);
  pinMode(XPIN, INPUT);
  pinMode(YPIN, INPUT);
  pinMode(CLOCKPIN, OUTPUT);
  //pinMode(trigPin, OUTPUT);
  //analogWrite(trigPin, EEPROM.read(0));
  lc.setIntensity(0, EEPROM.read(1));
  analogWrite(BRIGHTPIN, EEPROM.read(2));
  int index = 0;
  for (int i = 3; i <= 15; i += 6) {
    String name = "";
    for (int j = i; j < i + 4; j++) {
      name += (char)EEPROM.read(j);
    }
    highScore[index] = new Player(name, readIntFromEEPROM(i + 4));
    index += 1;
  }
  attachInterrupt(digitalPinToInterrupt(2), updateSW, FALLING);
  //Serial.begin(9600);
}

void loop() {  
  //If the menu is on, then scroll through menu options  
  if (menuState == true) {
    // If the board was not already turned on
    if (millis() > SHOWGREETINGINTERVAL && greetingMessage == true) {
      lcd.clear();
      lcd.setCursor(0, 1);
      greetingMessage = false;
      indexAbout = 0;
      joyMovedX = true;
    }
    if (greetingMessage == false) {
      showMenu(); 
    }
  }
  // If the menu is off, then the game is being played
  if (menuState == false) {
    showGameDisplay();
    playGame();
  }
}
// Turn on the led while point is moving on the matrix
void updateDisplay() {
  for (int row = 0; row < MATRIXSIZE; row++) {
    for (int col = 0; col < MATRIXSIZE; col++) {
      lc.setLed(0, row, col, matrix[row][col]);
    }
  }
}
void updatePositions() {
  xValue = analogRead(XPIN);
  yValue = analogRead(YPIN);
  xLastPos = xPos;
  yLastPos = yPos;
  if (xValue < MINTHRESHOLD) {
    if (xPos < MATRIXSIZE - 1) {
      xPos++;
    }
    else {
      xPos = 0;
    }
  }

  if (xValue > MAXTHRESHOLD) {
    if (xPos > 0) {
      xPos--;
    }
    else {
      xPos = MATRIXSIZE - 1;
    }
  }

  if (yValue > MAXTHRESHOLD) {
    if (yPos < MATRIXSIZE - 1) {
      yPos++;
    }
    else {
      yPos = 0;
    }
  }

  if (yValue < MINTHRESHOLD) {
    if (yPos > 0) {
      yPos--;
    }
    else {
      yPos = MATRIXSIZE - 1;
    }

  }

  if (xPos != xLastPos || yPos != yLastPos) {
    matrix[xPos][yPos] = 1;
    // If the last move was not a good one (the point was not in the figure),
    // Then turn off the led on the matrix
    if (playerMatrix[xLastPos][yLastPos] == 0) {
      matrix[xLastPos][yLastPos] = 0;
    }
    matrixUpdate = true;
  }
}
void updateSW() {
  if (millis() - lastDebounce > DEBOUNCEINTERVAL) {
    lastSwState = lockedIn;
    lockedIn = !lockedIn;
    // If a point was selected on the matrix
    // Then the code verifies the accuracy of the movement once
    if (lockedIn == true) {
      isVisited = true;
    }
  }
  lastDebounce = millis();
}
// Universal joyStickMovement for different x and y positions
// In order to work properly, the function gets the maximum limit of the position
void joyStickMovement(int upperLimit) {
  xValue = analogRead(XPIN);
  yValue = analogRead(YPIN);
  xLastPos = xPos;
  yLastPos = yPos;
  if (lockedIn == false) {
    if (xValue < MINTHRESHOLD && joyMovedX == false) {
      if (xPos < upperLimit - 1) {
        xPos++;
      }
      else {
        xPos = 0;
      }
      joyMovedX = true;
    }

    if (xValue > MAXTHRESHOLD && joyMovedX == false) {
      if (xPos > 0) {
        xPos--;
      }
      else {
        xPos = upperLimit - 1;
      }
      joyMovedX = true;
    }
    if (xValue >= MINTHRESHOLD && xValue <= MAXTHRESHOLD) {
      joyMovedX = false;
    }
  }
  else {
    if (yValue > MAXTHRESHOLD && joyMovedY == false) {
      if (yPos < upperLimit - 1) {
        yPos++;
      }
      else {
        yPos = 0;
      }
      joyMovedY = true;
    }

    if (yValue < MINTHRESHOLD && joyMovedY == false) {
      if (yPos > 0) {
        yPos--;
      }
      else {
        yPos = upperLimit - 1;
      }
      joyMovedY = true;
    }

    if (yValue >= MINTHRESHOLD && yValue <= MAXTHRESHOLD) {
      joyMovedY = false;
    }
  }
}
void showMenu() {
  // If we are going through the menu
  if (joyMovedX == true) {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(menu[xPos]);
    lcd.setCursor(15, 0);
    lcd.write(RIGHTARROW);
    lcd.setCursor(0, 0);
    lcd.write(LEFTARROW);
    yPos = 0;
  }
  // If we are in the submenu, show on lcd the submenu options
  else if (joyMovedY == true ) {
    lcd.clear();
    lcd.setCursor(15, 0);
    lcd.write(UPARROW);
    lcd.setCursor(15, 1);
    lcd.write(DOWNARROW);
    if (xPos == 1) {
      lcd.setCursor(0, 0);
      lcd.print(highScore[yPos]->getName());
      lcd.setCursor(5, 0);
      lcd.print(F("-"));
      lcd.setCursor(7, 0);
      lcd.print(HIGHSCORESIZE - yPos);
      lcd.setCursor(0, 1);
      lcd.print(String(highScore[yPos]->getScore()));
    }
    else if (xPos == 2) {
      lcd.setCursor(0, 0);
      lcd.print(settings[yPos]);
    }
  }
  // If the button was pressed
  // Navigate on the Y axis for Highscore and Setting
  // Otherwise, for start game - option just start the game
  // And for about show the scrolling text.
  // If the button is pressed twice, then we can exit from the submenus.
  if (lockedIn == true && inSettings == false) {
    if (xPos == 0) {
      menuState = false;
    }
    if (xPos == 1) {
      joyStickMovement(HIGHSCORESIZE);
    }
    if (xPos == 2) {
      joyStickMovement(SETTINGSSIZE);
      inSettings = true;
      yPos = 0;
    }
    if (xPos == 3) {
      showAbout();
    }
  }
  // Else move the joyStick on the other axis
  else if (lockedIn == false && inSettings == false) {
    showWholeMatrix(0);
    indexAbout = 0;
    joyStickMovement(MENUSIZE);
  }
  // If the player is in the settings submenu
  else if (inSettings == true && lockedIn == false) {
    showWholeMatrix(1);
    // Set the starting level
    // The maximum level to start is the previous one.
    // If the player has previously reached level 7 and lost,
    // Then he can start maximum from level 7.
    // Otherwise, if the player has not the played at all, then the starting level is 1.
    if (yPos == 0) {
      lcd.setCursor(0, 1);
      String startingLevel = "Start level:" + String(currentLevel);
      lcd.print(startingLevel);
      joyStickSettings(maxLevels + 1, 1);
      int newLevel = xPosSettings;
      if (newLevel != currentLevel) { 
        if (newLevel != 0) {       
        currentLevel = newLevel;
        //Increase the difficulty of the level     
        moveInterval = 180 - 10 * (newLevel - 1);       
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.write(LEFTARROW);
        lcd.setCursor(15, 0);
        lcd.write(RIGHTARROW);
        }
      }
    }
    // Set the contrast of the lcd.
    else if (yPos == 1) {
      joyStickSettings(BRIGHTNESSUPPERLIMIT, BRIGHTNESSSTEP);
      lcd.setCursor(0, 0);
      lcd.print(settings[yPos]);
      lcd.setCursor(0, 1);
      lcd.print(String(xPosSettings));
      lcd.setCursor(14, 1);
      lcd.write(LEFTARROW);
      lcd.setCursor(15, 1);
      lcd.write(RIGHTARROW);
      int newContrast = xPosSettings;
      if (newContrast != contrastValue) {
        contrastValue = newContrast;
        //analogWrite(trigPin, contrastValue);
        lcd.clear();
        EEPROM.update(0, contrastValue);
      }
    }
    // Set the matrix brightness.
    else if (yPos == 2) {
      joyStickSettings(MATRIXBRIGHTUPPERLIMIT, MATRIXBRIGHTSTEP);
      int newMatrixBrightness = xPosSettings;
      if (newMatrixBrightness != matrixBrightnessValue) {
        matrixBrightnessValue = xPosSettings;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(settings[yPos]);
        lc.setIntensity(0, matrixBrightnessValue);
        lcd.setCursor(0, 1);
        lcd.print(String(matrixBrightnessValue));
        lcd.setCursor(14, 1);
        lcd.write(LEFTARROW);
        lcd.setCursor(15, 1);
        lcd.write(RIGHTARROW);
        EEPROM.update(1, matrixBrightnessValue);
      }
    }
    // Set the brightness of the lcd.
    else if (yPos == 3) {
      joyStickSettings(BRIGHTNESSUPPERLIMIT, BRIGHTNESSSTEP);
      int newBrightness = xPosSettings;
      if (newBrightness != brightnessValue) {
        brightnessValue = xPosSettings;
        lcd.clear();
        analogWrite(BRIGHTPIN, brightnessValue);
        lcd.setCursor(0, 0);
        lcd.print(settings[yPos]);
        lcd.setCursor(0, 1);
        lcd.print(String(brightnessValue));
        lcd.setCursor(14, 1);
        lcd.write(LEFTARROW);
        lcd.setCursor(15, 1);
        lcd.write(RIGHTARROW);
        EEPROM.update(2, brightnessValue);
      }
    }
    // Back button.
    // Goes back to the main menu.
    else if (yPos == 4) {
      inSettings = false;
    }
  }
  else if (inSettings == true && lockedIn == true) {
    joyStickMovement(SETTINGSSIZE);
    xPosSettings = 0;
  }
}
void playGame() {
  // If the player is alive (the game is not over/the level is not done),
  // Continue the game.
  if (isAlive == true) {
    if (beginGame == false) {
      currentElapsedTime = millis();
      beginGame = true;
    }
    
      if (millis() - currentElapsedTime < SHOWFIGUREINTERVAL && playerTurn == false ) {
        lc.clearDisplay(0);
        showMatrix(1);
      }
      if (millis() - currentElapsedTime > SHOWFIGUREINTERVAL  && playerTurn == false && beginGame == true) {
        lc.clearDisplay(0);
        showMatrix(0);
        lockedIn = false;
        playerTurn = true;
      }
    if (millis() - lastMoved > moveInterval) {
      lastMoved = millis();
      if (playerTurn == true && lockedIn == false) {
        updatePositions();
      }
    }
    if (lockedIn == true && playerTurn == true && isVisited == true) {
      verifyMove(xPos, yPos);
      if (livesNumber <= 0) {
        matrixUpdate = false;
        lcd.clear();
        isAlive = false;
      }
      isVisited = false;
    }
    if (matrixUpdate == true ) {
      updateDisplay();
      matrixUpdate = false;
    }
    if (levelIsDone() == 1) {
      lcd.clear();
      isAlive = false;
    }
  }
  // If the level is done and the player did not die.
  if (isAlive == false && livesNumber > 0) {
    showMatrix(0);
    showPlayerMatrix(0);
    // Keep a variable for clearing the lcd.
    if (!hasWin) {
      currentTimeHighscore = millis();
      hasWin = true;
      lcd.clear();
    }
    // Check if a highscore was bet.
    if (isInSetName == false && nameWasSet == false) {
      if (clearLevelDisplay == false) {
        lcd.clear();
        clearLevelDisplay = true;
      }
      isNewHighscore();
    }
    // If the player is still setting a name then execute saveScore
    // Which replaces the previous highscore with the new score.
    // Hence the parameter indexOfHighScore.
    if (isInSetName == true && nameWasSet == false) {
      saveScore(indexOfHighscore);
    }
    if ((isInSetName == false && nameWasSet == true && newScore == 0) || (isInSetName == false && nameWasSet == false && newScore == 0)) {
      goNext();
    }
  }
  else if (isAlive == false && livesNumber <= 0) {
    showMatrix(0);
    showPlayerMatrix(0);
    if (!hasLost) {
      currentTimeHighscore = millis();
      lcd.clear();
      hasLost = true;
    }
    if (isInSetName == false && nameWasSet == false) {
      isNewHighscore();
    }
    if (isInSetName == true && nameWasSet == false) {
      saveScore(indexOfHighscore);
    }
    if ((isInSetName == false && nameWasSet == true && newScore == 0) || (isInSetName == false && nameWasSet == false && newScore == 0)) {
      lcd.clear();
      gameOver();
    }
  }
}
// If the game is still played and the player
// Have not lost or did not reach the end of the level yet,
// Show game information (lives, score, level).
void showGameDisplay() {
  if (levelIsDone() == 0 && livesNumber > 0 && isAlive == true) {
    if (previousLevel != currentLevel) {
      lcd.clear();
      lcd.setCursor(0, 0);
      String levelPrint = "Level:" + String(currentLevel);
      lcd.print(levelPrint);
      lcd.setCursor(15, 0);
      lcd.print(livesNumber);
      previousLevel = currentLevel;
    }
    if (previousScore != currentScore) {
      previousScore = currentScore;
      lcd.clear();
      lcd.setCursor(0, 0);
      String levelPrint = "Level:" + String(currentLevel);
      lcd.print(levelPrint);
      lcd.setCursor(0, 1);
      String scorePrint = "Score:" + String(currentScore);
      lcd.print(scorePrint);
      lcd.setCursor(15, 0);
      lcd.print(livesNumber);
    }
    if (previousLives != livesNumber) {
      previousLives = livesNumber;
      lcd.clear();
      lcd.setCursor(0, 0);
      String levelPrint = "Level:" + String(currentLevel);
      lcd.print(levelPrint);
      lcd.setCursor(0, 1);
      String scorePrint = "Score:" + String(currentScore);
      lcd.print(scorePrint);
      lcd.setCursor(15, 0);
      lcd.print(livesNumber);
    }
  }
}
void joyStickSettings(int upperLimit, int step) {
  xValue = analogRead(XPIN);
  if (xValue < MINTHRESHOLD && joyMovedXSettings == false) {
    if (xPosSettings < upperLimit - 1) {
      xPosSettings += step;
    }
    else {
      xPosSettings = 0;
    }
    joyMovedXSettings = true;
  }

  if (xValue > MAXTHRESHOLD && joyMovedXSettings == false) {
    if (xPosSettings > step) {
      xPosSettings -= step;
    }
    else {
      xPosSettings = upperLimit - 1;
    }
    joyMovedXSettings = true;
  }
  if (xValue >= MINTHRESHOLD && xValue <= MAXTHRESHOLD) {
    joyMovedXSettings = false;
  }
}
void showMatrix(int light) {
  for (int coord = 0; coord < NUMBEROFCOORD; coord++) {
    matrix[figures[currentLevel - 1][coord][0]][figures[currentLevel - 1][coord][1]] = light;
    lc.setLed(0, figures[currentLevel - 1][coord][0], figures[currentLevel - 1][coord][1], light);
  }
}
void showPlayerMatrix(int light) {
  for (int i = 0; i < MATRIXSIZE; i++) {
    for (int j = 0; j < MATRIXSIZE; j++) {
      playerMatrix[i][j] = light;
    }
  }
}
// Check if the player set a good position.
// Otherwise decrease the lives number.
void verifyMove(int x, int y) {
  for (int coord = 0; coord < NUMBEROFCOORD; coord++) {
    if (figures[currentLevel - 1][coord][0] == x && figures[currentLevel - 1][coord][1] == y && playerMatrix[x][y] == 0) {
      currentScore += SCORESTEP;
      playerMatrix[x][y] = 1;
      lc.setLed(0, x, y, matrix[x][y]);
      break;
    }
  }
  if (playerMatrix[x][y] == 0) {
    livesNumber -= 1;
  }
}
void gameOver() {
  showMatrix(0);
  showPlayerMatrix(0);
  lcd.setCursor(0, 0);
  lcd.print(F("You died!:("));
  lcd.setCursor(0, 1);
  lcd.print("Score:" + String(currentScore));
  lockedIn = false;
  maxLevels = currentLevel;
  menuState = true;
  lockedIn = false;
  isAlive = true;
  playerTurn = false;
  beginGame = false;
  nameWasSet = false;
  isInSetName =  false;
  hasLost = false;
  newScore = 0;
  yPos = 0;
  currentScore = 0;
  currentLevel = 1;
  livesNumber = 5;
  previousLevel = -1;
  previousScore = -1;
  for(int thisNote = 0; thisNote < 8; thisNote++){
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(BUZZERPIN, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(pauseBetweenNotes);
  }
}
// Check if the player remade the figure by verifying the marked positions
int levelIsDone() {
  for (int coord = 0; coord < NUMBEROFCOORD; coord++) {
    if (playerMatrix[figures[currentLevel - 1][coord][0]][figures[currentLevel - 1][coord][1]] == 0) {
      return 0;
    }
  }
  return 1;
}

// Check if the current player bet one of the top 3 players
void isNewHighscore () {
  int index;
  String newScoreStr;
  playerTurn = false;
  // Check f the player bet one of the highscores
  getMaximum();
  if (newScore == 1) {
    newScoreStr = "Congrats!" + String(currentScore);
    lcd.setCursor(3, 0);
    lcd.print(newScoreStr);
    lcd.setCursor(1, 1);
    lcd.print(F("New highscore!"));
    if (millis() - introMillis > SHOWGREETINGINTERVAL / 8) {
    tone(BUZZERPIN, melody[indexAbout + 1], SHOWGREETINGINTERVAL / 8);
    introMillis = millis();
    }  
  }
  //If a new highscore was established then show congratulation message for 5 seconds
  if (newScore == 1 && millis() - currentTimeHighscore > SHOWGREETINGINTERVAL) {
    lcd.clear();
    isInSetName = true;
    lockedIn = false;
    indexAbout = 0;       
  }
  if (newScore == 0) {
    lockedIn = false;
  }
}
// Save string to memory
void saveToMemory(String name, long long score) {
  byte len = name.length();
  for (int i = 0; i < len; i++)
  {
    EEPROM.update(offset + i, name[i]);
  }
  writeIntIntoEEPROM(offset + len, score);
}

// Ask the player if he wants to go to the next level
void goNext() {
  lcd.setCursor(1, 0);
  lcd.write(LEFTARROW);
  lcd.setCursor(14, 0);
  lcd.write(RIGHTARROW);
  lcd.setCursor(6, 1);
  lcd.print(F("Continue?"));
  joyStickMovement(2);
  // If player chose the next level
  // Then increase the current level, add a bonus to the score
  // Containing points equal to the level number
  // And add an extra life.
  // Increase the difficulty by decreasing the moving interval for the point.
  // This way it will be harder tot select a position.
  if (xPos == 0 && lockedIn == true) {
    previousScore = currentScore;
    currentScore += currentLevel;
    livesNumber += 1;
    currentLevel += 1;
    if (moveInterval >= 10) {
      moveInterval -= 10;
    }
    else {
      moveInterval = 10;
    }
    nameWasSet = false;
    isInSetName =  false;
    lockedIn = false;
    beginGame = false;
    isAlive = true;
    hasWin = false;
    newScore = 0;
    playerTurn = false;
    clearLevelDisplay = false;
    lcd.clear();
  }
  else if (xPos == 1 && lockedIn == true) {
    maxLevels = currentLevel;
    menuState = true;
    lockedIn = false;
    isAlive = true;
    beginGame = false;
    nameWasSet = false;
    isInSetName =  false;
    hasWin = false;
    hasLost = false;
    playerTurn = false;
    clearLevelDisplay = false;
    newScore = 0;
    yPos = 0;
    currentScore = 0;
    currentLevel = 1;
    livesNumber = 5;
    previousLevel = -1;
    previousScore = -1;
    xPos = 0;
    lcd.clear();
  }
  if (xPos == 1 && lockedIn == false && joyMovedX == true) {
    if (xLastPos != xPos) {
      lcd.clear();
    }
    lcd.setCursor(2, 0);
    lcd.print(F("No"));
    lcd.setCursor(14, 0);
    lcd.write(RIGHTARROW);
    lcd.setCursor(6, 1);
    lcd.print(F("Continue?"));
    joyStickMovement(2);
  }
  else if (xPos == 0 && lockedIn == false && joyMovedX == true) {
    if (xLastPos != xPos) {
      lcd.clear();
    }
    lcd.setCursor(11, 0);
    lcd.print(F("Yes"));
    lcd.setCursor(1, 0);
    lcd.write(LEFTARROW);
    lcd.setCursor(6, 1);
    lcd.print(F("Continue?"));
    joyStickMovement(2);
  }
}
// If new highscore has been established, then save a name.
// The name has maximum 4 letters.
void saveScore(int index) {
  lcd.setCursor(4, 0);
  lcd.print(F("OK"));
  if (lockedIn == false) {
    int lastXPos = xPos;
    joyStickMovement(NAMESIZE + 1);
    if (lastXPos != xPos) {
      lcd.clear();
    }
    for (int i = 0 ; i < NAMESIZE; i++) {
      lcd.setCursor(i, 0);
      if ( i == xPos) {
        lcd.print(F("_"));
      }
      else {
        lcd.print(name[i]);
      }
    }
    lcd.setCursor(4, 0);
    lcd.print(F("OK"));
    lcd.setCursor(0, 1);
    lcd.write(LEFTARROW);
    lcd.setCursor(15, 1);
    lcd.write(RIGHTARROW);
  }
  // If the button was pressed and the position is one for a letter,
  // Then clear the lcd and while changing the letter all the other letters disappear.
  else if (lockedIn == true && xPos != 4) {
    char lastPos = char(lettersArray[yPos]);
    joyStickMovement(ALPHABETSIZE);
    if (lastPos != char(lettersArray[yPos])) {
      lcd.clear();
      lcd.setCursor(xPos, 0);
      lcd.print(char(lettersArray[yPos]));
    }
    name[xPos] = char(lettersArray[yPos]);
    lcd.setCursor(0, 1);
    lcd.print(F(""));
    lcd.setCursor(15, 1);
    lcd.print(F(""));
    lcd.setCursor(15, 0);
    lcd.write(UPARROW);
    lcd.setCursor(15, 1);
    lcd.write(DOWNARROW);
  }
  // If the button was pressed and the player is on the last position("OK"),
  // Save the name and score in memory.
  else if (lockedIn == true && xPos == 4) {
    String newName = name[0] + name[1] + name[2] + name[3];
    highScore[index]->setScore(currentScore);
    highScore[index]->setName(newName);
    Serial.println(highScore[index]->getName());
    saveToMemory(newName, currentScore);
    newScore = 0;
    xPos = 0;
    isInSetName = false;
    nameWasSet = true;
    lockedIn = false;
    lcd.clear();
  }
}
// Scrolling text for "About" section
void showAbout() {
  if (indexAbout <= strlen(aboutInfo) - 16) {
    if (millis() - currentAboutMillis > ABOUTINTERVAL) {
      showLetter(0, indexAbout);
      currentAboutMillis = millis();
      indexAbout += 1;
    }
  }
}
void showLetter(int start, int startLetter) {
  lcd.setCursor(start, 0);
  lcd.print(F(" "));
  for (int letter = startLetter; letter <= startLetter + 15; letter++) {
    lcd.print(aboutInfo[letter]);
  }
}
// Turn on/off the leds on the matrix
void showWholeMatrix(int light) {
  for (int i = 0; i < MATRIXSIZE; i++) {
    for (int j = 0; j < MATRIXSIZE; j++) {
      matrix[i][j] = light;
      lc.setLed(0, i, j, light);
    }
  }
}
// Save integer into memory
void writeIntIntoEEPROM(int address, int number) {
  byte byte1 = number >> 8;
  byte byte2 = number & 0xFF;

  EEPROM.write(address, byte1);
  EEPROM.write(address + 1, byte2);
}
int readIntFromEEPROM(int address) {
  byte byte1 = EEPROM.read(address);
  byte byte2 = EEPROM.read(address + 1);

  return (byte1 << 8) + byte2;
}
int getMaximum() {
  for (int i = HIGHSCORESIZE - 1; i >= 0; i--) {
    Player* player = highScore[i];
    // If the current player obtained a new highscore
    if (currentScore > highScore[i]->getScore()) {
      newScore = 1;
      indexOfHighscore = i;
      offset = 6 * i + 3;
      break;
    }
  }
}
