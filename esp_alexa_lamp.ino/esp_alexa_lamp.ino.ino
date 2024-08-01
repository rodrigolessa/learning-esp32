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
// Defining buttons
#define MainLampButton 4
// variable for storing the pushbutton status 
bool buttonState;
bool buttonLastState;

unsigned long initialTime = 0;
unsigned long buttonTime = 0;

// prototypes
bool connectWifi();

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

int maxNumberOfAttemptsToReconnect = 450;

bool wifiConnected = false;

Espalexa espalexa;

void setup() {

  // The code in this setup method here, it will run once:
  Serial.begin(115200);

  // CHeck if serial por is available
  while (!Serial) {
    delay(500);
  }

  // Defining pin mode for all devices
  pinMode(LEDWifi, OUTPUT);
  pinMode(LEDsDaEstante, OUTPUT);
  digitalWrite(LEDsDaEstante, HIGH);
  pinMode(AreaDosQuadros, OUTPUT);
  digitalWrite(AreaDosQuadros, HIGH);
  pinMode(LEDDaParede, OUTPUT);
  digitalWrite(LEDDaParede, HIGH);
  pinMode(LampadaCentral, OUTPUT);
  digitalWrite(LampadaCentral, HIGH);
  pinMode(ChaveVentilador, OUTPUT);
  digitalWrite(ChaveVentilador, HIGH);
  pinMode(MainLampButton, INPUT_PULLUP);

  delay(250);

  // Initialise wifi connection
  wifiConnected = connectWifi();
  
  if (wifiConnected) {

    // TODO: Improve performance using events
    WiFi.onEvent(WiFiStationConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
    WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
    
    // Define your devices here. Set a name for each one.
    espalexa.addDevice("Estante", AcenderLEDsDaEstante); //simplest definition, default state off
    espalexa.addDevice("Quadros", IluminarAreaDosQuadros);
    espalexa.addDevice("Parede", AcenderLEDDaParede);
    espalexa.addDevice("Lampada Central", AcenderLampadaCentral);
    espalexa.addDevice("Ventilador", LigarChaveVentilador);

    espalexa.begin();    

    Serial.println("Setup done!!!");
    
  } else {
    //while (1) {
    Serial.println("Cannot connect to WiFi. Please check data and reset the ESP.");
    delay(2500);
    //}
  } 
}
 
void loop()
{
  if (wifiConnected) {
    espalexa.loop();
  }

  // Read the state of the pushbutton value
  buttonState = !digitalRead(MainLampButton);
  //Serial.print("ButtonState: ");
  //Serial.println(buttonState);

  // When the button is pressed
  if (buttonState && !buttonLastState){
    if (initialTime == 0) {
      initialTime = millis();
    }
  }

  // When releasing the button
  if (initialTime > 350) { // To avoid debounce
    if (!buttonState && buttonLastState) {
      buttonTime = millis() - initialTime;
      initialTime = 0;
    }
  }

  // First command
  if ((buttonTime > 1) && (buttonTime <= 500)) {
    buttonTime = 0;
    Serial.println("Executing first command...");
    AcenderLampadaCentral(LOW);
    delay(250);
  }

  // Second command
  if ((buttonTime > 500) && (buttonTime <= 1500)) {
    buttonTime = 0;
    Serial.println("Executing second command...");
    AcenderLampadaCentral(HIGH);
    delay(250);
  }

  buttonLastState = buttonState;

  delay(1);
}

//our callback functions
void LigarChaveVentilador(uint8_t brightness) {
  Serial.print("Device Fan changed to ");
    if (brightness) {
      Serial.print("OFF, ");
      Serial.println(brightness);
      digitalWrite(ChaveVentilador, LOW);
    }
    else {
      Serial.println("ON, brightness ");
      Serial.println(brightness);
      digitalWrite(ChaveVentilador, HIGH);
    }
}

void AcenderLampadaCentral(uint8_t brightness) {
  Serial.print("Device Main Ligth Bulb changed to ");
  if (brightness) {
    Serial.print("OFF, ");
    Serial.println(brightness);
    digitalWrite(LampadaCentral, LOW);
  }
  else {
    Serial.println("ON, brightness ");
    Serial.println(brightness);
    digitalWrite(LampadaCentral, HIGH);
  }
}

void AcenderLEDDaParede(uint8_t brightness) {
  Serial.print("Device Wall Light changed to ");
  if (brightness) {
    Serial.print("OFF, ");
    Serial.println(brightness);
    digitalWrite(LEDDaParede, LOW);
  }
  else {
    Serial.println("ON, brightness ");
    Serial.println(brightness);
    digitalWrite(LEDDaParede, HIGH);
  }
}

void IluminarAreaDosQuadros(uint8_t brightness) {
  Serial.print("Device Frame Light Bulb changed to ");
  if (brightness) {
    Serial.print("OFF, ");
    Serial.println(brightness);
    digitalWrite(AreaDosQuadros, LOW);
  }
  else {
    Serial.println("ON, brightness ");
    Serial.println(brightness);
    digitalWrite(AreaDosQuadros, HIGH);
  }
}

void AcenderLEDsDaEstante(uint8_t brightness) {
  Serial.print("Device Shelf Led changed to ");
  if (brightness) {
    Serial.print("OFF, ");
    Serial.println(brightness);
    digitalWrite(LEDsDaEstante, LOW);
  }
  else {
    Serial.println("ON, brightness ");
    Serial.println(brightness);
    digitalWrite(LEDsDaEstante, HIGH);
  }
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
  Serial.print(get_wifi_status(WiFi.status()));
  Serial.print(" - ");
  Serial.println(info.wifi_sta_disconnected.reason);
  
  // Turn On the connection LED 
  digitalWrite(LEDWifi, HIGH);

  // TODO: Try to reconnect
  Serial.print("Trying to reconnect to Wifi, ");
  Serial.println(ssid);

  WiFi.begin(ssid, password, channel);

  delay(500);
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi() {

  Serial.println("");
  Serial.print("Start connection to WiFi, ");
  Serial.println(ssid);
  
  // Set WiFi to station mode and disconnect from an AP if it was previously connected.
  WiFi.mode(WIFI_STA);
  //WiFi.setMinSecurity(WIFI_AUTH_WPA2_PSK);
  WiFi.disconnect(true);

  // Turn on wifi information led
  digitalWrite(LEDWifi, HIGH);
  
  delay(500);

  int status = WL_IDLE_STATUS;
  int waitingCount = 0;

  status = WiFi.begin(ssid, password, channel);

  Serial.println(" ");
  Serial.print("Connecting...");

  // Wait for connection
  while (status != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    status = WiFi.status();
    if (waitingCount > maxNumberOfAttemptsToReconnect){
      break;
    }
    waitingCount++;
  }

  if (status == WL_CONNECTED) {
    digitalWrite(LEDWifi, LOW);
    return true;
  }
  else {
    return false;
  }
}
