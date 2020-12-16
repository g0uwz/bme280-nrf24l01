
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
const uint64_t address = 0xF0F0F0F0E1LL;
int counter = 0;


float temperature;
float humidity;
float altitude;
float pressure;

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;

struct MyData
{
  int counter;
  float temperature;
  float humidity;
  float altitude;
  float pressure;
};
MyData data;

void setup()
{
  Serial.begin(9600);

  radio.begin();                    //Starting the Wireless communication
  radio.setDataRate(RF24_250KBPS);  // 250KBPS - 1MBPS - 2MBPS
  radio.setChannel(124);           // Use a channel unlikely to be used by Wifi, Microwave ovens etc
  radio.setPALevel(RF24_PA_LOW);   // Set the transmit power MIN.LOW,HIGH,MAX
  radio.openWritingPipe(address);  //Setting the address where we will send the dataradio.setPALevel(RF24_PA_MIN);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.stopListening();           //This sets the module as transmitter

  if (!bme.begin(0x76))
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

}

void loop()
{
  data.counter = counter;
  data.temperature = bme.readTemperature();
  data.pressure = bme.readPressure() / 100.0F;
  data.altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  data.humidity = bme.readHumidity();

  Serial.print("Packet No. = ");
  Serial.println(data.counter);

  Serial.print("Temperature = ");
  Serial.print(data.temperature);
  Serial.print(char(176));
  Serial.println("*C");

  Serial.print("Pressure = ");
  Serial.print(data.pressure);
  Serial.println("hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(data.altitude);
  Serial.println("m");

  Serial.print("Humidity = ");
  Serial.print(data.humidity);
  Serial.println("%");

  Serial.println();

  radio.write(&data, sizeof(MyData));

  Serial.println("Data Packet Sent");
  Serial.println("");

  counter++;
  delay(3000);
}
