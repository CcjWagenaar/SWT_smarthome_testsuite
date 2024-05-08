
#include "smarthome_helper.h"
#include "smarthome_software.h"

void print_output(output op) {
    printf("Garden Lamp: %d. Red Led: %d, Green Led: %d, Garage Door: %d, Front Door: %d, Curtain: %d, Fan: %d, Light0: %.2f, Light1: %.2f, Light2: %.2f, Heater: %.2f, display: %s\n", 
         op.Garden_Lamp,  op.redLed,   op.greenLed,   op.Garage_Door,  op.Front_Door,  op.Curtain,  op.fan,  op.light_pwm0,op.light_pwm1,op.light_pwm2,op.heater_pwm3, op.display_output);
}

int compare_outputs(output received_output, output expected_output, int test_id, const char* test_description, int cycle) {

    if(
        received_output.Garden_Lamp != expected_output.Garden_Lamp  || 
        received_output.redLed      != expected_output.redLed       ||
        received_output.greenLed    != expected_output.greenLed     ||
        received_output.Garage_Door != expected_output.Garage_Door  ||
        received_output.Front_Door  != expected_output.Front_Door   ||
        received_output.Curtain     != expected_output.Curtain      ||
        received_output.fan         != expected_output.fan          ||
        received_output.light_pwm0  != expected_output.light_pwm0   ||
        received_output.light_pwm1  != expected_output.light_pwm1   ||
        received_output.light_pwm2  != expected_output.light_pwm2   ||
        received_output.heater_pwm3 != expected_output.heater_pwm3  ||
        strncmp(received_output.display_output, expected_output.display_output, sizeof(expected_output)) != 0
    ) {
        printf("\n" BOLDRED "Failed Test %d" RESET " at cycle %d: %s\n", test_id, cycle, test_description);

        printf("Received Output: "); print_output(received_output);
        printf("Expected Output: "); print_output(expected_output);
        return -1;
    }

    return 0;
}

int run_test(int TEST_ID, const char* TEST_DESCRIPTION, int CYCLES, input* inputs, output* expected_outputs) {
    for(int cycle = 0; cycle < CYCLES; cycle++) {
        output received_output = run_cycle(inputs[cycle]);
        int res = compare_outputs(received_output, expected_outputs[cycle], TEST_ID, TEST_DESCRIPTION, cycle);
        if(res != 0) return res;
    }
    printf(BOLDGREEN "Passed Test %d" RESET ": %s\n", TEST_ID, TEST_DESCRIPTION);
    return 0;
}

int test0_car_opens_garage() {
    const int TEST_ID = 0;
    const int CYCLES = 3;
    const char* TEST_DESCRIPTION = "Car is placed in front of garage -> Garage opens, and then closes";

    input inputs[CYCLES] = {
        //Car_Sensor, temperature, brightness
        input(CAR_ABSENT,  TEMPERATURE_MEDIUM, BRIGHTNESS_HIGH),
        input(CAR_PRESENT, TEMPERATURE_MEDIUM, BRIGHTNESS_HIGH),
        input(CAR_ABSENT,  TEMPERATURE_MEDIUM, BRIGHTNESS_HIGH)
    };

    output expected_outputs[CYCLES] = {
        //     Garden_Lamp,    redLed,     greenLed,    Garage_Door,   Front_Door,       Curtain,        fan,     light_pwm0, light_pwm1, light_pwm2, heater_pwm3,display_output
        output(GARDEN_LAMP_ON, REDLED_OFF, GREENLED_ON, GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_OFF, LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_OFF, DISPLAY_MEDTEMP_HIGHBRIGHT),
        output(GARDEN_LAMP_ON, REDLED_OFF, GREENLED_ON, GARAGE_OPEN,   FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_OFF, LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_OFF, DISPLAY_MEDTEMP_HIGHBRIGHT),
        output(GARDEN_LAMP_ON, REDLED_OFF, GREENLED_ON, GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_OFF, LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_OFF, DISPLAY_MEDTEMP_HIGHBRIGHT)
    };

    return run_test(TEST_ID, TEST_DESCRIPTION, CYCLES, inputs, expected_outputs);
}

int test1_absent_car_garage_closed() {
    const int TEST_ID = 1;
    const int CYCLES = 2;
    const char* TEST_DESCRIPTION = "There is no car in front of the garage -> Garage stays closed";

    input inputs[CYCLES] = {
        //Car_Sensor, temperature, brightness
        input(CAR_ABSENT,  TEMPERATURE_MEDIUM, BRIGHTNESS_HIGH),
        input(CAR_ABSENT,  TEMPERATURE_MEDIUM, BRIGHTNESS_HIGH)
    };

    output expected_outputs[CYCLES] = {
        //     Garden_Lamp,    redLed,     greenLed,    Garage_Door,   Front_Door,       Curtain,        fan,     light_pwm0, light_pwm1, light_pwm2, heater_pwm3,display_output
        output(GARDEN_LAMP_ON, REDLED_OFF, GREENLED_ON, GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_OFF, LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_OFF, DISPLAY_MEDTEMP_HIGHBRIGHT),
        output(GARDEN_LAMP_ON, REDLED_OFF, GREENLED_ON, GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_OFF, LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_OFF, DISPLAY_MEDTEMP_HIGHBRIGHT)
    };

    return run_test(TEST_ID, TEST_DESCRIPTION, CYCLES, inputs, expected_outputs);
}

int test2_brightness_low_lights_on() {
    const int TEST_ID = 2;
    const int CYCLES = 3;
    const char* TEST_DESCRIPTION = "Brightness is 50 Lumen -> Light turns on inside";

    input inputs[CYCLES] = {
        //Car_Sensor, temperature, brightness
        input(CAR_ABSENT, TEMPERATURE_MEDIUM, BRIGHTNESS_HIGH),
        input(CAR_ABSENT, TEMPERATURE_MEDIUM, BRIGHTNESS_LOW),
        input(CAR_ABSENT, TEMPERATURE_MEDIUM, BRIGHTNESS_HIGH)
    };

    output expected_outputs[CYCLES] = {
        //     Garden_Lamp,    redLed,     greenLed,    Garage_Door,   Front_Door,       Curtain,        fan,     light_pwm0, light_pwm1, light_pwm2, heater_pwm3,display_output
        output(GARDEN_LAMP_ON, REDLED_OFF, GREENLED_ON, GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_OFF, LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_OFF, DISPLAY_MEDTEMP_HIGHBRIGHT),
        output(GARDEN_LAMP_ON, REDLED_OFF, GREENLED_ON, GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_OFF, LIGHT0_ON,  LIGHT1_ON,  LIGHT2_ON,  HEATER_OFF, DISPLAY_MEDTEMP_LOWBRIGHT ),
        output(GARDEN_LAMP_ON, REDLED_OFF, GREENLED_ON, GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_OFF, LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_OFF, DISPLAY_MEDTEMP_HIGHBRIGHT)
    };

    return run_test(TEST_ID, TEST_DESCRIPTION, CYCLES, inputs, expected_outputs);
}

int test3_brightness_med_lights_on() {
    const int TEST_ID = 3;
    const int CYCLES = 3;
    const char* TEST_DESCRIPTION = "Brightness is 75 Lumen -> Lights turn on inside";

    input inputs[CYCLES] = {
        //Car_Sensor, temperature, brightness
        input(CAR_ABSENT, TEMPERATURE_MEDIUM, BRIGHTNESS_HIGH),
        input(CAR_ABSENT, TEMPERATURE_MEDIUM, BRIGHTNESS_MED),
        input(CAR_ABSENT, TEMPERATURE_MEDIUM, BRIGHTNESS_HIGH)
    };

    output expected_outputs[CYCLES] = {
        //     Garden_Lamp,    redLed,     greenLed,    Garage_Door,   Front_Door,       Curtain,        fan,     light_pwm0, light_pwm1, light_pwm2, heater_pwm3,display_output
        output(GARDEN_LAMP_ON, REDLED_OFF, GREENLED_ON, GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_OFF, LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_OFF, DISPLAY_MEDTEMP_HIGHBRIGHT),
        output(GARDEN_LAMP_ON, REDLED_OFF, GREENLED_ON, GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_OFF, LIGHT0_ON,  LIGHT1_ON,  LIGHT2_ON,  HEATER_OFF, DISPLAY_MEDTEMP_MEDBRIGHT ),
        output(GARDEN_LAMP_ON, REDLED_OFF, GREENLED_ON, GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_OFF, LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_OFF, DISPLAY_MEDTEMP_HIGHBRIGHT)
    };

    return run_test(TEST_ID, TEST_DESCRIPTION, CYCLES, inputs, expected_outputs);
}

int test4_brightness_high_lights_off() {
    const int TEST_ID = 4;
    const int CYCLES = 3;
    const char* TEST_DESCRIPTION = "Brightness is 200 Lumen -> Lights turn off inside";

    input inputs[CYCLES] = {
        //Car_Sensor, temperature, brightness
        input(CAR_ABSENT, TEMPERATURE_MEDIUM, BRIGHTNESS_LOW),
        input(CAR_ABSENT, TEMPERATURE_MEDIUM, BRIGHTNESS_HIGH),
        input(CAR_ABSENT, TEMPERATURE_MEDIUM, BRIGHTNESS_LOW)
    };

    output expected_outputs[CYCLES] = {
        //     Garden_Lamp,    redLed,     greenLed,    Garage_Door,   Front_Door,       Curtain,        fan,     light_pwm0, light_pwm1, light_pwm2, heater_pwm3,display_output
        output(GARDEN_LAMP_ON, REDLED_OFF, GREENLED_ON, GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_OFF, LIGHT0_ON,  LIGHT1_ON,  LIGHT2_ON,  HEATER_OFF, DISPLAY_MEDTEMP_LOWBRIGHT ),
        output(GARDEN_LAMP_ON, REDLED_OFF, GREENLED_ON, GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_OFF, LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_OFF, DISPLAY_MEDTEMP_HIGHBRIGHT),
        output(GARDEN_LAMP_ON, REDLED_OFF, GREENLED_ON, GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_OFF, LIGHT0_ON,  LIGHT1_ON,  LIGHT2_ON,  HEATER_OFF, DISPLAY_MEDTEMP_LOWBRIGHT )    
    };

    return run_test(TEST_ID, TEST_DESCRIPTION, CYCLES, inputs, expected_outputs);
}

int test5_temperature_on_display() {
    const int TEST_ID = 5;
    const int CYCLES = 1;
    const char* TEST_DESCRIPTION = "An inside temperature of 27°C is measured -> A temperature of 27°C is displayed";

    input inputs[CYCLES] = {
        //Car_Sensor, temperature, brightness
        input(CAR_ABSENT,  TEMPERATURE_MEDIUM, BRIGHTNESS_HIGH)
    };

    output expected_outputs[CYCLES] = {
        //Garden_Lamp, redLed, greenLed, Garage_Door, Front_Door, Curtain, fan, light_pwm0, light_pwm1, light_pwm2, heater_pwm3
        output(GARDEN_LAMP_ON, REDLED_OFF, GREENLED_ON, GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_OFF, LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_OFF, DISPLAY_MEDTEMP_HIGHBRIGHT),
    };

    return run_test(TEST_ID, TEST_DESCRIPTION, CYCLES, inputs, expected_outputs);
}

int test6_brightness_on_display() {
    const int TEST_ID = 6;
    const int CYCLES = 1;
    const char* TEST_DESCRIPTION = "Brightness is 50 Lumen -> The brightness value is displayed";

    input inputs[CYCLES] = {
        //Car_Sensor, temperature, brightness
        input(CAR_ABSENT,  TEMPERATURE_MEDIUM, BRIGHTNESS_HIGH)
    };

    output expected_outputs[CYCLES] = {
        //Garden_Lamp, redLed, greenLed, Garage_Door, Front_Door, Curtain, fan, light_pwm0, light_pwm1, light_pwm2, heater_pwm3
        output(GARDEN_LAMP_ON, REDLED_OFF, GREENLED_ON, GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_OFF, LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_OFF, DISPLAY_MEDTEMP_HIGHBRIGHT),
    };

    return run_test(TEST_ID, TEST_DESCRIPTION, CYCLES, inputs, expected_outputs);
}

int test7_low_temp_heater_on() {
    const int TEST_ID = 7;
    const int CYCLES = 3;
    const char* TEST_DESCRIPTION = "Temperature 24.2°C -> Heater is turned on";

    input inputs[CYCLES] = {
        //Car_Sensor, temperature, brightness
        input(CAR_ABSENT, TEMPERATURE_MEDIUM, BRIGHTNESS_HIGH),
        input(CAR_ABSENT, TEMPERATURE_LOW,    BRIGHTNESS_HIGH),
        input(CAR_ABSENT, TEMPERATURE_MEDIUM, BRIGHTNESS_HIGH)
    };

    output expected_outputs[CYCLES] = {
        //     Garden_Lamp,    redLed,     greenLed,    Garage_Door,   Front_Door,       Curtain,        fan,     light_pwm0, light_pwm1, light_pwm2, heater_pwm3,display_output
        output(GARDEN_LAMP_ON, REDLED_OFF, GREENLED_ON,  GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_OFF, LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_OFF, DISPLAY_MEDTEMP_HIGHBRIGHT),
        output(GARDEN_LAMP_ON, REDLED_ON,  GREENLED_OFF, GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_ON,  LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_ON,  DISPLAY_LOWTEMP_HIGHBRIGHT),
        output(GARDEN_LAMP_ON, REDLED_OFF, GREENLED_ON,  GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_OFF, LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_OFF, DISPLAY_MEDTEMP_HIGHBRIGHT)
    };

    return run_test(TEST_ID, TEST_DESCRIPTION, CYCLES, inputs, expected_outputs);
}

int test8_low_temp_fan_on() {
    const int TEST_ID = 8;
    const int CYCLES = 3;
    const char* TEST_DESCRIPTION = "Temperature 24.2°C -> Fan is turned on";

    input inputs[CYCLES] = {
        //Car_Sensor, temperature, brightness
        input(CAR_ABSENT, TEMPERATURE_MEDIUM, BRIGHTNESS_HIGH),
        input(CAR_ABSENT, TEMPERATURE_LOW,    BRIGHTNESS_HIGH),
        input(CAR_ABSENT, TEMPERATURE_MEDIUM, BRIGHTNESS_HIGH)
    };

    output expected_outputs[CYCLES] = {
        //     Garden_Lamp,    redLed,     greenLed,     Garage_Door,   Front_Door,       Curtain,        fan,     light_pwm0, light_pwm1, light_pwm2, heater_pwm3,display_output
        output(GARDEN_LAMP_ON, REDLED_OFF, GREENLED_ON,  GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_OFF, LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_OFF, DISPLAY_MEDTEMP_HIGHBRIGHT),
        output(GARDEN_LAMP_ON, REDLED_ON,  GREENLED_OFF, GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_ON,  LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_ON,  DISPLAY_LOWTEMP_HIGHBRIGHT),
        output(GARDEN_LAMP_ON, REDLED_OFF, GREENLED_ON,  GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_OFF, LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_OFF, DISPLAY_MEDTEMP_HIGHBRIGHT)
    };

    return run_test(TEST_ID, TEST_DESCRIPTION, CYCLES, inputs, expected_outputs);
}

int test9_low_temp_redled_on() {
    const int TEST_ID = 9;
    const int CYCLES = 3;
    const char* TEST_DESCRIPTION = "Heater is turned on, hence the red led is turned on -> Heater is turned on, hence the red led is turned on";

    input inputs[CYCLES] = {
        //Car_Sensor, temperature, brightness
        input(CAR_ABSENT, TEMPERATURE_MEDIUM, BRIGHTNESS_HIGH),
        input(CAR_ABSENT, TEMPERATURE_LOW,    BRIGHTNESS_HIGH),
        input(CAR_ABSENT, TEMPERATURE_MEDIUM, BRIGHTNESS_HIGH)
    };

    output expected_outputs[CYCLES] = {
        //     Garden_Lamp,    redLed,     greenLed,     Garage_Door,   Front_Door,       Curtain,        fan,     light_pwm0, light_pwm1, light_pwm2, heater_pwm3,display_output
        output(GARDEN_LAMP_ON, REDLED_OFF, GREENLED_ON,  GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_OFF, LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_OFF, DISPLAY_MEDTEMP_HIGHBRIGHT),
        output(GARDEN_LAMP_ON, REDLED_ON,  GREENLED_OFF, GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_ON,  LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_ON,  DISPLAY_LOWTEMP_HIGHBRIGHT),
        output(GARDEN_LAMP_ON, REDLED_OFF, GREENLED_ON,  GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_OFF, LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_OFF, DISPLAY_MEDTEMP_HIGHBRIGHT)
    };

    return run_test(TEST_ID, TEST_DESCRIPTION, CYCLES, inputs, expected_outputs);
}


int test10_high_temp_fan_on() {
    const int TEST_ID = 10;
    const int CYCLES = 3;
    const char* TEST_DESCRIPTION = "Temperature is 31 °C -> Fan is turned on";

    input inputs[CYCLES] = {
        //Car_Sensor, temperature, brightness
        input(CAR_ABSENT, TEMPERATURE_MEDIUM, BRIGHTNESS_HIGH),
        input(CAR_ABSENT, TEMPERATURE_HIGH,   BRIGHTNESS_HIGH),
        input(CAR_ABSENT, TEMPERATURE_MEDIUM, BRIGHTNESS_HIGH)
    };

    output expected_outputs[CYCLES] = {
        //     Garden_Lamp,    redLed,     greenLed,     Garage_Door,   Front_Door,       Curtain,        fan,     light_pwm0, light_pwm1, light_pwm2, heater_pwm3,display_output
        output(GARDEN_LAMP_ON, REDLED_OFF, GREENLED_ON,  GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_OFF, LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_OFF, DISPLAY_MEDTEMP_HIGHBRIGHT),
        output(GARDEN_LAMP_ON, REDLED_OFF, GREENLED_OFF, GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_ON,  LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_OFF, DISPLAY_HIGHTEMP_HIGHBRIGHT),
        output(GARDEN_LAMP_ON, REDLED_OFF, GREENLED_ON,  GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_OFF, LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_OFF, DISPLAY_MEDTEMP_HIGHBRIGHT)
    };

    return run_test(TEST_ID, TEST_DESCRIPTION, CYCLES, inputs, expected_outputs);
}

int test11_low_temp_fan_off() {
    const int TEST_ID = 11;
    const int CYCLES = 2;
    const char* TEST_DESCRIPTION = "Temperature falls below 30 °C -> Fan is turned off";

    input inputs[CYCLES] = {
        //Car_Sensor, temperature, brightness
        input(CAR_ABSENT, TEMPERATURE_HIGH,   BRIGHTNESS_HIGH),
        input(CAR_ABSENT, TEMPERATURE_MEDIUM, BRIGHTNESS_HIGH)
    };

    output expected_outputs[CYCLES] = {
        //     Garden_Lamp,    redLed,     greenLed,     Garage_Door,   Front_Door,       Curtain,        fan,     light_pwm0, light_pwm1, light_pwm2, heater_pwm3,display_output
        output(GARDEN_LAMP_ON, REDLED_OFF, GREENLED_OFF, GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_ON,  LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_OFF, DISPLAY_HIGHTEMP_HIGHBRIGHT),
        output(GARDEN_LAMP_ON, REDLED_OFF, GREENLED_ON,  GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_OFF, LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_OFF, DISPLAY_MEDTEMP_HIGHBRIGHT)
    };

    return run_test(TEST_ID, TEST_DESCRIPTION, CYCLES, inputs, expected_outputs);
}

int test12_med_temp_greenled_on() {
    const int TEST_ID = 12;
    const int CYCLES = 2;
    const char* TEST_DESCRIPTION = "Inside temperature reaches above 25 °C -> The green led turns on";

    input inputs[CYCLES] = {
        //Car_Sensor, temperature, brightness
        input(CAR_ABSENT, TEMPERATURE_LOW,    BRIGHTNESS_HIGH),
        input(CAR_ABSENT, TEMPERATURE_MEDIUM, BRIGHTNESS_HIGH)
    };

    output expected_outputs[CYCLES] = {
        //     Garden_Lamp,    redLed,     greenLed,     Garage_Door,   Front_Door,       Curtain,        fan,     light_pwm0, light_pwm1, light_pwm2, heater_pwm3,display_output
        output(GARDEN_LAMP_ON, REDLED_ON,  GREENLED_OFF, GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_ON,  LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_ON,  DISPLAY_LOWTEMP_HIGHBRIGHT),
        output(GARDEN_LAMP_ON, REDLED_OFF, GREENLED_ON,  GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_OFF, LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_OFF, DISPLAY_MEDTEMP_HIGHBRIGHT)
    };

    return run_test(TEST_ID, TEST_DESCRIPTION, CYCLES, inputs, expected_outputs);
}

int test13_high_temp_greenled_off() {
    const int TEST_ID = 13;
    const int CYCLES = 2;
    const char* TEST_DESCRIPTION = "The inside temperature reaches 30.1°C -> The green led turns off";

    input inputs[CYCLES] = {
        //Car_Sensor, temperature, brightness
        input(CAR_ABSENT, TEMPERATURE_MEDIUM, BRIGHTNESS_HIGH),
        input(CAR_ABSENT, TEMPERATURE_30_1,   BRIGHTNESS_HIGH)
    };

    output expected_outputs[CYCLES] = {
        //     Garden_Lamp,    redLed,     greenLed,     Garage_Door,   Front_Door,       Curtain,        fan,     light_pwm0, light_pwm1, light_pwm2, heater_pwm3,display_output
        output(GARDEN_LAMP_ON, REDLED_OFF, GREENLED_ON,  GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_OFF, LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_OFF, DISPLAY_MEDTEMP_HIGHBRIGHT),
        output(GARDEN_LAMP_ON, REDLED_OFF, GREENLED_OFF, GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_ON,  LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_OFF, DISPLAY_TEMP30_1_HIGHBRIGHT)
    };

    return run_test(TEST_ID, TEST_DESCRIPTION, CYCLES, inputs, expected_outputs);
}

int test14_low_temp_greenled_off() {
    const int TEST_ID = 14;
    const int CYCLES = 2;
    const char* TEST_DESCRIPTION = "The inside temperature is 24.2 °C -> The green led is off";

    input inputs[CYCLES] = {
        //Car_Sensor, temperature, brightness
        input(CAR_ABSENT, TEMPERATURE_MEDIUM, BRIGHTNESS_HIGH),
        input(CAR_ABSENT, TEMPERATURE_LOW,    BRIGHTNESS_HIGH)
    };

    output expected_outputs[CYCLES] = {
        //     Garden_Lamp,    redLed,     greenLed,     Garage_Door,   Front_Door,       Curtain,        fan,     light_pwm0, light_pwm1, light_pwm2, heater_pwm3,display_output
        output(GARDEN_LAMP_ON, REDLED_OFF, GREENLED_ON,  GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_OFF, LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_OFF, DISPLAY_MEDTEMP_HIGHBRIGHT),
        output(GARDEN_LAMP_ON, REDLED_ON,  GREENLED_OFF, GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_ON,  LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_ON,  DISPLAY_LOWTEMP_HIGHBRIGHT)
    };

    return run_test(TEST_ID, TEST_DESCRIPTION, CYCLES, inputs, expected_outputs);
}

int test15_garden_lamp_on() {
    const int TEST_ID = 15;
    const int CYCLES = 1;
    const char* TEST_DESCRIPTION = "House is powered -> Garden lamp is on";

    input inputs[CYCLES] = {
        //Car_Sensor, temperature, brightness
        input(CAR_ABSENT, TEMPERATURE_MEDIUM, BRIGHTNESS_HIGH)
    };

    output expected_outputs[CYCLES] = {
        //     Garden_Lamp,    redLed,     greenLed,     Garage_Door,   Front_Door,       Curtain,        fan,     light_pwm0, light_pwm1, light_pwm2, heater_pwm3,display_output
        output(GARDEN_LAMP_ON, REDLED_OFF, GREENLED_ON,  GARAGE_CLOSED, FRONTDOOR_CLOSED, CURTAIN_CLOSED, FAN_OFF, LIGHT0_OFF, LIGHT1_OFF, LIGHT2_OFF, HEATER_OFF, DISPLAY_MEDTEMP_HIGHBRIGHT)
    };

    return run_test(TEST_ID, TEST_DESCRIPTION, CYCLES, inputs, expected_outputs);
}

int run_test_suite() {
    if( test0_car_opens_garage()            != 0 )  return -1;
    if( test1_absent_car_garage_closed()    != 0 )  return -1;
    if( test2_brightness_low_lights_on()    != 0 )  return -1;
    if( test3_brightness_med_lights_on()    != 0 )  return -1;
    if( test4_brightness_high_lights_off()  != 0 )  return -1;
    if( test5_temperature_on_display()      != 0 )  return -1;
    if( test6_brightness_on_display()       != 0 )  return -1;
    if( test7_low_temp_heater_on()          != 0 )  return -1;
    if( test8_low_temp_fan_on()             != 0 )  return -1;
    if( test9_low_temp_redled_on()          != 0 )  return -1;
    if(test10_high_temp_fan_on()            != 0 )  return -1;
    if(test11_low_temp_fan_off()            != 0 )  return -1;
    if(test12_med_temp_greenled_on()        != 0 )  return -1;
    if(test13_high_temp_greenled_off()      != 0 )  return -1;
    if(test14_low_temp_greenled_off()       != 0 )  return -1;
    if(test15_garden_lamp_on()              != 0 )  return -1;

    return 0;
}

int main() {
    
    int res = run_test_suite();

    if(res != 0) return -1;

    printf("\n" BOLDGREEN "All Tests Passed!\n\n" RESET);

    return 0;
}