// set this to the hardware serial port you wish to use
#define HWSERIAL Serial1
#define BIT_0        0b00000001
#define BIT_1        0b00000010
#define BIT_2        0b00000100
#define BIT_3        0b00001000
#define BIT_4        0b00010000
#define BIT_5        0b00100000
#define BIT_6        0b01000000
#define BIT_7        0b10000000
#define BIT_CLEAR    0b00000000

#define UP      0
#define DOWN    2
#define LEFT    1
#define RIGHT   3

#define FORWARD 0
#define STOPPED 1

unsigned char bitmask[8] = {BIT_0, BIT_1, BIT_2, BIT_3, BIT_4, BIT_5, BIT_6, BIT_7};

int led = 13;

int x_pos = 0;
int y_pos = 0;
char direction;
char forward;
unsigned char maze[20001];
long xdim=400, ydim=400;
unsigned long elapsed_time = 0;
unsigned long current_time = 0;

void bit_on(long xidx, long yidx){
    xidx+=200;
    yidx+=200;
    long index = (xdim * yidx) + xidx;
    maze[index>>3] |= bitmask[index%8];
}

void bit_off(long xidx, long yidx){
    xidx+=200;
    yidx+=200;
    long index = (xdim * yidx) + xidx;
    maze[index>>3] &= !bitmask[index%8];
}

bool check_bit(long xidx, long yidx){
    xidx+=200;
    yidx+=200;
    long index = (xdim * yidx) + xidx;
    return ((maze[index>>3]&bitmask[index%8]) == bitmask[index%8]);
}

void init_bit_array(){
    long bit_array_size = (20001/8)+1;
    for(long i=0; i<bit_array_size; i++){
        maze[i] = BIT_CLEAR;
    }
    return;
}

void setup() {
    Serial.begin(115200);
    HWSERIAL.begin(115200);
    pinMode(led, OUTPUT);
    init_bit_array();
    direction = UP;
    forward = FORWARD;
}

bool led_on = false;
bool routine = false;
bool loop_detected = false;

void blink_led(int time){
        delay(time);
        digitalWrite(led, HIGH);
        delay(time);
        digitalWrite(led, LOW);
}

void print_status(int range){
    Serial.println();
    for(int i=range; i>-range;i--){
        for(int j=-range; j<range;j++){
            Serial.print(check_bit(j,i) ? 1 : 0);
        }
        Serial.println();
    }
    Serial.println("Le* Matrix");
    Serial.println(String("Time: ") + elapsed_time);
    Serial.println(String("Current Time: ") + current_time);
    Serial.println( String("Forward: ") + ((forward==FORWARD)? "yes" : "no" ));
    Serial.println((int) direction);
    Serial.print( String("Direction: "));
    switch (direction) {
        case LEFT:
          Serial.println("Left");
          break;
        case RIGHT:
          Serial.println("Right");
          break;
        case UP:
          Serial.println("Up");
          break;
        case DOWN:
          Serial.println("Down");
          break;
        default:
          break;
    }
}

void loop() {
    current_time = micros();
    int incomingByte;

    if(loop_detected){
        // blink_led(50);
    }

/************ OPERACIONES USB-TEENSY ***********/
    if (Serial.available() > 0) {
        digitalWrite(led, HIGH); 

        incomingByte = Serial.read();

        //If the robot stops
        if(incomingByte == 'S'){
            routine = false;
            Serial.println("AVR stop signal transmitted.");
            HWSERIAL.write('S');
        }

        //If status is requested
        if(incomingByte == 'P'){
            print_status(50);
        }

        //
        if(incomingByte == 'G'){
            init_bit_array();
            direction = UP;
            forward = FORWARD;
            routine = true;
            loop_detected = false;
            x_pos = 0;
            y_pos = 0;
            elapsed_time = 0;
            current_time = 0;
            Serial.println("AVR start signal transmitted.");
            HWSERIAL.write('F');
        }
        else{
            Serial.print("USB received → AVR Transmitted: ");
            Serial.println((char)incomingByte);
            HWSERIAL.write((char)incomingByte);
        }
        // delay(100);
        digitalWrite(led, LOW);
        // delay(100);
    } //Fin de operaciones USB-Teensy

/************ CICLO PRINCIPAL ***********/

    //cada 0.5 segundos agrega una lectura
    if( routine  && !loop_detected){
        // Serial.print('.');
        if((elapsed_time >= 1000000/2) && forward == FORWARD){
            elapsed_time = 1;
            if (direction == UP)
                y_pos++;
            if (direction == DOWN)
                y_pos--;
            if (direction == LEFT)
                x_pos--;
            if (direction == RIGHT)
                x_pos++;

            //si ya estaba marcado ese lugar...
            if(check_bit(x_pos, y_pos)){
                HWSERIAL.write('S');
                print_status(50);
                loop_detected = true;
                //TODO, enviar señal de detecnción
            }
            bit_on(x_pos, y_pos);
            
            if(!led_on)
                digitalWrite(led, HIGH);
            else
                digitalWrite(led, LOW);
            led_on = !led_on;

            //TODO: Marcha adelante 
        }
    }

/************ OPERACIONES TEENSY-AVR ***********/
    if (HWSERIAL.available() > 0){
        digitalWrite(led, HIGH); 
        Serial.print("AVR received → USB Transmitted: ");
        char character;
        character = HWSERIAL.read();
        Serial.print((char)character);
        if(character == 'P'){
            print_status(50);
        }

        //operación de giro
        else if(character == 'G'){
            init_bit_array();
            direction = UP;
            forward = FORWARD;
            routine = true;
            loop_detected = false;
            x_pos = 0;
            y_pos = 0;
            elapsed_time = 0;
            current_time = 0;
            Serial.print("AVR start signal transmitted.");
            HWSERIAL.write('F');
        }

        //operación de giro
        else if(character == 'L'){
            Serial.print("Turn left!");
            Serial.println();
            direction = (direction + 1)%4;
            HWSERIAL.write('L');
            HWSERIAL.write('F');
        }
        else if(character == 'R'){
            Serial.print("Turn right!");
            Serial.println();
            direction = (direction - 1)%4;
            HWSERIAL.write('R');
            HWSERIAL.write('F');
        }
        else if(character == 'T'){
            Serial.print("Turn around!");
            Serial.println();
            direction = (direction - 2)%4;
            HWSERIAL.write('T');
            HWSERIAL.write('F');
        }
        else if(character == 'I'){
            Serial.print("Intersection!");
            Serial.println();
            HWSERIAL.write('I');
            HWSERIAL.write('F');
            //add to array
        }
        else if(character == 'F'){
            Serial.print("Forward...");
            Serial.println();
            forward = FORWARD;
            // HWSERIAL.write('F');
        }
        else if(character == 'S'){
            Serial.print("Stop!");
            Serial.println();
            forward = STOPPED;
            // HWSERIAL.write('S');
        }
        else if(character == 'H'){
            Serial.print("Routine halted");
            Serial.println();
            routine = false;
        }
        else if(character == 'N'){
            Serial.print("Routine resumed");
            Serial.println();
            routine = true;
        }
    }

    if(direction == 255){
        direction = RIGHT;
    }
    if(direction == 254){
        direction = DOWN;
    }
    //TEENSY-AVR routine end


/*************************** TIME CALCULATIONS *******************/     
    if(forward == FORWARD)
        elapsed_time += micros() - current_time;

    digitalWrite(led, LOW); 
}