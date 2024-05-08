#ifndef SMARTHOME_HELPER_H
#define SMARTHOME_HELPER_H

#include <cstdint>
#include <cstdio>
#include <string.h>

//Terminal Colours
#define RESET       "\033[0m"
#define BLACK       "\033[30m"     
#define RED         "\033[31m"     
#define GREEN       "\033[32m"     
#define WHITE       "\033[37m"     
#define BOLDBLACK   "\033[1m\033[30m"  
#define BOLDRED     "\033[1m\033[31m"     
#define BOLDGREEN   "\033[1m\033[32m"    
#define BOLDWHITE   "\033[1m\033[37m"

//Helpful sensor values for input
#define CAR_PRESENT 0
#define CAR_ABSENT 1
#define TEMPERATURE_LOW 24.2
#define TEMPERATURE_MEDIUM 27.0
#define TEMPERATURE_HIGH 31.0
#define TEMPERATURE_30_1 30.1
#define BRIGHTNESS_LOW 0.03     //manually measured equivalent of 50  lumen
#define BRIGHTNESS_MED 0.05     //manually measured equivalent of 75  lumen
#define BRIGHTNESS_HIGH 0.15    //manually measured equivalent of 200 lumen
//Helpful ON/OFF and OPEN/CLOSED constants for output
#define GARDEN_LAMP_ON 1
#define GARDEN_LAMP_OFF 0
#define REDLED_ON 1
#define REDLED_OFF 0
#define GREENLED_ON 1
#define GREENLED_OFF 0
#define GARAGE_OPEN 1
#define GARAGE_CLOSED 0
#define FRONTDOOR_OPEN 1
#define FRONTDOOR_CLOSED 0
#define CURTAIN_OPEN 1
#define CURTAIN_CLOSED 0
#define FAN_ON 1
#define FAN_OFF 0
#define LIGHT0_ON 0.9
#define LIGHT0_OFF 0.0
#define LIGHT1_ON 0.9
#define LIGHT1_OFF 0.0
#define LIGHT2_ON 0.9
#define LIGHT2_OFF 0.0
#define HEATER_ON 1.0
#define HEATER_OFF 0.0
//Common display output strings
#define DISPLAY_MEDTEMP_HIGHBRIGHT_CONST       "Temp: 27.00C  Brightness: 0.15/1.0\n"
#define DISPLAY_MEDTEMP_HIGHBRIGHT      (char*)"Temp: 27.00C  Brightness: 0.15/1.0\n"
#define DISPLAY_MEDTEMP_LOWBRIGHT       (char*)"Temp: 27.00C  Brightness: 0.03/1.0\n"
#define DISPLAY_MEDTEMP_MEDBRIGHT       (char*)"Temp: 27.00C  Brightness: 0.05/1.0\n"
#define DISPLAY_LOWTEMP_HIGHBRIGHT      (char*)"Temp: 24.20C  Brightness: 0.15/1.0\n"
#define DISPLAY_HIGHTEMP_HIGHBRIGHT     (char*)"Temp: 31.00C  Brightness: 0.15/1.0\n"
#define DISPLAY_TEMP30_HIGHBRIGHT       (char*)"Temp: 30.00C  Brightness: 0.15/1.0\n"
#define DISPLAY_LOWTEMP_HIGHBRIGHT      (char*)"Temp: 24.20C  Brightness: 0.15/1.0\n"
#define DISPLAY_TEMP30_1_HIGHBRIGHT     (char*)"Temp: 30.10C  Brightness: 0.15/1.0\n"


struct input {
    uint8_t Car_Sensor;
    float temperature;
    float brightness;

    //Default constructor
    input()
        : Car_Sensor(CAR_ABSENT), temperature(TEMPERATURE_MEDIUM), brightness(BRIGHTNESS_HIGH) {}

    //Parameterized constructor
    input(uint8_t Car_Sensor, float temperature, float brightness)
        : Car_Sensor(Car_Sensor), temperature(temperature), brightness(brightness) {}
};

//Define the output struct containing other variables
struct output {
    uint8_t Garden_Lamp;
    uint8_t redLed;
    uint8_t greenLed;
    uint8_t Garage_Door;
    uint8_t Front_Door;
    uint8_t Curtain;
    uint8_t fan;
    float light_pwm0;
    float light_pwm1;
    float light_pwm2;
    float heater_pwm3;
    char display_output[1024];

    //Default constructor
    output()
        : Garden_Lamp(GARDEN_LAMP_ON), redLed(REDLED_OFF), greenLed(GREENLED_ON), Garage_Door(GARAGE_CLOSED),
          Front_Door(FRONTDOOR_CLOSED), Curtain(CURTAIN_CLOSED), fan(FAN_OFF), light_pwm0(LIGHT0_OFF),
          light_pwm1(LIGHT1_OFF), light_pwm2(LIGHT2_OFF), heater_pwm3(HEATER_OFF), display_output(DISPLAY_MEDTEMP_HIGHBRIGHT_CONST) {}

    //Parameterized constructor
    output(uint8_t Garden_Lamp, uint8_t redLed, uint8_t greenLed,
           uint8_t Garage_Door, uint8_t Front_Door, uint8_t Curtain,
           uint8_t fan, float light_pwm0, float light_pwm1,
           float light_pwm2, float heater_pwm3, char* _display_output)
        : Garden_Lamp(Garden_Lamp), redLed(redLed), greenLed(greenLed),
          Garage_Door(Garage_Door), Front_Door(Front_Door), Curtain(Curtain),
          fan(fan), light_pwm0(light_pwm0), light_pwm1(light_pwm1),
          light_pwm2(light_pwm2), heater_pwm3(heater_pwm3) {
            strncpy(display_output, _display_output, sizeof(display_output) - 1);
            display_output[sizeof(display_output) - 1] = '\0'; 
          }
};

#endif
