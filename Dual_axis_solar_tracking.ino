#include <dht11.h>
#include<LiquidCrystal.h>
#include <Servo.h>

dht11 DHT11;
#define DHT11PIN A1

// 180 horizontal MAX
Servo horizontal;   // horizontal servo
int servoh = 180;   // 90;     // stand horizontal servo

int servohLimitHigh = 180;
int servohLimitLow = 65;

// 65 degrees MAX
Servo vertical;     // vertical servo 
int servov = 45;    //   90;     // stand vertical servo

int servovLimitHigh = 80;
int servovLimitLow = 15;


// LDR pin connections
//  name  = analogpin;
int ldrlt = A2; //LDR top left - BOTTOM LEFT    <--- BDG
int ldrrt = A3; //LDR top rigt - BOTTOM RIGHT 
int ldrld = A5; //LDR down left - TOP LEFT
int ldrrd = A4; //ldr down rigt - TOP RIGHT

LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

byte termometru[8] = //icon for termometer
{
  B00100,
  B01010,
  B01010,
  B01110,
  B01110,
  B11111,
  B11111,
  B01110
};

byte picatura[8] = //icon for water droplet
{
  B00100,
  B00100,
  B01010,
  B01010,
  B10001,
  B10001,
  B10001,
  B01110,
};


int humidityVal = 0, tempVal = 0;
int humidityDisp0 = 0, humidityDisp1 = 0;

int b;
String weather_data;
int sensorValue = 0;
float rain = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
// servo connections
// name.attacht(pin);
  horizontal.attach(5); 
  vertical.attach(6);
  horizontal.write(180);
  vertical.write(45);
//  delay(3000);

  
  lcd.begin(16, 2);
  lcd.createChar(1, termometru);
  lcd.createChar(2, picatura);
  lcd.clear();    
  lcd.print("Dual Axis Solar ");
  lcd.setCursor(0, 1);
  lcd.print("    Tracking    ");
  delay(3000);
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  b = get_rain();
  weather_data = String(b);
  lcd.setCursor(0, 0);
  lcd.print("Rain=");
  lcd.print(weather_data);
  lcd.print(F("mm/hour   "));
 
  humid_temp_read();
//  delay(300);


  int lt = analogRead(ldrlt); // top left
  int rt = analogRead(ldrrt); // top right
  int ld = analogRead(ldrld); // down left
  int rd = analogRead(ldrrd); // down rigt
  
  // int dtime = analogRead(4)/20; // read potentiometers  
  // int tol = analogRead(5)/4;
  int dtime = 10;
  int tol = 30;
  
  int avt = (lt + rt) / 2; // average value top
  int avd = (ld + rd) / 2; // average value down
  int avl = (lt + ld) / 2; // average value left
  int avr = (rt + rd) / 2; // average value right

  int dvert = avt - avd; // check the diffirence of up and down
  int dhoriz = avl - avr;// check the diffirence og left and rigt
  
  
  Serial.print(avt);
  Serial.print(" ");
  Serial.print(avd);
  Serial.print(" ");
  Serial.print(avl);
  Serial.print(" ");
  Serial.print(avr);
  Serial.print("   ");
  Serial.print(dtime);
  Serial.print("   ");
  Serial.print(tol);
  Serial.println(" ");
  
    
  if (-1*tol > dvert || dvert > tol) // check if the diffirence is in the tolerance else change vertical angle
  {
  if (avt > avd)
  {
    servov = ++servov;
     if (servov > servovLimitHigh) 
     { 
      servov = servovLimitHigh;
     }
  }
  else if (avt < avd)
  {
    servov= --servov;
    if (servov < servovLimitLow)
  {
    servov = servovLimitLow;
  }
  }
  vertical.write(servov);
  }
  
  if (-1*tol > dhoriz || dhoriz > tol) // check if the diffirence is in the tolerance else change horizontal angle
  {
  if (avl > avr)
  {
    servoh = --servoh;
    if (servoh < servohLimitLow)
    {
    servoh = servohLimitLow;
    }
  }
  else if (avl < avr)
  {
    servoh = ++servoh;
     if (servoh > servohLimitHigh)
     {
     servoh = servohLimitHigh;
     }
  }
  else if (avl = avr)
  {
    // nothing
  }
  horizontal.write(servoh);
  }
  delay(dtime);  
}


void humid_temp_read()
{
  int chk = DHT11.read(DHT11PIN);
  lcd.setCursor(1, 1);
  lcd.write(1);
  lcd.setCursor(3, 1);
  tempVal = int(DHT11.temperature);
  lcd.print(tempVal);
  //Serial.println(tempVal);
  lcd.setCursor(5, 1);
  lcd.print((char)223); //degree sign
  lcd.print("C");

  lcd.setCursor(9, 1);
  lcd.write(2);
  lcd.setCursor(11, 1);

  humidityVal = int(DHT11.humidity);
  // humdityDisp0 = char(humdityVal);
  // humidityVal = humidityVal / 10;

  // lcd.print(humidityDisp0);
  lcd.print((float)DHT11.humidity, 2);
  //Serial.println(humidityVal);
  lcd.print("%");
  //  delay(100);
}

int get_rain()
{
  sensorValue = analogRead(A0);
  // print out the value you read:
  Serial.println(sensorValue);
  rain = 0;
  if ((sensorValue <= 950) && (sensorValue > 500))
  {
    rain = map(sensorValue, 950, 500, 0, 25);
  }
  else if ((sensorValue < 500) && (sensorValue >= 300))
  {
    rain = map(sensorValue, 500, 300, 26, 50);
  }
  else if ((sensorValue < 300) && (sensorValue >= 150))
  {
    rain = map(sensorValue, 300, 150, 51, 75);
  }
  else if (sensorValue < 150)
  {
    rain = map(sensorValue, 150, 0, 76, 100);
  }
  return rain;
}
