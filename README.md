# Matrix Game Arduino
Nerdy game which has the purpose to test your intelligence. Do you think you can remember the position of each point on a 8x8 matrix and its power of lightening? Think twice! First, you are welcomed with a greeting message and then entered in the navigation menu. All of them are displayed on a Liquid Crystal Display.
## Requirements
 ### Main menu
 The main menu appears after 5 seconds, while greeting message is shown on the display, then the player can navigate through the menu with a joystick using the X axis. In order to selected one option from the menu the player must press the joystick button.
 The main menu consists:
 - Start game: starts the game with the current selected level (or by default, level 1). The player can start only with the last played level(e.g. if he reached level 9, he can start with maximum level 9).
 - Settings: lets you set the starting level of the game, matrix brightness or the contrast and brightness LCD.
 - Highscore: top 3 best highscores read from the memory containing the name of the user and the score.
 - About: informations about the game (name, author and github link).
  If one option is played once more after selected, then the player can scroll back through the menu.
  ### Submenu - settings
  Consists:
  - Starting level: sets the level one player wants to start from.
  - Contrast: sets the contrast of the lcd (increased by 10, the maximum reached value is 240).
  - Brightness: sets the brightness of the lcd (increased by 10, the maximum reached value is 240).
  - Matrix brightness: sets the intensity of the matrix (increased by 1, the maximum reached value is 15).
  - Back: once pressed, displays the main menu.
  The contrast, the brightness and matrix brightness are saved in EEPROM, so everytime the Arduino Uno restarts the values will remain the same.
  ### Submenu - highscore
  Displays the top 3 highscores, saved in EEPROM with the name and score reached by one. Entering into this setting occus only if he beats one of those already saved. 
## Implementation Details

## Game logic
First, a figure is shown on the matrix displayed for 10 seconds, containing points in different positions wih different types of blinking (depending on the difficulty of the level). Then, the player needs to remember the location of the points and their type. Based on that, he is navigating on the matrix with the joystick (left-right, top-bottom and diagonally) and presses the button of the joystick to fix the point on the matrix. After fixing the point, he can use the y-axis to chose the blinking type and press once more the button to validate that choice for the point. If it was a good move, the score will increase and the point will remain on the matrix, otherwise the number of lives will be decreased. While playing, the number of lives, the score and level are displayed on the LCD.
### How to play
The game starts with a figure shown on the matrix for 5 seconds and the player must remake it by slecting positions for the point. When all the points from the figure are selected, if the player has more than 1 live, then he is asked if he goes to the next level.  
## #Components
- ARDUINO UNO board
- 8x8 matrix (used to play the game)
- 15x2 lcd (displays the menu of the game)
- joystick (used to navigate through the menu and to move in the game)
- buzzer (for loosing the game and establishing a new highscoe sound).
## Setup picture  
![alt text](https://user-images.githubusercontent.com/41392462/145302452-fbd130a6-b22b-41c2-a7ef-3f7df38b8b08.jpeg)
