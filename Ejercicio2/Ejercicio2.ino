 // Define el enum LedCommand y la función randomCommand()
#include "data.h"

// Cola para compartir comandos LED entre productor y consumidor
QueueHandle_t myQueue;

// Pines para los LEDs
#define LED_RED_PIN 23
#define LED_GREEN_PIN 22
#define LED_YELLOW_PIN 21

// Configuración inicial
void setup() {
  
  // Configuración de pines como salidas
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_YELLOW_PIN, OUTPUT);

  // Crear una cola con capacidad para 5 comandos LED
  myQueue = xQueueCreate(5, sizeof(LedCommand));
  
  // Iniciar la comunicación serial
  Serial.begin(9600);

  // Crear tarea del consumidor en el Core 1
  xTaskCreatePinnedToCore(
  consumidor, // Referencia a la funcion que vamos a ejecutar
  "Task_Core1", // Nombre para la funcion, sirve solamente para propositos de debugging
  4096, // Tamaño del stack la tarea
  NULL, // Parametro que recibe la funcion que le vamos a pasar
  1, // Prioridad de la tarea
  NULL, // no es importante
  1 // El core donde queremos que corra la task (0/1)
  );

  // Crear tarea productor en el Core 0
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

// Función que se usa en la tarea Consumidor para encender los LEDS, de acuerdo al comando
void setRGB(bool R, bool G, bool B) {

  // LED rojo
  if(R){

    digitalWrite(LED_RED_PIN, HIGH);
  }

  // LED amarillo
  if(G){

    digitalWrite(LED_YELLOW_PIN, HIGH);
  }

  // LED verde
  if(B){

    digitalWrite(LED_GREEN_PIN, HIGH);
  }

  delay(1000);
  digitalWrite(LED_RED_PIN, LOW);
  digitalWrite(LED_YELLOW_PIN, LOW);
  digitalWrite(LED_GREEN_PIN, LOW);
}

// Tarea Productor (Core 0)
void productor(void * pvParameters) {

  for(;;){

    delay(1000);

    // Generar un comando LED aleatorio
    LedCommand cmd = randomCommand();

    // Insertar en la cola: LED_BLINK tiene prioridad (al frente), el resto al final
    if(cmd == LED_BLINK){

      xQueueSendToFront(myQueue, &cmd, portMAX_DELAY); // prioridad
    } 
    
    else {
      
      xQueueSendToBack(myQueue, &cmd, portMAX_DELAY); // normal
    }

    /* Informar por Monitor Serial
    Serial.print("Producido: ");
    Serial.println(cmd);*/
  }
}

// Tarea Consumidor (Core 1)
void consumidor(void * pvParameters) {

  for(;;){

    delay(1000);

    LedCommand cmd;

    // Esperar y recibir un comando de la cola
    if(xQueueReceive(myQueue, &cmd, portMAX_DELAY) == pdTRUE){

      Serial.print("Ejecutando: ");
      Serial.println(cmd);

      if(cmd == LED_RED){

        setRGB(1, 0, 0);
      }
      if(cmd == LED_GREEN){

        setRGB(0, 1, 0);
      }
      if(cmd == LED_YELLOW){

        setRGB(0, 0, 1);
      }

      // Enciende todos los LEDs
      if(cmd == LED_BLINK){

        setRGB(1, 1, 1);
      }
    }
  }
}

// Loop vacío: no se utiliza en FreeRTOS, todo corre en las tareas
void loop() {
  

}
