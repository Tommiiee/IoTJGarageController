# IoTJGarage Controller
The IoTJGarage Controller is a project to controler a garage door using the cheap
but powerful ESP32 microcontroler in conjunction with Arduino IDE, io.Adafruit, 
and IFTTT. Adafruit and IFTTT are used to send commands from a smart speaker as
another means to control your garage door. 

## To use: 
* Create an account at both [adafruit](https://io.adafruit.com/) and [IFTTT](https://ifttt.com/home).
* In adafruit, Create a feed for your door. 
* In adafruit, navigate to "My Key" and transer your "Username" and "Active Key" to config.h
* In IFTT, create an applet linking to your adafruit feed with voice commands/responses.
* Lastly, add your wifi SSID username and password the GarageFinal.ino http_username and password. 
* You can also change the default login credentials from "admin" and "password" to something else. 

### Preview of Landing Page
![LandingPage](https://user-images.githubusercontent.com/40500151/103262980-61682980-495b-11eb-98ec-4220e75a701a.png)
