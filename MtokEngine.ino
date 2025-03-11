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
    String name = "object1";

    Point position = { 0, 0, -4 };

    Point corners[8] = { // the coords of each point
        { 1, -1, -1},
        { 1, -1, 1},
        { 1,  1, -1},
        { 1,  1, 1},
        {-1, -1, -1},
        {-1, -1, 1},
        {-1,  1, -1},
        {-1,  1, 1}
    };
    
    Point r_corners[8];// the position of each point ON screen

    float collisions[6] = {// hitbox
        0.5, -0.5, 0.5, -0.5, 0.5, -0.5 // x x   y y   z z  (ranges) (colliders are all cuboids)
    };





    void move(float x, float y, float z){ // moves the object by x y z
        position.x += x;
        position.y += y;
        position.z += z;
    }

    void setPosition(float x, float y, float z){ // moves the object by x y z
        position.x = x;
        position.y = y;
        position.z = z;
    }

    
    // collision related functions
    float calculateCollision(GameObject object) {
        float posX = position.x;
        float posY = position.y;
        float posZ = position.z;
        float left = collisions[0] + posX;
        float right = collisions[1] + posX;
        float top = collisions[2] + posY;
        float bottom = collisions[3] + posY;
        float front = collisions[4] + posZ;
        float back = collisions[5] + posZ;

        float objLeft = object.collisions[0] + object.position.x;
        float objRight = object.collisions[1] + object.position.x;
        float objTop = object.collisions[2] + object.position.y;
        float objBottom = object.collisions[3] + object.position.y;
        float objFront = object.collisions[4] + object.position.z;
        float objBack = object.collisions[5] + object.position.z;

        Serial.println(String(objLeft) + ">" +  String(right)  + " " + String(objRight) + "<" +  String(left));

        bool xCol = (objLeft > right) & (objRight < left);
        bool yCol = (objTop < bottom) & (objBottom > top);
        bool zCol = (objFront < back) & (objBack > front);
        //Serial.println(String(xCol) + " " +  String(yCol)  + " " + String(zCol));
        if(xCol && yCol && zCol){
            Serial.println("COLLISION!!");
        }
        return 0.0;
    }


    ///////////// rendering related functions
    void calculateSC(){ // calculate r_corners
        for(int i = 0; i < 8; i++){
            float x = (corners[i].x + position.x) / (corners[i].z + position.z);
            float y = (corners[i].y + position.y) / (corners[i].z + position.z);
            float end_x = (int)((x*64.0+128.0)/2);
            float end_y = (int)((y*64.0+64.0)/2);
            r_corners[i].x = end_x;
            r_corners[i].y = end_y;
            //Serial.println(String(end_x) + " " + String(end_y));
        }
    }
    void drawLine(int i1, int i2){
        if((corners[i1].z + position.z) >= 0 || (corners[i2].z + position.z) >= 0) return;

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
///////////////////////////////////////////////////////////////////////////////
GameObject object;
GameObject object2;
void setup(void){
    d.begin(0, true);
    d.clearDisplay();
    Serial.begin(115200); // for debugging purposes
    Serial.println("hii");

    // for input from pin 
    pinMode(21, INPUT);
    digitalWrite (21, HIGH);
    pinMode(19, INPUT);
    digitalWrite (19, HIGH);

    // temporary
    object2.setPosition(0.0,0.0,-4.0);
}




;
void gameLoop(){
    if (button1 == HIGH) object.move(0.1, 0, 0); // Move object based on input
    if (button2 == HIGH) object.move(-0.1, 0, 0);

    //if (button1 == HIGH) object2.move(0, 0, 0.1); // Move object based on input
    //if (button2 == HIGH) object2.move(0, 0, -0.1);
    //if (object2.position.y > 3) direction2 = -0.1;
    //if (object2.position.y < -3) direction2 = 0.1;
    //if (button2 == HIGH) object2.move(0, direction2, 0); // Move object2 based on input
}

void renderLoop(){

    object.calculateSC();

    object.render();

    object.calculateCollision(object2);

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


    delay(8);
}

