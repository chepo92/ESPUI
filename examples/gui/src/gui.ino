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

const char *ssid = "ESPUI";
const char *password = "espui";

const char *hostname = "espui";

int statusLabelId;
int graphId;
int millisLabelId;
int testSwitchId;


/* */

int pwm_A = D1;//Right side  D1
int dir_A = D2;//Right reverse  D2
int pwm_B = D3;//Right side  D3
int dir_B = D4;//Right reverse  D4

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
  int sliderValueWithOffset = sender->value.toInt() + 100;
  Serial.print("SliderValue with offset ");
  Serial.println(sliderValueWithOffset);
  int pwmFromSlider = sender->value.toInt();

    if (sender->id == 8 )
    {
      if (pwmFromSlider > 0 )
      {
        analogWrite(pwm_A, pwmFromSlider); 
        analogWrite(dir_A, HIGH );  
      } else
      {
        digitalWrite(pwm_A, LOW); 
        digitalWrite(dir_A, LOW ); 
      }
    }
    else if (sender->id == 11 )
    {
      if (pwmFromSlider > 0 )
      {
        analogWrite(pwm_B, pwmFromSlider); 
        analogWrite(dir_B, HIGH );  
      } else
      {
        digitalWrite(pwm_B, LOW); 
        digitalWrite(dir_B, LOW ); 
      }
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
    ESPUI.print(statusLabelId, "Start");
    break;

  case B_UP:
    Serial.println("Status: Stop");
    ESPUI.print(statusLabelId, "Stop");
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
    Serial.print("Active:");
    break;

  case S_INACTIVE:
    Serial.print("Inactive");
    break;
  }

  Serial.print(" ");
  Serial.println(sender->id);
}

void otherSwitchExample(Control *sender, int value) {
  switch (value) {
  case S_ACTIVE:
    Serial.print("Active:");
    break;

  case S_INACTIVE:
    Serial.print("Inactive");
    break;
  }

  Serial.print(" ");
  Serial.println(sender->id);
}

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
    uint8_t timeout = 10;

    // Wait for connection, 5s timeout
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
      WiFi.softAP(ssid);

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

  statusLabelId = ESPUI.label("Status:", ControlColor::Turquoise, "Stop");
  millisLabelId = ESPUI.label("Millis:", ControlColor::Emerald, "0");
  ESPUI.button("Push Button", &buttonCallback, ControlColor::Peterriver, "Press");
  ESPUI.button("Other Button", &buttonExample, ControlColor::Wetasphalt, "Press");
  ESPUI.padWithCenter("Pad with center", &padExample, ControlColor::Sunflower);
  ESPUI.pad("Pad without center", &padExample, ControlColor::Carrot);
  testSwitchId = ESPUI.switcher("Switch one", &switchExample, ControlColor::Alizarin, false);
  ESPUI.switcher("Switch two", &otherSwitchExample, ControlColor::None, true);
  ESPUI.slider("Slider one", &slider, ControlColor::Alizarin, 0);
  ESPUI.slider("Slider two", &slider, ControlColor::Turquoise, 0);
  ESPUI.text("Text Test:", &textCall, ControlColor::Alizarin, "a Text Field");
  ESPUI.number("Numbertest", &numberCall, ControlColor::Alizarin, 5, 0, 10);

  graphId = ESPUI.graph("Graph Test", ControlColor::Wetasphalt);

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

  static long oldTime = 0;
  static bool testSwitchState = false;

  if (millis() - oldTime > 5000) {
    ESPUI.print(millisLabelId, String(millis()));

    ESPUI.addGraphPoint(graphId, random(1, 50));

    testSwitchState = !testSwitchState;
    ESPUI.updateSwitcher(testSwitchId, testSwitchState);
    
    oldTime = millis();
  }
}