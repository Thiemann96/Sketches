// include guard
#ifndef Sb_display_H
#define Sb_display_H
#define OLED_RESET 4
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include "Sb_display.cpp"
class Sb_display{


    public:
        Sb_display();
        void print2Display(String message,int x,int y,int textSize);
        void startDisplay(Adafruit_SSD1306 display);

    private:

};
Sb_display Sb_displa;
#endif