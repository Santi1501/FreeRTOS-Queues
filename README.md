📦 Ejercicio 1: Productor y Consumidor con estructuras de personas

Consistencia del trabajo:

El objetivo es simular una situación donde una tarea "productor" genera datos (información de personas con nombre, apellido y correo) y otra tarea "consumidor" los procesa, en este caso imprimiéndolos por el monitor serie.

Para coordinar esta interacción entre tareas y evitar conflictos de acceso, se utiliza una cola (Queue), una estructura que permite el paso seguro de datos entre tareas concurrentes. 

Pasos a seguir:

Definiciones globales:
Se define una estructura llamada Person con tres campos (nombre, apellido, correo) para almacenar la información de una persona. Luego, se declara una variable de tipo QueueHandle_t llamada queue, que actuará como intermediaria entre las tareas.

Función setup():
Se inicializa la comunicación serie con Serial.begin(9600) para visualizar los datos. Se crea una cola de hasta 30 elementos tipo Person usando xQueueCreate(). Después, se crean dos tareas con xTaskCreatePinnedToCore():

-Una para el productor (que se fija al core 0).

-Otra para el consumidor (que se fija al core 1).

Esto permite que ambas tareas funcionen en paralelo, aprovechando los dos núcleos del ESP32.

Función loop():
Se deja vacía, ya que toda la lógica del programa ocurre en las tareas creadas manualmente, gestionadas por el sistema FreeRTOS.

Tarea "productor":
Esta tarea contiene un arreglo con datos de 5 personas diferentes. En cada iteración:

-Selecciona una persona del arreglo y la copia a una estructura temporal.

-Envia esa estructura a la cola mediante xQueueSend().

-Muestra en el monitor serie que se produjo una persona.

-Espera un segundo antes de continuar con la siguiente persona, simulando el tiempo de producción.

Tarea "consumidor":
Esta tarea permanece esperando elementos en la cola. Cuando recibe uno:

Lo imprime en el monitor serie, mostrando el nombre, apellido y correo.

Luego, espera un segundo antes de continuar, simulando el tiempo de procesamiento.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

💡 Ejercicio 2: Comandos de LED con prioridad

Consistencia del trabajo:

En este segundo ejercicio se implementa un sistema donde una tarea "productor" genera comandos para controlar el encendido de LEDs y una tarea "consumidor" los ejecuta. El sistema también utiliza FreeRTOS y colas para la comunicación entre tareas, pero incorpora prioridad en el procesamiento. Si el comando recibido es "parpadeo", se le da prioridad, insertándolo al principio de la cola para ser procesado antes que los demás. Esta lógica simula una situación real donde ciertos eventos deben ser tratados con urgencia.

Pasos a seguir:

Definiciones globales:
Se definen constantes para los pines de los LEDs rojo, verde y amarillo. Se declara un tipo enum llamado LedCommand con las opciones: LED_RED, LED_GREEN, LED_YELLOW, LED_BLINK. También se define una variable QueueHandle_t llamada ledCommandQueue, que almacenará los comandos generados.

Función setup():
Se inicia la comunicación serie y se configuran los pines como salidas. Luego, se crea una cola capaz de almacenar hasta 5 comandos tipo LedCommand. Se lanzan dos tareas:

-La del productor (asignada a un core).

-La del consumidor (asignada al otro).

Función loop():
También queda vacía ya que el control se transfiere completamente a las tareas de FreeRTOS.

Tarea "productor":
En cada ciclo:

-Se genera aleatoriamente un número entre 0 y 3, que corresponde a un comando LED.

-Si el comando es LED_BLINK, se lo envía al frente de la cola con xQueueSendToFront(), dándole máxima prioridad.

-Si es cualquier otro (rojo, verde, amarillo), se lo envía al final de la cola con xQueueSendToBack().

-Se imprime por serie el comando generado y se espera 1 segundo antes de repetir el ciclo.

Tarea "consumidor":
Esta tarea espera comandos de la cola. Cuando recibe uno:

Ejecuta la acción correspondiente:

-Enciende el LED rojo, verde o amarillo, según el comando recibido.

-Si el comando es LED_BLINK, hace parpadear los tres LEDs dos veces con una pausa de 250 ms entre encendido y apagado.

-Después de ejecutar cualquier comando, se apagan todos los LEDs y se espera un segundo.