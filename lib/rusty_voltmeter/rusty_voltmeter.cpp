#include <rusty_voltmeter.h>

RustyVoltmeter::RustyVoltmeter(uint8_t pin, float r1, float r2, unsigned long delay_ms,float error_correction, float adc_res, float ref_v)
{
    pinMode(pin, INPUT);
    measurement_pin = pin;
    R1 = r1;
    R2 = r2;
    measurement_delay_ms = delay_ms;
    last_measurement_ms = millis();
    adc_resolution = adc_res;
    ref_voltage = ref_v;
    current_voltage = 0.0F;
    changeListener = 0;
    raw_adc = 0;   
    ecc=error_correction;
}

RustyVoltmeter::~RustyVoltmeter()
{
}

void RustyVoltmeter::addChangeListener(void (*listener)(float))
{
    changeListener = listener;
}

float RustyVoltmeter::getVoltage()
{
    update();
    return current_voltage;
}

void RustyVoltmeter::update()
{
    if (!canIRead())
    {
        return;
    }
    int adcValue = analogRead(measurement_pin);
    float voltageMeasured = (adcValue * ref_voltage) / adc_resolution;
    float realVoltage = (voltageMeasured * ((R1 + R2) / R2))+ecc;    
    if (raw_adc != adcValue)
    {
        raw_adc = adcValue;
        current_voltage = realVoltage;
        if (changeListener != NULL)
        {

            changeListener(current_voltage);
        }
    }
    last_measurement_ms = millis();
}

String RustyVoltmeter::toString()
{
    int int_part = (int)current_voltage;
    int dec_part = (int)((current_voltage - int_part) * 100);

    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%d.%02d", int_part, dec_part);
    return String(buffer);
}

bool RustyVoltmeter::canIRead()
{
    return ((millis() - last_measurement_ms) > measurement_delay_ms);
}
