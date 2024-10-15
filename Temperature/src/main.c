
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <nvs_flash.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdlib.h>
#include <time.h>

static const char *TAG = "SensorTask";

#define SERVER_IP "172.232.208.10"
#define SERVER_PORT 8080
#define BASE_TEMP 25   
#define TEMP_VARIATION 2 // Maximum variation in temperature
#define SENSOR_NAME "Temperature Sensor" 

void run_sensor() {
    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(SERVER_PORT);

    srand(time(NULL)); 

    int current_temp = BASE_TEMP;  

    while(1) {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            continue;
        }

        ESP_LOGI(TAG, "Socket created, connecting to %s:%d", SERVER_IP, SERVER_PORT);

        int err = connect(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        if (err != 0) {
            ESP_LOGE(TAG, "Socket unable to connect: errno %d", errno);
            close(sock);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "Successfully connected to server");

        // Send initial connection message
        char connect_msg[128];
        sprintf(connect_msg, "TARGET:main computer,ID:%s,VALUE:%d", SENSOR_NAME, current_temp);
        err = send(sock, connect_msg, strlen(connect_msg), 0);
        if (err < 0) {
            ESP_LOGE(TAG, "Failed to send connection message: errno %d", errno);
            close(sock);
            continue;
        }
        ESP_LOGI(TAG, "Connection message sent: %s", connect_msg);

        while (1) {
            // Update temperature with slight variation
            current_temp += (rand() % (2 * TEMP_VARIATION + 1)) - TEMP_VARIATION;
            // Ensure temperature stays within reasonable bounds
            if (current_temp < BASE_TEMP - TEMP_VARIATION) {
                current_temp = BASE_TEMP - TEMP_VARIATION;
            } else if (current_temp > BASE_TEMP + TEMP_VARIATION) {
                current_temp = BASE_TEMP + TEMP_VARIATION;
            }

            // Send the temperature to the server
            char message[128];
            snprintf(message, sizeof(message), "TARGET:main computer,ID:%s,VALUE:%d", SENSOR_NAME, current_temp);

            err = send(sock, message, strlen(message), 0);
            if (err < 0) {
                ESP_LOGE(TAG, "Failed to send message: errno %d", errno);
                break;
            }
            ESP_LOGI(TAG, "Message sent: %s", message);

            vTaskDelay(2000 / portTICK_PERIOD_MS);  // Send temperature every 2 seconds
        }

        close(sock);
        ESP_LOGI(TAG, "Socket closed, retrying connection");
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}



void wifi_init_sta() {
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_wifi_set_mode(WIFI_MODE_STA);
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "Wokwi-GUEST",
            .password = "",
        },
    };
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    esp_wifi_start();

    ESP_LOGI(TAG, "Connecting to WiFi...");
    esp_wifi_connect();
}

void app_main(void) {
    nvs_flash_init();
    wifi_init_sta();

    // Create the sensor task
    xTaskCreate(run_sensor, "run_sensor", 4096, NULL, 5, NULL);
}
