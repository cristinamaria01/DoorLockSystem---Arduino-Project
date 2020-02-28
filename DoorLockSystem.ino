#include <Servo.h>
  //CLOSED = 1  //OPEN   = 0

#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define pass_length 5

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7 ,3, POSITIVE);
Servo ServoMotor;

char data[pass_length];     
byte data_count = 0;
char password[pass_length] = "4815"; 
byte pass_count = 0;
bool Pass_is_good;
char key;



bool door = true;

int pos = 0;         // Variable to store the Servo position

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};

byte rowPins[ROWS] = { 9, 8, 7, 6};
byte colPins[COLS] = { 5, 4, 3, 2};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void clearData()
{
  while(data_count != 0)
  { data[data_count--] = 0;
  }
  return;
}

void ServoOpen()
{/*
  for(pos = 90; pos >= 0; pos -= 5)
  { ServoMotor.write(pos);
    delay(15);
  }*/
  ServoMotor.write(0);
}

void ServoClose()
{/*
  for(pos = 0;pos <= 90;pos +=5)
  { ServoMotor.write(pos);
    delay(15);
  }*/

  ServoMotor.write(90);

}

void Open()
{
  lcd.setCursor(0, 0);
  lcd.print(" Enter password");

  key = keypad.getKey();
  if (key)  //ma asigur ca s-a apasat butonul
  {
      data[data_count] = key;  //stochez caracterul intr-un sir de caractere
      lcd.setCursor(data_count, 1);//muta cursorul pt a scrie caracterul introdus de la tastatura
      lcd.print(data[data_count]);//afiseara caracterul de la tastatura
      data_count++;
  }

  if(data_count == pass_length-1)
  {
    if(strcmp(data, password) == 0)
    {
      lcd.clear();
      ServoOpen();
      lcd.print("  Door is open");
      tone(10, 500);
      delay(500);
      noTone(10);
      digitalWrite(12, HIGH);
      door = 0;
    }
    else 
    {
      lcd.clear();
      tone(10, 500);
      delay(300);
      noTone(10);
      delay(100);
      tone(10, 500);
      delay(300);
      noTone(10);
      lcd.print("  Wrong password");
      delay(1000);
      door = 1;
    }
    clearData();
  }
}


void setup() {
  ServoMotor.attach(11);
  ServoMotor.write(90);
  lcd.begin(16, 2);
  lcd.print("    Welcome!");
  lcd.setCursor(0, 1);
  lcd.print("  Proiect 2019");
  delay(3000);
  lcd.clear();
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(10, OUTPUT);
  

  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
}

void loop(){
 if (door == 0) // Daca usa este deschisa, o pot inchide prin apsarea tastei #;
             // Daca este inchisa, o voi deschide cu ajutorul functiei Open();
 {
   digitalWrite(12, HIGH);
   digitalWrite(13, LOW);
   key = keypad.getKey();

   if(key == '#')
   {
    digitalWrite(12,LOW);
    digitalWrite(13,HIGH);
    lcd.clear();
    ServoMotor.write(90);
    lcd.print("  Door is closed");
    delay(3000);
    door = 1;   //CLOSED
    }
  }
    
  else {
    digitalWrite(12, LOW);
    digitalWrite(13, HIGH);
    Open();
  }
  
}
