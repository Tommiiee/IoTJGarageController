/*********
Thomas Jones
CSUDH Senior Project
IoT Garage Door Controller
*********/
// Import required libraries
#include "config.h"
#include "WiFi.h"
#include "ESPAsyncWebServer.h"

//HTTP user/pass for authentication
const char * http_username = "admin";
const char * http_password = "password";
const char * PARAM_INPUT_1 = "state";
const int output = 2;

// Set to true to define Relay as Normally Open (NO)
#define RELAY_NO false
// Set number of relays (Garage Doors) 
#define NUM_RELAYS 1

// Assign each GPIO to a relay
int relayGPIOs[NUM_RELAYS] = {23};

// Adafruit communication via "digital" feed. 
AdafruitIO_Feed * digital = io.feed("door1");

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

//Stores HTML page in program memory instead RAM (ESP32 has small RAM)
const char index_html[] PROGMEM = R "rawliteral(
<!DOCTYPE HTML>
<html>
  <head>
    <meta charset="utf-8">
      <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1">
          <title>GarageOpener</title>
          <script>var __adobewebfontsappname__="dreamweaver"</script>
          <script src="http://use.edgefonts.net/source-sans-pro:n2:default.js" type="text/javascript"></script>
          <script>function toggleCheckbox(element) {
            var xhr = new XMLHttpRequest();
            if(element.checked){ xhr.open("GET", "/update?relay="+element.id+"&state=1", true); }
            else { xhr.open("GET", "/update?relay="+element.id+"&state=1", true); }
            xhr.send();}
          </script>
          <style>
         body {
         font-family: source-sans-pro;
         background-color: #f2f2f2;
         margin-top: 0px;
         margin-right: 0px;
         margin-bottom: 0px;
         margin-left: 0px;
         font-style: normal;
         font-weight: 200;
         }
         .container {
         width: 90%;
         margin-left: auto;
         margin-right: auto;
         height: 810px;
         background-color: #FFFFFF;
         box-shadow: 0px 8px 15px rgba(0, 0, 0, 0.4);
         }
         .switch {
         position: relative;
         top: 50px; 
         display: inline-block;
         width: 240px;
         height: 136px;
         }
         .switch input { 
         opacity: 0;
         width: 0;
         height: 0;
         }
         .slider {
         position: absolute;
         cursor: pointer;
         top: 0;
         left: 0;
         right: 0;
         bottom: 0;
         background-color: #FF5D5D;
         border-radius: 20px;
         transform: rotate(270deg);
         transition: 5s;
         box-shadow: 5px 8px 10px rgba(0, 0, 0, 0.35);
         }
         .slider:before {
         position: absolute;
         content: "";
         height: 120px;
         width: 222px;
         left: -222px;
         bottom: 8px;
         background-color: white;
         transition: 19s;
         }
         input:checked + .slider {
         background-color: #2196F3;
         }
         input:checked + .slider:before {
         -webkit-transform: translateX(220px);
         -ms-transform: translateX(220px);
         transform: translateX(220px);
         }
         header {
         width: 100%;
         height: 5%;
         background-color: #2C2C2C;
         border-bottom: 1px solid #2C2C2C;
         }
         .logo {
         color: #fff;
         font-weight: bold;
         text-align: undefined;
         width: 10%;
         float: left;
         margin-top: 15px;
         margin-left: 25px;
         letter-spacing: 4px;
         }
         nav {
         float: right;
         width: 50%;
         text-align: right;
         margin-right: 25px;
         }
         header nav ul {
         list-style: none;
         float: right;
         }
         nav ul li {
         float: left;
         color: #FFFFFF;
         font-size: 14px;
         text-align: left;
         margin-right: 25px;
         letter-spacing: 2px;
         font-weight: bold;
         transition: all 0.3s linear;
         }
         ul li a {
         color: #FFFFFF;
         text-decoration: none;
         }
         ul li:hover a {
         color: #2196F3;
         }
         .mainBanner_header {
         color: #FFFFFF;
         text-align: center;
         margin-top: 0px;
         margin-right: 0px;
         margin-bottom: 0px;
         margin-left: 0px;
         letter-spacing: 4px;
         }
         .mainBanner {
         background-color: #B3B3B3;
         padding-top: 100px;
         padding-bottom: 100px;
         }
         .light {
         font-weight: bold;
         color: #717070;
         }
         .tagline {
         text-align: center;
         color: #FFFFFF;
         margin-top: 4px;
         font-weight: lighter;
         text-transform: uppercase;
         letter-spacing: 1px;
         }
         .text_column {
         width: 29%;
         text-align: justify;
         font-weight: lighter;
         line-height: 25px;
         float: left;
         padding-left: 20px;
         padding-right: 20px;
         color: #A3A3A3;
         }
         .about {
         padding-left: 25px;
         padding-right: 25px;
         padding-top: 35px;
         display: inline-block;
         background-color: #FFFFFF;
         margin-top: 0px;
         }
         .stats {
         color: #717070;
         margin-bottom: 5px;
         }
         .toggleSwitch {
         text-align: center;
         padding-top: 20px;
         padding-bottom: 20px;
         width: 100%;
         background-color: #FFFFFF;
         height: 300px;
         padding-bottom: 35px;
         padding-top: 0px;
         margin-top: -5px;
         margin-bottom: 0px;
         overflow: hidden;
         }
         .banner {
         background-color: #2C2C2C;
         height: 100px;
         background-attachment: fixed;
         background-size: cover;
         background-repeat: no-repeat;
         }
         footer {
         background-color: #FFFFFF;
         padding-bottom: 35px;
         }
         .footer_column {
         width: 50%;
         text-align: center;
         padding-top: 30px;
         float: left;
         }
         footer .footer_column h3 {
         color: #B3B3B3;
         text-align: center;
         }
         footer .footer_column p {
         color: #717070;
         background-color: #FFFFFF;
         }
         .cards {
         width: 100%;
         height: auto;
         max-width: 400px;
         max-height: 200px;
         }
         footer .footer_column p {
         padding-left: 30px;
         padding-right: 30px;
         text-align: justify;
         line-height: 25px;
         font-weight: lighter;
         margin-left: 20px;
         margin-right: 20px;
         }
         .button {
         width: 200px;
         margin-top: 40px;
         margin-right: auto;
         margin-bottom: auto;
         margin-left: auto;
         padding-top: 20px;
         padding-right: 10px;
         padding-bottom: 20px;
         padding-left: 10px;
         text-align: center;
         vertical-align: middle;
         border-radius: 0px;
         text-transform: uppercase;
         font-weight: bold;
         letter-spacing: 2px;
         border: 3px solid #FFFFFF;
         color: #FFFFFF;
         transition: all 0.3s linear;
         }
         .button:hover {
         background-color: #FEFEFE;
         color: #C4C4C4;
         cursor: pointer;
         }
         .copyright {
         text-align: center;
         padding-top: 20px;
         padding-bottom: 20px;
         background-color: #717070;
         color: #FFFFFF;
         text-transform: uppercase;
         font-weight: lighter;
         letter-spacing: 2px;
         border-top-width: 2px;
         }
         .hidden {
         display: none;
         }
         @media (max-width: 320px) {
         .logo {
         width: 100%;
         text-align: center;
         margin-top: 13px;
         margin-right: 0px;
         margin-bottom: 0px;
         margin-left: 0px;
         }
         .container header nav {
         margin-top: 0px;
         margin-right: 0px;
         margin-bottom: 0px;
         margin-left: 0px;
         width: 100%;
         float: none;
         display: none;
         }
         nav ul li {
         margin-top: 0px;
         margin-right: 0px;
         margin-bottom: 0px;
         margin-left: 0px;
         width: 100%;
         text-align: center;
         }
         .text_column {
         width: 100%;
         text-align: justify;
         padding-top: 0px;
         padding-right: 0px;
         padding-bottom: 0px;
         padding-left: 0px;
         }
         .banner {
         background-color: #2C2C2C;
         background-image: none;
         }
         .tagline {
         margin-top: 20px;
         line-height: 22px;
         }
         .mainBanner_header {
         padding-left: 10px;
         padding-right: 10px;
         line-height: 22px;
         text-align: center;
         }
         }
         @media (min-width: 321px)and (max-width: 767px) {
         .logo {
         width: 100%;
         text-align: center;
         margin-top: 10px;
         margin-right: 0px;
         margin-bottom: 0px;
         margin-left: 0px;
         color: white;
         }
         .container header nav {
         margin-top: 0px;
         margin-right: 0px;
         margin-bottom: 0px;
         margin-left: 0px;
         width: 100%;
         float: none;
         overflow: auto;
         display: inline-block;
         background: #545454;
         }
         header nav ul {
         padding: 0px;
         float: none;
         }
         nav ul li {
         margin-top: 0px;
         margin-right: 0px;
         margin-bottom: 0px;
         margin-left: 0px;
         width: 100%;
         text-align: center;
         padding-top: 6px;
         padding-bottom: 6px;
         box-shadow: 0px 4px 6px rgba(0, 0, 0, 0.2);
         }
         .mainBanner_header {
         padding-left: 10px;
         padding-right: 10px;
         padding-top:60px;
         text-align: center;
         }
         .container {
         height: 874px;
         }
         @media (min-width: 768px) and (max-width: 1096px) {
         .text_column {
         width: 100%;
         }
         }
         }
      </style>
        </head>
        <body>
          <div class="container">
            <header>
              <a href="">
                <h4 class="logo">IoT.J.</h4>
              </a>
              <nav>
                <ul>
                  <li>
                    <a href="#mainBanner">ABOUT</a>
                  </li>
                  <li>
                    <a href="https://io.adafruit.com/Tommiiee/feeds/door1">ADAFRUIT</a>
                  </li>
                  <li>
                    <a href="https://ifttt.com/home?filter=created_by_me">IFTTT</a>
                  </li>
                </ul>
              </nav>
            </header>
            <section class="mainBanner" id="mainBanner">
              <h2 class="mainBanner_header">
                <u>Garage </u>
                <span class="light">
                  <u>Opener</u>
                </span>
              </h2>
              <p class="tagline">Senior Project</p>
            </section>
            <div class="toggleSwitch">
              <h2>
                <span class="light">Toggle</span> Switch
              </h2>
              <label class="switch">
                <input type="checkbox" onchange="toggleCheckbox(this)">
                  <span class="slider"></span>
                </label>
              </div>
              <section class="banner"></section>
              <div class="copyright">&copy;2020- 
                <strong>Thomas Jones&nbsp;</strong>
              </div>
            </div>
          </body>
        </html>
)rawliteral";

void handleMessage(AdafruitIO_Data * data) {
  Serial.print("received <- ");
  if (data -> toPinLevel() == HIGH) {
    Serial.println("HIGH");
    digitalWrite(relayGPIOs[0], data -> toPinLevel());
    delay(1000);
    digitalWrite(relayGPIOs[0], 0);
  } else {
    Serial.println("LOW");
  }
}
//Serial port for debugging purposes
void setup() {
  Serial.begin(115200); 
  
  //Starts program with relays turned off
  for (int i = 1; i <= NUM_RELAYS; i++) { 
    pinMode(relayGPIOs[i - 1], OUTPUT);
    digitalWrite(relayGPIOs[i - 1], LOW);
  }

  // connect to io.adafruit/internet
  Serial.println("Connecting to Adafruit IO..");
  io.connect();
  // set up a message handler for the 'digital' feed.
  digital -> onMessage(handleMessage); 
  while (io.status() < AIO_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }
  Serial.println();
  //Print ESP32 Local IP Address
  Serial.println(io.statusText());
  digital -> get(); 
  
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) { 
    if (!request -> authenticate(http_username, http_password))
      return request -> requestAuthentication();
    request -> send_P(200, "text/html", index_html);
  });

  // Send a GET request to <ESP_IP>/update?relay
  server.on("/update", HTTP_GET, [](AsyncWebServerRequest * request) { 
    digitalWrite(relayGPIOs[0], 1);
    delay(1000);
    digitalWrite(relayGPIOs[0], 0);
    request -> send_P(200, "text/html", index_html);
  });
  //HTTP authentication 
  server.on("/update", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (!request -> authenticate(http_username, http_password))
      return request -> requestAuthentication();
    String inputMessage;
    String inputParam;
    if (request -> hasParam(PARAM_INPUT_1)) {
      inputMessage = request -> getParam(PARAM_INPUT_1) -> value();
      inputParam = PARAM_INPUT_1;
      digitalWrite(output, inputMessage.toInt());
    } else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request -> send(200, "text/plain", "OK");
  });
  server.begin();
}
void loop() {
  //Keeps the client connected to io.adafruit.com, and processes any incoming data.
  io.run(); 
}
