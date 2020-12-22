/*Trevor A John
  https://github.com/g0uwz
  https://www.facebook.com/groups/323136102086500

  Wireless Weather Station

  LCD 2004
  SDA A4 yellow
  SCL A5 green
  GND  0v  black
  VCC  5v  red
  Hex address 0x27

  NRF24L01      ARDUINO NANO
  VCC               3.3v
  GND               GND
  CSN                8
  CE                 7
  SCK                13
  MOSI               11
  MISO               12

  10uF capacitor on nrf24l01 supply not required on receiver.

  To conserve battery on this receiver i have created on demand only backlight on LCD unit.
  Connect push button between D3 & GND.

*/




#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4); // Set the LCD address to 0x27

RF24 radio(7, 8); // CE, CSN
const uint64_t address = 0xF0F0F0F0E1LL;
const int buttonPin = 3;
int buttonState = 0;
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
  pinMode(3, INPUT_PULLUP);
  radio.begin();
  lcd.init();                 // initialize the lcd

  lcd.print("Starting...");
  delay(500);
  lcd.clear();

  lcd.print("NRF24L01 Connecting");
  delay(500);
  lcd.clear();
  radio.setDataRate(RF24_250KBPS);  // 250KBPS - 1MBPS - 2MBPS
  radio.setChannel(124);
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
  if (digitalRead(buttonPin) == LOW) // connect pushbutton to D3 & GND, press and hold button for backlight.
  {
    lcd.backlight(); // Backlight on
  }
  if (digitalRead(buttonPin) == HIGH) // D3 held high at default with internal 20k
  {
    lcd.noBacklight(); // Backlight off when pushbutton not activated
  }



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

    }
  }
}
