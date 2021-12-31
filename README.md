# DrawEQ
The idea was a classic multiband EQ with many columns of sliders on a mixing board.

Allows lightning-fast input of an array of float values in a second.  Can also be used for a display of normalized float values.

![ScreenShot](images/EQ.png)

In input mode, simply left-click and mouse over the levels you want to input the values.
Click just below the lowest LED to set the level to zero.

Uses RayLib and RayGUI.  @raysan5: It could be made into a standard RayGUI control with a little work to integrate GuiState handling and GetStyle() for colors.
