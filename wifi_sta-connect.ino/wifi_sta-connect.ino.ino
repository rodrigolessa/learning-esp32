#include <WiFi.h>

#define CONNECTION_TIMEOUT 10

const char* ssid = "Manu Oi Fibra 2.4";
const char* password = "manU!262910";

String get_wifi_status(int status){
    switch(status){
        case WL_IDLE_STATUS:
        return "WL_IDLE_STATUS";
        case WL_SCAN_COMPLETED:
        return "WL_SCAN_COMPLETED";
        case WL_NO_SSID_AVAIL:
        return "WL_NO_SSID_AVAIL";
        case WL_CONNECT_FAILED:
        return "WL_CONNECT_FAILED";
        case WL_CONNECTION_LOST:
        return "WL_CONNECTION_LOST";
        case WL_CONNECTED:
        return "WL_CONNECTED";
        case WL_DISCONNECTED:
        return "WL_DISCONNECTED";
    }
}

void setup(){
    Serial.begin(115200);
    delay(2500);
    int status = WL_IDLE_STATUS;
    Serial.println("\nConnecting");
    Serial.println(get_wifi_status(status));
    delay(2500);
    WiFi.mode(WIFI_STA); //Optional
    status = WiFi.begin(ssid, password, 11);
    delay(2500);
    while(status != WL_CONNECTED){
        delay(500);
        status = WiFi.status();
        Serial.println(get_wifi_status(status));
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
}

void loop(){}