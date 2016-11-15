#ifdef ROBOT_H_
#else
#define ROBOT_H_

#include <pololu/Pololu3pi.h>
#include "algorithms/algorithms.c"
#include "algorithms/Serial.cpp"


class Robot
{
public:
    Robot(int refractance_delay_us = 1000, unsigned int deadzone=100, unsigned int threshold=300, unsigned int timing = 35):
    left_motor(0),
    right_motor(0),
    deadzone(deadzone),
    threshold(threshold),
    timing(timing){
        this->sensor_timing[0] = 0;
        this->sensor_timing[1] = 0;
        this->sensor_timing[2] = 0;
        this->sensor_timing[3] = 0;
        this->sensor_timing[4] = 0;
        Pololu3pi().init(refractance_delay_us);
        this->robot = Pololu3pi();
        this->serial = Serial();
        this->robot.emittersOn();
        this->reset_warnings();
    };
    ~Robot(){};

    Serial serial;

    void set_timing(unsigned int timing){
        this->timing = timing;
    }

    bool intersection_warning(){
        return this->intersection_detected;
    };

    bool deadend_warning(){
        return this->deadend_detected;
    };

    bool boundary_warning(){
        return this->left_boundary || this->right_boundary;
    };

    bool straight_line_warning(){
        return this->straight_line;
    };

    bool right_warning(){
        return this->right_boundary;
    };

    bool left_warning(){
        return this->left_boundary;
    };

    void reset_warnings(){
        this->left_inner_boundary = false;
        this->right_inner_boundary = false;
        this->left_boundary = false;
        this->right_boundary = false;
        this->intersection_detected = false;
        this->deadend_detected = false;
        this->straight_line = true;
        this->sensor_timing[1] = timing + 1;
        this->sensor_timing[2] = timing + 1;
        this->sensor_timing[3] = timing + 1;
    }

    void reset_intersection(){
        this->intersection_detected = false;
    }
    void reset_straight_line(){
        this->intersection_detected = false;
    }

    inline bool on(unsigned int index){
        return sensor_values[index] >= threshold + deadzone;
    }

    inline bool off(unsigned int index){
        return sensor_values[index] < threshold - deadzone;
    }

    void scan(){
        // play_from_program_space(tune1);
        //DIRECT COMPUTE
        read_line_sensors_calibrated(this->sensor_values, IR_EMITTERS_ON);

        if(this->on(0) && this->on(1) && this->on(2) && this->on(3) && this->on(4)){
            intersection_detected = true;
        }

        if(this->on(4)){
            right_boundary = true;
        }

        if(this->on(0)){
            left_boundary = true;
        }

        if((this->on(1) || this->on(2) || this->on(3)) && !(this->on(0) || this->on(4))){
            straight_line = true;
        }
        if(!( this->on(1) || this->on(2) || this->on(3))){
            deadend_detected = true;
            // play_from_program_space(tune1);
        }
    };

    unsigned int read_line(){
        this->scan();
        return this->robot.readLine(this->sensor_values, IR_EMITTERS_ON,0);
    };         

    /*
        IT'S SUPPOSED TO BRING VALUES BETWEEN 1000~3000
    */
    int read_narrow_line(){
        unsigned char i, on_line = 0;
        unsigned long avg;
        unsigned int sum;
        static int last_value=0;

        // this->robot.readLineSensorsCalibrated(this->sensor_values, IR_EMITTERS_ON);
        this->scan();

        avg = 0;
        sum = 0;
      
        for(i=1;i<4;i++) {
            int value = sensor_values[i];

            if(value > 200) {
                on_line = 1;
            }
            
            // only average in values that are above a noise threshold
            if(value > 50) {
                avg += (long)(value) * (i * 1000);
                sum += value;
            }
        }


        if(!on_line)
        {
            // If it last read to the left of center, return 0.
            if(last_value < (5-1)*1000/2)
                return 0;
            
            // If it last read to the right of center, return the max.
            else
                return (5-1)*1000;

        }

        last_value = avg/sum;

        return last_value;
    }

    void calibrate(){
        delay_ms(300);
        for(int counter=0;counter<80;counter++){                                    //CALIBRATION ROUTINE BEGIN
            if(counter < 20 || counter >= 60)
                set_motors(65,-65);
            else
                set_motors(-65,65);
            this->robot.calibrateLineSensors(IR_EMITTERS_ON);
            delay_ms(2);
        }
        set_motors(0,0);                                                            //CALIBRATION ROUTINE END
    } 

    void wait_for_button(const char *string, unsigned int button, bool debug = false){
        while(!button_is_pressed(button)){
            clear();
            print_from_program_space(string);
            // print(string);
            lcd_goto_xy(0,1);
            if(debug){
                this->scan();
                print_unsigned_long(this->sensor_values[0]/110);
                lcd_goto_xy(1,1);
                print_unsigned_long(this->sensor_values[1]/110);
                lcd_goto_xy(2,1);
                print_unsigned_long(this->sensor_values[2]/110);
                lcd_goto_xy(3,1);
                print_unsigned_long(this->sensor_values[3]/110);
                lcd_goto_xy(4,1);
                print_unsigned_long(this->sensor_values[4]/110);
            }
            else{
                switch(button){
                    case BUTTON_A:
                    print("Press A");   
                    break;
                    case BUTTON_B:
                    print("Press B");   
                    break;
                    case BUTTON_C:
                    print("Press C");   
                    break;
                    default:
                    break;
                }
            }
            delay_ms(20);
        }
        delay_ms(100);
    }

    void wait_for_button(int value, unsigned int button, bool debug = false){
        while(!button_is_pressed(button)){
            clear();
            print_long((long)value);
            lcd_goto_xy(0,1);
            if(debug){
                this->scan();
                print_unsigned_long(this->sensor_values[0]/110);
                lcd_goto_xy(1,1);
                print_unsigned_long(this->sensor_values[1]/110);
                lcd_goto_xy(2,1);
                print_unsigned_long(this->sensor_values[2]/110);
                lcd_goto_xy(3,1);
                print_unsigned_long(this->sensor_values[3]/110);
                lcd_goto_xy(4,1);
                print_unsigned_long(this->sensor_values[4]/110);
            }
            else{
                switch(button){
                    case BUTTON_A:
                    print("Press A");   
                    break;
                    case BUTTON_B:
                    print("Press B");   
                    break;
                    case BUTTON_C:
                    print("Press C");   
                    break;
                    default:
                    break;
                }
            }
            delay_ms(20);
        }
        delay_ms(500);
    }



    void center(){
        int powerdifference = (((int)this->read_line())-2000)/10;
        
        clear();
        while(abs(powerdifference) > 30){
            powerdifference = (((int)this->read_line())-2000)/10;
            set_motors(powerdifference*2/3,-powerdifference*2/3);
        }
    }


    void u_turn(){
        // this->set_motors(115,-130);
        // delay_ms(230);

        this->set_motors(-90,90);
        delay_ms(400);
    }

    void left_turn(){

        // stop_motors();
        // delay_ms(75);
        this->set_motors(-100,100);
        delay_ms(180);
        // this->set_motors(-40,40);
        // delay_ms(40);
        // this->set_motors(-115,115);
        // delay_ms(90);
        // this->set_motors(-40,40);
        // delay_ms(30);
    }

    void right_turn(){

        // stop_motors();
        // delay_ms(75);
        this->set_motors(100,-100);
        delay_ms(180);
        // this->set_motors(40,-40);
        // delay_ms(40);
        // this->set_motors(115,-115);
        // delay_ms(90);
        // this->set_motors(40,-40);
        // delay_ms(30);
    }


    void set_motors(int left, int right){
        this->left_motor = left;
        this->right_motor = right;
        OrangutanMotors::setSpeeds(left, right);
    }
    void ponderate_motors(int num, int den){
        OrangutanMotors::setSpeeds(this->left_motor*num/den, this->right_motor*num/den);
    }

    // void set_motors(int left_power, int right_power, int msecs){
    //   float factor;
    //   for(int i=0; i<msecs; i++){
    //     factor = normal_distribution(i, msecs);
    //     this->left_motor = (int)(factor*left_power);
    //     this->right_motor = (int)(factor*right_power);
    //     this->set_motors(this->left_motor, this->right_motor);
    //     delay_ms(1);
    //   }
    // }

    // void invert_motor_proportion(){
    //     int bubble = this->left_motor;
    //     this->left_motor = this->right_motor;
    //     this->right_motor = bubble;
    //     OrangutanMotors::setSpeeds(left_motor, right_motor);
    // }

    // void brake(int msecs){
    //   float factor = 0;
    //   int left_power = this->left_motor;
    //   int right_power = this->right_motor;
    //   int i =0;
    //   for(; i<msecs && !(this->left_motor ==0 && this->right_motor == 0); i++){
    //     factor += normal_distribution_factor(i, msecs,0,2.19);
    //     this->set_motors((int)((1.0-factor)*(float)left_power), (int)((1.0-factor)*(float)right_power));
    //     delay_ms(1);
    //   }
    //   wait_for_button(i,BUTTON_B);
    // }

    void stop_motors(){
        this->set_motors(0,0);
    }

    void finish(){
        while(1){}
    }
private:
    Pololu3pi robot;
    int left_motor = 0, right_motor = 0;
    unsigned int sensor_values[5];
    unsigned int sensor_timing[5];
    unsigned int deadzone, threshold, timing;
    bool left_boundary = false , right_boundary = false;
    bool left_inner_boundary = false , right_inner_boundary = false;
    bool intersection_detected = false, deadend_detected = false, straight_line = false;
};

#endif