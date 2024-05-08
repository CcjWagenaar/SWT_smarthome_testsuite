//TESTING-NOTE: header file includes output struct (to return to test suite), cstdint, and cstdio:
/*
    #include <cstdint>
    #include <cstdio>
*/
#include "smarthome_helper.h"
#include "smarthome_software.h"

//TESTING-NOTE: replacing ThisThread::sleep_for() in milliseconds with usleep in microseconds. Changed value from '125ms' and '250ms' to '125' and '250'
#include <unistd.h>

#define LOW_TEMPERATURE         25.0
#define HIGH_TEMPERATURE        30.0
#define BRIGHTNESS_THRESHOLD    0.1
#define ENABLE_BUG              0
#define DEBUG_PRINT             0
#define SHORT_SLEEP             125
#define LONG_SLEEP              250
uint64_t cycle = 0;

//TESTING-NOTE: replacing Arduino inputs and outputs with integer variables:
/*
    DigitalOut Garden_Lamp(PTB18);
    DigitalOut redLed(PTB2);
    DigitalOut greenLed(PTB3);
    DigitalIn  Car_Sensor(PTE25);
    DigitalOut Garage_Door (PTB11);
    DigitalOut Front_Door (PTE24);
    DigitalOut Curtain (PTC7);  
    DigitalOut fan(PTC12);
*/
uint8_t Garden_Lamp     = 0;
uint8_t redLed          = 0;
uint8_t greenLed        = 0;
uint8_t Car_Sensor      = 0;
uint8_t Garage_Door     = 0;
uint8_t Front_Door      = 0;
uint8_t Curtain         = 0;  
uint8_t fan             = 0;

//TESTING-NOTE: replaced arduino PWM's with float variables:
float light_pwm0        = 0.0;
float light_pwm1        = 0.0;
float light_pwm2        = 0.0;
float heater_pwm3       = 0.0;

//TESTING-NOTE: replaced Arduino ADC-reads with float:
float temperature       = 0.0;
float brightness        = 0.0;

//TESTING-NOTE: replaced Arduino Display with string buffer
char display_output[1024]; 

void open_door()        { Front_Door = 0;   }
void close_door()       { Front_Door = 1;   }
void open_curtain()     { Curtain = 0;      }
void close_curtain()    { Curtain = 1;      }
void fan_on()           { fan = 1;          }
void fan_off()          { fan = 0;          }

void close_garage()     { Garage_Door = 0;  }

//TESTING-NOTE: store whether the garage door has opened in Garage_Door_Opened, set to 0 at the start of each cycle
uint8_t Garage_Door_Opened = 0;
void open_garage()      { 
    Garage_Door = 1;  
    if(DEBUG_PRINT) printf("sleeping...\t"); 
    //TESTING-NOTE: replacing ThisThread::sleep_for() in milliseconds with usleep in microseconds, 
    //  and storing whether garage door was opened in Garage_Door_Opened:
    /*
        ThisThread::sleep_for(SHORT_SLEEP); 
    */
    Garage_Door_Opened = 1;
    usleep(SHORT_SLEEP * 1000);

    if(DEBUG_PRINT) printf("done!\n"); 
    close_garage();
}

void heater_off() {
    redLed = 0;
    //TESTING-NOTE: replaced Arduino PWM's with floats:
    /*
        pwm3.write(0.0); //heater off
        pwm3.period_ms(10);
    */
    heater_pwm3 = 0.0; //heater off
}

void heater_on() {
    redLed = 1;
    //TESTING-NOTE: replaced Arduino PWM's with floats:
    /*
        pwm3.write(1.0); //heater on
        pwm3.period_ms(10);
    */
    heater_pwm3 = 1.0; //heater on
    
}

void lights_on() {
    //TESTING-NOTE: replaced Arduino PWM's with floats:
    /*
        pwm0.write (0.9);
        pwm0.period_ms(10);
        pwm1.write (0.9);
        pwm1.period_ms(10);  
        pwm2.write (0.9);
        pwm2.period_ms(10);
    */
    light_pwm0 = 0.9;
    light_pwm1 = 0.9;
    light_pwm2 = 0.9;
    
}

void lights_off() {
    //TESTING-NOTE: replaced Arduino PWM's with floats:
    /*
        pwm0.write (0.0);
        pwm0.period_ms(10);
        pwm1.write (0.0);
        pwm1.period_ms(10);  
        pwm2.write (0.0);
        pwm2.period_ms(10);
    */
    light_pwm0 = 0.0;
    light_pwm1 = 0.0;
    light_pwm2 = 0.0;
}

float read_temperature() {
    //TESTING-NOTE: replaced Arduino ADC-reads with float:
    /*
        uint16_t analog_temperature = adc_read(0);
        // Convert 16 bit value to voltage and temperature.
        float voltage = analog_temperature * 3 / 65535.0 ;
        float temp    = (voltage * 1000 - 400) / 19.5 ;
        return temp;
    */
    return temperature;
}

//returns brightness value, with 0 being darkest and 1 being brightest.
float read_brightness() {
    //TESTING-NOTE: replaced Arduino ADC-reads with float:
    /*
        uint16_t analog_brightness = adc_read(1);
        // Convert 16 bit value to voltage and brightness.
        float brightness = analog_brightness / 65535.0 ;
        return brightness;
    */
    return brightness;
}

void write_on_display(char* print_str) {
    //TESTING-NOTE: Replaced print to display with string buffer:
    /*
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
    */
    strncpy(display_output, print_str, sizeof(display_output) - 1);
    display_output[sizeof(display_output) - 1] = '\0'; 
}

// main() runs in its own thread in the OS
//TESTING-NOTE: renamed main() to run_cycle(), now returns 'output' struct:
/*
int main() {
*/
output run_cycle(input input_values) {
    //TESTING-NOTE: setting custom input values based on testing suite parameter provided:
    Car_Sensor  = input_values.Car_Sensor;
    temperature = input_values.temperature;
    brightness  = input_values.brightness;
    //TESTING-NOTE: store whether the garage door has opened in Garage_Door_Opened, set to 0 at the start of each cycle
    Garage_Door_Opened = 0;
    //TESTING-NOTE: Board_init() call no longer relevant, since Arduino is decoupled:
    /*
        board_init();
    */

    //TESTING-NOTE: while-loop removed. Test suite calls one cycle at a time, and then verifies output:
    /*
        while (true) {
    */
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
        if(DEBUG_PRINT) printf("%llu, Car: %d, heat: %.0f\n", cycle, Car_Sensor, heater_pwm3);
        
        cycle++;
        //TESTING-NOTE: replacing ThisThread::sleep_for() in milliseconds with usleep in microseconds:
        /*
            ThisThread::sleep_for(1000ms); 
        */
        usleep(LONG_SLEEP * 1000);
    //TESTING-NOTE: while-loop removed. Test suite calls one cycle at a time, and then verifies output:
    /*
        }
    */
    //TESTING-NOTE: saving actuator values in output struct, which is returned to the test suite:
    output output_values(Garden_Lamp, redLed, greenLed, Garage_Door_Opened, Front_Door, Curtain, fan, light_pwm0, light_pwm1, light_pwm2, heater_pwm3, display_output);
    return output_values;
}
