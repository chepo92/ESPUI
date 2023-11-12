#include <DNSServer.h>
#include <ESPUI.h>

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;

#if defined(ESP32)
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

#include "secrets.h"

const char *fallback_ssid = "ESPUI";

const char *hostname = "espui";

// int statusLabelId;
int graphId;
int millisLabelId;
// int testSwitchId;
int sliderId [3];

/* DC motor control L298 */

  byte L298_pwm_A = D1;//Right side  D1
  byte L298_dir_A = D2;//Right reverse  D2
  byte L298_pwm_B = D3;//Right side  D3
  byte L298_dir_B = D4;//Right reverse  D4


/* Stepper motor control ULN-2003 (4 pins control) */
  #include <Stepper.h>
  const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution for your motor
  // Pin definition
  byte stepper_A = D5;//Right side  D1
  byte stepper_B = D6;//Right reverse  D2
  byte stepper_C = D7;//Right side  D3
  byte stepper_D = D8;//Right reverse  D4
  // initialize the stepper library on pins 8 through 11:
  Stepper myStepper(stepsPerRevolution, stepper_A, stepper_B, stepper_C, stepper_D);



void numberCall(Control *sender, int type) { Serial.println(sender->value); }

void textCall(Control *sender, int type) {
  Serial.print("Text: ID: ");
  Serial.print(sender->id);
  Serial.print(", Value: ");
  Serial.println(sender->value);
}

void slider(Control *sender, int type) {
  Serial.print("Slider: ID: ");
  Serial.print(sender->id);
  Serial.print(", Value: ");
  Serial.println(sender->value);
  // Like all Control Values in ESPUI slider values are Strings. To use them as int simply do this:
  int pwmFromSlider = sender->value.toInt();
  pwmFromSlider = pwmFromSlider*40;
    if (sender->id == sliderId[0] ) // 1st motor 
    {
      if (pwmFromSlider > 0 )
      {
        analogWrite(L298_pwm_A, pwmFromSlider); 
        analogWrite(L298_dir_A, HIGH );  
      } else
      {
        digitalWrite(L298_pwm_A, LOW); 
        digitalWrite(L298_dir_A, LOW ); 
      }
    }
    else if (sender->id == sliderId[1] ) // 2nd motor 
    {
      if (pwmFromSlider > 0 )
      {
        analogWrite(L298_pwm_B, pwmFromSlider); 
        analogWrite(L298_dir_B, HIGH );  
      } else
      {
        digitalWrite(L298_pwm_B, LOW); 
        digitalWrite(L298_dir_B, LOW ); 
      }
    }
    else if (sender->id == sliderId[2] ) // 3rd motor 
    {
        myStepper.setSpeed(pwmFromSlider); // set the speed
    }    

}

void buttonCallback(Control *sender, int type) {
  switch (type) {
  case B_DOWN:
    Serial.println("Button DOWN ");
    break;

  case B_UP:
    Serial.println("Button UP ");
    break;
  }
}

void buttonExample(Control *sender, int type) {
  switch (type) {
  case B_DOWN:
    Serial.println("Status: Start");
    // ESPUI.print(statusLabelId, "Start");
    break;

  case B_UP:
    Serial.println("Status: Stop");
    // ESPUI.print(statusLabelId, "Stop");
    break;
  }
}
void padExample(Control *sender, int value) {
  switch (value) {
  case P_LEFT_DOWN:
    Serial.print("left down");
    break;

  case P_LEFT_UP:
    Serial.print("left up");
    break;

  case P_RIGHT_DOWN:
    Serial.print("right down");
    break;

  case P_RIGHT_UP:
    Serial.print("right up");
    break;

  case P_FOR_DOWN:
    Serial.print("for down");
    break;

  case P_FOR_UP:
    Serial.print("for up");
    break;

  case P_BACK_DOWN:
    Serial.print("back down");
    break;

  case P_BACK_UP:
    Serial.print("back up");
    break;

  case P_CENTER_DOWN:
    Serial.print("center down");
    break;

  case P_CENTER_UP:
    Serial.print("center up");
    break;
  }

  Serial.print(" ");
  Serial.println(sender->id);
}

void switchExample(Control *sender, int value) {
  switch (value) {
  case S_ACTIVE:
    Serial.print("Active: ");
    break;

  case S_INACTIVE:
    Serial.print("Inactive ");
    break;
  }

  Serial.print(" ");
  Serial.println(sender->id);
}

void otherSwitchExample(Control *sender, int value) {
  switch (value) {
  case S_ACTIVE:
    Serial.print("Active: ");
    break;

  case S_INACTIVE:
    Serial.print("Inactive: ");
    break;
  }

  Serial.print(" ");
  Serial.println(sender->id);
}


/* Setup function */

void setup(void) {
  ESPUI.setVerbosity(Verbosity::VerboseJSON);
  Serial.begin(115200);

#if defined(ESP32)
  WiFi.setHostname(hostname);
#else
  WiFi.hostname(hostname);
#endif

  // try to connect to existing network
  WiFi.begin(ssid, password);
  Serial.print("\n\nTry to connect to existing network");

  {
    uint8_t timeout = 30;

    // Wait for connection,
    do {
      delay(500);
      Serial.print(".");
      timeout--;
    } while (timeout && WiFi.status() != WL_CONNECTED);

    // not connected -> create hotspot
    if (WiFi.status() != WL_CONNECTED) {
      Serial.print("\n\nCreating hotspot");

      WiFi.mode(WIFI_AP);
      WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
      WiFi.softAP(fallback_ssid);

      timeout = 5;

      do {
        delay(500);
        Serial.print(".");
        timeout--;
      } while (timeout);
    }
  }

  dnsServer.start(DNS_PORT, "*", apIP);

  Serial.println("\n\nWiFi parameters:");
  Serial.print("Mode: ");
  Serial.println(WiFi.getMode() == WIFI_AP ? "Station" : "Client");
  Serial.print("IP address: ");
  Serial.println(WiFi.getMode() == WIFI_AP ? WiFi.softAPIP() : WiFi.localIP());

  // statusLabelId = ESPUI.label("Status:", ControlColor::Turquoise, "Stop");
  // millisLabelId = ESPUI.label("Millis:", ControlColor::Emerald, "0");
  // ESPUI.button("Push Button", &buttonCallback, ControlColor::Peterriver, "Press");
  // ESPUI.button("Other Button", &buttonExample, ControlColor::Wetasphalt, "Press");
  // ESPUI.padWithCenter("Pad with center", &padExample, ControlColor::Sunflower);
  // ESPUI.pad("Pad without center", &padExample, ControlColor::Carrot);
  ESPUI.switcher("Switch one", &switchExample, ControlColor::Alizarin, true);
  sliderId[0] = ESPUI.slider("Motor one", &slider, ControlColor::Alizarin, 0);
  ESPUI.switcher("Switch two", &otherSwitchExample, ControlColor::None, true);
  sliderId[1] = ESPUI.slider("Motor two", &slider, ControlColor::Turquoise, 0);
  ESPUI.switcher("Switch 3", &otherSwitchExample, ControlColor::None, true);
  sliderId[2] = ESPUI.slider("Motor three", &slider, ControlColor::Emerald, 0);
  // ESPUI.text("Text Test:", &textCall, ControlColor::Alizarin, "a Text Field");
  // ESPUI.number("Numbertest", &numberCall, ControlColor::Alizarin, 5, 0, 10);

  // graphId = ESPUI.graph("Graph Test", ControlColor::Wetasphalt);

  /*
   * .begin loads and serves all files from PROGMEM directly.
   * If you want to serve the files from SPIFFS use ESPUI.beginSPIFFS
   * (.prepareFileSystem has to be run in an empty sketch before)
   */
 
  // Enable this option if you want sliders to be continuous (update during move) and not discrete (update on stop)
  // ESPUI.sliderContinuous = true;

  /*
   * Optionally you can use HTTP BasicAuth. Keep in mind that this is NOT a
   * SECURE way of limiting access.
   * Anyone who is able to sniff traffic will be able to intercept your password
   * since it is transmitted in cleartext. Just add a string as username and
   * password, for example begin("ESPUI Control", "username", "password")
   */


  ESPUI.begin("ESPUI Control");
}

void loop(void) {
  dnsServer.processNextRequest();

  // static long oldTime = 0;
  // static bool testSwitchState = false;

  // if (millis() - oldTime > 5000) {
  //   // ESPUI.print(millisLabelId, String(millis()));

  //   // ESPUI.addGraphPoint(graphId, random(1, 50));

  //   // testSwitchState = !testSwitchState;
  //   // ESPUI.updateSwitcher(testSwitchId, testSwitchState);
    
  //   oldTime = millis();
  // }

  // run the stepper, blocking function, step 1/100 of a revolution 
  myStepper.step(stepsPerRevolution / 100);

}