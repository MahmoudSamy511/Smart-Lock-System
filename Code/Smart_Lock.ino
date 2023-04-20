#include <LiquidCrystal.h>
#include <Servo.h>
#include <Keypad.h>
//Pin connection
#define rs 0
#define E 1
#define d4 2
#define d5 3
#define d6 4
#define d7 5
#define RED A1
#define GREEN A0
Servo myservo;
//Macros to change values
#define Enter_Button '#'
#define Number_of_Tries 3
//initialization LCD
LiquidCrystal lcd(rs, E, d4, d5, d6, d7);
//counters used in the code
int i = 0 ,count =0 ,checkCounter = 0;
//Password
char pass_saved[] = "5112001";
int pass_length = sizeof(pass_saved) - 1;
char pass[sizeof(pass_saved) - 1];
//initialization Keypad
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};
byte rowPins[ROWS] = { 6, 7, 8, 9 };
byte colPins[COLS] = { 10, 11, 12 };
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
void setup() {
  lcd.begin(16, 2);
  lcd.print("Enter Password:");
  lcd.setCursor(0, 1);
  myservo.attach(A2);
  myservo.write(0);
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
}
void setPassword() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter Password:");
  lcd.setCursor(0, 1);
}
void Warning() {
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, LOW);
  lcd.setCursor(0, 0);
  lcd.print("Wrong Password");
  //Time for switching to get new input
  delay(500);
  i = 0;
  count = 0;
  lcd.clear();
  setPassword();
  checkCounter++;
}
void loop() {
  while (checkCounter != Number_of_Tries) {
    char key = keypad.getKey();
    if (key >= '0' && key != Enter_Button) {
      lcd.print('*');
      pass[i] = key;
      i++;
    } else if (key == Enter_Button) {
      //check for Password Length
      if (i == pass_length) {
        //check each char of password entered by user
        for (int j = 0; j < pass_length; j++) {
          if (pass[j] == pass_saved[j]) count++;
        }
        if (count == pass_length) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Welcome ,User!");
          lcd.setCursor(0, 1);
          lcd.print("ZAG ENG FAMILY");
          digitalWrite(GREEN, HIGH);
          digitalWrite(RED, LOW);
          myservo.write(180);
          //Time for take Action[Turn on Led Green and move Servo]
          delay(2000);
          i = 0;
          count = 0;
          digitalWrite(GREEN, LOW);
          digitalWrite(RED, LOW);
          myservo.write(0);
          checkCounter = 0;
          setPassword();


        } else {
          Warning();
        }

      } else {
        Warning();
      }
    }
  }
  //if user entered more than 3 incorrect passwords
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Access Denied");
  lcd.setCursor(0, 1);
  lcd.print("Try After 4sec");
  digitalWrite(GREEN, LOW);
  digitalWrite(RED, LOW);
  //Time for deny access
  delay(4000);
  checkCounter = 0;
  setPassword();
}
