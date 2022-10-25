#include <dht11.h>

dht11 DHT11;
#define DHT11PIN A1

int humidityVal = 0, tempVal = 0;
int humidityDisp0 = 0, humidityDisp1 = 0;
String weather_data;

void setup()
{
  
Serial.begin(9600);
}

void loop()
{
  int chk = DHT11.read(DHT11PIN);
  tempVal = int(DHT11.temperature);
  Serial.print(tempVal);
  Serial.print("C");
  Serial.println("");
  humidityVal = int(DHT11.humidity);
  Serial.print(humidityVal);
  Serial.print("%");
  Serial.println("");
    delay(1000);

}
