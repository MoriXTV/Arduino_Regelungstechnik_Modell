#include <LiquidCrystal_I2C.h>
#include <Smoothed.h>
#include <max6675.h>
#include <PWM.h>
#include <PID_v1.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);



/////////////////////////////////////////////////////////////////////////////////////////////
//Temperatursensor
/////////////////////////////////////////////////////////////////////////////////////////////
static int ktcSO = 5;
static int ktcCS = 6;
static int ktcSCK = 7;
MAX6675 ktc(ktcSCK, ktcCS, ktcSO);

long unsigned int lastRead = 0;
static int readRate = 230;

Smoothed <float> tempSensor;
Smoothed <int> smoothPotentiometer;

double Temperature;
double tempReading;



/////////////////////////////////////////////////////////////////////////////////////////////
//Potentiometer
/////////////////////////////////////////////////////////////////////////////////////////////
static int analogLowDeadband = 50;
static int analogHighDeadband = 30;

int analogPot;
static int pot = A0;

double setTemperature;

double Power;
int Potentiometer;

static int maxTemp = 100;
static int minTemp = 40;

static int potiSteps = 7;

int stepPoti;



/////////////////////////////////////////////////////////////////////////////////////////////
//PWM
/////////////////////////////////////////////////////////////////////////////////////////////
static int OUT = 3;
static int32_t frequency = 100;



/////////////////////////////////////////////////////////////////////////////////////////////
//Knöpfe
/////////////////////////////////////////////////////////////////////////////////////////////
static int upButton = 8;
static int downButton = 9;

bool upButtonPressed = 0;
bool downButtonPressed = 0;

long unsigned int upButtonMillis = 0;
long unsigned int downButtonMillis = 0;

static int buttonTimer = 200;

int mode = 0;

bool clearSwitch = 0;



/////////////////////////////////////////////////////////////////////////////////////////////
//Modi
/////////////////////////////////////////////////////////////////////////////////////////////
static char* names[] = { "Deaktiviert", "Offener Regelkr.", "On/Off Regelkr.", "P Regelkreis",
                         "I Regelkreis", "PI Regelkreis", "PD Regelkreis", "PID Regelkreis"
                       };

const int modeCount = 7;
const int modeDisplayTime = 1000;



/////////////////////////////////////////////////////////////////////////////////////////////
//PID
/////////////////////////////////////////////////////////////////////////////////////////////
double Kp, Ki, Kd;

PID tempControl(&Temperature, &Power, &setTemperature, Kp, Ki, Kd, DIRECT);



/////////////////////////////////////////////////////////////////////////////////////////////
//LCD
/////////////////////////////////////////////////////////////////////////////////////////////
static int refreshInterval = 100;
unsigned long int lastRefresh = 0;




void startup()
{

  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  tempSensor.begin(SMOOTHED_EXPONENTIAL, 1);
  smoothPotentiometer.begin(SMOOTHED_AVERAGE, 5);

  lcd.init();
  lcd.backlight();

  InitTimersSafe();
  SetPinFrequencySafe(OUT, frequency);

  lcd.setCursor(1, 0);
  lcd.print("Umax = 12V DC");

  lcd.setCursor(1, 1);
  lcd.print("Achtung Heiss!");

  delay(4000);

  lcd.clear();

  lcd.setCursor(4, 0);
  lcd.print("03/2024");

  lcd.setCursor(2, 1);
  lcd.print("Moritz Adler");

  delay(500);

  lcd.clear();

  tempControl.SetMode(AUTOMATIC);

  Serial.begin(115200);

  Serial.print(0);
  Serial.print(",");
  Serial.print(0);
  Serial.print(",");
  Serial.print(0);
  Serial.print(",");
  Serial.print(0);
  Serial.print(",");
  Serial.println(120);
}





void IO()
{

  /////////////////////////////////////////////////////////////////////////////////////////////
  //Temperatur messen
  /////////////////////////////////////////////////////////////////////////////////////////////
  if (millis() - lastRead > readRate)
  {
    tempReading = ktc.readCelsius();
    lastRead = millis();
  }
  tempSensor.add(tempReading);
  Temperature = tempSensor.get();




  /////////////////////////////////////////////////////////////////////////////////////////////
  //Potentiometer auswerten
  /////////////////////////////////////////////////////////////////////////////////////////////
  smoothPotentiometer.add(analogRead(pot));
  analogPot = smoothPotentiometer.get();

  Potentiometer = map(analogPot, analogLowDeadband, 1024 - analogHighDeadband , 0 , 255);

  if (analogPot < analogLowDeadband)
  {
    Potentiometer = 0;
  }
  if (analogPot > 1024 - analogHighDeadband)
  {
    Potentiometer = 255;
  }

  if (mode > 1)
  {
    stepPoti = map(Potentiometer, 0, 255, 0, potiSteps);

    if (stepPoti > 0)
    {
      setTemperature = map(stepPoti, 1, potiSteps, minTemp, maxTemp);
    }
    else
    {
      setTemperature = 0;
    }
  }
  else
  {
    setTemperature = 0;
  }

  pwmWrite(OUT, Power);





  /////////////////////////////////////////////////////////////////////////////////////////////
  //Knöpfe auslesen
  /////////////////////////////////////////////////////////////////////////////////////////////
  if (upButtonPressed == 0 && digitalRead(upButton) == 0 && mode < modeCount)
  {
    mode++;
    upButtonPressed = 1;
    upButtonMillis = millis();

    lcd.clear();

    lcd.setCursor(5, 0);
    lcd.print("Mode ");
    lcd.print(mode);

    lcd.setCursor(0, 1);
    lcd.print(names[mode]);
    clearSwitch = 0;
  }

  if (upButtonMillis - millis() > buttonTimer && digitalRead(upButton) == 1)
  {
    upButtonPressed = 0;
  }



  if (downButtonPressed == 0 && digitalRead(downButton) == 0 && mode > 0)
  {
    mode--;
    downButtonPressed = 1;
    downButtonMillis = millis();

    lcd.clear();

    lcd.setCursor(5, 0);
    lcd.print("Mode ");
    lcd.print(mode);

    lcd.setCursor(0, 1);
    lcd.print(names[mode]);
    clearSwitch = 0;
  }

  if (downButtonMillis - millis() > buttonTimer && digitalRead(downButton) == 1)
  {
    downButtonPressed = 0;
  }


  if (millis() - lastRefresh > refreshInterval)
  {
    /////////////////////////////////////////////////////////////////////////////////////////////
    //LCD Anzeige
    /////////////////////////////////////////////////////////////////////////////////////////////
    if (millis() - downButtonMillis  > modeDisplayTime && millis() - upButtonMillis > modeDisplayTime)
    {
      if (clearSwitch == 0)
      {
        lcd.clear();
        clearSwitch = 1;
      }

      lcd.setCursor(0, 0);
      lcd.print(" ist:" + String(Temperature, 1) + "C ");

      lcd.setCursor(13, 0);
      lcd.print("(" + String(mode) + ")");



      if (mode > 1)
      {
        lcd.setCursor(0, 1);
        lcd.print("soll:" + String(setTemperature, 0) + "C ");
      }


      if (mode > 0)
      {
        lcd.setCursor(12, 1);
        lcd.print(String(Power / 2.55, 0) + "%  ");
      }
      else
      {
        lcd.setCursor(2, 1);
        lcd.print("deaktiviert");
      }
    }
    /////////////////////////////////////////////////////////////////////////////////////////////
    //Serielle Anzeige
    /////////////////////////////////////////////////////////////////////////////////////////////

    Serial.print(Temperature);
    Serial.print(",");
    Serial.print(setTemperature);
    Serial.print(",");
    Serial.println(Power / 2.55);

    lastRefresh = millis();

  }
  else
  {
    delay(5);
  }


}
