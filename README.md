
<pre>
  ....        ...   
 :::::........:::.  
   .:.        .:.    Welcome to the Rusty Voltmeter Project
   .:          :.    ##################################################
  ..:          :.    
.::::..........:...  Hello! With this library,
   .:::........::..  you can measure voltage using a voltage divider 
   .:.:.       :.    with an Arduino analog pin. I’ve tried to add filters 
   .:. .::           as much as possible, and with your contributions,
   .:.   .::.        we can make it even better.
   .:.      ::.      Feel free to use and modify it as you like.
   .:         .:.    Wishing you all great work!
    :           .:  
                  . 
</pre>

# Measuring Voltage with an Arduino Using a Voltage Divider

## What You Need:
1. **Arduino Board** (e.g., Arduino Uno)
2. **Two Resistors** (R1 and R2 for the voltage divider)
3. **Breadboard and Jumper Wires**
4. **Voltage Source** (the voltage you want to measure)

## Step 1: Understand the Voltage Divider Circuit
A voltage divider circuit allows you to measure a voltage that is higher than what the Arduino can handle (usually up to 5V). The formula for the output voltage (\( V_{out} \)) from a voltage divider is:

$$
V_{out} = V_{in} \times \left(\frac{R2}{R1 + R2}\right)
$$

where:
- \( Vin \) is the input voltage you want to measure.
- \( R1 \) is the resistor connected to the input voltage.
- \( R2 \) is the resistor connected to ground.

## Step 2: Connect the Resistors
1. **Connect Resistors**: Connect R1 between the voltage source (the input voltage) and the Arduino analog pin (e.g., A0). Connect R2 between the Arduino analog pin and ground.
2. **Connect Voltage Source**: Connect the voltage source (the voltage you want to measure) to the top of R1.
3. **Ground Connection**: Ensure that the Arduino ground is connected to the ground of the voltage source.

<pre>
       V_in
        |
        |
       R1
        |
        +--------> A0 (Arduino)
        |
       R2
        |
       GND
</pre>

## Step 3: Write the Arduino Code
Here’s a simple Arduino sketch to read the voltage:

```cpp
#include <Arduino.h>
#include <rusty_voltmeter.h>

RustyVoltmeter *voltmeter;

void voltageChange(float val)
{
 // Serial.println(voltmeter->toString());
}

void avgChange(float val)
{
  Serial.println(voltmeter->toAvgString());
}

void setup()
{
  Serial.begin(9600);
  voltmeter = new RustyVoltmeter(
      A5,       // PIN
      90000.0F, // R1
      10000.0F, // R2
      50UL,     // INTERVAL millisec.
      0.0F,     //ERROR CORRECTION
      5,       // SAMPLE LIMIT
      1023.0F, // ADC RESOLUTION
      5.0F     // REF VOLTAGE
  );
  voltmeter->addChangeListener(voltageChange);
  voltmeter->addAvgChangeListener(avgChange);
}

void loop()
{
  voltmeter->update();
}

```


| **Function**                                   | **Description**                                                                                                                                                                                                                                 | **Parameters**                                                                                                                                                                               |
|------------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `RustyVoltmeter(uint8_t pin, float r1, float r2, unsigned long delay_ms = 1UL, float error_correction = 0.0, uint8_t sample_count = 5, float adc_res = 1023.0, float ref_v = 5.0)` | Constructs a RustyVoltmeter object to measure voltage. Initializes the object with parameters required for accurate voltage measurement using a voltage divider and ADC. Supports configurable error correction, sampling, and delay settings. | `pin`: The analog pin number for voltage measurement. <br> `r1`: The value of the first resistor in the voltage divider (in ohms). <br> `r2`: The value of the second resistor (in ohms). <br> `delay_ms`: (Optional) Delay between samples (default 1 ms). <br> `error_correction`: (Optional) Correction factor for measurement errors (default 0.0). <br> `sample_count`: (Optional) Number of samples for averaging (default 5). <br> `adc_res`: (Optional) ADC resolution (default 1023.0). <br> `ref_v`: (Optional) Reference voltage for ADC (default 5.0 V). |
| `~RustyVoltmeter()`                             | Destructor for the RustyVoltmeter class. Releases any resources or memory allocated by the object. Ensures proper cleanup when the object goes out of scope or is no longer needed.                                                              | None                                                                                                                                                                                         |
| `void update()`                                 | Continuously updates the voltage reading. Call this method within the main loop of the Arduino program to monitor and update the voltage reading. It performs sampling, applies error correction, and updates the internal voltage measurement.  | None                                                                                                                                                                                         |
| `float getVoltage()`                           | Returns the current measured voltage. Retrieves the most recent voltage value calculated from the analog pin. Applies scaling based on the voltage divider and any error correction.                                                                | None                                                                                                                                                                                         |
| `float getAvgVoltage()`                        | Returns the average voltage based on multiple samples. Calculates and returns the average voltage from a set of recent samples, smoothing out noise or fluctuations in the readings.                                                            | None                                                                                                                                                                                         |
| `String toString()`                            | Converts the current voltage measurement to a string format. Returns the current measured voltage as a String formatted to display two decimal places for clarity and precision.                                                                    | None                                                                                                                                                                                         |
| `String toAvgString()`                         | Converts the average voltage measurement to a string format. Returns the average voltage as a String formatted to display two decimal places for clarity and precision.                                                                             | None                                                                                                                                                                                         |
| `void addChangeListener(void (*listener)(float))` | Adds a listener that will be called when the voltage changes. Registers a callback function invoked whenever the measured voltage changes, allowing external components to react to voltage changes dynamically.                                  | `listener`: A pointer to the callback function that takes a float parameter representing the updated voltage.                                                                                |
| `void addAvgChangeListener(void (*listener)(float))` | Adds a listener that will be called when the average voltage changes. Registers a callback function invoked whenever the average voltage changes, allowing external components to react dynamically.                                           | `listener`: A pointer to the callback function that takes a float parameter representing the updated average voltage.                                                                         |
