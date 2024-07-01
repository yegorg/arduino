#include <WiFi.h>  
#include <HTTPClient.h>  
#include <ArduinoJson.h>  
#include <TFT_eSPI.h>
#include <Wire.h>  
#include <Adafruit_GFX.h>  
#include <Adafruit_SSD1306.h>  
#include <SPI.h> // Include the graphics library  
#include <time.h>  

  
// Replace with your network credentials  
const char* ssid = "<SSID>";  
const char* password = "<PASS>";  
  
// OpenWeatherMap API details  
const char* city = "<CITY>";  
const char* apiKey = "<API_KEY>";  

// NTP server to fetch time  
const char* ntpServer = "pool.ntp.org";  
const long  gmtOffset_sec = 7200;  // GMT +2
const int   daylightOffset_sec = 3600;  // DST

// Timer variables for weather update  
unsigned long previousMillis = 0;  
const long interval = 1800000; // 30 minutes in milliseconds 

TFT_eSPI tft = TFT_eSPI();  // Create an instance of the display

// Definitions for SSD1306 OLED display  
#define SCREEN_WIDTH 128  
#define SCREEN_HEIGHT 64  
#define OLED_RESET    -1  // Reset pin # (or -1 if sharing Arduino reset pin)  
#define SCREEN_ADDRESS 0x3C  // I2C address for the OLED display  
  
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
  
void setup() {  
  Serial.begin(115200);  
  
  // Initialize the display  
  tft.init();  
  tft.setRotation(0); // Set the rotation (0, 1, 2, or 3)  

   // Initialize SSD1306 display  
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {  
    Serial.println(F("SSD1306 allocation failed"));  
    for(;;);  
  }  
  display.display();  
  display.clearDisplay();  
  display.setTextSize(1);  
  display.setTextColor(SSD1306_WHITE);  
  display.setCursor(0, 0);  
  display.println("SSD1306 Display");  
  display.display();  
  
  // Optional: Control the backlight  
  pinMode(15, OUTPUT);  
  digitalWrite(15, HIGH); // Turn on the backlight  
   // Clear the screen with black color  
  tft.fillScreen(TFT_BLACK);  
  
  // Set text color and size  
  tft.setTextColor(TFT_WHITE);  
  tft.setTextSize(2);  
  
  // Initialize Wi-Fi  
  WiFi.begin(ssid, password);  
  while (WiFi.status() != WL_CONNECTED) {  
    delay(1000);  
    Serial.println("Connecting to WiFi...");  
  }  
  Serial.println("Connected to WiFi");  
  
   // Initialize NTP  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); 

  // Fetch and display weather data  
  fetchAndDisplayWeather();  
}  
  
void loop() {  
  // Get the current time in milliseconds  
  unsigned long currentMillis = millis();  
  
  // Check if 30 minutes have passed since the last weather update  
  if (currentMillis - previousMillis >= interval) {  
    // Save the last time you updated the weather data  
    previousMillis = currentMillis;  
    // Fetch and display new weather data  
    fetchAndDisplayWeather();  
  }  
  // Update the time every second  
  displayCurrentTime();  
  delay(1000);

// Print something on 2nd display
  display.setCursor(0, 20);  
  display.setTextColor(SSD1306_WHITE);  
  display.setTextSize(1);  
  display.println("Hello, OLED!");  
  display.display();  
 
}  
  
void fetchAndDisplayWeather() {  
  if (WiFi.status() == WL_CONNECTED) {  
    HTTPClient http;  
    String url = "http://api.openweathermap.org/data/2.5/weather?q=" + String(city) + "&appid=" + String(apiKey) + "&units=metric";  
    http.begin(url);  
    int httpCode = http.GET();  
  
    if (httpCode > 0) {  
      String payload = http.getString();  
      Serial.println(payload);  
  
      // Parse JSON  
      DynamicJsonDocument doc(1024);  
      deserializeJson(doc, payload);  
  
      float temperature = doc["main"]["temp"];  
      float humidity = doc["main"]["humidity"];  
      const char* weatherDescription = doc["weather"][0]["description"];  
  
      // Display data  
      tft.fillScreen(TFT_BLACK);  
      tft.setCursor(0, 0);  
      tft.setTextSize(2);  
      tft.print("City: ");  
      tft.println(city);  
      tft.print("Temp: ");  
      tft.print(temperature);  
      tft.println("C");  
      tft.print("Humidity: ");  
      tft.print(humidity);  
      tft.println("%");  
      tft.print("Desc: ");  
      tft.println(weatherDescription);  
    } else {  
      Serial.println("Error on HTTP request");  
    }  
    http.end();  
  } else {  
    Serial.println("WiFi not connected");  
  }
}  

void displayCurrentTime() {  
  struct tm timeinfo;  
  if (!getLocalTime(&timeinfo)) {  
    Serial.println("Failed to obtain time");  
    return;  
  }  
  
  // Clear the previous time display  
  tft.fillRect(10, 150, 200, 30, TFT_BLACK);  
  
 // Format the time as HH:MM:SS  
  char timeStr[9];  
  strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeinfo);  
  
  // Format the date as Mon, July 1 2024  
  char dateStr[30];  
  strftime(dateStr, sizeof(dateStr), "%a, %B %d %Y", &timeinfo);  
  
  // Display the time  
  tft.setCursor(10, 150);  
  tft.setTextColor(TFT_WHITE, TFT_BLACK);  // Set text color with background color for overdraw  
  tft.setTextSize(2);  
  tft.println(timeStr);  
  
  // Display the date  
  tft.setCursor(10, 180);  
  tft.setTextColor(TFT_WHITE, TFT_BLACK);  // Set text color with background color for overdraw  
  tft.setTextSize(2);  
  tft.println(dateStr);  
}  
