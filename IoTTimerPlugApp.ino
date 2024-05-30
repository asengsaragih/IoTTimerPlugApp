/* for development */
boolean DEBUG = true;

/* LCD */
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

/* Relay */
#define RELAY 6

/* Buzzer */
#define BUZZER 7

/* Button */
#define BUTTON_STOP_RESET 8
#define BUTTON_HOUR 9
#define BUTTON_MINUTE 10
#define BUTTON_SECOND 11
#define BUTTON_START 12

/* Tmp timer */
int hour = 0;
int minute = 0;
int second = 5;
boolean is_start = false;

void setup() {
  Serial.begin(9600);

  // lcd
  lcd.init();
  lcd.clear();
  lcd.backlight();


  // buzzer
  pinMode(BUZZER, OUTPUT);

  // relay
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);

  // button
  pinMode(BUTTON_HOUR, INPUT);
  pinMode(BUTTON_MINUTE, INPUT);
  pinMode(BUTTON_SECOND, INPUT);
  pinMode(BUTTON_START, INPUT);
  pinMode(BUTTON_STOP_RESET, INPUT);

  turnBuzzer(3, 50);

  initTime();
}

void initTime() {
  lcd.setCursor(0, 0);
  lcd.print("H:");

  if(hour < 10) {
    lcd.setCursor(2, 0);
    lcd.print("0" + (String) hour);
  } else {
    lcd.setCursor(2, 0);
    lcd.print((String) hour);
  }

  lcd.setCursor(5, 0);
  lcd.print("M:");

  if(minute < 10) {
    lcd.setCursor(7, 0);
    lcd.print("0" + (String) minute);
  } else {
    lcd.setCursor(7, 0);
    lcd.print((String) minute);
  }

  lcd.setCursor(10, 0);
  lcd.print("S:");

  if(second < 10) {
    lcd.setCursor(12, 0);
    lcd.print("0" + (String) second);
  } else {
    lcd.setCursor(12, 0);
    lcd.print((String) second);
  }
}

void loop() {
  //BUTTON START
  if (digitalRead(BUTTON_START) == HIGH) {
    if (second > 0 || minute > 0 || hour > 0) {
      is_start = true;
    }
  }

  //validate start
  if (is_start) {
    startTimer();
  }

  //button stop
  if (digitalRead(BUTTON_STOP_RESET) == HIGH && is_start == true) {
    stopTimer();
  }

  //button reset
  if (digitalRead(BUTTON_STOP_RESET) == HIGH && is_start == false) {
    hour = 0;
    minute = 0;
    second = 0;

    updateTime();
    delay(300);
  }


  //update hour
  if (digitalRead(BUTTON_HOUR) == HIGH && is_start == false) {
    if (hour >= 23)
      hour = 0;
    else
      hour++;

    updateTime();
    delay(300);
  }

  //update minute
  if (digitalRead(BUTTON_MINUTE) == HIGH && is_start == false) {
    if (minute >= 59)
      minute = 0;
    else
      minute++;

    updateTime();
    delay(300);
  }

  //update second
  if (digitalRead(BUTTON_SECOND) == HIGH && is_start == false) {
    if (second >= 59)
      second = 0;
    else
      second++;

    updateTime();
    delay(300);
  }
}

void startTimer() {
  if (second == 0 && minute == 0 && hour == 0) {
    stopTimer();
    return;
  }

  //start relay
  digitalWrite(RELAY, HIGH);

  if (second == 0) {
    second = 59;

    if (minute == 0) {
      minute = 59;
      hour--;
    } else {
      minute--;
    }

  } else {
    second--;
  }

  updateTime();
  delay(1000);
}

void stopTimer() {
  hour = 0;
  minute = 0;
  second = 0;

  //start relay
  digitalWrite(RELAY, LOW);

  updateTime();
  turnBuzzer(3, 50);
  is_start = false;

  return;
}

void updateTime() {
  if(hour < 10) {
    lcd.setCursor(2, 0);
    lcd.print("0" + (String) hour);
  } else {
    lcd.setCursor(2, 0);
    lcd.print((String) hour);
  }

  if(minute < 10) {
    lcd.setCursor(7, 0);
    lcd.print("0" + (String) minute);
  } else {
    lcd.setCursor(7, 0);
    lcd.print((String) minute);
  }

  if(second < 10) {
    lcd.setCursor(12, 0);
    lcd.print("0" + (String) second);
  } else {
    lcd.setCursor(12, 0);
    lcd.print((String) second);
  }
}

// function for turning buzzer
void turnBuzzer(size_t looping, int delayTime) {
  for (size_t i = 0; i < looping; i++) {
    digitalWrite(BUZZER, HIGH);
    delay(delayTime);
    digitalWrite(BUZZER, LOW);
    delay(delayTime);
  }
}

// function for print
void print(String text, uint8_t row) {
  lcd.clear();
  lcd.setCursor(0, (row - 1));
  lcd.print(text);
}

//function for debug
void debug(String text) {
  if (DEBUG) {
    Serial.println(text);
  }
}