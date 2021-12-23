# Matrix Game Arduino
 
 ## Requirements
Nerdy game which has the purpose to test your intelligence. Do you think you can remember the position of each point on a 8x8 matrix and its power of lightening? Think twice! First, you are welcomed with a greeting message and then entered in the navigation menu. All of them are displayed on a Liquid Crystal Display.
 ## Implementation Details
The greeting message is shown on the display for a few moments, then you can navigate through the menu with a joystick. Depending on the option (Start Game, HighScore, Settings, About) you can either start the game or enter in a submenu, like how it happens in Settings.
 - Start game: starts the game with the current selected level (or by default, level 1).
 - Settings: lets you set the matrix brightness or the contrast and brightness LCD.
 - HighScore: top 3 best highscores read from the memory containing the name of the user and the score.
 - About: informations about the game (name, author and github link).
 ### Game logic
First, a figure is shown on the matrix displayed for 10 seconds, containing points in different positions wih different types of blinking (depending on the difficulty of the level). Then, the player needs to remember the location of the points and their type. Based on that, he is navigating on the matrix with the joystick (left-right, top-bottom and diagonally) and presses the button of the joystick to fix the point on the matrix. After fixing the point, he can use the y-axis to chose the blinking type and press once more the button to validate that choice for the point. If it was a good move, the score will increase and the point will remain on the matrix, otherwise the number of lives will be decreased. While playing, the number of lives, the score and level are displayed on the LCD.
### How to play
The game starts with a figure shown on the matrix for 5 seconds and the player must remake it by slecting positions for the point. When all the points from the figure are selected, if the player has more than 1 live, then he is asked if he goes to the next level.  
## Setup picture  
![alt text](https://user-images.githubusercontent.com/41392462/145302452-fbd130a6-b22b-41c2-a7ef-3f7df38b8b08.jpeg)
