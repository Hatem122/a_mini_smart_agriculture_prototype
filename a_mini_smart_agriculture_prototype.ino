#include <SoftwareSerial.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

SoftwareSerial bluetooth(2, 3);  //RX, TX

#define mois A0
#define DHTPIN 7
#define pump 8
#define fan 9

DHT dht = DHT(DHTPIN, DHT22);

int moisVal;
int lim = 200;
int device = A1;
char val;

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
  dht.begin();

  pinMode(mois, INPUT);
  pinMode(device, INPUT);
  pinMode(DHTPIN, INPUT);
  pinMode(pump, OUTPUT);
  pinMode(fan, OUTPUT);

  digitalWrite(pump, LOW);
  digitalWrite(fan, LOW);
}

void loop() {
  //----------------Soil Moisture---------------------
  moisVal = analogRead(mois);

  if (moisVal <= 500) {
    digitalWrite(pump, HIGH);
  } else {
    digitalWrite(pump, LOW);
  }

  //----------------Air Temp & Humidity---------------
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (t >= 27.0) {
    digitalWrite(fan, HIGH);
  } else {
    digitalWrite(fan, LOW);
  }

  //-------------Receive Data via Bluetooth-----------
  if (Serial.available()) {
    val = Serial.read();
    
    //-------------Turn on System---------
    if (val == 'o') {
      digitalWrite(pump, HIGH);
      digitalWrite(fan, HIGH);
      bluetooth.print("\nThe Whole System is Running Manually for 1 min!\n");
      Serial.println("\nThe Whole System is Running Manually for 1 min!\n");
      delay(60000);
    }
    //-------------Turn off System--------
    if (val == 'x') {
      digitalWrite(pump, LOW);
      digitalWrite(fan, LOW);
      bluetooth.print("\nThe System was Shut Down Manually for 1min!\n");
      Serial.print("\nThe System Was Shut Down Manually for 1min!\n");
      delay(60000);
    }

    //-------------Fan Only-------------
    if (val == 'f') {
      digitalWrite(fan, HIGH);
      digitalWrite(pump, LOW);
      bluetooth.print("\nThe Pump Only is Running Manually for 1min!\n");
      Serial.println("\nThe Pump Only is Running Manually for 1min!\n");
      delay(60000);
    }
    //-------------Pump Only------------
    if (val == 'p') {
      digitalWrite(pump, HIGH);
      digitalWrite(fan, LOW);
      bluetooth.print("\nThe Pump Only is Running Manually for 1min!\n");
      Serial.println("\nThe Pump Only is Running Manually for 1min!\n");
      delay(60000);
    }
  }

  //----------------Serial Print---------------
  Serial.print("\nSoil Moisture Sensor Value = ");
  Serial.print(moisVal);
  Serial.print(" || Air Humidity: ");
  Serial.print(h);
  Serial.print(" % ");
  Serial.print(" | Air Temperature: ");
  Serial.print(t);
  Serial.print(" \xC2\xB0");
  Serial.print("C\n");

  //----------------Bluetooth Print-------------
  bluetooth.print("\nSoil Moisture Sensor Value = ");
  bluetooth.print(moisVal);
  bluetooth.print(" || Air Humidity: ");
  bluetooth.print(h);
  bluetooth.print(" % ");
  bluetooth.print("Temperature: ");
  bluetooth.print(t);
  bluetooth.print(" \xC2\xB0");
  bluetooth.print("C\n");

  delay(100);
}