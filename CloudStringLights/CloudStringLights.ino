//Basic Setup for LED strip came from: https://github.com/thehookup/WS2812B_RGB_Classroom_Timer/blob/d47049ffef5eea104cd8c8b9bcbe626ade7ed95f/FastLED_Minimal.ino
#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ESP8266_RAW_PIN_ORDER

/******************Weather API Information**********************************
Modified from:
https://randomnerdtutorials.com/esp8266-nodemcu-http-get-open-weather-map-thingspeak-arduino/
Documentation for the API:
https://openweathermap.org/current
//Color Codes: https://colorcodes.io/blue/
*/

// THE DEFAULT TIMER IS SET TO 10 SECONDS FOR TESTING PURPOSES
// For a final application, check the API call limits per hour/minute to avoid getting blocked/banned
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 10 seconds (10000)
unsigned long timerDelay = 20000;


String jsonBuffer;


/******************  LIBRARY SECTION *************************************/
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>
/*****************  LED LAYOUT AND SETUP *********************************/
#define NUM_LEDS 33
/*****************  DECLARATIONS  ****************************************/

CRGB leds[34];

/*****************  VARIABLES  ************************************/
const int ledPin = 2; //marked as D2 on the board
//Lat and Long data can be retrieved from: https://www.latlong.net/
String lat="42.970620";
String lon="-85.660700";
int windData;
int rainData=  -1;
int cloudsData;
int snowData = -1;


const char* ssid = "CoreyDanielle";
//"Pixel_1479";
const char* password = "Dh586586";
//"roseisthebest";

/*****************  Weather & Weather Helper Functions  ****************************************/
  //Rain
   void rain() {
    for (int i=0; i<11; i++){
      //light Florescent Blue
      leds[i] = CRGB (55, 172, 238);
    };
    for (int i=11; i<23; i++){
      //Sky Blue
      leds[i] = CRGB (181, 0, 226);
    };

    for (int i=24; i<27; i++){
      //Light Pastel Blue
      leds[i] = CRGB (72, 135, 218);
    };
    
     //Light pastel Green 
    for (int i=27; i<NUM_LEDS+1; i++){
      leds[i] = CRGB (135, 72, 218);
    };
  };
  
  //Sun
     
   void sun(){
      for (int i=0; i<NUM_LEDS+1; i++){
       //yellow
       leds[i] = CRGB (139, 237, 0);
      };
   };
   
  //Snow
    void snow(){
      for (int i=0; i<NUM_LEDS+1; i++){
        //Whiteish
        leds[i] = CRGB (60, 100, 100);
       };
       for (int i=0; i<NUM_LEDS+1; i++){
        //Sky Blue
        leds[i] = CRGB (0,0,255);
        i=i+6;
       };
     };
     
 //Clouds
  void cloudEffect(int percent){  
    if (percent > 0){
        for (int i=0; i<NUM_LEDS+1; i++){
          leds[i] = CRGB (0,20,64);
          i=i + (NUM_LEDS/(NUM_LEDS * (percent/100) ));
        };
      }
  };

  //DataError
  void utOhDataError(){
  for (int i=0; i<11; i++){
   leds[i] = CRGB (218, 135, 72);
  };
  for (int j=12; j<23; j++){
    leds[j] = CRGB (135, 72, 218);
  };
   for (int k=24; k<NUM_LEDS; k++){
    leds[k] = CRGB (72, 218, 135);
  };
  };


/*****************  SETUP FUNCTIONS  ****************************************/
void setup() 
{
  Serial.begin(115200);
  FastLED.addLeds<WS2812B, ledPin, RGB>(leds, NUM_LEDS);

ESP.eraseConfig(); 
   WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 }
 // Serial.println("Timer set to 10 seconds (timerDelay variable), it will take 10 seconds before publishing the first reading.");
/*****************  MAIN LOOP  ****************************************/
void loop()  
  {
  //Decision on LED color From Weather Data
    if (snowData==0 && rainData==0){
      //Sun
      sun();
    }else if ( rainData >0 && rainData > snowData) {
      //rain
      rain();
    }else if( snowData > 0 && rainData < snowData ) {
    //snow
      snow();
    }else{
      //data Error
      utOhDataError();
    };

  //Setting Cloudiness 
   if (cloudsData >0){
    cloudEffect(cloudsData);
   };
   
  FastLED.show();
};

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
};
