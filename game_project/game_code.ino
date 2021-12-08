#include <Dictionary.h>
#include <NodeArray.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "LedControl.h" //  need the library
const int dinPin = 12;
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
int currentMillis = 0;
const int minThreshold = 200;
const int maxThreshold = 600;
const int menuSize = 4;
const int maxLevels = 12;
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
volatile bool lockedIn = false;
volatile bool lastSwState = false;
volatile long long lastDebounce = 0;
static int currentLevel = 1;
static long long currentScore = 0;
int previousScore = -1;
int previousLevel = 0;
int livesNumber = 5;
int subMenu = 1;
int scoreStep = 5;
int foundPoints = 0;
const int figuresNumber = 1;
const int numberOfCoord = 12;
const int pointTypesSize = 3;
String aboutInfo = "Brainstorming Game.Created by Larisuk.Find me at https://github.com/Larisuk/Matrix-Game-Arduino.";
const int debounceInterval = 200;
const int showFigureInterval = 200;
int currentElapsedTime;
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
String menu[menuSize] = {
  "Start game",
  "Highscore",
  "Settings",
  "About"
};
String settings[settingsSize] = {
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
  public: Player(String nameP, long long scoreP) {
    name = nameP;
    score = scoreP;
  }
  public: long long getScore() {
    return score;   
  }
  public: String getName() {
    return name;
  }  
  public: void setScore (long long scoreP) {
    score = scoreP;    
  }
  public: void setName (String nameP) {
    name = nameP;
  }
};
Player* highScore[highScoreSize] = {
  new Player(), new Player(), new Player()
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
  pinMode(swPin, INPUT_PULLUP);
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  //EEPROM.update(0, 120);
  analogWrite(trigPin, EEPROM.read(0));
  lc.setIntensity(0, EEPROM.read(1));      
  analogWrite(brightPin, EEPROM.read(2));
  //analogWrite(trigPin, 120);
  int index = 0;  
  for (int i = 3; i < 19; i += 4) {
    String name = "";
    for (int j = i; j < i+3; j++) {
      name += EEPROM.read(j);
    }
    highScore[index] = new Player(name, EEPROM.read(i+4));
    index += 1;
  }
  attachInterrupt(digitalPinToInterrupt(2), updateSW, FALLING);
  Serial.begin(9600);
}

void loop() {
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
  else if (menuState == false) {
    showGameDisplay();
    playGame();
  }
  
  if (isAlive == false) {
    gameOver();
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
  //lcd.print(menu[xPos]);
  // If we are going through the menu
  if (joyMovedX == true) {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(menu[xPos]); 
  }
  else if (joyMovedY == true) {
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
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(aboutInfo); 
      lcd.scrollDisplayLeft();
      joyStickMovement(menuSize);
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
      lcd.setCursor(0, 0);
      lcd.print("Start level:" + xPosSettings); 
      Serial.println(currentLevel);
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
  currentElapsedTime = millis();
  if (millis() - lastMoved > moveInterval) {
    // game logic
    if (millis() - currentElapsedTime < showFigureInterval && playerTurn == false) {
      lc.clearDisplay(0);
      showMatrix(1);
    }
    else if(millis() - currentElapsedTime > showFigureInterval && playerTurn == false) {
      lc.clearDisplay(0);
      showMatrix(0);
      lastMoved = millis();
      lockedIn = false;
      playerTurn = true;
    }
    if (playerTurn == true && lockedIn == false) {
      updatePositions();        
    }
  }
  if (lockedIn != false && playerTurn == true) {
    verifyMove(xPos, yPos); 
    if(playerMatrix[xLastPos][yLastPos] == 0) {
      if (lockedIn == true) {
        livesNumber -= 1;
        Serial.println(livesNumber);
        if (livesNumber <= 0) {
        isAlive = false;
        }  
      }   
    }
  }
  if (matrixUpdate == true) {
    // display logic
    updateDisplay();
    matrixUpdate = false;
  }
  if (levelIsDone() == 1) {
    showMatrix(0);
    isNewHighscore();
    goNext();
  }
}
void showGameDisplay() {
  if (previousLevel != currentLevel) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Level:"+ currentLevel);
    lcd.setCursor(10, 0);
    lcd.print(livesNumber);
    previousLevel = currentLevel;
  } 
  if (previousScore != currentScore) {
    previousScore = currentScore;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Level:"+ currentLevel);
    lcd.setCursor(0, 1);
    lcd.print("Score:"+ currentScore);     
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
        lc.setLed(0, figures[item][coord][0], figures[item][coord][1], matrix[figures[item][coord][0]][figures[item][coord][1]]);
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
}
void gameOver() {
  showMatrix(0);
  lcd.clear();
  lcd.print("You lost!:(");
  Serial.println(currentScore+"");
  currentScore = 0;
  currentLevel = 1;
  menuState = true;
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
  bool newScore =  false;
  int currentTime;
  int index;  
  for (int i = 0; i < highScoreSize; i++) {
    Player* player = highScore[i];
    index = i;
    // If the current player obtained a new highscore
    if(currentScore > player->getScore()) {
      currentTime = millis();
      lcd.clear();
      lcd.setCursor(0, 5);
      lcd.print("Congrats!");
      lcd.setCursor(0, 4);
      lcd.print("New highscore!");
      newScore = true;
      break;
    } 
  }
  if (newScore == true) {
    if (millis() - currentTime > 200) {
      lcd.clear();
      saveScore(index);
      newScore = false;     
    }
  }
}

void saveToMemory(int offset,String name, long long score) {
  byte len = name.length();
  for (int i = 0; i < len; i++)
  {
    EEPROM.write(offset + 1 + i, name[i]);
  }
  EEPROM.write(offset + 1 + len, score);  
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
  lcd.clear();
  lcd.setCursor(0, 1);
  String name[4] = {"","","",""};
  if (lockedIn == false) {
    joyStickMovement(4);
    lcd.setCursor(0, xPos);
  }  
  else {
    joyStickMovement(26);
    lcd.print(lettersArray[yPos]);
    name[xPos] = lettersArray[yPos];
  }
  if (xPos == 3) {
    String newName = name[0] + name[1] + name[2] + name[3];
    highScore[index]->setScore(currentScore);
    highScore[index]->setName(newName);
    saveToMemory(3+index,newName,currentScore);
  } 
}