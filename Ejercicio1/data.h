#ifndef DATA_H
#define DATA_H

#include <Arduino.h>

struct Person
{
    int id;
    String name;
    String last_name;
    String email;
};

// Devuelve un id aleatorio de cualquier persona
int randomId();

// Devuelve la persona correspondiente al id
Person getPersonById(int id);

#endif