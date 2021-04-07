#include "mbed.h"
#include "uLCD_4DGL.h"

uLCD_4DGL uLCD(D1, D0, D2);

AnalogOut  aout(D7);
AnalogIn ain(D13);

DigitalIn btn_up(D6);
DigitalIn btn_down(D10);
DigitalIn btn_sel(D3);

int sample = 1000;
float ADCdata[1500];

void menu(int freq_pos){
    uLCD.color(BLACK);
    uLCD.locate(1, 2);
    uLCD.printf("\n10Hz\n\n");
    uLCD.locate(1, 4);
    uLCD.printf("\n25Hz\n\n");
    uLCD.locate(1, 6);
    uLCD.printf("\n50Hz\n\n");
    uLCD.locate(1, 8);
    uLCD.printf("\n100Hz\n");

    if(freq_pos == 0){
        uLCD.color(BLUE);
        uLCD.locate(1, 2);
        uLCD.printf("\n10Hz\n\n");
    }
    else if(freq_pos == 1){
        uLCD.color(BLUE);
        uLCD.locate(1, 4);
        uLCD.printf("\n25Hz\n\n");
    }
    else if(freq_pos == 2){
        uLCD.color(BLUE);
        uLCD.locate(1, 6);
        uLCD.printf("\n50Hz\n\n");
    }
    else{
        uLCD.color(BLUE);
        uLCD.locate(1, 8);
        uLCD.printf("\n100Hz\n\n");
    }
}

int main(void)
{
    int freq_pos = 0; // 0 -> 10Hz, 1 -> 25Hz, 2 => 50Hz, 3 -> 100Hz
    float freq_scale = 0.1; // default 10Hz

    uLCD.background_color(WHITE);
    uLCD.cls();
    uLCD.textbackground_color(WHITE);
    menu(freq_pos);

    while(1){
        if(btn_up){
            if(freq_pos < 3) freq_pos++;
            menu(freq_pos);
        }
        if(btn_down){
            if(freq_pos > 0) freq_pos--;
            menu(freq_pos);
        }
        if(btn_sel){
            if(freq_pos == 0) freq_scale = 0.1;
            else if(freq_pos == 1) freq_scale = 0.25;
            else if(freq_pos == 2) freq_scale = 0.5;
            else freq_scale = 1;
            break;
        }
    }

    float wavelength_scale = 1 / freq_scale;
    float S = (1+0+7+0+6+0+0+0+2) % 10;
    float up_len = 1.0 / S;
    float down_len = 1.0 / (10-S);

    int idx = 0;
    int flag = 0;

    while (1) {
        for (float i = 0.0f; i < wavelength_scale; i += up_len) {
            aout = i * freq_scale;

            if(idx < sample){
                ADCdata[idx++] = ain;
            }
            ThisThread::sleep_for(1ms);
        }
        for (float j = wavelength_scale; j > 0.0f; j -= down_len){
            aout = j * freq_scale;

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