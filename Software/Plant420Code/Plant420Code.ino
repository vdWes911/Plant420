#include <Adafruit_Si7021.h>

//#include <OneWire.h>
#include <WiFi.h>
#include <Wire.h>

// WiFi network name and password:
const char * networkName = "haus";
const char * networkPswd = "Fortnite2311";

// Internet domain to request from:
const char * hostDomain = "google.com";
const int hostPort = 80;

const int BUTTON_PIN = 0;
const int LED_PIN = 17;

#define SDA1 18
#define SCL1 19

#define SDA2 5
#define SCL2 4

TwoWire twoWire = TwoWire(1); //17
//OneWire  i2c(17);  // on pin 10 (a 4.7K resistor is necessary)

void setup()
{
  // Initilize hardware:
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  
  pinMode(21, OUTPUT);
  digitalWrite(21, LOW);
  delay(100);
  digitalWrite(21, HIGH);
  delay(100);
  digitalWrite(21, LOW);
  digitalWrite(LED_PIN, LOW);

  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  digitalWrite(18, HIGH);
  digitalWrite(19, HIGH);
  delay(100);
  
  digitalWrite(18, LOW);
  digitalWrite(19, LOW);

  // Connect to the WiFi network (see function below loop)
  //connectToWiFi(networkName, networkPswd);

  //digitalWrite(LED_PIN, LOW); // LED off
  //Serial.print("Press button 0 to connect to ");
  //Serial.println(hostDomain);
  
}

void loop()
{
  digitalWrite(LED_PIN, HIGH);
  digitalWrite(21, LOW);
  if (digitalRead(BUTTON_PIN) == LOW)
  { // Check if button has been pressed
    scan1();
  }
}

void scan1() {
  Serial.println("Scanning I2C Addresses Channel 1");
  uint8_t cnt = 0;
  Wire.begin(SDA1, SCL1);
  for (uint8_t i = 0; i < 128; i++) {
    Wire.beginTransmission(i);
    delay(10);
    uint8_t ec = Wire.endTransmission(true);
    if (ec == 0) {
      if (i < 16)
      {
      }
      else
      {
        Serial.print(i, HEX);
        cnt++;
      }
    }
    else Serial.print("..");
    Serial.print(' ');
    if ((i & 0x0f) == 0x0f)Serial.println();
  }
  Serial.print("Scan Completed, ");
  Serial.print(cnt);
  Serial.println(" I2C Devices found.");

}

void connectToWiFi(const char * ssid, const char * pwd)
{
  int ledState = 0;

  printLine();
  Serial.println("Connecting to WiFi network: " + String(ssid));

  WiFi.begin(ssid, pwd);

  while (WiFi.status() != WL_CONNECTED)
  {
    // Blink LED while we're connecting:
    digitalWrite(LED_PIN, ledState);
    ledState = (ledState + 1) % 2; // Flip ledState
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


void requestURL(const char * host, uint8_t port)
{
  printLine();
  Serial.println("Connecting to domain: " + String(host));

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, port))
  {
    Serial.println("connection failed");
    return;
  }
  Serial.println("Connected!");
  printLine();

  // This will send the request to the server
  client.print((String)"GET / HTTP/1.1\r\n" +
               "Host: " + String(host) + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0)
  {
    if (millis() - timeout > 5000)
    {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available())
  {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");
  client.stop();
}

void printLine()
{
  Serial.println();
  for (int i = 0; i < 30; i++)
    Serial.print("-");
  Serial.println();
}
