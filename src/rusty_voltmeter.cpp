#include <rusty_voltmeter.h>

RustyVoltmeter::RustyVoltmeter(uint8_t pin, float r1, float r2, unsigned long delay_ms, float error_correction, uint8_t sample_count, float adc_res, float ref_v)
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
    changeAvgListener = 0;
    raw_adc = 0;
    ecc = error_correction;
    sample_limit = sample_count;
    current_sample = 0;
    sample_sum = 0.0F;
    avg_voltage = 0.0F;
}

RustyVoltmeter::~RustyVoltmeter()
{
}

void RustyVoltmeter::addChangeListener(void (*listener)(float))
{
    changeListener = listener;
}

void RustyVoltmeter::addAvgChangeListener(void (*listener)(float))
{
    changeAvgListener = listener;
}

float RustyVoltmeter::getVoltage()
{
    update();
    return current_voltage;
}

float RustyVoltmeter::getAvgVoltage()
{
    update();
    return avg_voltage;
}

void RustyVoltmeter::update()
{
    if (!canIRead())
    {
        return;
    }
    int adc_value = analogRead(measurement_pin);
    float voltage_measured = (adc_value * ref_voltage) / adc_resolution;
    float real_voltage = (voltage_measured * ((R1 + R2) / R2)) + ecc;
    if (raw_adc != adc_value)
    {
        raw_adc = adc_value;
        current_voltage = real_voltage;
        if (changeListener != NULL)
        {
            changeListener(current_voltage);
        }
    }
    checkAvg();
    last_measurement_ms = millis();
}

String RustyVoltmeter::toString()
{
    return floatToString(&current_voltage);
}

String RustyVoltmeter::toAvgString()
{
    return floatToString(&avg_voltage);
}

String RustyVoltmeter::floatToString(float *op_val)
{
    int int_part = (int)(*op_val);
    int dec_part = (int)((*op_val - int_part) * 100);

    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%d.%02d", int_part, dec_part);
    return String(buffer);
}

bool RustyVoltmeter::canIRead()
{
    return ((millis() - last_measurement_ms) > measurement_delay_ms);
}

bool RustyVoltmeter::floatEquals(float a, float b, float epsilon)
{
    return fabs(a - b) < epsilon;
}

void RustyVoltmeter::checkAvg()
{
    current_sample++;
    sample_sum += current_voltage;
    if (current_sample < sample_limit)
    {
        return;
    }
    float new_voltage = sample_sum / sample_limit;
    current_sample = 0;
    sample_sum = 0;
    if (floatEquals(new_voltage, avg_voltage))
    {
        return;
    }

    avg_voltage = new_voltage;

    if (changeAvgListener != NULL)
    {
        changeAvgListener(avg_voltage);
    }
}