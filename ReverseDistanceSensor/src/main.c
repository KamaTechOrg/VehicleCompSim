#include <stdio.h>
#include <string.h>
#include <sys/param.h>
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
#include "lwip/sockets.h"
#include <stdlib.h>    // For rand()
#include <time.h>      // For time()
#include <unistd.h>    // For sleep()

static const char *TAG = "ReverseDistanceSensor";

#define SERVER_IP "10.13.37.1"        // Localhost IP address
#define SERVER_PORT 8100         // Server port
#define LISTENING_PORT 8101      // This device port
#define BUFLEN 1024


#define SENSOR_NAME "Reverse Distance Sensor" // Define the sensor name

void run_server() {
    char rx_buffer[1024];
    char addr_str[128];
    int addr_family = AF_INET;
    int ip_protocol = IPPROTO_IP;

    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(LISTENING_PORT);

    int listen_sock = socket(addr_family, SOCK_STREAM, ip_protocol);
    if (listen_sock < 0) {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
        return;
    }
    ESP_LOGI(TAG, "Socket created");

    int err = bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err != 0) {
        ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
        close(listen_sock);
        return;
    }
    ESP_LOGI(TAG, "Socket bound, port %d", LISTENING_PORT);

    err = listen(listen_sock, 1);
    if (err != 0) {
        ESP_LOGE(TAG, "Error occurred during listen: errno %d", errno);
        close(listen_sock);
        return;
    }
    ESP_LOGI(TAG, "Socket listening");

    while (1) {
        struct sockaddr_in source_addr;
        socklen_t addr_len = sizeof(source_addr);
        int sock = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);
            continue;
        }

        inet_ntoa_r(source_addr.sin_addr, addr_str, sizeof(addr_str) - 1);
        ESP_LOGI(TAG, "Socket accepted IP address: %s", addr_str);

        // Receive the initial message from the client
        int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
        if (len > 0) {
            rx_buffer[len] = 0;
            ESP_LOGI(TAG, "Received %d bytes: %s", len, rx_buffer);

            // Acknowledge the received message
            const char *response = "Message received!";
            err = send(sock, response, strlen(response), 0);
            if (err < 0) {
                ESP_LOGE(TAG, "Failed to send response: errno %d", errno);
                close(sock);
                continue;
            }
            ESP_LOGI(TAG, "Sent acknowledgment to client");

            // Now, keep sending random numbers every 2 seconds
            srand(time(NULL));  // Seed the random number generator
            while (1) {
                int random_num = rand() % 100;  // Generate a random number between 0 and 99
                char num_str[128];
               sprintf(num_str, "ID:%s,VALUE:%d", SENSOR_NAME, random_num);

                // Send the random number to the client
                err = send(sock, num_str, strlen(num_str), 0);
                if (err < 0) {
                    ESP_LOGE(TAG, "Failed to send random number: errno %d", errno);
                    break;
                }
                ESP_LOGI(TAG, "Sent random number to client: %d", random_num);

                sleep(2);  // Wait for 2 seconds before sending the next number
            }
        }

        close(sock);
        ESP_LOGI(TAG, "Socket closed");
    }

    close(listen_sock);
    ESP_LOGI(TAG, "Socket server stopped");
}

void run_client() {
    while(1) {
        ESP_LOGI(TAG, "Running again");

        struct sockaddr_in dest_addr;
        dest_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(SERVER_PORT);

        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            continue;
        }
        ESP_LOGI(TAG, "Socket created, connecting to %s:%d", SERVER_IP, SERVER_PORT);

        int err = connect(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        if (err != 0) {
            ESP_LOGE(TAG, "Socket unable to connect: errno %d", errno);
            close(sock);
            continue;
        }

        ESP_LOGI(TAG, "Successfully connected to %s:%d", SERVER_IP, SERVER_PORT);
        const char *payload = "Hello from ESP32";
        send(sock, payload, strlen(payload), 0);
        ESP_LOGI(TAG, "Message sent");

        close(sock);
        ESP_LOGI(TAG, "Socket closed");
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

    ESP_LOGI(TAG, "Connecting to WiFi..");
    esp_wifi_connect();
}

void app_main(void) {
    nvs_flash_init();
    wifi_init_sta();

    xTaskCreate(run_server, "run_server", 4096, NULL, 5, NULL);
    //xTaskCreate(run_client, "run_client", 4096, NULL, 5, NULL);
}
