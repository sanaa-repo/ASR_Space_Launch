//Arduino Uno ASR Space Launch Code
//Author: @Sanaa Kapur

//Variable initializations

//temperature variables/pins
const int TEMP_SENSOR_PIN = A0;
int temp;
float R1 = 1000;
float logR2, R2, tKelvin, tCelsius, tFahrenheit;
float RR1 = 1000;
float logRR2, RR2, RtKelvin, RtCelsius, RtFahrenheit;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
const int white_pin = A3;
const int black_pin = A1;

//heating variables/pin
const int HEATER_PIN = 8; //directly to power supply, 5V is 0.71A
const int tempMinF = 32;//change
const int tempMaxF = 50;

//buzzer variables/pin
const int BUZZER_PIN = 12;
const int onTime = 5000;
const int offTime = 2000;
const int pressureSL = 1013; 
boolean turnON = false;

//cutdown and pressure variables/pins
int cutdown = 5;
int pressure = A2;
int pressvalue = 0;
int pressureAvg[12]; 
int currentPos = 0; 
double averagePressure = 0;
int time = 0;
int actualPressure = 0;


void setup() {
  Serial.begin(9600);//set baud rate

  //initialize pins
  pinMode(TEMP_SENSOR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(HEATER_PIN, OUTPUT);
  pinMode(white_pin, INPUT);
  pinMode(black_pin, INPUT);
  pinMode(cutdown, OUTPUT);
  digitalWrite(HEATER_PIN, LOW);

  //print start time
  time = 0;
  Serial.println(__TIME__);

  Serial.println("First:" + String(time));
}

void loop() {
  //temperature sensing
  temp = analogRead(TEMP_SENSOR_PIN);

  /**********************************************************************
  analog to celsius/fahrenheit Steinhard-Hart equation, credit to:
  https://www.circuitgeeks.com/arduino-temperature-sensor-thermistor/#Arduino-Code
  ***********************************************************************/
 
  R2 = R1 * (1023.0 / (float)temp - 1.0); // resistance of the Thermistor
  logR2 = log(RR2);
  tKelvin = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
  tCelsius = tKelvin - 273.15;
  tFahrenheit = (tCelsius * 9.0) / 5.0 + 32.0;
 
  Serial.print("inner temp in farhenheit: ");
  Serial.println(tFahrenheit);

  // color absorption sensing
  int white_temp = analogRead(white_pin);
  int black_temp = analogRead(black_pin);
   
  RR2 = RR1 * (1023.0 / (float)white_temp - 1.0); // resistance of the Thermistor
  logRR2 = log(RR2);
  RtKelvin = (1.0 / (c1 + c2 * logRR2 + c3 * logRR2 * logRR2 * logRR2));
  RtCelsius = RtKelvin - 273.15;
  RtFahrenheit = (RtCelsius * 9.0) / 5.0 + 32.0;
  int outside_temp = RtFahrenheit;

  Serial.print("outer temp in farhenheit: ");
  Serial.println(outside_temp);
  Serial.print("white pin voltage: ");
  Serial.println(white_temp);
  Serial.print("black pin voltage: ");
  Serial.println(black_temp);    
 
  // heating regulation
  
  if (tFahrenheit <= tempMinF) {
    digitalWrite(HEATER_PIN, HIGH);
    Serial.println("heating on");
   }
   else if (tFahrenheit > tempMaxF) {
    digitalWrite(HEATER_PIN, LOW);
    Serial.println("heating off");
  }

  // cutdown mechanism
  pressvalue = analogRead(pressure);
  time = time + 1;
  actualPressure = 2.52 * pressvalue - 244;
  Serial.println(actualPressure);
  
  if (actualPressure < 9 && outside_temp < -36){ //determines cutdown threshold
    digitalWrite(cutdown, HIGH);
    delay(500);
    digitalWrite(cutdown, HIGH);
    delay(500);
  }
 
  //buzzer begins buzzing after pressure returns to sea level

  if(actualPressure < 500)
  {
    turnON = true;
  }
  if(actualPressure <= pressureSL && turnON == true)
  {
    tone(BUZZER_PIN, 85);
    delay(onTime);
    noTone(BUZZER_PIN);
    delay(offTime);
  }
  
}
