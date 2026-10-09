#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "temp_humi_monitor.h"
#include "global.h"
#include "mainserver.h"

#define NEO_PIN 45
#define LED_COUNT 1 


/*void neo_blinky(void *pvParameters){

    Adafruit_NeoPixel strip(LED_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);
    strip.begin();
    // Set all pixels to off to start
    strip.clear();
    strip.show();

    while(1) {                          
        strip.setPixelColor(0, strip.Color(255, 0, 0)); // Set pixel 0 to red
        strip.show(); // Update the strip

        // Wait for 1000 milliseconds
        vTaskDelay(500);

        // Set the pixel to off
        strip.setPixelColor(0, strip.Color(0, 0, 0)); // Turn pixel 0 off
        strip.show(); // Update the strip

        // Wait for another 1000 milliseconds
        vTaskDelay(500);
    }
}

void led_blinky(void *pvParameters) {
  pinMode(GPIO_NUM_48, OUTPUT); // Initialize LED pin

  while(1) {
    digitalWrite(GPIO_NUM_48, HIGH); // Turn ON LED
    vTaskDelay(1000);
    digitalWrite(GPIO_NUM_48, LOW); // Turn OFF LED
    vTaskDelay(1000);
  }
}*/

void internet_task(void *pvParameters);   

void internet_task(void *pvParameters)
{
  while (1)
  {
    // Bị chặn ở đây đến khi main_server_task
    // gọi xSemaphoreGive()
    if (xSemaphoreTake(xBinarySemaphoreInternet,
                       portMAX_DELAY) == pdTRUE)
    {
      Serial.println("[NET] Internet ready");
      // kết nối CoreIOT (MQTT), NTP ...
    }
  }
}


void setup() {
  // put your setup code here, to run once for task creation
  Serial.begin(115200);
  /*xTaskCreate(led_blinky, "LED Control", 2048, NULL, 2, NULL);
  xTaskCreate(neo_blinky, "NEO Control", 2048, NULL, 2, NULL);
  xTaskCreate(temp_humi_monitor, "TEMP and HUMI", 2048, NULL, 2, NULL);*/
  xBinarySemaphoreInternet = xSemaphoreCreateBinary();

  xTaskCreate(main_server_task, "server",   8192, NULL, 2, NULL);
  xTaskCreate(internet_task,    "internet", 4096, NULL, 1, NULL);


}

void loop() {
  Serial.println("Hello Custom Board");
  vTaskDelay(pdMS_TO_TICKS(1000));
}