
#ifndef RUSTY_VOLTMETER_H
#define RUSTY_VOLTMETER_H

#include <stdint.h>
#include <Arduino.h>
#include <stdlib.h>


class RustyVoltmeter
{
public:
    RustyVoltmeter(uint8_t pin, float r1, float r2, unsigned long delay_ms = 1UL, float error_correction = 0.0, float adc_res = 1023.0, float ref_v = 5.0);

    ~RustyVoltmeter();

    void update();

    float getVoltage();

    String toString();

    void addChangeListener(void (*listener)(float));

private:
    uint8_t measurement_pin;
    float R1;
    float R2;
    unsigned long measurement_delay_ms;
    unsigned long last_measurement_ms;
    float adc_resolution;
    float ref_voltage;
    float current_voltage;
    float ecc;
    bool canIRead();
    void (*changeListener)(float);
    int raw_adc;    
};
#endif