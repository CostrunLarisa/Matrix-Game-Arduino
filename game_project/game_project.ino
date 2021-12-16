#include <Dictionary.h>
#include <NodeArray.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "LedControl.h" //  need the library
const int dinPin = 12;
const int potPin = A2;
const int clockPin = 11;
const int loadPin = 10;
const int trigPin = 9;
const int brightPin = 6;
const int RS = 13;
const int enable = 7;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 8;

const int xPin = A0;
const int yPin = A1;

const int matrixSize = 8;
const int highScoreSize = 3;
const int settingsSize = 5;
int xValue = 0;
int yValue = 0;
int xPosSettings = 0;
byte xPos = 0;
byte yPos = 0;

byte xLastPos = 0;
byte yLastPos = 0;

int matrixBrightness = 2;
unsigned long  currentMillis = 0;
const int minThreshold = 200;
const int maxThreshold = 600;
const int menuSize = 4;
int maxLevels = 1;
const int swPin = 2;
unsigned int lastMoved = 0;
const int moveInterval = 100;
const int showGreetingInterval = 5000;
bool matrixUpdate = true;
bool menuState = true;
bool joyMovedX = false;
bool joyMovedY = false;
bool joyMovedXSettings = false;
bool greetingMessage = true;
bool inSettings = false;
bool playerTurn = false;
bool isAlive = true;
bool wasChecked = false;
bool isInSetName = false;
bool beginGame = false;
volatile bool lockedIn = false;
volatile bool lastSwState = false;
volatile long long lastDebounce = 0;
static int currentLevel = 1;
static int currentScore = 0;
int previousScore = -1;
int previousLevel = 0;
int livesNumber = 5;
int subMenu = 1;
int scoreStep = 5;
int foundPoints = 0;
int newScore =  0;
int potValue;
int dataOffset = 3;
const int figuresNumber = 1;
const int numberOfCoord = 12;
const int pointTypesSize = 3;
String name[4] = {"","","",""};
char * aboutInfo = "Brainstorming Game.Created by Larisuk.Find me at https://github.com/Larisuk/Matrix-Game-Arduino.";
const int debounceInterval = 200;
const int showFigureInterval = 5000;
unsigned long currentElapsedTime = 0;
unsigned long  currentGameOverTime = 0;
unsigned long  currentTimeHighscore = 0;
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); //DIN, CLK, LOAD, No. DRIVER
LiquidCrystal lcd(RS,enable,d4,d5,d6,d7);
bool matrix[matrixSize][matrixSize] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}  
};
bool playerMatrix[matrixSize][matrixSize] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}  
};
int lettersArray[26] = {
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
int figures[figuresNumber][numberOfCoord][2] = {
  {{0, 1}, {1, 1}, {2, 1}, {3, 1}, {0, 6}, {1, 6}, {2, 6}, {3, 6}, {5, 2}, {5, 3}, {5, 4}, {5, 5}},
};
String pointTypes[pointTypesSize] = {
    "normal",
    "locked",
    "fast"
};
char* menu[menuSize] = {
  "Start game",
  "Highscore",
  "Settings",
  "About"
};
char* settings[settingsSize] = {
  "Starting level",
  "Contrast",
  "Matrix brightness",
  "Brightness",
  "Back"
};
class Player {
  private: String name;
  private: long long score;
  public: Player() {
    name = "";
    score = 0;
  }
  public: Player(String nameP, int scoreP) {
    name = nameP;
    score = scoreP;
  }
  public: int getScore() {
    return score;   
  }
  public: String getName() {
    return name;
  }  
  public: void setScore (int scoreP) {
    score = scoreP;    
  }
  public: void setName (String nameP) {
    name = nameP;
  }
};
Player* highScore[highScoreSize] = {
  new Player("",0), new Player("",0), new Player("",0)
};
void setup() {
  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, matrixBrightness); // sets brightness (0~15 possible values)
  lcd.begin(16,2);// Print a message to the LCD.
  lcd.print(">>Welcome to a");
  lcd.setCursor(0, 1);
  lcd.print("nerdy world!<<");
  lc.clearDisplay(0);// clear screen
  pinMode(potPin,INPUT);
  pinMode(swPin, INPUT_PULLUP);
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  //EEPROM.update(0, 120);
  //analogWrite(trigPin, EEPROM.read(0));
  lc.setIntensity(0, EEPROM.read(1));      
  analogWrite(brightPin, EEPROM.read(2));
  //analogWrite(trigPin, 120);
  int index = 0;
  for (int i = 2; i <= 12; i += 5) {
    String name = "";
    for (int j = i + 1; j < i+4; j++) {
      name += EEPROM.read(j);
    }
    highScore[index] = new Player(name, EEPROM.read(i+4));
    index += 1;
  }
  attachInterrupt(digitalPinToInterrupt(2), updateSW, FALLING);
  Serial.begin(9600);
}

void loop() {
  //saveScore(0);
  potValue = analogRead(potPin);
  analogWrite(trigPin, potValue);
  if (menuState == true) {
    if (millis() >= showGreetingInterval && greetingMessage == true) {      
      lcd.clear();    
      lcd.setCursor(0, 1);
      greetingMessage = false;
      joyMovedX = true;
    }
    if (greetingMessage == false) { 
      showMenu();
    }     
  }
  if (menuState == false) {
    showGameDisplay();
    playGame();
  }
}

void updateDisplay() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrix[row][col]);
    }
  }
}
void updatePositions() {
	xValue = analogRead(xPin);
  yValue = analogRead(yPin);
  xLastPos = xPos;
  yLastPos = yPos;
  if (xValue < minThreshold) {
    if (xPos < matrixSize - 1) {
      xPos++;
    } 
    else {
      xPos = 0;
    }
  }
  
  if (xValue > maxThreshold) {
    if (xPos > 0) {
      xPos--;
    }
    else {
      xPos = matrixSize - 1;
    }
  }
 
  if (yValue > maxThreshold) {
    if (yPos < matrixSize - 1) {
      yPos++;
    } 
    else {
      yPos = 0;
    }
  }
  
  if (yValue < minThreshold) {
    if (yPos > 0) {
      yPos--;
    }
    else {
      yPos = matrixSize - 1;
    }
  }
  
  if (xPos != xLastPos || yPos != yLastPos) {
    matrix[xPos][yPos] = 1;
    if(playerMatrix[xLastPos][yLastPos] == 0) {
      matrix[xLastPos][yLastPos] = 0; 
    }
    matrixUpdate = true;
  }
}
void updateSW() {
  if (millis() - lastDebounce > debounceInterval) {
    lastSwState = lockedIn;
    lockedIn = !lockedIn;
    /*if (lastSwState == true && lockedIn == false) {
      saveToMemory();
    }*/
  }
  lastDebounce = millis();
}
void joyStickMovement(int upperLimit) {
  xValue = analogRead(xPin);
  yValue = analogRead(yPin);
  xLastPos = xPos;
  yLastPos = yPos;
  if (lockedIn == false) {
    if (xValue < minThreshold && joyMovedX == false) {
      if (xPos < upperLimit - 1) {
        xPos++;
      } 
      else {
        xPos = 0;
      }
      joyMovedX = true;
    }
    
    if (xValue > maxThreshold && joyMovedX == false) {
      if (xPos > 0) {
        xPos--;
      }
      else {
        xPos = upperLimit - 1;
      }
      joyMovedX = true;    
    }
    if (xValue >= minThreshold && xValue <= maxThreshold) {
        joyMovedX = false;
    }
  }
  else {  
    if (yValue > maxThreshold && joyMovedY == false) {
      if (yPos < upperLimit - 1) {
        yPos++;
      } 
      else {
        yPos = 0;
      }
      joyMovedY = true;    
    }
    
    if (yValue < minThreshold && joyMovedY == false) {
      if (yPos > 0) {
        yPos--;
      }
      else {
        yPos = upperLimit - 1;
      }
      joyMovedY = true;
    }
    
    if (yValue >= minThreshold && yValue <= maxThreshold) {
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
  }
  else if (joyMovedY == true ) {
    lcd.clear();    
    if (xPos == 1) {
      //lcd.clear();
      lcd.setCursor(0, 0);
      Player* player = highScore[yPos];     
      lcd.print(player->getName()); 
      lcd.setCursor(0, 1);  
      lcd.print(player->getScore() + ""); 
    }
    else if (xPos == 2) {
      lcd.setCursor(0, 0);
      lcd.print(settings[yPos]);
      //inSettings = true; 
    }
  }
  // If the button was pressed
  // Navigate on the Y axis for Highscore and Setting
  if (lockedIn == true && inSettings == false) {
    if (xPos == 0) {
      menuState = false;
    }     
    if (xPos == 1) {
      joyStickMovement(highScoreSize);      
    }
    if (xPos == 2) {      
      joyStickMovement(settingsSize);
      inSettings = true;
      yPos = 0;
    }
    if (xPos == 3) {
      showAbout();      
      //joyStickMovement(menuSize);
    }
  }
  // Else move the joyStick on the other axis
  else if (lockedIn == false && inSettings == false) {
    joyStickMovement(menuSize);
  }
  else if (inSettings == true && lockedIn == false) {
    if (yPos == 0) {      
      joyStickSettings(maxLevels + 1, 1);
      currentLevel = xPosSettings;      
      lcd.clear();
      lcd.setCursor(0, 1);
      String startingLevel = "Start level:" + String(xPosSettings);
      lcd.print(startingLevel); 
      Serial.println(settings[0]);      
    }
    else if (yPos == 1) {
      joyStickSettings(240, 10);
      analogWrite(trigPin, xPosSettings);
      EEPROM.update(0, xPosSettings);
    }
    else if (yPos == 2) {
      joyStickSettings(15, 1);
      lc.setIntensity(0, xPosSettings);
      EEPROM.update(1, xPosSettings);      
    }
    else if (yPos == 3) {
      joyStickSettings(240, 10);
      analogWrite(brightPin, xPosSettings);
      EEPROM.update(2, xPosSettings);      
    }
    else if (yPos == 4) {
      inSettings = false;
    }
  }
  else if (inSettings == true && lockedIn == true) {
    joyStickMovement(settingsSize);
  }
}
void playGame() {
  if (isAlive == true) {
    if (beginGame == false) {
      lastMoved = millis();
      currentElapsedTime = millis();
      beginGame = true;  
    }
    if (millis() - lastMoved > moveInterval) {
      // game logic
      if(millis() - currentElapsedTime < showFigureInterval && playerTurn == false ) {  
        lc.clearDisplay(0);
        showMatrix(1);    
      } 
      if (millis() - currentElapsedTime > showFigureInterval  && playerTurn == false && beginGame == true) {
        lc.clearDisplay(0);
        showMatrix(0);
        lockedIn = false;
        playerTurn = true;
      }
      if (playerTurn == true && lockedIn == false) {
        updatePositions();        
      }
    
    if (lockedIn == true && playerTurn == true) {
      verifyMove(xPos, yPos); 
      Serial.println(livesNumber);
      if (livesNumber <= 0) {
        matrixUpdate = false;
        lcd.clear();
          isAlive = false;
          //lockedIn = false;
          //menuState = true;
          //gameOver();     
      }   
    }
    if (matrixUpdate == true ) {
      // display logic
      updateDisplay();
      matrixUpdate = false;
    }
    if (levelIsDone() == 1 && livesNumber > 0) {
      showMatrix(0);
      showPlayerMatrix(0);
      if(isInSetName == false) {
        isNewHighscore();
      }
      if (isInSetName == true) {
        saveScore(0);
      }    
    }}
  } 
  else if (isAlive == false) {
    showMatrix(0);
    showPlayerMatrix(0);
    //currentGameOverTime = millis();
    //gameOver();
    if ( isInSetName == false) {
      gameOver();
      isNewHighscore();
    }
    if (isInSetName == true) {
      saveScore(0);
    }
  }
}
void showGameDisplay() {
  if (previousLevel != currentLevel) {
    lcd.clear();
    lcd.setCursor(0, 0);
    String levelPrint = "Level:"+ String(currentLevel);
    lcd.print(levelPrint);   
    lcd.setCursor(15, 0);
    lcd.print(livesNumber);
    Serial.println(livesNumber);
    previousLevel = currentLevel;
  } 
  if (previousScore != currentScore) {
    previousScore = currentScore;
    lcd.clear();
    lcd.setCursor(0, 0);
    String levelPrint = "Level:"+ String(currentLevel);
    lcd.print(levelPrint);
    lcd.setCursor(0, 1);
    String scorePrint = "Score:"+ String(currentScore);
    lcd.print(scorePrint);  
    lcd.setCursor(15, 0);
    lcd.print(livesNumber);   
  }
}
void joyStickSettings(int upperLimit, int step) {
  xValue = analogRead(xPin);
  if (xValue < minThreshold && joyMovedX == false) {
      if (xPosSettings < upperLimit - 1) {
        xPosSettings += step;
      } 
      else {
        xPosSettings = 0;
      }
      joyMovedXSettings = true;
    }
    
  if (xValue > maxThreshold && joyMovedX == false) {
      if (xPos > 0) {
        xPosSettings -= step;
      }
      else {
        xPosSettings = upperLimit - 1;
      }
      joyMovedXSettings = true;    
    }
  if (xValue >= minThreshold && xValue <= maxThreshold) {
      joyMovedXSettings = false;
  }
}
void showMatrix(int light) { 
  for (int item = 0; item < figuresNumber; item++){
    for (int coord = 0; coord < numberOfCoord; coord++) {
        matrix[figures[item][coord][0]][figures[item][coord][1]] = light;      
        lc.setLed(0, figures[item][coord][0], figures[item][coord][1], light);
    }
  }
}
void showPlayerMatrix(int light) {
  for (int item = 0; item < matrixSize; item++){
    for (int coord = 0; coord < matrixSize; coord++) {
      playerMatrix[item][coord] = light;   
    }
  }
}
void verifyMove(int x, int y) {
  for (int item = 0; item < figuresNumber; item++) {
    for (int coord = 0; coord < numberOfCoord; coord++) {
      if (figures[item][coord][0] == x && figures[item][coord][1] == y && playerMatrix[x][y] == 0) {
        currentScore += scoreStep;
        matrix[x][y] = 1;
        playerMatrix[x][y] = 1;
        lc.setLed(0, x, y, matrix[x][y]);  
        break;
      }
    }
  }
  if(playerMatrix[x][y] == 0) {
    livesNumber -= 1;
  }
}
void gameOver() {
  showMatrix(0);
  showPlayerMatrix(0);
  lcd.print("You died!:(");
  //if (millis() - currentGameOverTime  > debounceInterval) {
    lockedIn = false;
    currentGameOverTime = millis();
    isNewHighscore();
    maxLevels = currentLevel;
    //menuState = true;
    lockedIn = false;
    isAlive = true;
    playerTurn = false;
    beginGame = false;
    currentScore = 0;
    currentLevel = 1;
    livesNumber = 5;
 // }
}

// Check if the player remade the figure, 
// If so, then level is done
int levelIsDone() {
  for (int item = 0; item < figuresNumber; item++) {
    for (int coord = 0; coord < numberOfCoord; coord++) {
      if (playerMatrix[figures[item][coord][0]][figures[item][coord][1]] == 0) {
        return 0;
      }
    }
  }
  return 1;
}

// Check if the current player bet one of the top 3 players
void isNewHighscore () {
  int index;  
  String newScoreStr;
  for (int i = 0; i < highScoreSize; i++) {
    Player* player = highScore[i];
    index = i;
    // If the current player obtained a new highscore
    
    if(currentScore > 3) {
      Serial.println(currentScore);
      newScoreStr = "Congrats!"+String(currentScore);
      newScore = 1; 
      break;
    } 
  }
  if (millis() - currentGameOverTime > debounceInterval) {
    currentTimeHighscore = millis();
    lcd.clear();
    lcd.setCursor(3, 0);        
    lcd.print(newScoreStr);
    lcd.setCursor(1, 1);
    lcd.print("New highscore!");        
  }

  if (newScore == 1 && millis() - currentTimeHighscore > 200) {
    saveScore(index);
    newScore = 0;     
  }
  else if (newScore == 0) {
    menuState = true;
  }
}

void saveToMemory(int offset,String name, long long score) {
  byte len = name.length();
  for (int i = 0; i < len; i++)
  {
    EEPROM.write(offset + i, name[i]);
  }
  EEPROM.write(offset + len, score);  
  dataOffset += 5;
  Serial.println("Here");
}
void goNext() {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Yes"); 
  lcd.setCursor(0, 14);
  lcd.print("No");  
  lcd.setCursor(0, 6);
  lcd.print("Continue?");
  joyStickMovement(2);
  if (xPos == 0) {
    currentLevel += 1;
    playGame();
  }
  else {
    currentLevel = 1;  
    currentScore = 0;      
    menuState = true;
  }
}
void saveScore(int index) {
  lcd.setCursor(4, 0); 
  lcd.print("OK");
  if (lockedIn == false) {
    lcd.clear();    
    joyStickMovement(5);
    for (int i = 0 ; i < 4; i++) {
      lcd.setCursor(i, 0);
      if ( i == xPos) {
        lcd.print("_");
      }
      else {          
        lcd.print(name[i]);        
      }
    }
    lcd.setCursor(4, 0);
    lcd.print("OK");
  }  
  else if (lockedIn == true && xPos!= 4) {
    char lastPos = char(lettersArray[yPos]);
    joyStickMovement(26);
    if (lastPos != char(lettersArray[yPos])) {
      lcd.clear();   
      lcd.setCursor(xPos, 0);   
      lcd.print(char(lettersArray[yPos]));
    }
    name[xPos] = char(lettersArray[yPos]);
  }
  else if (lockedIn == true && xPos == 4) {
    String newName = name[0] + name[1] + name[2] + name[3];
    highScore[index]->setScore(currentScore);
    highScore[index]->setName(newName);
    saveToMemory(dataOffset,newName,currentScore);   
    newScore = 0; 
    menuState = true;
    lockedIn = false;
  }
}
void showAbout() {
  for (int i = 0; i <= strlen(aboutInfo) - 16; i++) {
    showLetter(0, i);
  }
}
void showLetter(int start, int startLetter) {
  lcd.setCursor(start, 0);
  for (int letter = startLetter; letter <= startLetter + 15; letter++) {
    lcd.print(aboutInfo[letter]);
  }
  lcd.print(" ");
  delay(500);
}

