#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//LiquidCrystal_I2C lcd(0x38, 16, 2); // I2C address 0x27, 16 column and 2 rows
LiquidCrystal_I2C lcd(0x3F,20,4);  // set the LCD address to 0x27 for a 16 chars and 4 line display
//LiquidCrystal_I2C lcd(0x27,20,4);  
// I2C connections
// vcc = +5V of arduino
// GND to ground pin of arduino
// SDA = to A4 pin of arduino
// SCK = to A5 pin of arduino

float sensorValue1 = 0;
float sensorValue2 = 0;
float voltageValue = 0;
float currentValue = 0;
float Power_now = 0, Power_anc=0, Current_anc =0,Voltage_anc=0, deltaI=0, deltaV=0 ; 
float delta = 1.4;                 
float pwm = 128;                        
const int pwmPin = 6;
void setup()
{
  lcd.init(); 
  lcd.init();
  //Wire.begin();
  pinMode(pwmPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("\nI2C Scanner");
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("SOLAR MPPT");
  lcd.setCursor(0,1);
  lcd.print("CHARGE");
  lcd.setCursor(0,2);
  lcd.print("CONTROLLER");
  delay(2000);
  lcd.clear();
}

void loop() 
{
  sensorValue1 = analogRead(A0);   // voltage
  sensorValue2 = analogRead(A1);   // current
  voltageValue= (sensorValue1 * 5.0 /1023.0) *5;
  currentValue= (sensorValue2 * 5.0 /1023.0);
  //lcd.setCursor(0, 0);
  Power_now = voltageValue * currentValue;
  lcd.setCursor(0, 0);
  lcd.print("Ppv=");
  lcd.print(Power_now);
  lcd.print("W");  
  lcd.setCursor(0, 1);
  lcd.print("V=");
  lcd.print(voltageValue);
  lcd.print("V I=");
  lcd.print(currentValue);
  lcd.print("A");
  deltaI= currentValue-Current_anc;
  deltaV= voltageValue-Voltage_anc;
 if(deltaV==0)
  { if(deltaI==0)
     {// nothing to do
     }
   else 
    { if(deltaI>0)
       pwm=pwm-delta;
     else
       pwm=pwm+delta;
    }
  }
 else
  { if((voltageValue*deltaI)+(currentValue*deltaV)==0)
     {// nothing to do
     }
  
    else
     { if((deltaI/deltaV)+(currentValue/voltageValue)>0)
        
         pwm=pwm-delta;  
    
      else
        pwm=pwm+delta;
     
     }
  }
      
Voltage_anc= voltageValue;
Current_anc= currentValue;
Power_anc=Power_now;
if(pwm > 240)
   pwm=240;
if (pwm < 15)
   pwm=15;
 analogWrite(pwmPin, pwm);

}
