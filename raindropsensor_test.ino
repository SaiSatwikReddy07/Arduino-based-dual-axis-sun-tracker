
int b;
String weather_data;
int sensorValue = 0;
float rain = 0;

void setup() 
{
   Serial.begin(9600);
}

void loop() 
{
  b = get_rain();
  weather_data = String(b);
  Serial.print("Rain=");
  Serial.print(weather_data);
  Serial.print(F("mm/hour   "));
   Serial.println("");
//  delay(2000);
  
}

int get_rain()
{
  sensorValue = analogRead(A0);
  Serial.println(sensorValue);
  rain = 0;
  if ((sensorValue <= 950) && (sensorValue > 500))
  {
    rain = map(sensorValue, 900, 500, 0, 25);
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
