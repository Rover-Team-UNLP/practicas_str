/* 
    Ejercicios 2 y 4. Ir comentando y sacando comentarios para comprobar la ejecucion.
    Se utilizo platformio para correr el codigo. El platformio.ini es asi:
    
    [env:uno]
    platform = atmelavr
    board = uno
    framework = arduino
    lib_deps = feilipu/FreeRTOS@^11.1.0-3 
*/


#include <stdlib.h>
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

typedef struct {
    const char *taskName;
    uint32_t delayMs;  
    uint32_t initialDelayMs;
} ParametrosTarea_t;


//! Hacerlo con semaforos
void vTaskFunction(void *pvParameters)
{
    ParametrosTarea_t *params = (ParametrosTarea_t *)pvParameters;

    if (params->initialDelayMs > 0)
        vTaskDelay(params->initialDelayMs / portTICK_PERIOD_MS);

    while (1)
    {
        Serial.println(params->taskName);
        vTaskDelay(params->delayMs / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

//! ---- Ejercicio 1 ----

// void setup()
// {
//     Serial.begin(9600);

//     static const ParametrosTarea_t paramsTask1 = {(char *)"Task 1 is running", 200, 0};
//     static const ParametrosTarea_t paramsTask2 = {(char *)"Task 2 is running", 500, 0};
//     static const ParametrosTarea_t paramsTask3 = {(char *)"Task 3 is running", 800, 0};
    
//     xTaskCreate(vTaskFunction, "Tarea 1", 128, (void *)&paramsTask1, 1, NULL);
//     xTaskCreate(vTaskFunction, "Tarea 2", 128, (void *)&paramsTask2, 2, NULL);
//     xTaskCreate(vTaskFunction, "Tarea 3", 128, (void *)&paramsTask3, 3, NULL);
    
//     vTaskStartScheduler();
// }


//! ---- Ejercicio 4, A. ----


// void setup()
// {
//     Serial.begin(9600);
//     static const ParametrosTarea_t paramsTask1 = {(char *)"Tarea 1", 1000, 0};
//     static const ParametrosTarea_t paramsTask2 = {(char *)"Tarea 2", 1000, 20};
//     static const ParametrosTarea_t paramsTask3 = {(char *)"Tarea 3", 1000, 10};

//     xTaskCreate(vTaskFunction, "Tarea 1", 128, (void *)&paramsTask1, 1, NULL);
//     xTaskCreate(vTaskFunction, "Tarea 2", 128, (void *)&paramsTask2, 1, NULL);
//     xTaskCreate(vTaskFunction, "Tarea 3", 128, (void *)&paramsTask3, 1, NULL);
    
//     vTaskStartScheduler();
// }

//! ---- Ejercicio 4, B. ----

// void setup()
// {
//     Serial.begin(9600);
//     static const ParametrosTarea_t paramsTask1 = {(char *)"Tarea 1", 2000, 20};
//     static const ParametrosTarea_t paramsTask2 = {(char *)"Tarea 2", 1000, 0};
//     static const ParametrosTarea_t paramsTask3 = {(char *)"Tarea 3", 2000, 10};

//     xTaskCreate(vTaskFunction, "Tarea 1", 128, (void *)&paramsTask1, 1, NULL);
//     xTaskCreate(vTaskFunction, "Tarea 2", 128, (void *)&paramsTask2, 1, NULL);
//     xTaskCreate(vTaskFunction, "Tarea 3", 128, (void *)&paramsTask3, 1, NULL);
    
//     vTaskStartScheduler();
// }

//! ---- Ejercicio 4, C. ----

void setup()
{
    Serial.begin(9600);
    static const ParametrosTarea_t paramsTask1 = {(char *)"Tarea 1", 3000, 10};
    static const ParametrosTarea_t paramsTask2 = {(char *)"Tarea 2", 3000, 20};
    static const ParametrosTarea_t paramsTask3 = {(char *)"Tarea 3", 1000, 0};

    xTaskCreate(vTaskFunction, "Tarea 1", 128, (void *)&paramsTask1, 1, NULL);
    xTaskCreate(vTaskFunction, "Tarea 2", 128, (void *)&paramsTask2, 1, NULL);
    xTaskCreate(vTaskFunction, "Tarea 3", 128, (void *)&paramsTask3, 1, NULL);
    
    vTaskStartScheduler();
}

void loop()
{
}

