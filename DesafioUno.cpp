#ifdef DESAFIOUNO_H_
#else
#define DESAFIOUNO_H_
#include <stdlib.h>
#include "Robot.cpp"
#include "algorithms/PID.cpp"
class DesafioUno{
public:
    DesafioUno(Robot &robot, int laps, int velocidad = 150, float k_p = 20.0f, float k_i = 10000.0f, float k_d = 1.5f) :
        robot(robot),
        laps(laps)
        {
            pid = PID(velocidad, k_p, k_i, k_d);
            robot.set_timing(1);
        };

    ~DesafioUno(){

    };

    void run(){
        // static unsigned long elapsed_time = 0;
        static unsigned long last_time = 0;
        static bool waiting = false;
        int current_laps = 0;
        while(current_laps < laps){
            if(robot.on(0) && robot.on(4) && !waiting ){
                last_time = get_ms();
                current_laps++;                                         //encuentra una vuelta
                robot.reset_warnings();                                 //reinicia el contador
                waiting = true;                                         //indica que esperará un tiempo
                // return;
            }
            if(waiting && (get_ms() - last_time  > 1000))
                    waiting = false;

            // while(1){
                pid.add_reading(robot.read_line());                  //sigue la línea
                robot.set_motors(pid.left, pid.right);                      //potencia
            // }
        }
        // clear();
        // print("Finish.\n");
    }
private:
    Robot robot;
    int laps;
    PID pid;

};
#endif
