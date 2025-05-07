#include "data.h"

QueueHandle_t myQueue;

Person persona;

void setup() {
  
  myQueue = xQueueCreate(30, sizeof(Person));

  Serial.begin(9600);
  xTaskCreatePinnedToCore(
  consumidor, // Referencia a la funcion que vamos a ejecutar
  "Task_Core1", // Nombre para la funcion, sirve solamente para propositos de debugging
  4096, // Tamaño del stack la tarea
  NULL, // Parametro que recibe la funcion que le vamos a pasar
  1, // Prioridad de la tarea
  NULL, // no es importante
  1 // El core donde queremos que corra la task (0/1)
  );

  xTaskCreatePinnedToCore(
  productor, // Referencia a la funcion que vamos a ejecutar
  "Task_Core0", // Nombre para la funcion, sirve solamente para propositos de debugging
  4096, // Tamaño del stack la tarea
  NULL, // Parametro que recibe la funcion que le vamos a pasar
  1, // Prioridad de la tarea
  NULL, // no es importante
  0 // El core donde queremos que corra la task (0/1)
  );
}

void productor(void * pvParameters) {

  for(;;){

    delay(1000);

    int Id = RandomId();
    xQueueSend(myQueue, &getPersonById(Id), portMAX_DELAY);

    if(){


    }
  }
}

void consumidor(void * pvParameters) {

  for(;;){

    delay(1000);

    xQueueReceive(myQueue, &persona, portMAX_DELAY);

    if(){


    }

    Serial.println(persona.id);
    Serial.println(persona.name);
    Serial.println(persona.last_name);
    Serial.println(persona.email);
  }
}

void loop() {
  

}