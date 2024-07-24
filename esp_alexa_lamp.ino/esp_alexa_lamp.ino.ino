#include <WiFi.h>
#include <Espalexa.h>

// Defining pins for lights
#define LEDWifi 23
#define LEDsDaEstante 21
#define AreaDosQuadros 19
#define LEDDaParede 18
#define LampadaCentral 17
// Defining pins other devices
#define ChaveVentilador 16 // let the breeze in

// prototypes
boolean connectWifi();

// Callback functions
void AcenderLEDsDaEstante(uint8_t brightness);
void IluminarAreaDosQuadros(uint8_t brightness);
void AcenderLEDDaParede(uint8_t brightness);
void AcenderLampadaCentral(uint8_t brightness);
void LigarChaveVentilador(uint8_t brightness);

// Wifi settings
const char* ssid = "Manu Oi Fibra 2.4";
const char* password = "manU!262910";
const int channel = 11;

boolean wifiConnected = false;

Espalexa espalexa;

void setup() {

  // put your setup code here, to run once:
  Serial.begin(115200);

  while (!Serial) {
    delay(500);
  }

  // Defining pin mode for all devices
  pinMode(LEDWifi, OUTPUT);
  pinMode(LEDsDaEstante, OUTPUT);
  pinMode(AreaDosQuadros, OUTPUT);
  pinMode(LEDDaParede, OUTPUT);
  pinMode(LampadaCentral, OUTPUT);
  pinMode(ChaveVentilador, OUTPUT);

  delay(500);

  // TODO: Improve performance using events
  WiFi.onEvent(WiFiStationConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

  // Initialise wifi connection
  wifiConnected = connectWifi();
  
  if (wifiConnected) {
    
    // Define your devices here. Set a name for each one.
    espalexa.addDevice("Estante", AcenderLEDsDaEstante, 0); //simplest definition, default state off
    espalexa.addDevice("Quadros", IluminarAreaDosQuadros, 0);
    espalexa.addDevice("Parede", AcenderLEDDaParede, 0);
    espalexa.addDevice("Lampada Central", AcenderLampadaCentral, 0);
    espalexa.addDevice("Ventilador", LigarChaveVentilador, 0);

    espalexa.begin();

    Serial.println("Setup done!!!");
    
  } else {
    while (1) {
      Serial.println("Cannot connect to WiFi. Please check data and reset the ESP.");
      delay(2500);
    }
  } 
}
 
void loop()
{
   espalexa.loop();
   delay(1);
}

//our callback functions
void LigarChaveVentilador(uint8_t brightness) {
  Serial.print("Device Fan changed to ");
  digitalWrite(ChaveVentilador, brightness);
  PrintDeviceStatus(brightness);
}

void AcenderLampadaCentral(uint8_t brightness) {
  Serial.print("Device Main Ligth Bulb changed to ");
  digitalWrite(LampadaCentral, brightness);
  PrintDeviceStatus(brightness);
}

void AcenderLEDDaParede(uint8_t brightness) {
  Serial.print("Device Wall Light changed to ");
  digitalWrite(LEDDaParede, brightness);
  PrintDeviceStatus(brightness);
}

void IluminarAreaDosQuadros(uint8_t brightness) {
  Serial.print("Device Frame Light Bulb changed to ");
  digitalWrite(AreaDosQuadros, brightness);
  PrintDeviceStatus(brightness);
}

void AcenderLEDsDaEstante(uint8_t brightness) {
  Serial.print("Device Shelf Led changed to ");
  digitalWrite(LEDsDaEstante, brightness);
  PrintDeviceStatus(brightness);
}

void PrintDeviceStatus(uint8_t brightness) {
    if (brightness) {
      Serial.print("ON, brightness ");
      Serial.println(brightness);
    }
    else {
      Serial.println("OFF");
    }
}

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

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Connected to Wifi successfully!");
  // When connection was successful, turn off the connection LED
  digitalWrite(LEDWifi, LOW);
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Disconnected from WiFi access point");
  Serial.print("WiFi lost connection. Reason: ");
  Serial.println(info.wifi_sta_disconnected.reason);
  
  // Turn On the connection LED 
  digitalWrite(LEDWifi, HIGH);

  // TODO: Try to reconnect
  //Serial.println("Trying to Reconnect");
  //status = WiFi.begin(ssid, password, channel);
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi() {

  Serial.println("");
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);

  int status = WL_IDLE_STATUS;
  int statusCount = 0;
  
  // Set WiFi to station mode and disconnect from an AP if it was previously connected.
  WiFi.mode(WIFI_STA);
  //WiFi.setMinSecurity(WIFI_AUTH_WPA2_PSK);
  WiFi.disconnect(true);
  
  delay(500);

  status = WiFi.begin(ssid, password, channel);

  Serial.println("");

  // Wait for connection
  while (status != WL_CONNECTED) {
    delay(500);
    status = WiFi.status();
    Serial.print(get_wifi_status(status));
    Serial.print(" - ");
    Serial.println(statusCount);
    statusCount++;
  }

  if (status == WL_CONNECTED) {
    return true;
  }
  else {
    return false;
  }
}
