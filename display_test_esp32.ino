/*********************************************************************
  This is an example for our Monochrome OLEDs based on SH110X drivers

  This example is for a 128x64 size display using SPi to communicate
  5 pins are required to interface 

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada  for Adafruit Industries.
  BSD license, check license.txt for more information
  All text above, and the splash screen must be included in any redistribution

  SPi SH1106 modified by Rupert Hirst  12/09/21
*********************************************************************/



#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>


#define OLED_MOSI     23
#define OLED_CLK      18
#define OLED_DC       16
#define OLED_CS       5
#define OLED_RST      17


// Create the OLED display
Adafruit_SH1106G d = Adafruit_SH1106G(128, 64,OLED_MOSI, OLED_CLK, OLED_DC, OLED_RST, OLED_CS);


#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define DISPLAY_HEIGHT 64
#define DISPLAY_WIDTH  128











struct Point {
    float x, y, z;
};

int button1, button2; // input
class GameObject{ // 192 bytes size, so I can only have one object?

    public:
    Point position = { 0, 0, -4 };

    Point corners[8] = { // the coords of each point
        { 1, -1, -5},
        { 1, -1, -3},
        { 1,  1, -5},
        { 1,  1, -3},
        {-1, -1, -5},
        {-1, -1, -3},
        {-1,  1, -5},
        {-1,  1, -3}
    };

    Point r_corners[8];// the position of each point ON screen



    void calculateSC(){ // calculate r_corners
        for(int i = 0; i < 8; i++){
            float x = corners[i].x / corners[i].z;
            float y = corners[i].y / corners[i].z;
            float end_x = (int)((x*64.0+128.0)/2);
            float end_y = (int)((y*64.0+64.0)/2);
            r_corners[i].x = end_x;
            r_corners[i].y = end_y;
            //Serial.println(String(end_x) + " " + String(end_y));
        }
    }

    void move(float x, float y, float z){ // moves the object by x y z
        for(int i = 0; i < 8; i++){
            corners[i].x += x; // the *0.1 is to compensate because when I switched to a function is started overspeeding
            corners[i].y += y;
            corners[i].z += z;
        }
        position.x += x;
        position.y += y;
        position.z += z;
    }

    void drawLine(int i1, int i2){
        if(corners[i1].z >= 0 || corners[i2].z >= 0) return;

        if(r_corners[i1].x < 0) r_corners[i1].x = 0;
        if(r_corners[i1].y < 0) r_corners[i1].y = 0;
        if(r_corners[i2].x < 0) r_corners[i2].x = 0;
        if(r_corners[i2].y < 0) r_corners[i2].y = 0;

        d.drawLine(r_corners[i1].x, r_corners[i1].y, r_corners[i2].x, r_corners[i2].y, 1);
    }

    void render(){
        drawLine(0, 2); // back square
        drawLine(2, 6);
        drawLine(6, 4);
        drawLine(4, 0);

        drawLine(1, 3); // front square
        drawLine(3, 7);
        drawLine(7, 5);
        drawLine(5, 1);

        drawLine(3, 2); // connectors
        drawLine(7, 6);
        drawLine(5, 4);
        drawLine(1, 0);
    }
};

GameObject object;
GameObject object2;
void setup(void){
    d.begin(0, true);
    d.clearDisplay();
    Serial.begin(9600); // for debugging purposes

    // for input from pin 
    pinMode(21, INPUT);
    digitalWrite (21, HIGH);
    pinMode(19, INPUT);
    digitalWrite (19, HIGH);

    // temporary
    object2.move(1.3,0.5,0);
}





float direction = 0.1;
float direction2 = 0.1;
void gameLoop(){
    if (object.position.y > 3) direction = -0.1;
    if (object.position.y < -3) direction = 0.1;
    if (button1 == HIGH) object.move(0, direction, 0); // Move object based on input

    if (object2.position.y > 3) direction2 = -0.1;
    if (object2.position.y < -3) direction2 = 0.1;
    if (button2 == HIGH) object2.move(0, direction2, 0); // Move object2 based on input
}

void renderLoop(){

    object.calculateSC();

    object.render();

    object2.calculateSC();

    object2.render();
}



void loop(void){
    d.clearDisplay();
    
    gameLoop();
    renderLoop();

    d.display();


    // input
    button1 = digitalRead(21);
    button2 = digitalRead(19);
    Serial.println(button2);


    delay(8);
}

