// Define la estructura Person y funciones como getPersonById() y randomId()
#include "data.h"

// Cola para compartir personas entre productor y consumidor
QueueHandle_t myQueue;

// Configuración inicial
void setup() {
  
  // Inicializar la cola con capacidad para 30 elementos tipo Person
  myQueue = xQueueCreate(30, sizeof(Person));

  // Iniciar comunicación serial
  Serial.begin(9600);

  // Crear tarea consumidor en el Core 1
  xTaskCreatePinnedToCore(
  consumidor, // Función que ejecuta la tarea
  "Task_Core1", // Nombre de la tarea (para debugging)
  4096, // Tamaño del stack 
  NULL, // Parametro que recibe la funcion que le vamos a pasar
  1, // Prioridad de la tarea
  NULL, // no es importante
  1 // El core donde queremos que corra la task (0/1)
  );

  // Crear tarea productor en el Core 0
  xTaskCreatePinnedToCore(
  productor, // Función que ejecuta la tarea
  "Task_Core0", // Nombre de la tarea
  4096, // Tamaño del stack 
  NULL, // Parametro que recibe la funcion que le vamos a pasar
  1, // Prioridad de la tarea
  NULL, // no es importante
  0 // El core donde queremos que corra la task (0/1)
  );
}

// Tarea Productor (Core 0)
void productor(void * pvParameters) {

  for(;;){
    
    // Esperar 1 segundo antes de producir una nueva persona
    delay(1000);

    // Obtener un ID aleatorio
    int Id = randomId();

    // Obtener la estructura Person correspondiente al ID
    Person p = getPersonById(Id);

    // Enviar la persona a la cola
    xQueueSend(myQueue, &p, portMAX_DELAY);

  }
}

// Tarea Consumidor (Core 1)
void consumidor(void * pvParameters) {

  for(;;){

    // Esperar 1 segundo antes de consumir
    delay(1000);

    // Variable global para almacenar la persona recibida
    Person persona;

    // Recibir una persona de la cola
    if (xQueueReceive(myQueue, &persona, portMAX_DELAY) == pdTRUE) {

      Serial.print("ID: ");
      Serial.println(persona.id);
      Serial.print("Nombre: ");
      Serial.println(persona.name);
      Serial.print("Apellido: ");
      Serial.println(persona.last_name);
      Serial.print("Email: ");
      Serial.println(persona.email);
      Serial.println("--------------------");
    }
  }
}

// No se usa loop() en FreeRTOS
void loop() {
  

}