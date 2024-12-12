// include the library code:
#include <LiquidCrystal.h>

// Pin definitions
#define BUTTON_PIN1 6
#define BUTTON_PIN2 7
#define START_BUTTON_PIN 13
#define WIN_LED_PIN 10
#define GAME_TIME_MS 5000 // Time in milliseconds for the timer

// Game settings
const int MAX_SEQUENCE_LENGTH = 3;
int ledPins[] = {9, 10};        // LEDs for Simon game
int buttonPins[] = {6, 7};      // Buttons for Simon game

// Variables
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int simonSequence[MAX_SEQUENCE_LENGTH]; // Simon game sequence
int playerSequence[MAX_SEQUENCE_LENGTH];
int currentSequenceLength = 0;
int gameState = 0;
unsigned long startTime;

void setup() {
  lcd.begin(16, 2);                          // Initialize LCD
  lcd.print("Press start!");                 // Initial message

  pinMode(START_BUTTON_PIN, INPUT_PULLUP);   // Start button
  pinMode(WIN_LED_PIN, OUTPUT);              // Win LED
  for (int i = 0; i < 2; i++) {
    pinMode(ledPins[i], OUTPUT);             // Set LED pins as outputs
    pinMode(buttonPins[i], INPUT_PULLUP);    // Set button pins as inputs
  }
  randomSeed(analogRead(0));                 // Initialize random seed
  Serial.begin(9600);
}

void loop() {
  switch (gameState) {
    case 0: // Wait for start button press
      if (digitalRead(START_BUTTON_PIN) == LOW) {
        startTime = millis();
        lcd.clear();
        lcd.print("Timer running...");
        gameState = 1;
        delay(200);
      }
      break;

    case 1: // Timer countdown
      if (millis() - startTime < GAME_TIME_MS) {
        lcd.setCursor(0, 1);
        lcd.print((GAME_TIME_MS - (millis() - startTime)) / 1000);
      } else {
        lcd.clear();
        lcd.print("Press button!");
        gameState = 2;
      }
      break;

    case 2: // Wait for Simon game start button
      if (digitalRead(BUTTON_PIN1) == LOW || digitalRead(BUTTON_PIN2) == LOW) {
        lcd.clear();
        lcd.print("Simon game start");
        delay(1000);
        gameState = 3;
      }
      break;

    case 3: // Play Simon game
      playSimonGame();
      gameState = 4;
      break;

    case 4: // End of game
      blinkWinLED();
      resetGame();
      gameState = 0;
      break;
  }
}

void playSimonGame() {
  for (int i = 0; i < MAX_SEQUENCE_LENGTH; i++) {
    generateSequence(i);
    playSequence(i);
    if (!getPlayerInput(i)) {
      lcd.clear();
      lcd.print("You lost!");
      delay(2000);
      return;
    }
  }
  lcd.clear();
  lcd.print("You won!");
  delay(2000);
}

void generateSequence(int index) {
  simonSequence[index] = random(0, 2); // Generate random sequence
}

void playSequence(int length) {
  for (int i = 0; i <= length; i++) {
    digitalWrite(ledPins[simonSequence[i]], HIGH);
    delay(500);
    digitalWrite(ledPins[simonSequence[i]], LOW);
    delay(250);
  }
}

bool getPlayerInput(int length) {
  for (int i = 0; i <= length; i++) {
    bool buttonPressed = false;
    while (!buttonPressed) {
      for (int j = 0; j < 2; j++) {
        if (digitalRead(buttonPins[j]) == LOW) {
          playerSequence[i] = j;
          buttonPressed = true;
          delay(200); // Debounce
        }
      }
    }
    if (playerSequence[i] != simonSequence[i]) {
      return false;
    }
  }
  return true;
}

void blinkWinLED() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(WIN_LED_PIN, HIGH);
    delay(500);
    digitalWrite(WIN_LED_PIN, LOW);
    delay(500);
  }
}

void resetGame() {
  currentSequenceLength = 0;
  lcd.clear();
  lcd.print("Press start!");
}

