#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128                                                        // OLED display width, in pixels
#define SCREEN_HEIGHT 32                                                        // OLED display height, in pixels
#define OLED_RESET     -1                                                       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C                                                     ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int iSec = 0;
int iMin = 0;
int iHora = 0;
int iLiga = 0;

void setup() 
{
  Serial.begin(115200);
  pinMode(17, OUTPUT);
  digitalWrite(17, LOW);  
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))                      // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }

  delay(1000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("Sistema de Irrigacao");
  display.setCursor(25, 20);
  display.println("Covil do Lobo");
  display.display();

  pinMode(26, OUTPUT);                                                          // Definimos o pino 2 (LED) como saída.
  ledcAttachPin(26, 0);                                                         // Atribuimos o pino 2 ao canal 0.
  ledcSetup(0, 1000, 10);
  ledcWrite(0, 0);
}

void loop() 
{
  delay(1000);                      
  
  if (iSec == 59)
  {
    iSec = 0;
    display.clearDisplay();
    display.setCursor(0, 10);
    display.println("Proxima irrigacao em:");
    display.setCursor(35, 20);
    display.print(23-iHora);
    display.print("h ");
    display.print(59-iMin);
    display.println("min");
    display.display();  
    
    if (iMin == 59)
    {
      iMin = 0;
      if (iHora == 23)
      {
        iHora = 0;
        iLiga = 0;
      }
      else iHora++;
    }
    else iMin++;
  }
  else iSec++;

  if ((iSec == 5) && (iLiga == 0))
  {
    iLiga = 1;
    display.clearDisplay();
    display.setCursor(25, 10);
    display.println("Plantas sendo");
    display.setCursor(35, 20);
    display.println("Irrigadas!");
    display.display();

    digitalWrite(17, HIGH); 
    ledcWrite(0, 1024);
    delay(500);
    ledcWrite(0, 0);
    delay(500);
    ledcWrite(0, 1024);
    delay(500);
    ledcWrite(0, 0);
    delay(500);
    ledcWrite(0, 1024);
    delay(500);
    ledcWrite(0, 0);
    delay(500);
    ledcWrite(0, 1024);
    delay(500);
    ledcWrite(0, 0);
    delay(500);
    ledcWrite(0, 1024);
    delay(500);
    ledcWrite(0, 0);
    delay(500);
               
    digitalWrite(17, LOW);
    ledcWrite(0, 0); 

    display.clearDisplay();
    display.setCursor(0, 10);
    display.println("Proxima irrigacao em:");
    display.setCursor(35, 20);
    display.print(23-iHora);
    display.print("h ");
    display.print(59-iMin);
    display.println("min");
    display.display();   
  }

  Serial.print(iSec);
  Serial.print(" ");
  Serial.print(iMin);
  Serial.print(" ");
  Serial.println(iHora);                   
}
