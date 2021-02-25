/*
   Inkplate_Low_Power example for e-radionica.com Inkplate 6
   For this example you will need USB cable and Inkplate 6.
   Select "Inkplate 6(ESP32)" from Tools -> Board menu.
   Don't have "Inkplate 6(ESP32)" option? Follow our tutorial and add it:
   https://e-radionica.com/en/blog/add-inkplate-6-to-arduino-ide/

   This example builds on the e-radionica examples and shows how to load an image file from a URL, display and then deep sleep.

   Visit www.inkplate.io for more info, details on capturing the web image to display available at digitalurban.org
   
*/

#include "HTTPClient.h"          //Include library for HTTPClient
#include "Inkplate.h"      //Include Inkplate library to the sketch
#include "driver/rtc_io.h" //ESP32 library used for deep sleep and RTC wake up pins
#include "WiFi.h"                //Include library for WiFi

#define uS_TO_S_FACTOR 1000000 // Conversion factor for micro seconds to seconds
#define TIME_TO_SLEEP  900      // How long ESP32 will be in deep sleep (in seconds) - edit accordingly to your use

Inkplate display(INKPLATE_3BIT); // Create an object on Inkplate library and also set library into 3 Bit mode (gray)

const char *ssid = "Enter your SSID";     // Your WiFi SSID
const char *password = "Enter your Wifi Password"; // Your WiFi password

void setup()
{
    display.begin();        // Init Inkplate library (you should call this function ONLY ONCE)
    display.clearDisplay(); // Clear frame buffer of display
    display.display();      // Put clear image on display

    display.print("Connecting to WiFi...");
    display.partialUpdate();

    // Connect to the WiFi network added in reset if fails to connect
    int counter =0;
   // WiFi.mode(WIFI_MODE_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED && counter < 7 )
    {
        delay(5000);
        display.print(".");
        counter++;
        display.partialUpdate();
    }
       if (WiFi.status() != WL_CONNECTED){

       ESP.restart();
}

    
    display.println("\nWiFi OK! Downloading...");
    display.partialUpdate();

    
// Edit below to provide the location of your jpg to display

    if (!display.drawImage("http://locationofyour.jpg", 0, 0, true, false))
    {
        // If is something failed (wrong filename or format), write error message on the screen.
        display.println("Image open error");
        display.display();
    }
    display.display();

    


    rtc_gpio_isolate(GPIO_NUM_12); // Isolate/disable GPIO12 on ESP32 (only to reduce power consumption in sleep)
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR); // Activate wake-up timer -- wake up after 20s here
    esp_deep_sleep_start();                                        // Put ESP32 into deep sleep. Program stops here.
}

void loop()
{
    // Nothing! If you use deep sleep, whole program should be in setup() because each time the board restarts, not in a
    // loop()! loop() must be empty!
}
