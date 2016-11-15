#ifdef DESAFIODOS_H_
#else
#define DESAFIODOS_H_
#include <stdlib.h>
#include <math.h>
#include "Robot.cpp"
// #include "algorithms/Maze.cpp"
// #define PI 3.14163264

// #define LEFT    'L'
// #define RIGHT   'R'
// #define LEFT    'F'
// #define RIGHT   'S'


class DesafioDos{
public:
    DesafioDos(Robot &robot, int velocidad = 150, float k_p = 20.0f, float k_i = 10000.0f, float k_d = 1.5f):
        robot(robot)
        {
            // robot.set_timing(30);
            pid = PID(velocidad, k_p, k_i, k_d);
            // motors_on = false;
            // pending_actions = false;
            // maze = Maze(400L,400L);
            route_lenght[0] = 0;
            route_lenght[1] = 0;
            // route = "";
        };


    //TODO path simplification

    // void add_step(char direction){
    // //     if(direction == 'T' && route_lenght[route_lenght-1] = 'R'){
    // //         route[route_lenght-1] = 'L';    
    // //     }

    // //     if(direction == 'T' && route_lenght[route_lenght-1] = 'R'){
    // //         route[route_lenght-1] = 'L';    
    // //     }
    // //     route[route_lenght] = direction;
    // //     route_lenght++;
    // //     route[route_lenght] = '\0';
    // }

    void follow_line_until_event(){
        this->pid.reset();
        this->robot.scan();
        while(  !(this->robot.on(0) || this->robot.on(4)) &&
                (this->robot.on(0) || this->robot.on(1) || this->robot.on(2) || this->robot.on(3) || this->robot.on(4))    ){
            pid.add_reading(robot.read_narrow_line());                  //sigue la línea
            robot.set_motors(pid.left, pid.right);                      //potencia
        }
    }

    void follow_line_until_msecs(unsigned long msecs){
        unsigned long current_time = get_ms();
        this->pid.reset();
        while(  (get_ms() - current_time < msecs) &&
                (this->robot.on(0) || this->robot.on(1) || this->robot.on(2) || this->robot.on(3) || this->robot.on(4))    ){
            pid.add_reading(robot.read_narrow_line());                  //sigue la línea
            robot.set_motors(pid.left, pid.right);                      //potencia
        }
    }

    void follow_line_ms(unsigned long msecs){
        unsigned long current_time = get_ms();
        while(get_ms() - current_time < msecs){
            // pid.add_reading(robot.read_narrow_line());                  //sigue la línea
            pid.add_reading(robot.read_narrow_line());                  //sigue la línea
            this->robot.set_motors(pid.left, pid.right);                      //potencia
        }
    }

    void advance_ms(unsigned long msecs){
        unsigned long current_time = get_ms();
        while(get_ms() - current_time < msecs){
            // int left_m = this->robot.left
            this->robot.set_motors(180, 180);                      //potencia
        }
    }

    // Displays the current path on the LCD, using two rows if necessary.
    void display_path(int stage)
    {
        // Set the last character of the path to a 0 so that the print()
        // function can find the end of the string.  This is how strings
        // are normally terminated in C.
        route[stage][route_lenght[stage]] = 0;

        clear();
        print(route[stage]);

        if(route_lenght[stage] > 8)
        {
            lcd_goto_xy(0,1);
            print(route[stage]+8);
        }
    }


    //RIGHT HAND ALGORITHM
    char select_turn(bool left, bool forward, bool right){
        if(!lefthanded){
            if(right)
                return 'R';
            if(forward)
                return 'S';
            if(left)
                return 'L';
            else
                return 'B';   
        }
        else{
            if(left)
                return 'L';
            if(forward)
                return 'S';
            if(right)
                return 'R';
            else
                return 'B';      
        }
    }

    void simplify_path(int stage)
    {
    //     // only simplify the route if the second-to-last turn was a 'B'
        if(this->route_lenght[stage] < 3 || this->route[stage][this->route_lenght[stage]-2] != 'B')
            return;

        int total_angle = 0;
        int i;
        for(i=1;i<=3;i++)
        {
            switch(this->route[stage][this->route_lenght[stage]-i])
            {
            case 'R':
                total_angle += 90;
                break;
            case 'L':
                total_angle += 270;
                break;
            case 'B':
                total_angle += 180;
                break;
            }
        }

        // Get the angle as a number between 0 and 360 degrees.
        total_angle = total_angle % 360;

        // Replace all of those turns with a single one.
        switch(total_angle)
        {
        case 0:
            route[stage][route_lenght[stage] - 3] = 'S';
            break;
        case 90:
            route[stage][route_lenght[stage] - 3] = 'R';
            break;
        case 180:
            route[stage][route_lenght[stage] - 3] = 'B';
            break;
        case 270:
            route[stage][route_lenght[stage] - 3] = 'L';
            break;
        }

        // The route is now two steps shorter.
        route_lenght[stage] -= 2;
    }

    void learn(int stage, int count_down){

        bool applied = false;
        if(count_down == 0){
            applied = true;
        }

        this->pid.reset();
        this->robot.reset_warnings();

        this->follow_line_ms(250);
        while(1){
            // this->follow_line_ms(200);

            if(!applied && count_down == 0){
                this->follow_line_ms(150);
                this->robot.u_turn();        
                route[stage][route_lenght[stage]] = 'B';
                route_lenght[stage] ++ ;
                this->pid.reset();
                simplify_path(stage);
                applied = true;
            }

            bool left_avaliable = false;
            bool right_avaliable = false;
            bool forward_avaliable = false;
            this->follow_line_until_event();
            // this->advance_ms(1);
            // for(int i = 0; i<first+second+third ; i++){
            //     if(!left_avaliable && this->robot.on(0))
            //         left_avaliable = true;
            //     if(!right_avaliable && this->robot.on(4))
            //        right_avaliable = true;
            //     if(forward_avaliable && !this->robot.on(2)){
            //        forward_avaliable = false;
            //        break;
            //     }
            // }


            //first scanning
            for (int i = 0; i < first; ++i)
            {
                advance_ms(1);
                this->robot.scan();
                if(this->robot.on(0))
                    left_avaliable = true;
                if(this->robot.on(4))
                    right_avaliable = true;

            }

            for (int i = 0; i < second; ++i)
            {
                advance_ms(1);
                if(this->robot.on(1) || this->robot.on(2) || this->robot.on(3))
                    forward_avaliable = true;

            }
            

            //second scanning
            advance_ms(third);
            this->robot.scan();
            if(this->robot.on(0) && this->robot.on(1) && this->robot.on(2) && this->robot.on(3) && this->robot.on(4))
                break;
            if(this->robot.on(1) || this->robot.on(2) || this->robot.on(3))
                    forward_avaliable = true;


            char turn = this->select_turn(left_avaliable, forward_avaliable, right_avaliable);
            
            clear();
            if(left_avaliable){
                print_character('L');
            }
            if(forward_avaliable){
                print_character('F');
            }
            if(right_avaliable){
                print_character('R');
            }
            this->robot.stop_motors();

                print_character(' ');
                print_character(turn);





            this->pid.reset();
            //doing corrective action
            switch(turn){
                case 'L':
                    print_character(' ');
                    print_character('L');
                    // this->robot.stop_motors();
                    // delay_ms(100);

                    set_motors(-107,107);
                    delay_ms(180);

                    // this->robot.left_turn();
                    this->follow_line_ms(follow_line_delay);
                    break;

                case 'R':
                    print_character(' ');
                    print_character('R');
                    // this->robot.stop_motors();
                    // delay_ms(100);

                    set_motors(107,-107);
                    delay_ms(180);


                    // this->robot.right_turn();
                    this->follow_line_ms(follow_line_delay);
                    break;

                case 'B':
                    print_character(' ');
                    print_character('B');
                    // this->robot.stop_motors();
                    // delay_ms(100);
                    this->robot.u_turn();
                    this->follow_line_ms(follow_line_delay);
                    break;

                case 'S':
                    print_character(' ');
                    print_character('S');
                    this->follow_line_ms(follow_line_delay);
                    break;
                default:
                break;
            }

            route[stage][route_lenght[stage]] = turn;
            route_lenght[stage] ++ ;
            this->pid.reset();
            // Simplify the learned path.
            simplify_path(stage);

            // this->robot.set_motors(0,0);
            // while(1);

            // return;

            /*****countdown!*****/
            if(!applied)
                count_down--;
        }



        display_path(stage);
        this->robot.stop_motors();

        this->robot.u_turn();
        this->robot.stop_motors();
        delay_ms(50);
        this->follow_line_ms(120);

        // while(!button_is_pressed(BUTTON_B))
        // {
        //     if(get_ms() % 2000 < 1000)
        //     {
        //         clear();
        //         print("Solved!");
        //         lcd_goto_xy(0,1);
        //         print("Press B");
        //     }
        //     else
        //         display_path();
        //     delay_ms(30);
        // }

        

            clear();
            lcd_goto_xy(0,0);
        for(int i=route_lenght[stage]-1; 0 <= i; i--){
            this->follow_line_until_event();

            //first scanning
            advance_ms(third + second + first+3);

            this->pid.reset();
            switch(route[stage][i]){
                case 'R':

                    print_character('L');
                    this->robot.stop_motors();
                    delay_ms(50);
                    // this->robot.left_turn();

                    set_motors(-107,107);
                    delay_ms(180);
                    this->follow_line_ms(follow_line_delay);
                    break;
                case 'L':
                    print_character('R');
                    this->robot.stop_motors();
                    delay_ms(50);
                    // this->robot.right_turn();

                    set_motors(107,-107);
                    delay_ms(180);

                    this->follow_line_ms(follow_line_delay);
                    break;
                case 'B':
                    this->robot.u_turn();
                    this->follow_line_ms(follow_line_delay);
                    break;
                case 'S':
                    print_character('S');
                    this->follow_line_ms(follow_line_delay);
                    break;
                default:
                    break;
            }   

            this->pid.reset();
        }
        // delay_ms(100);
        this->follow_line_ms(75); 
        this->follow_line_until_event();
        this->advance_ms(150);

        this->robot.stop_motors();

        delay_ms(500);

        this->robot.u_turn();


    }


    void run_maze(int stage){
        this->advance_ms(175);
        for(int i=0; i < route_lenght[stage]; i++){
            this->follow_line_until_event();
            follow_line_ms(first + second + third);

            this->pid.reset();
            switch(route[stage][i]){
                case 'R':
                    print_character('R');
                    this->robot.right_turn();
                    this->follow_line_ms(follow_line_delay);
                    break;
                case 'L':
                    print_character('L');
                    // this->robot.stop_motors();
                    // delay_ms(100);
                    this->robot.left_turn();
                    this->follow_line_ms(follow_line_delay);
                    break;
                case 'B':
                    this->robot.u_turn();
                    this->follow_line_ms(follow_line_delay);
                    break;
                case 'S':
                    print_character('S');
                    this->follow_line_ms(follow_line_delay);
                    break;
                default:
                    break;
            }   
            this->pid.reset();
        }
       
        this->follow_line_until_event();
        this->robot.stop_motors();
        this->robot.u_turn();
        this->follow_line_ms(100);
        // this->advance_ms(150);
        this->robot.stop_motors();
        
        this->pid.reset();
        for(int i=route_lenght[stage]-1; 0 <= i; i--){
            this->follow_line_until_event();
            follow_line_ms(20);

            this->pid.reset();
            switch(route[stage][i]){
                case 'L':
                    print_character('R');
                    // this->robot.stop_motors();
                    // delay_ms(100);
                    this->robot.right_turn();
                    this->follow_line_ms(follow_line_delay);
                    break;
                case 'R':
                    print_character('L');
                    // this->robot.stop_motors();
                    // delay_ms(100);
                    this->robot.left_turn();
                    this->follow_line_ms(follow_line_delay);
                    break;
                case 'B':
                    this->robot.u_turn();
                    this->follow_line_ms(follow_line_delay);
                    break;
                case 'S':
                    print_character('S');
                    this->follow_line_ms(follow_line_delay);
                    break;
                default:
                    break;
            }   
            this->pid.reset();
        }
        // delay_ms(100);

        this->follow_line_until_event();
        this->follow_line_ms(200);
        this->robot.stop_motors();
        this->robot.u_turn();
        this->robot.stop_motors();
        
    }




    bool lefthanded = false;
private:
    const int first = 8, second = 5, third = 3, follow_line_delay = 350;

    Robot robot;
    PID pid;
    bool motors_on;
    bool pending_actions;
    char route[2][150];
    int route_lenght[2];

    char directions[4] = {'U','R','D','L'};
    char current_direction = 0;
    // Maze maze;
};
#endif
