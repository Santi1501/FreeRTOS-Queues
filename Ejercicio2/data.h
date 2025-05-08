#ifndef DATA_H
#define DATA_H

#include <Arduino.h>

enum LedCommand {
    LED_RED, // Enciende el LED rojo
    LED_GREEN, // Enciende el LED verde
    LED_BLUE, // Enciende el LED azul
    LED_BLINK // Parpadean los 3 LEDs
};

// Devuelve un comando LED aleatorio
LedCommand randomCommand();

#endif 