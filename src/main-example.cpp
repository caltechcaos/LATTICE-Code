#include <Arduino.h>

#include "Subsystems/ExampleSubsystem.h"

ExampleSubsystem m_subsystem{};
void setup() {}

void loop() {
    m_subsystem.ReturnTestFunc();
}