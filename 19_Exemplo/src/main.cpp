/*********************************************************
* Exemplo para demonstrar o uso de contagem das
* task Notification
* Por: Fábio Souza
*********************************************************/

/*inclusão da Biblioteca Arduino*/
#include <Arduino.h>

/*inclusão das Biblioteca do FreeRTOS*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

/*definição dos pinos*/
#define LED 2       /*LED ligado ao GPIO2*/
#define BT 12


/* Variáveis para armazenar o handle da Task */
TaskHandle_t xTaskTrataISRBTHandle;

/* Protótipo das Tasks*/
void vTaskTrataISRBT( void *pvParameters );

/* Funções auxiliares */
void vInitHW(void);

/* callback da interrupção do BT */
void ISRcallback() {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  vTaskNotifyGiveFromISR( xTaskTrataISRBTHandle, &xHigherPriorityTaskWoken );

}

void setup() {

  vInitHW();
  
  /* Cria task para tratar BT */
  xTaskCreatePinnedToCore(vTaskTrataISRBT,  "Task Trata ISR BT",  configMINIMAL_STACK_SIZE + 1024,  NULL,  1,  &xTaskTrataISRBTHandle,APP_CPU_NUM);    

}

void loop() {

}

/* Função Init Hardware */
void vInitHW(void)
{
    Serial.begin(9600); /* Inicializa comunicação serial com baudrate de 9600 bps */
    pinMode(LED,OUTPUT); /* configura pino do LED como saída*/
    pinMode(BT, INPUT_PULLUP); /*configura pino do Botão */ 
    /*Configura interrupção do pino do botão*/
    attachInterrupt(digitalPinToInterrupt(BT), ISRcallback, FALLING);

}

/* Task para tratar BT pressionado*/
void vTaskTrataISRBT( void *pvParameters )
{
  (void) pvParameters;
  uint32_t quant = 0;

  while(1)
  {
    /* Aguarda a notificação*/
    quant = ulTaskNotifyTake( pdFALSE, portMAX_DELAY );

    Serial.print("Tratando a interrupção: ");
    Serial.println(quant);
    /*inverte estado do LED*/
    digitalWrite(LED, !digitalRead(LED));
    delay(500);
  }

}

