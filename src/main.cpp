#include <sunsetLED.h>
#include <FastLED.h>
#include <Ticker.h>

#define LED_PIN_D4 D4 // Pin podłączony do taśmy LED
#define LED_COUNT 60 // Liczba LEDów na taśmie
#define LIGHT_TIME 20

// Utwórz tablicę LEDów
CRGB leds[LED_COUNT];

Ticker timerLEDS_ON, timerLEDS_OFF;;
volatile uint32_t counterToOFF = LIGHT_TIME;
volatile uint32_t counterToON = 0;

void countSecondsToON() {
  counterToON--;
  Serial.print("time to ON: ");
  Serial.println(counterToON);
}
void countSecondsToOFF() {
  counterToOFF--;
  Serial.print("time to OFF: ");
  Serial.println(counterToOFF);
}

const char* ssid = "DE2SI565HK";
const char* password = "12344321";

// NTP settings
const char* ntpServer = "pool.ntp.org";
const int timeZone = 1; // Change this based on your timezone (GMT+1 for Poland)

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, timeZone * 3600, 60000);

ESP8266WebServer server(80);

void handlePost() {
  String receivedData = server.arg("data");
  Serial.println("");
  Serial.println("Received data: " + receivedData);
  server.send(200, "text/plain", "Data received successfully");

  counterToON = timeToSunset(convertToString(receivedData), timeClient);

  Serial.print("Time to sunset3: ");
  Serial.println(counterToON);
  // Set the timer to call the countSeconds function every 1000 milliseconds (1 second)
  timerLEDS_ON.attach(1.0, countSecondsToON);
}

void setup() {
  Serial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("Wifi local IP address: ");  
  Serial.print(WiFi.localIP());  // obtain your ESP IP address in serial monitor

  // Define endpoint for receiving data
  server.on("/receiveData", HTTP_POST, handlePost);
  server.begin();

  // Initialize the NTP client
  timeClient.begin();
  // Update the NTP client
  timeClient.update();

  // Ustaw pin D4 jako wyjście
  pinMode(LED_PIN_D4, OUTPUT);
  // Zainicjuj taśmę LED
  FastLED.addLeds<WS2812B, LED_PIN_D4, GRB>(leds, LED_COUNT);
  FastLED.clear(true);
}

void loop() {
    if (counterToON == 0){
    FastLED.clear(true);
    FastLED.show();
  }

  if (counterToON == 1){
    timerLEDS_ON.detach();
    counterToON = 2;
    fill_solid(leds, LED_COUNT, CRGB::Red);
    FastLED.show();
    timerLEDS_OFF.attach(1.0,countSecondsToOFF);
  }
  if (counterToOFF == 1){
    counterToON = 0;
    timerLEDS_OFF.detach();
    counterToOFF = LIGHT_TIME;
    FastLED.clear(true);
    FastLED.show();
  }
  
  server.handleClient();
}
