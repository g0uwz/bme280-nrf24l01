
#include <SPI.h>
//#include <Wire.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x27 or 0x3F for a 16 chars and 2 line display
//LiquidCrystal_I2C lcd(0x27, 20, 04);
LiquidCrystal_I2C lcd(0x27, 20, 4);
RF24 radio(7, 8); // CE, CSN
const uint64_t address = 0xF0F0F0F0E1LL;

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
  radio.begin();
  lcd.init();                 // initialize the lcd

  lcd.backlight();           // backlight on
  // lcd.clear();

  lcd.print("Starting...");
  delay(500);
  lcd.clear();

  lcd.print("NRF24L01 Connecting");
  delay(500);
  lcd.clear();
  radio.setDataRate(RF24_250KBPS);  // 250KBPS - 1MBPS - 2MBPS
  radio.setChannel(124);           // Use a channel unlikely to be used by Wifi, Microwave ovens etc

  radio.openWritingPipe(address);  //Setting the address where we will send the data
  Serial.print("ADDRESS :");
  radio.openReadingPipe(0, address);  //Setting the address at which we will receive the data

  radio.startListening();              //This sets the module as receiver
}

int recvData()
{
  if ( radio.available() )
  {
    radio.read(&data, sizeof(MyData));
    return 1;
  }
  return 0;
}


void loop()
{
  if (recvData())
  {

    Serial.println("Data Received:");
    Serial.print("Packet No. = ");
    Serial.println(data.counter);

    Serial.print("Temperature = ");
    Serial.print(data.temperature);
   
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
    //lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Humidity  ");
    lcd.print(data.humidity);
    lcd.print("%");

    lcd.setCursor(0, 0);
    lcd.print("Temp ");
    lcd.print(data.temperature);
    lcd.print(" C");


    lcd.setCursor(0, 2);
    lcd.print("Altitude ");
    lcd.print(data.altitude);
    lcd.print("m");

    lcd.setCursor(0, 3);
    lcd.print("Air Pres' ");
    lcd.print(data.pressure);
    lcd.print("hPa");

    delay(10);
   
  }
}
