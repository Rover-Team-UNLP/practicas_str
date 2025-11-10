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
} ParametrosTarea_t;

void vTaskFunction(void *pvParameters)
{
    ParametrosTarea_t *params = (ParametrosTarea_t *)pvParameters;

    for (;;)
    {
        Serial.println(params->taskName);
        vTaskDelay(params->delayMs / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

//! ---- Ejercicio 1 ----

void setup()
{
    Serial.begin(9600);

    static const ParametrosTarea_t paramsTask1 = {(char *)"Task 1 is running", 200};
    static const ParametrosTarea_t paramsTask2 = {(char *)"Task 2 is running", 500};
    static const ParametrosTarea_t paramsTask3 = {(char *)"Task 3 is running", 800};
    
    xTaskCreate(vTaskFunction, "Tarea 1", 128, (void *)&paramsTask1, 1, NULL);
    xTaskCreate(vTaskFunction, "Tarea 2", 128, (void *)&paramsTask2, 2, NULL);
    xTaskCreate(vTaskFunction, "Tarea 3", 128, (void *)&paramsTask3, 3, NULL);
    
    vTaskStartScheduler();
}


//! ---- Ejercicio 4, A. ----


// void setup()
// {
//     Serial.begin(9600);
//     static const ParametrosTarea_t paramsTask1 = {(char *)"Tarea 1", 1000};
//     static const ParametrosTarea_t paramsTask2 = {(char *)"Tarea 2", 1010};
//     static const ParametrosTarea_t paramsTask3 = {(char *)"Tarea 3", 1005};

//     xTaskCreate(vTaskFunction, "Tarea 1", 128, (void *)&paramsTask1, 1, NULL);
//     xTaskCreate(vTaskFunction, "Tarea 2", 128, (void *)&paramsTask2, 1, NULL);
//     xTaskCreate(vTaskFunction, "Tarea 3", 128, (void *)&paramsTask3, 1, NULL);
    
//     vTaskStartScheduler();
// }

//! ---- Ejercicio 4, B. ----

// void setup()
// {
//     Serial.begin(9600);
//     static const ParametrosTarea_t paramsTask1 = {(char *)"Tarea 1", 4040};
//     static const ParametrosTarea_t paramsTask2 = {(char *)"Tarea 2", 2000};
//     static const ParametrosTarea_t paramsTask3 = {(char *)"Tarea 3", 4020};

//     xTaskCreate(vTaskFunction, "Tarea 1", 128, (void *)&paramsTask1, 1, NULL);
//     xTaskCreate(vTaskFunction, "Tarea 2", 128, (void *)&paramsTask2, 1, NULL);
//     xTaskCreate(vTaskFunction, "Tarea 3", 128, (void *)&paramsTask3, 1, NULL);
    
//     vTaskStartScheduler();
// }

//! ---- Ejercicio 4, C. ----

// void setup()
// {
//     Serial.begin(9600);
//     static const ParametrosTarea_t paramsTask1 = {(char *)"Tarea 1", 3050};
//     static const ParametrosTarea_t paramsTask2 = {(char *)"Tarea 2", 3100};
//     static const ParametrosTarea_t paramsTask3 = {(char *)"Tarea 3", 1000};

//     xTaskCreate(vTaskFunction, "Tarea 1", 128, (void *)&paramsTask1, 1, NULL);
//     xTaskCreate(vTaskFunction, "Tarea 2", 128, (void *)&paramsTask2, 1, NULL);
//     xTaskCreate(vTaskFunction, "Tarea 3", 128, (void *)&paramsTask3, 1, NULL);
    
//     vTaskStartScheduler();
// }

void loop()
{
}

