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
#include <semphr.h>

typedef struct {
    const char *taskName;
    uint32_t delayMs;  
    uint32_t initialDelayMs;
} ParametrosTarea_t;


//! Hacerlo con semaforos

// --- Variables Globales para Semáforos ---
SemaphoreHandle_t semT1;
SemaphoreHandle_t semT2;
SemaphoreHandle_t semT3;


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

// /**
//  * @brief Tarea 1: Espera semT1, imprime, libera semT3
//  */
// void vTask1(void *pvParameters)
// {
//     while (1)
//     {
//         // Espera a que sea su turno (bloquea) 
//         if (xSemaphoreTake(semT1, portMAX_DELAY) == pdTRUE)
//         {
//             Serial.println("Tarea 1");
//             // Pausa opcional para legibilidad
//             vTaskDelay(1000 / portTICK_PERIOD_MS); 
            
//             // Libera el semáforo de la *siguiente* tarea en la secuencia (T3) 
//             xSemaphoreGive(semT3);
//         }
//     }
// }

// /**
//  * @brief Tarea 2: Espera semT2, imprime, libera semT1
//  */
// void vTask2(void *pvParameters)
// {
//     while (1)
//     {
//         // Espera a que sea su turno (bloquea) 
//         if (xSemaphoreTake(semT2, portMAX_DELAY) == pdTRUE)
//         {
//             Serial.println("Tarea 2");
//             vTaskDelay(1000 / portTICK_PERIOD_MS);
            
//             // Libera el semáforo de la *siguiente* tarea (T1, reinicia el ciclo) 
//             xSemaphoreGive(semT1);
//         }
//     }
// }

// /**
//  * @brief Tarea 3: Espera semT3, imprime, libera semT2
//  */
// void vTask3(void *pvParameters)
// {
//     while (1)
//     {
//         // Espera a que sea su turno (bloquea) 
//         if (xSemaphoreTake(semT3, portMAX_DELAY) == pdTRUE)
//         {
//             Serial.println("Tarea 3");
//             vTaskDelay(1000 / portTICK_PERIOD_MS);
            
//             // Libera el semáforo de la *siguiente* tarea (T2) 
//             xSemaphoreGive(semT2);
//         }
//     }
// }


// void setup()
// {
//     Serial.begin(9600);
    
//     // Crear los semáforos binarios 
//     // Inician "vacíos" (tomados)
//     semT1 = xSemaphoreCreateBinary();
//     semT2 = xSemaphoreCreateBinary();
//     semT3 = xSemaphoreCreateBinary();

//     // Crear las tareas. Todas con la misma prioridad [cite: 228]
//     xTaskCreate(vTask1, "Tarea 1", 128, NULL, 1, NULL);
//     xTaskCreate(vTask2, "Tarea 2", 128, NULL, 1, NULL);
//     xTaskCreate(vTask3, "Tarea 3", 128, NULL, 1, NULL);
    
//     // Damos el primer semáforo (T1) para que la secuencia comience
//     xSemaphoreGive(semT1);

//     vTaskStartScheduler();
// }

//! ---- Ejercicio 4, B. ----
// /**
//  * @brief Tarea 1: Espera semT1, imprime, libera semT2 (reinicia el ciclo)
//  */
// void vTask1(void *pvParameters)
// {
//     while (1)
//     {
//         // 4. Espera el turno de T3
//         if (xSemaphoreTake(semT1, portMAX_DELAY) == pdTRUE)
//         {
//             Serial.println("Tarea 1");
//             vTaskDelay(1000 / portTICK_PERIOD_MS);
            
//             // 5. Cede el paso a T2 para reiniciar el ciclo
//             xSemaphoreGive(semT2);
//         }
//     }
// }

// /**
//  * @brief Tarea 2: Espera semT2, imprime DOS VECES, libera semT3
//  */
// void vTask2(void *pvParameters)
// {
//     while (1)
//     {
//         // 1. Espera el turno (inicial o de T1)
//         if (xSemaphoreTake(semT2, portMAX_DELAY) == pdTRUE)
//         {
//             // Primera ejecución
//             Serial.println("Tarea 2");
//             vTaskDelay(1000 / portTICK_PERIOD_MS);

//             // Segunda ejecución
//             Serial.println("Tarea 2");
//             vTaskDelay(1000 / portTICK_PERIOD_MS);
            
//             // 2. Cede el paso a T3
//             xSemaphoreGive(semT3);
//         }
//     }
// }

// /**
//  * @brief Tarea 3: Espera semT3, imprime, libera semT1
//  */
// void vTask3(void *pvParameters)
// {
//     while (1)
//     {
//         // 3. Espera el turno de T2
//         if (xSemaphoreTake(semT3, portMAX_DELAY) == pdTRUE)
//         {
//             Serial.println("Tarea 3");
//             vTaskDelay(1000 / portTICK_PERIOD_MS);
            
//             // 4. Cede el paso a T1
//             xSemaphoreGive(semT1);
//         }
//     }
// }


// void setup()
// {
//     Serial.begin(9600);
    
//     // Crear los semáforos binarios 
//     semT1 = xSemaphoreCreateBinary();
//     semT2 = xSemaphoreCreateBinary();
//     semT3 = xSemaphoreCreateBinary();

//     // Crear las tareas.
//     xTaskCreate(vTask1, "Tarea 1", 128, NULL, 1, NULL);
//     xTaskCreate(vTask2, "Tarea 2", 128, NULL, 1, NULL);
//     xTaskCreate(vTask3, "Tarea 3", 128, NULL, 1, NULL);
    
//     //La secuencia empieza con Tarea 2
//     xSemaphoreGive(semT2);

//     vTaskStartScheduler();
// }

//! ---- Ejercicio 4, C. ----
/**
 * @brief Tarea 1: Espera semT1, imprime, libera semT2
 */
void vTask1(void *pvParameters)
{
    while (1)
    {
        // 2. Espera el turno de T3
        if (xSemaphoreTake(semT1, portMAX_DELAY) == pdTRUE)
        {
            Serial.println("Tarea 1");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            
            // 3. Cede el paso a T2
            xSemaphoreGive(semT2);
        }
    }
}

/**
 * @brief Tarea 2: Espera semT2, imprime, libera semT3 (reinicia el ciclo)
 */
void vTask2(void *pvParameters)
{
    while (1)
    {
        // 4. Espera el turno de T1
        if (xSemaphoreTake(semT2, portMAX_DELAY) == pdTRUE)
        {
            Serial.println("Tarea 2");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            
            // 5. Cede el paso a T3 para reiniciar el ciclo
            xSemaphoreGive(semT3);
        }
    }
}

/**
 * @brief Tarea 3: Espera semT3, imprime TRES VECES, libera semT1
 */
void vTask3(void *pvParameters)
{
    while (1)
    {
        // 1. Espera el turno 
        if (xSemaphoreTake(semT3, portMAX_DELAY) == pdTRUE)
        {
            // Primera ejecución
            Serial.println("Tarea 3");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            
            // Segunda ejecución
            Serial.println("Tarea 3");
            vTaskDelay(1000 / portTICK_PERIOD_MS);

            // Tercera ejecución
            Serial.println("Tarea 3");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            
            // 2. Cede el paso a T1
            xSemaphoreGive(semT1);
        }
    }
}


void setup()
{
    Serial.begin(9600);
    
    // Crear los semáforos binarios 
    semT1 = xSemaphoreCreateBinary();
    semT2 = xSemaphoreCreateBinary();
    semT3 = xSemaphoreCreateBinary();

    // Crear las tareas.
    xTaskCreate(vTask1, "Tarea 1", 128, NULL, 1, NULL);
    xTaskCreate(vTask2, "Tarea 2", 128, NULL, 1, NULL);
    xTaskCreate(vTask3, "Tarea 3", 128, NULL, 1, NULL);
    
    // La secuencia empieza con Tarea 3
    xSemaphoreGive(semT3);

    vTaskStartScheduler();
}

void loop()
{
}

