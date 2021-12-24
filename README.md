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
  Contains:
  - Starting level: sets the level one player wants to start from.
  - Contrast: sets the contrast of the lcd (increased by 10, the maximum reached value is 240).
  - Brightness: sets the brightness of the lcd (increased by 10, the maximum reached value is 240).
  - Matrix brightness: sets the intensity of the matrix (increased by 1, the maximum reached value is 15).
  - Back: once pressed, displays the main menu.
  The contrast, the brightness and matrix brightness are saved in EEPROM, so everytime the Arduino Uno restarts the values will remain the same. If one option is selected, then the player has to set the value scrolling on X-axis.
  ### Submenu - highscore
  Displays the top 3 highscores, saved in EEPROM with the name and score reached by one. Entering into this setting occus only if he beats one of those already saved. 
## Implementation Details
- The brightness values, contrast and intensity are kept in EEPROM, read and set everytime the ARDUINO turns on, as well as the highscore values.
- For keeping the values name-score there is an array of Player objects.
- There are customized arrows displayed on the menu (up, down,left, right) saved as values in binary.
- 2 files with constants, one for buzzer notes(buzzer_tones.h) and one for game constants(game_constants.h) and one for class Player(player.h).
- The game logic is separated of the lcd logic by the variable menuState. If menuState is True, then the player is scrolling through the lcd, otherwise that means he is playing the game.
## Game logic
First, a figure is shown on the matrix displayed for 10 seconds, containing points in different positions wih different types of blinking (depending on the difficulty of the level). Then, the player needs to remember the location of the points and their type. Based on that, he is navigating on the matrix with the joystick (left-right, top-bottom and diagonally) and presses the button of the joystick to fix the point on the matrix. After fixing the point, he can use the y-axis to chose the blinking type and press once more the button to validate that choice for the point. If it was a good move, the score will increase and the point will remain on the matrix, otherwise the number of lives will be decreased. While playing, the number of lives, the score and level are displayed on the LCD and after each passed level the player receives an exta life and extra points equal to the level number(e.g level 2 = 2 extra points). The game ends when the player doesn't have any lives. This action is possible, because the difficulty of the game reaches a point where the level cannot be passed (the moving speed of the point becomes too high). After each level, if a new highscore has been established then the player is required to save his name and a congratulation message is shown on display with a short melody from the buzzer. Then, if the player looses on the lcd appears the text "You died!:("

## Components
- ARDUINO UNO board
- 8x8 matrix (used to play the game)
- 15x2 lcd (displays the menu of the game)
- joystick (used to navigate through the menu and to move in the game)
- buzzer (for loosing the game and establishing a new highscoe sound).
## Video of the game
https://www.youtube.com/watch?v=3CC1-aWnnN8&ab_channel=BiancaLarisa
## Setup picture  
![alt text](https://user-images.githubusercontent.com/41392462/145302452-fbd130a6-b22b-41c2-a7ef-3f7df38b8b08.jpeg)
