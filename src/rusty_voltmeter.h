
#ifndef RUSTY_VOLTMETER_H
#define RUSTY_VOLTMETER_H

#include <stdint.h>
#include <Arduino.h>
#include <stdlib.h>

class RustyVoltmeter
{
public:
    /**
     * @brief Constructs a RustyVoltmeter object to measure voltage.
     *
     * This constructor initializes a RustyVoltmeter object with parameters required to accurately measure voltage using a voltage divider and analog-to-digital conversion (ADC). It supports configurable error correction, sampling, and delay settings to improve accuracy.
     *
     * @param pin The analog pin number where the voltage measurement will be read.
     * @param r1 The value of the first resistor in the voltage divider (in ohms).
     * @param r2 The value of the second resistor in the voltage divider (in ohms).
     * @param delay_ms Optional. The delay between each sample reading (in milliseconds). Default is 1 ms.
     * @param error_correction Optional. A correction factor for adjusting any measurement errors. Default is 0.0.
     * @param sample_count Optional. The number of samples to take for averaging the voltage reading. Default is 5.
     * @param adc_res Optional. The resolution of the ADC (e.g., 1023 for a 10-bit ADC). Default is 1023.0.
     * @param ref_v Optional. The reference voltage for the ADC (in volts). Default is 5.0 V.
     */
    RustyVoltmeter(uint8_t pin, float r1, float r2, unsigned long delay_ms = 1UL, float error_correction = 0.0, uint8_t sample_count = 5, float adc_res = 1023.0, float ref_v = 5.0);

    /**
     * @brief Destructor for the RustyVoltmeter class.
     *
     * This destructor releases any resources or memory allocated by the RustyVoltmeter object. It ensures that the object is properly cleaned up when it goes out of scope or is no longer needed.
     */
    ~RustyVoltmeter();

    /**
     * @brief Continuously updates the voltage reading.
     *
     * This method should be called within the main loop of the Arduino program to continuously monitor and update the voltage reading. It performs the necessary sampling, applies error correction, and updates the internal voltage measurement at the specified intervals.
     *
     * Call this function repeatedly inside the `loop()` to ensure accurate and up-to-date voltage measurements.
     */
    void update();

    /**
     * @brief Returns the current measured voltage.
     *
     * This method retrieves the most recent voltage value calculated from the analog pin. It applies the necessary scaling based on the voltage divider and any error correction, returning the final voltage in volts.
     *
     * @return float The most recently measured voltage in volts.
     */
    float getVoltage();

    /**
     * @brief Returns the average voltage based on multiple samples.
     *
     * This method calculates and returns the average voltage from a set of recent samples. The number of samples used for averaging is defined during object construction. This helps in smoothing out any noise or fluctuations in the voltage readings.
     *
     * @return float The average voltage in volts.
     */
    float getAvgVoltage();

    /**
     * @brief Converts the current voltage measurement to a string format.
     *
     * This method returns the current measured voltage as a String. The voltage value is formatted to display two decimal places, ensuring clarity and precision in the representation of the voltage measurement.
     *
     * @return String The current voltage formatted as a string with two decimal places.
     */
    String toString();

    /**
     * @brief Converts the average voltage measurement to a string format.
     *
     * This method returns the average voltage, calculated from multiple samples, as a String. The voltage value is formatted to display two decimal places, ensuring clarity and precision in the representation of the average voltage measurement.
     *
     * @return String The average voltage formatted as a string with two decimal places.
     */
    String toAvgString();

    /**
     * @brief Adds a listener that will be called when the voltage changes.
     *
     * This method allows the registration of a callback function that will be invoked whenever the measured voltage changes. The callback receives the updated voltage value as a parameter, enabling external components or systems to react to voltage changes dynamically.
     *
     * @param listener A pointer to the callback function that takes a float parameter representing the updated voltage.
     */
    void addChangeListener(void (*listener)(float));

    /**
     * @brief Adds a listener that will be called when the average voltage changes.
     *
     * This method allows the registration of a callback function that will be invoked whenever the average voltage, calculated from multiple samples, changes. The callback receives the updated average voltage value as a parameter, enabling external components or systems to react to changes in the average voltage dynamically.
     *
     * @param listener A pointer to the callback function that takes a float parameter representing the updated average voltage.
     */
    void addAvgChangeListener(void (*listener)(float));

    /**
     * @brief Converts a float value to a String representation with two decimal places.
     *
     * @param op_val Pointer to the float value to be converted.
     * @return String The String representation of the float value.
     */
    String floatToString(float *op_val);

private:
    /**
     * @brief The pin number used for voltage measurement.
     */
    uint8_t measurement_pin;

    /**
     * @brief The value of the first resistor in the voltage divider (in ohms).
     */
    float R1;

    /**
     * @brief The value of the second resistor in the voltage divider (in ohms).
     */
    float R2;

    /**
     * @brief The delay between measurements (in milliseconds).
     */
    unsigned long measurement_delay_ms;

    /**
     * @brief The timestamp of the last measurement taken (in milliseconds since the program started).
     */
    unsigned long last_measurement_ms;

    /**
     * @brief The resolution of the analog-to-digital converter (ADC).
     */
    float adc_resolution;

    /**
     * @brief The reference voltage for the ADC (in volts).
     */
    float ref_voltage;

    /**
     * @brief The current measured voltage (in volts).
     */
    float current_voltage;

    /**
     * @brief Error correction coefficient for voltage measurements.
     */
    float ecc;

    /**
     * @brief Checks if a new voltage reading can be taken.
     *
     * This function determines if the measurement can be taken based on timing and sampling conditions.
     *
     * @return bool True if a measurement can be taken; otherwise, false.
     */
    bool canIRead();

    /**
     * @brief Pointer to a callback function for change listener on voltage changes.
     */
    void (*changeListener)(float);

    /**
     * @brief Pointer to a callback function for average voltage change listener.
     */
    void (*changeAvgListener)(float);

    /**
     * @brief The raw value read from the ADC.
     */
    int raw_adc;

    /**
     * @brief The maximum number of samples to take for averaging.
     */
    uint8_t sample_limit;

    /**
     * @brief The index of the current sample being processed.
     */
    uint8_t current_sample;

    /**
     * @brief The sum of the samples taken for averaging the voltage.
     */
    float sample_sum;

    /**
     * @brief The average voltage calculated from the samples (in volts).
     */
    float avg_voltage;

    /**
     * @brief Compares two float values for equality within a specified tolerance.
     *
     * This function checks if two float values are approximately equal, allowing for a small margin of error defined by epsilon.
     *
     * @param a The first float value.
     * @param b The second float value.
     * @param epsilon The tolerance for equality comparison. Default is 0.1.
     * @return bool True if the values are approximately equal; otherwise, false.
     */
    bool floatEquals(float a, float b, float epsilon = 0.1);

    /**
     * @brief Checks and updates the average voltage based on the current samples.
     *
     * This function calculates the average voltage from the collected samples and updates the average voltage value accordingly.
     */
    void checkAvg();
};
#endif