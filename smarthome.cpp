#include "mbed.h"
#include "board_freedom.h"
#include "oled_ssd1322.h"
#include "adc.h"
#include <cstdint>
#include <cstdio>

#define LOW_TEMPERATURE         25.0
#define HIGH_TEMPERATURE        30.0
#define BRIGHTNESS_THRESHOLD    0.1
#define ENABLE_BUG              0
#define DEBUG_PRINT             1
#define SHORT_SLEEP             125ms
#define LONG_SLEEP              250ms
uint64_t cycle = 0;

DigitalOut Garden_Lamp(PTB18);
DigitalOut redLed(PTB2);
DigitalOut greenLed(PTB3);
DigitalIn  Car_Sensor(PTE25);
DigitalOut Garage_Door (PTB11);
DigitalOut Front_Door (PTE24);
DigitalOut Curtain (PTC7);  
DigitalOut fan(PTC12);

void open_door()        { Front_Door = 0;   }
void close_door()       { Front_Door = 1;   }
void open_curtain()     { Curtain = 0;      }
void close_curtain()    { Curtain = 1;      }
void fan_on()           { fan = 1;          }
void fan_off()          { fan = 0;          }

void close_garage()     { Garage_Door = 0;  }
void open_garage()      { 
    Garage_Door = 1;  
    if(DEBUG_PRINT) printf("sleeping...\t"); 
    ThisThread::sleep_for(SHORT_SLEEP); 
    if(DEBUG_PRINT) printf("done!\n"); 
    close_garage();
}

void heater_off() {
    redLed = 0;
    pwm3.write(0.0); //heater off
    pwm3.period_ms(10);
}

void heater_on() {
    redLed = 1;
    pwm3.write(1.0); //heater on
    pwm3.period_ms(10);
}

void lights_on() {
    pwm0.write (0.9);
    pwm0.period_ms(10);
    pwm1.write (0.9);
    pwm1.period_ms(10);  
    pwm2.write (0.9);
    pwm2.period_ms(10);
}

void lights_off() {
    pwm0.write (0.0);
    pwm0.period_ms(10);
    pwm1.write (0.0);
    pwm1.period_ms(10);  
    pwm2.write (0.0);
    pwm2.period_ms(10);
}

float read_temperature() {
    uint16_t analog_temperature = adc_read(0);
    // Convert 16 bit value to voltage and temperature.
    float voltage = analog_temperature * 3 / 65535.0 ;
    float temp    = (voltage * 1000 - 400) / 19.5 ;
    return temp;
}

//returns brightness value, with 0 being darkest and 1 being brightest.
float read_brightness() {
    uint16_t analog_brightness = adc_read(1);
    // Convert 16 bit value to voltage and brightness.
    float brightness = analog_brightness / 65535.0 ;
    return brightness;
}

void write_on_display(char* print_str) {
    // Clear the buffer (will clear the screen when sent)
    u8g2_ClearBuffer(&oled);
    
    // Ensure a font is set (only required once)
    u8g2_SetFont(&oled, u8g2_font_nokiafc22_tr);
    
    // Draw text to the screen
    u8g2_int_t x = 30;
    u8g2_int_t y = 20;
    u8g2_DrawUTF8(&oled, x, y, print_str);

    // Put all the changes in the buffer onto the screen.
    u8g2_SendBuffer(&oled);
}

// main() runs in its own thread in the OS
int main() {
    board_init();
    
    while (true) {
        //Garden lamp is always on, just to signify 
        //the House is powered and the program flashed correctly. 
        Garden_Lamp = 1;

        float temperature = read_temperature();
        float brightness  = read_brightness();        
        
        //When car by Hall Effect Sensor, open Garage Servomotor. Otherwise, close it. 
        if (Car_Sensor == 0) open_garage(); 
        else                 close_garage(); 

        //When Inside Temperature Sensor notices temperature below LOW_TEMP, 
        //turn on Heater, and Freedom Controller LED will be red.
        if (temperature < LOW_TEMPERATURE) {
            greenLed = 0;
            heater_on();
            fan_on();
        }
        //When temperature above HIGH_TEMP, turn on Fan
        else if (temperature > HIGH_TEMPERATURE) {
            greenLed = 0;
            heater_off();
            fan_on();
        }
        //When temperature between or equal to 25C and 30C, 
        //turn off Heater and Fan, and the LED on the Freedom controller will turn green. 
        else {
            greenLed = 1;
            heater_off();
            fan_off();
        }

        //When the Light sensor detects brightness below a constant value BRIGHTNESS_THRESHOLD, 
        //turn on Smart House Light. Otherwise, turn inside lights off.
        if (brightness < BRIGHTNESS_THRESHOLD)  {
            lights_on(); 
            if (ENABLE_BUG) Garden_Lamp = 0;
        }
        else lights_off();

        //The Freedom Controller Display will display the temperature 
        //from the Inside Temperature Sensor in Degrees Celsius, 
        //as well as the brightness of the Light sensor with a value between 0 (darkest) and 1 (brightest). 
        char display_str[1024];
        snprintf(display_str, 1024, "Temp: %.2fC  Brightness: %.2f/1.0\n", temperature, brightness);
        write_on_display(display_str);

        if(DEBUG_PRINT) printf("%s", display_str);
        if(DEBUG_PRINT) printf("%llu, Car: %d, heat: %.0f\n", cycle, Car_Sensor.read(), pwm3.read());
        
        cycle++;
        ThisThread::sleep_for(LONG_SLEEP);
    }
}
