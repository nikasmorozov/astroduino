//Libraries
#include <DHT.h>

//Constants
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino


//Variables
int chk;
float RH;  //Stores humidity value
float TEMP; //Stores temperature value

int ThermistorPin1 = 3;
int ThermistorPin2 = 2;
int Vo1;
int Vo2;
float R1 = 10000;
float logR2, R2, T, Tc1, Tc2, dewPoint;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

//Initializing LED Pin
int heater_pin1 = 5;
int heater_pin2 = 3;

void setup() {
  Serial.begin(9600);

  dht.begin();

 //Declaring LED pin as output
  pinMode(heater_pin1, OUTPUT);
  pinMode(heater_pin2, OUTPUT);
}

void loop() {
  //Read data and store it to variables hum and temp
    RH = dht.readHumidity();
    TEMP = dht.readTemperature();
   
  Vo1 = analogRead(ThermistorPin1);
  Vo2 = analogRead(ThermistorPin2);

  Tc1 = calculateTemperature(Vo1);
  Tc2 = calculateTemperature(Vo2);

  dewPoint = 243.04*(log(RH/100)+((17.625*TEMP)/(243.04+TEMP)))/(17.625-log(RH/100)-((17.625*TEMP)/(243.04+TEMP)));

   //Print temp and humidity values to serial monitor
   Serial.print("Temp: ");
    Serial.print(TEMP);
    Serial.print(" C");
    
    Serial.print(", Humidity: ");
    Serial.print(RH);
    
    Serial.print(" %, Dew Point: ");
    Serial.print(dewPoint);
    Serial.println(" C");

  int heaterCycle1 = map(Tc1, dewPoint, dewPoint + 5, 255, 0);
  int heaterCycle2 = map(Tc2, dewPoint, dewPoint + 5, 255, 0);

  Serial.print("Probe 1:: "); 
  Serial.print(Tc1);
  Serial.print(" C");
  Serial.print(", heaterCycle1: ");
  Serial.println(heaterCycle1);

  analogWrite(heater_pin1, heaterCycle1);
  analogWrite(heater_pin2, heaterCycle2);

  delay(2000);
}

float calculateTemperature(int Vo){
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  return T - 273.15;
}
