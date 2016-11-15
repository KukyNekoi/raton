#ifdef SERIAL_H_
#else
#define SERIAL_H_
#include <stdio.h>
#include <string.h>
#include <pololu/Pololu3pi.h>

class Serial
{
public:
    Serial(unsigned long baud_rate = 115200UL){
        OrangutanSerial::setBaudRate(baud_rate);
        OrangutanSerial::setMode(SERIAL_AUTOMATIC);
        OrangutanSerial::receiveRing(this->input_buffer, sizeof(100));
    };
    ~Serial(){};
    
    void write(char value){
        this->clear_buffer(this->output_buffer);
        this->output_buffer[0] = value;
        this->send_buffer(1);
    }
    
    void write(int value){
        char *data = (char *)&value;
        this->clear_buffer(this->output_buffer);
        this->output_buffer[0] = data[0];
        this->output_buffer[1] = data[1];
        this->output_buffer[2] = data[2];
        this->output_buffer[3] = data[3];
        this->send_buffer(4);
    }
    


    bool data_avaliable(){
        return OrangutanSerial::getReceivedBytes() != this->read_index;
    }

    char read_byte(){
        print_long((long)(OrangutanSerial::getReceivedBytes()));
        while(!this->data_avaliable()){}
            delay_ms(20);
        char ret = this->input_buffer[this->read_index];
        // print("-");
        // print_long(this->read_index);
        if((size_t)(this->read_index) >= sizeof(this->input_buffer)-1)
            this->read_index = 0;
        else
            this->read_index ++;

        // print("-");
        // print_long((OrangutanSerial::getReceivedBytes()));
        // print("-");
        // print_long(this->read_index);
        return ret;
    }



    // char* read_string(){
    //     this->clear_buffer(this->content);
    //     char character[1];
    //     while(this->data_avaliable() && character[0] != '\n') {
    //         character[0] = this->read_byte();
    //         strcat ( this->content, character );
    //     }

    //     return this->content;
    // }

    // int read_int(){
    //     return atoi(this->read_string());
    // }

    // int read_long(){
    //     return atoi(this->read_string());
    // }

    void previous_byte(){
        this->read_index --;
        if(this->read_index == 255)
            this->read_index = 99;
    }

    void send_message( const char *msg, int size){
        while(!serial_send_buffer_empty());
        for (int i = 0; i < size; ++i)
            this->write(msg[i]);
    }

private:
    void send_buffer(int size){
        while(!serial_send_buffer_empty());
        print_long((long) size);
        OrangutanSerial::send(this->output_buffer, size);
    }

    void clear_buffer(char *target){
        for(int i=0; i<30; i++){
            target[i] = '\0';
        }
    }

    char content[30];
    char output_buffer[30];
    char input_buffer[30];
    unsigned char read_index = 0;
 
};

#endif