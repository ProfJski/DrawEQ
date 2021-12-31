#include <iostream>
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

using namespace std;

// Draws a multiband equalizer that can be used to quickly enter many float values or to display data
// float* values points to a float array of at least numBands elements where the values are stored.  Output range is 0.0-1.0
// numBands = the number of EQ bands.   numLevels = the number of levels per band
// You can have zero padding between bands or levels.  Some border around the whole control (borderWidth) is necessary to select 0 which requires clicking beneath the lowest
// "LED" light but still within the bounds of the control.
// drawValues will display the current float value of each band beneath it.  enabled allows input; set to false for display only

void DrawEQ(Rectangle bounds, float* values, int borderWidth, int numBands, float bandsPadding, int numLevels, float levelsPadding, bool enabled, bool drawValues) {

    float bandsWidth = (bounds.width - 2*borderWidth - bandsPadding*(numBands+1))/numBands;
    float levelHeight = (bounds.height - 2*borderWidth - levelsPadding*(numLevels+1))/numLevels;
    Rectangle littleLED = (Rectangle) {0.0,0.0,bandsWidth,levelHeight};
    float levelStep = 1.0/(float) numLevels;
    Vector2 mousePosition = (Vector2) {0.0,0.0};
    char text[6];

    GuiDrawRectangle(bounds,5,LIGHTGRAY,LIGHTGRAY);
    for (int i=0; i<numBands; i++) {
        littleLED.x=bounds.x+bandsPadding+borderWidth+(bandsPadding+bandsWidth)*i;
        for (int j=0;j<numLevels;j++) {
            littleLED.y=bounds.y+bounds.height-borderWidth-(levelsPadding+levelHeight)*(j+1);
            DrawRectangleRec(littleLED,(values[i]>j*levelStep*1.01)?ORANGE:DARKGRAY); //levelStep*1.01 so float comparison works OK
            if ( IsMouseButtonDown(MOUSE_LEFT_BUTTON) && enabled ) {
                mousePosition = GetMousePosition();
                if (CheckCollisionPointRec(mousePosition,littleLED)) {
                    values[i]=(j+1)*levelStep;
                }
                //Else if the mouse is below our lowest LED but still within the bounding box of the EQ control, set the value to zero
                else if ( (mousePosition.x>=littleLED.x) && (mousePosition.x<=littleLED.x+littleLED.width) && (mousePosition.y>(bounds.y+bounds.height-levelsPadding-borderWidth)) && (mousePosition.y<=bounds.y+bounds.height) ) {
                    values[i]=0.0;
                }
            }
            if (drawValues) {
                snprintf(text,6,"%1.6f",values[i]);
                DrawText(text,littleLED.x,bounds.y+bounds.height+10,10,RED);
            }
        }
    }

return;
}

int main()
{

    const int screenWidth = 1200;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "");
    SetTargetFPS(60);

    Rectangle EQ_Area = (Rectangle) {200,300,800,400};
    float EQvals[32] = {0.5,0.2,0.1,0.0,1.0,0.9,0.8,0.7,0.6,0.5,0.4,0.3,0.2,0.1,0.0,1.0,0.5,0.2,0.1,0.0,1.0,0.9,0.8,0.7,0.6,0.5,0.4,0.3,0.2,0.1,0.0,1.0};

    //Some controls to demonstrate the various parameters of the equalizer
    Rectangle gc_nb_spinner = (Rectangle) {200,50,100,50};
    int nB = 20; //Number of EQ bands
    Rectangle gc_nl_spinner = (Rectangle) {500,50,100,50};
    int nL = 20; //Number of EQ levels per band
    Rectangle gc_bandpadding_spinner = (Rectangle) {200,150,100,50};
    int bPad = 10; // Padding between EQ bands
    Rectangle gc_levelpadding_spinner = (Rectangle) {500,150,100,50};
    int lPad = 2; // Padding between EQ levels
    Rectangle gc_drawvalues_checkbox = (Rectangle) {700,50,20,20};
    bool drawvalues = true;
    Rectangle gc_movingdisplay_checkbox = (Rectangle) {700,150,20,20};
    bool movingdisplay = false;

    float timer = 0.0;

while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        //Controls to show all the options
        GuiSpinner(gc_nb_spinner,"Number of Bands",&nB,1,30,false);
        GuiSpinner(gc_nl_spinner,"Number of Levels",&nL,1,30,false);
        GuiSpinner(gc_bandpadding_spinner,"Padding between Bands",&bPad,0,10,false);
        GuiSpinner(gc_levelpadding_spinner,"Padding between Levels",&lPad,0,10,false);
        drawvalues=GuiCheckBox(gc_drawvalues_checkbox,"Draw Values Beneath",drawvalues);
        movingdisplay=GuiCheckBox(gc_movingdisplay_checkbox,"Moving display (like audio)",movingdisplay);

        //The function call itself
        DrawEQ(EQ_Area,EQvals,5,nB,bPad,nL,lPad,true,drawvalues);

        EndDrawing();

        //Makes random waves to demonstrate using the EQ as a display rather than an input
        if (movingdisplay) {
            timer+=0.005;
            for (unsigned int i=0;i<32;i++) {
                EQvals[i] = sin( (timer+(float)i/32)*20 )+ sin( (timer+(float)i/32)*40 ) + (1.0/(float)(3+rand()%10));

            }
        }
    }

    CloseWindow();
    return 0;
}
