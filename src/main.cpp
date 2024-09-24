#include <Arduino.h>
#include <rusty_voltmeter.h>

RustyVoltmeter *voltmeter;

void voltageChange(float val)
{
  Serial.println(voltmeter->toString());
}

void setup()
{
  Serial.begin(9600);
  voltmeter = new RustyVoltmeter(
      A5,       // PIN
      90000.0F, // R1
      10000.0F, // R2
      50UL,     // INTERVAL millisec.
      0.0F,     // ERROR CORRECTION
      1023.0F,  // ADC RESOLUTION
      5.0F      // REF VOLTAGE
  );
  voltmeter->addChangeListener(voltageChange);
}

void loop()
{
  voltmeter->update();
}
