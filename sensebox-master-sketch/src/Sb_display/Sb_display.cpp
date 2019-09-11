#include "Sb_display.h"


void Sb_display::startDisplay(Adafruit_SSD1306 display){

    display.begin(SSD1306_SWITCHCAPVCC, 0x3D);
    display.display();
    delay(1000);
    display.clearDisplay();

}
void Sb_display::print2Display(String message,int x, int y,int textSize){

    display.setCursor(x,y);
    display.setTextSize(textSize);
    display.setTextColor(WHITE,BLACK);
    display.println(message);
    display.display();
}
