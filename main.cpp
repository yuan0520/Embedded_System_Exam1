#include "mbed.h"
#include "uLCD_4DGL.h"

uLCD_4DGL uLCD(D1, D0, D2);

AnalogOut  aout(D7);
AnalogIn ain(D13);

InterruptIn btn_up(D6);
InterruptIn btn_down(D10);
InterruptIn btn_sel(D3);

int sample = 1000;
float ADCdata[1500];
EventQueue menu_queue(32 * EVENTS_EVENT_SIZE);
EventQueue wave_queue(32 * EVENTS_EVENT_SIZE);
Thread menu_t;
Thread wave_t;

int freq_pos = 3; // 0 -> 10ms, 1 -> 20ms, 2 => 40ms, 3 -> 80ms
float freq_scale = 1; // default 80ms
float wavelength_scale = 1.0;
float S = 80;
float len = 1.0 / (S*freq_scale);

void menu(){
    uLCD.color(BLACK);
    uLCD.locate(1, 2);
    uLCD.printf("\n1/8\n\n");
    uLCD.locate(1, 4);
    uLCD.printf("\n1/4\n\n");
    uLCD.locate(1, 6);
    uLCD.printf("\n1/2\n\n");
    uLCD.locate(1, 8);
    uLCD.printf("\n1\n");

    if(freq_pos == 0){
        uLCD.color(BLUE);
        uLCD.locate(1, 2);
        // uLCD.printf("\n10Hz\n\n");
        uLCD.printf("\n1/8\n\n");
    }
    else if(freq_pos == 1){
        uLCD.color(BLUE);
        uLCD.locate(1, 4);
        // uLCD.printf("\n25Hz\n\n");
        uLCD.printf("\n1/4\n\n");
    }
    else if(freq_pos == 2){
        uLCD.color(BLUE);
        uLCD.locate(1, 6);
        // uLCD.printf("\n50Hz\n\n");
        uLCD.printf("\n1/2\n\n");
    }
    else{
        uLCD.color(BLUE);
        uLCD.locate(1, 8);
        // uLCD.printf("\n100Hz\n\n");
        uLCD.printf("\n1\n\n");
    }
}

void up_btn_task(){
    if(freq_pos < 3) freq_pos++;
    menu_queue.call(menu);
}

void down_btn_task(){
    if(freq_pos > 0) freq_pos--;
    menu_queue.call(menu);
}

void sel_btn_task(){
    if(freq_pos == 0) freq_scale = 0.125;
    else if(freq_pos == 1) freq_scale = 0.25;
    else if(freq_pos == 2) freq_scale = 0.5;
    else freq_scale = 1;
    wave_queue.call(generate_wave);
}

void generate_wave(){
    int idx = 0;
    int flag = 0;

    while (1) {
        for (float i = 0.0f; i < wavelength_scale; i += len) {
            // aout = i * freq_scale;
            aout = i;

            if(idx < sample){
                ADCdata[idx++] = ain;
            }
            ThisThread::sleep_for(1ms);
        }
        
        if(freq_scale == 1) ThisThread::sleep_for(80ms);
        else if(freq_scale == 0.5) ThisThread::sleep_for(160ms);
        else if(freq_scale == 0.25) ThisThread::sleep_for(200ms);
        else ThisThread::sleep_for(220ms);

        for (float j = wavelength_scale; j > 0.0f; j -= len){
            // aout = j * freq_scale;
            aout = j;

            if(idx < sample){
                ADCdata[idx++] = ain;
            }
            ThisThread::sleep_for(1ms);
        }

        if(idx >= sample && !flag){
            for(int i=0; i<sample; i++){
                printf("%f\r\n", ADCdata[i]);
            }
            flag = 1;
            ThisThread::sleep_for(10ms); 
        }
    }
}

int main(void)
{
    menu_t.start(callback(&menu_queue, &EventQueue::dispatch_forever));
    wave_t.start(callback(&wave_queue, &EventQueue::dispatch_forever));
    btn_up.rise(&up_btn_task);
    btn_down.rise(&down_btn_task);
    btn_sel.rise(&sel_btn_task);

    uLCD.background_color(WHITE);
    uLCD.cls();
    uLCD.textbackground_color(WHITE);
    menu();

    
    // int freq_pos = 3; // 0 -> 10ms, 1 -> 20ms, 2 => 40ms, 3 -> 80ms
    // float freq_scale = 1; // default 80ms
    // float wavelength_scale = 1.0;
    // float S = 80;
    // float len = 1.0 / (S*freq_scale);

    // int idx = 0;
    // int flag = 0;

    // while (1) {
    //     for (float i = 0.0f; i < wavelength_scale; i += up_len) {
    //         // aout = i * freq_scale;
    //         aout = i;

    //         if(idx < sample){
    //             ADCdata[idx++] = ain;
    //         }
    //         ThisThread::sleep_for(1ms);
    //     }
    //     for (float j = wavelength_scale; j > 0.0f; j -= down_len){
    //         // aout = j * freq_scale;
    //         aout = j;

    //         if(idx < sample){
    //             ADCdata[idx++] = ain;
    //         }
    //         ThisThread::sleep_for(1ms);
    //     }

    //     if(idx >= sample && !flag){
    //         for(int i=0; i<sample; i++){
    //             printf("%f\r\n", ADCdata[i]);
    //         }
    //         flag = 1;
    //         ThisThread::sleep_for(10ms); 
    //     }
    // }
}