#include <pololu/Pololu3pi.h>
#include "Tunes.c"
#include "DesafioUno.cpp"
#include "DesafioDos.cpp"
#include "DesafioDosDual.cpp"
#include "Robot.cpp"

#include <avr/pgmspace.h>







// const char start[] PROGMEM = "c32>d32>e32>f32>g32>a32>b32";

// int main()
// {
//     play_from_program_space(start);

//     Robot raton(2000, 100, 250, 10);

//     raton.wait_for_button(READY,BUTTON_B);
//     raton.calibrate();
//     // raton.wait_for_button(DESAFIO_UNO, BUTTON_B, true);

//     DesafioUno desafio1(raton, 2, 255, 25.0f, 15000.0f, 1.3f);

//     // DesafioDos desafio2(raton, 130, 9.0f, 7000.0f, 1.8f);


//     // clear();
//     // lcd_goto_xy(0,0);
//     // print("DesuDesu");

//     while(!button_is_pressed(BUTTON_B)){};
//     // while(button_is_pressed(BUTTON_B)){};
//     unsigned long current_time = get_ms();|
//     clear();
//     lcd_goto_xy(0,0);
//     print("ETA:");
//     // unsigned long remaining_time = 0;
//     while(get_ms() - current_time < 3000){
//         lcd_goto_xy(0,1);
//         print_long(get_ms() - current_time);
//     }

//     lcd_goto_xy(0,1);
//     print_long(3000);

// 	// raton.set_motors(150,150);
// 	// delay_ms(200);
// 	// raton.brake(1000);
// 	// play_from_program_space(end);
// 	// for(int i=50; i< 300; i+=10){
// 	// 	raton.wait_for_button(i, BUTTON_B, true);
//  //        raton.set_motors(150,150);
//  //        delay_ms(150);
//  //        raton.stop_motors();
//  //        delay_ms(100);
//  //        raton.set_motors(-150,150);
//  //        delay_ms(i);
//  //        raton.stop_motors();
//  //        delay_ms(100);
//  //        raton.set_motors(150,150);
//  //        delay_ms(150);
//  //        raton.stop_motors();
//  //    }
//  //        raton.set_motors(-20,20);
//  //        delay_ms(50);
//  //        raton.set_motors(-40,40);
//  //        delay_ms(100);
//  //        raton.set_motors(-120,120);
//  //        delay_ms(i);
//  //        raton.set_motors(-40,40);
// 	// 	delay_ms(100);
//  //        raton.set_motors(-20,20);
//  //        delay_ms(50);
//  //        raton.set_motors(150,150);
//  //        delay_ms(200);
// 	// 	raton.stop_motors();
//  // 	}
//     // while(1){
//     // 	raton.center();
//     // }


//     // while(!button_is_pressed(BUTTON_B));
//     // if(button_is_pressed(BUTTON_B)){
//     //     clear();
//     //     while(button_is_pressed(BUTTON_B));
//     //     delay_ms(100);
//     //     raton.serial.write('G');
//     // }

//     clear();
//     lcd_goto_xy(0,0);
//     print("ONIGOKKO");
//     lcd_goto_xy(0,1);
//     print("  MODE  ");
    

// 	desafio1.run();
//     // raton.center();
// 	// desafio2.run();
// 	raton.stop_motors();

// 	play_from_program_space(end);
//  //    lcd_goto_xy(0,1);
// 	// print("Done.");
// 	raton.finish();	

// 	return 0;
// }






void start_onigokko(Robot &raton){
    DesafioUno desafio1(raton, 4, 240, 15.0f, 13000.0f, 1.25f);

    raton.wait_for_button(READY,BUTTON_B);
    raton.calibrate();
    raton.wait_for_button(READY,BUTTON_B);

    clear();
    lcd_goto_xy(0,0);
    print("ONIGOKKO");
    lcd_goto_xy(0,1);
    print("  MODE  ");
    desafio1.run();
    raton.stop_motors();
    play_from_program_space(end);
}

void start_mayoi(Robot &raton){
    // set up the 3pi
    // initialize();

    // // Call our maze solving routine.
    // maze_solve();

    // // This part of the code is never reached.  A robot should
    // // never reach the end of its program, or unpredictable behavior
    // // will result as random code starts getting executed.  If you
    // // really want to stop all actions at some point, set your motors
    // // to 0,0 and run the following command to loop forever:

    // while(1);

    DesafioDos desafio2(raton, 165, 12.0f, 20000.0f, 1.8f);
    bool lefthanded = false;

    clear();
    lcd_goto_xy(0,0);
    print("MAZE 1:");
    lcd_goto_xy(0,1);
    print("LFT  RGH");
    int count_down = 0;
    while(!button_is_pressed(BUTTON_A) && !button_is_pressed(BUTTON_C)){
        if(button_is_pressed(BUTTON_B)){
            count_down++;
            clear();
            lcd_goto_xy(0,0);
            print("MAZE 1:");
            lcd_goto_xy(0,1);
            print("L ");
            print_long(count_down);
            print(" R");
            while(button_is_pressed(BUTTON_B)){}
            delay_ms(100);
        }
    };
    clear();
    lcd_goto_xy(0,0);
    if(button_is_pressed(BUTTON_A)){
        lefthanded = true;       
        print("LEFTHAND");
        lcd_goto_xy(0,1);
        print("MAZE 1");
    }
    if(button_is_pressed(BUTTON_C)){
        lefthanded = false;
        print("RGHTHAND");
        lcd_goto_xy(0,1);
        print("MAZE 1");
    }

    delay_ms(300);
    desafio2.lefthanded = lefthanded;

    raton.wait_for_button(READY,BUTTON_B);
    raton.calibrate();
    raton.wait_for_button(READY,BUTTON_B);

    clear();
    lcd_goto_xy(0,0);
    print("  MAYOI ");
    lcd_goto_xy(0,1);
    print("  MODE  ");
    
    desafio2.learn(0, count_down);
    delay_ms(300);
    clear();
    lcd_goto_xy(0,0);
    print("MAZE 1: ");
    lcd_goto_xy(0,1);
    print("LEARNED.");
    raton.stop_motors();

/********************************************/

    count_down = 0;
    clear();
    lcd_goto_xy(0,0);
    print("MAZE 2:");
    lcd_goto_xy(0,1);
    print("LFT  RGH");
    while(!button_is_pressed(BUTTON_A) && !button_is_pressed(BUTTON_C)){
        if(button_is_pressed(BUTTON_B)){
            count_down++;
            clear();
            lcd_goto_xy(0,0);
            print("MAZE 1:");
            lcd_goto_xy(0,1);
            print("L ");
            print_long(count_down);
            print(" R");
            while(button_is_pressed(BUTTON_B)){}
            delay_ms(100);
        }
    };
    clear();
    lcd_goto_xy(0,0);
    if(button_is_pressed(BUTTON_A)){
        lefthanded = true;       
        print("LEFTHAND");
        lcd_goto_xy(0,1);
        print("MAZE 2");
    }
    if(button_is_pressed(BUTTON_C)){
        lefthanded = false;
        print("RGHTHAND");
        lcd_goto_xy(0,1);
        print("MAZE 2");
    }

    delay_ms(300);
    desafio2.lefthanded = lefthanded;

    raton.wait_for_button(READY,BUTTON_B);
    raton.calibrate();
    
    raton.wait_for_button(READY,BUTTON_B);
    clear();
    lcd_goto_xy(0,0);
    print("  MAYOI ");
    lcd_goto_xy(0,1);
    print("  MODE  ");
    
    desafio2.learn(1, count_down);
    delay_ms(300);
    clear();
    lcd_goto_xy(0,0);
    print("MAZE 2: ");
    lcd_goto_xy(0,1);
    print("LEARNED.");
    raton.stop_motors();


/**************************************/

    while(1){

        clear();
        lcd_goto_xy(0,0);
        print("SELECT :");
        lcd_goto_xy(0,1);
        print("M1   M2");
        while(!button_is_pressed(BUTTON_A) && !button_is_pressed(BUTTON_C)){};
        clear();
        lcd_goto_xy(0,0);
        if(button_is_pressed(BUTTON_A)){
            print("Maze 1:");
            lcd_goto_xy(0,1);
            print("RUN!!!");
            delay_ms(400);
            desafio2.run_maze(0);
        }
        if(button_is_pressed(BUTTON_C)){
            print("Maze 2:");
            lcd_goto_xy(0,1);
            print("RUN!!!");
            delay_ms(400);
            desafio2.run_maze(1);
        }


        play_from_program_space(end);
        lcd_goto_xy(0,1);

        raton.stop_motors();

    }
}






int main()
{
    play_from_program_space(start);

    Robot raton(2000, 75, 200, 10);


    clear();
    lcd_goto_xy(0,0);
    print("Desafio:");
    lcd_goto_xy(0,1);
    print("01  02");

    while(!button_is_pressed(BUTTON_A) && !button_is_pressed(BUTTON_C)){}

    if(button_is_pressed(BUTTON_A)){
        print("Onigokko");
        lcd_goto_xy(0,1);
        print(" Mode! ");
        
        delay_ms(300);
        start_onigokko(raton);

    }
    if(button_is_pressed(BUTTON_C)){
        print(" Mayoi ");
        lcd_goto_xy(0,1);
        print(" Mode! ");
        
        delay_ms(300);
        start_mayoi(raton);

    }

    print("Done.");
    raton.finish(); 

    return 0;
}

















// /********************************************
// SLAVE TESTING!
// *********************************************/

// int main()
// {
//     Robot raton(1000, 100, 250, 10);
//     play_mode(PLAY_CHECK);
 
//     clear();
//     print("Slave");
 
//     // raton.serial.writeln("Slave Conneted!");
//     int counter = 0;
//     unsigned char counter_data = 0;
//     while(1)
//     {
//         // OrangutanSerial::check();

//         // clear();
//         // wait for a command
//         if(raton.serial.data_avaliable()){
//             clear();
//             long value = (long)raton.serial.read_byte();
//             lcd_goto_xy(0,1);
//             print_long( (long)counter_data++ );
//             print("-");
//             print_long(value);
//             delay_ms(50);    
//         }
            
 
//         // The list of commands is below: add your own simply by
//         // choosing a command byte and introducing another case
//         // statement.
        
//         if(button_is_pressed(BUTTON_A)){
//             clear();
//             raton.serial.write('L');
//             while(button_is_pressed(BUTTON_A));
//             delay_ms(100);
//         }
        
//         if(button_is_pressed(BUTTON_B)){
//             clear();
//             raton.serial.write('P');
//             while(button_is_pressed(BUTTON_B));
//             delay_ms(100);
//         }

//         if(button_is_pressed(BUTTON_C)){
//             clear();
//             raton.serial.write('R');
//             while(button_is_pressed(BUTTON_C));
//             delay_ms(100);
//         }
    
//     }
// }