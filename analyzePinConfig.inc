//#include "stm32f1xx.h"  // Replace with appropriate header file for your STM32 family

void analyzePinConfig(GPIO_TypeDef *port, uint16_t pin) {
    uint32_t moder = port->MODER;
    uint32_t otyper = port->OTYPER;
    uint32_t ospeedr = port->OSPEEDR;
    uint32_t pupdr = port->PUPDR;
    uint32_t afrl = port->AFR[0];
    uint32_t afrh = port->AFR[1];
    
    uint32_t pin_pos = pin * 2;  // Each pin has two bits in most registers

    // Determine mode
    uint32_t mode = (moder >> pin_pos) & 0x3;
    switch (mode) {
        case 0x00: SerialUSB.println("Input mode"); break;
        case 0x01: SerialUSB.println("Output mode"); break;
        case 0x02: SerialUSB.println("Alternate function mode"); break;
        case 0x03: SerialUSB.println("Analog mode"); break;
    }

    // Determine output type (only relevant if in output mode)
    if (mode == 0x01) {
        uint32_t otype = (otyper >> pin) & 0x1;
        SerialUSB.println(otype == 0 ? "Push-pull" : "Open-drain");
    }

    // Determine pull-up/pull-down
    uint32_t pupd = (pupdr >> pin_pos) & 0x3;
    switch (pupd) {
        case 0x00: SerialUSB.println("No pull-up, pull-down"); break;
        case 0x01: SerialUSB.println("Pull-up"); break;
        case 0x02: SerialUSB.println("Pull-down"); break;
        case 0x03: SerialUSB.println("Reserved"); break;
    }

    // Determine alternate function (only relevant if in AF mode)
    if (mode == 0x02) {
        uint32_t afr;
        if (pin < 8) {
            afr = (afrl >> (pin * 4)) & 0xF;
        } else {
            afr = (afrh >> ((pin - 8) * 4)) & 0xF;
        }
        SerialUSB.print("Alternate Function: AF");
        SerialUSB.println(afr);
    }
}

//void setup() {
//    SerialUSB.begin(9600);
//    while (!SerialUSB); // Wait for SerialUSB to be ready
//    analyzePinConfig(GPIOA, 3); // Analyze configuration for PA3
//}
