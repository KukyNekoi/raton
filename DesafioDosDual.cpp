#ifdef DESAFIODOSDUAL_H_
#else
#define DESAFIODOSDUAL_H_
#include <stdlib.h>
#include <math.h>
#include "Robot.cpp"
// #include "algorithms/Maze.cpp"
// #define PI 3.14163264

// #define LEFT    'L'
// #define RIGHT   'R'
// #define LEFT    'F'
// #define RIGHT   'S'


class DesafioDosDual{
public:
    DesafioDosDual(Robot &robot, int velocidad = 150, float k_p = 20.0f, float k_i = 10000.0f, float k_d = 1.5f):
        robot(robot)
        {
            robot.set_timing(30);
            pid = PID(velocidad, k_p, k_i, k_d);
            motors_on = false;
            pending_actions = false;
            // maze = Maze(400L,400L);
        };

    void resume_logger(){
        this->robot.serial.write('N');
    }

    void halt_logger(){
        this->robot.serial.write('H');
    }

    void wait_for_teensy(){
        this->robot.stop_motors();
        while(!this->do_teensy_action()){};
        return;
    }

    bool do_teensy_action(){
        if(robot.serial.data_avaliable()){
            switch (robot.serial.read_byte()) {
                case 'L':
                    robot.left_turn();
                    play_from_program_space(tune1);
                    robot.center();
                    play_from_program_space(tune2);
                    robot.stop_motors();
                    pid.reset();
                    robot.reset_warnings();
                    break;
                case 'R':
                    robot.right_turn();
                    play_from_program_space(tune1);
                    robot.center();
                    play_from_program_space(tune2);
                    robot.stop_motors();
                    pid.reset();
                    robot.reset_warnings();
                    break;
                case 'T':
                    robot.u_turn();
                    play_from_program_space(tune1);
                    robot.center();
                    play_from_program_space(tune2);
                    robot.stop_motors();
                    pid.reset();
                    robot.reset_warnings();
                    break;
                case 'F':
                    this->robot.serial.write('F');
                    motors_on = true;
                    break;
                case 'S':
                    this->robot.serial.write('S');
                    motors_on = false;
                    break;
                case 'C':
                    robot.center();
                    motors_on = false;
                    break;
            }
            return true;
        }
        return false;
    }

    void run(){
        static unsigned int elapsed_time = 0;
        while(1){                                                   //Mientras no complete dos vueltas
            
            if (this->do_teensy_action())
                continue;
            
            if(motors_on){
                if(robot.boundary_warning()){
                    // last_time = TCNT2;                                      //Setea el contador de tiempo 
                    this->halt_logger();
                    while(elapsed_time < 60){
                        elapsed_time ++;
                        pid.add_reading(robot.read_narrow_line());              //sigue la línea
                        robot.set_motors(pid.left, pid.right);                  //potencia
                        delay_ms(1);
                    } elapsed_time = 0;

                    if(robot.left_warning() && !robot.right_warning()){
                        robot.serial.write('L');
                        this->wait_for_teensy();
                        // robot.left_turn();
                    }

                    else if(!robot.left_warning() && robot.right_warning()){
                        robot.serial.write('R');
                        this->wait_for_teensy();
                        // robot.left_turn();
                    }

                    else if(robot.intersection_warning()){
                        robot.serial.write('I');
                        this->wait_for_teensy();
                        // robot.left_turn();
                    }

                    else if(robot.straight_line_warning()){
                        robot.reset_warnings();
                        // continue;
                    }

                    // this->do_teensy_action();
                    this->resume_logger();

                    continue;
                }
                else if(robot.deadend_warning() && !robot.boundary_warning()){                            //Detecta callejón sin salida
                    this->halt_logger();
                    robot.serial.write('T');
                    this->wait_for_teensy();
                    this->resume_logger();
                    continue;
                }
                
                robot.scan();
                pid.add_reading(robot.read_line());              //sigue la línea
                robot.set_motors(pid.left, pid.right);                  //potencia

            }
            else{
                robot.stop_motors();
            }
        }
        clear();
        print("Finish.\n");
    }
private:
    Robot robot;
    PID pid;
    bool motors_on;
    bool pending_actions;
    // Maze maze;
};
#endif
