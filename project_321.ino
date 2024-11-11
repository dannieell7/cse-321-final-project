// include the library code:
#include <LiquidCrystal.h>

#define LED_PIN1 8
#define LED_PIN2 9
#define BUTTON_PIN1 7
#define BUTTON_PIN2 6
#define WIN_LED_PIN 10      
#define LOSS_LED_PIN 11 

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int buttonPin[] = {6,7};  // Button pins
int ledPin[] = {8,9};     // LED pins

int seq[2];               // game sequence
int playerSeq[2];         // player sequence
int sequenceLength = 0;

void setup() {

  lcd.begin(16, 2);                                       // set up lcd number of columns and rows
  lcd.print("press button to start simon game");          // print a message to the lcd

  for (int i = 0; i < 2; i++) {
    pinMode(ledPin[i], OUTPUT);                           // set the led pins as outputs
    pinMode(buttonPin[i], INPUT);                         // set the button pin as inputs
  }
  randomSeed(analogRead(0));                              // Initialize random seed
  Serial.begin(9600);


}

void loop() {

  // process of printing the message to the screen
  for (in positionCounter = 0; positionCounter < 34; positionCounter++) {
    lcd.scrollDisplayLeft();                             // moves all the text one space to the left each time a letter is added.
    delay(100);
  }
  delay(1000);

  generateSequence();
  playSequence();

  if (checkPlayerInput()) {
    Serial.println("You won");
    indicateWin();
  } else {
    Serial.println("You lost");
    indicateLoss();
  }
  delay(1000);
}

// this function generates the sequence
void generateSequence() {
  seq[sequenceLength] = random(0,2);
  sequenceLength++;
}

// this function plays the sequence for the user.
void playSequence() {
  for (int i=0; i < sequenceLength; i++) {
    digitalWrite(ledPin[seq[i]], HIGH);
    delay(500);
    digitalWrite(ledPin[seq[i]], LOW);
    delay(250);

    bool correctButtonPressed = false;
    while (!correctButtonPressed) {
      if (digitalRead(buttonPin[seq[i]]) == HIGH) { // Check if player pressed correct button
        correctButtonPressed = true;
        delay(200);  // Debounce delay
      }
    }
  }
}

// this function checks the players input sequence
bool checkPlayerInput() {
  for (int i=0; i < sequenceLength; i++) {
    bool pressedButton = false;
    while (!pressedButton) {
      for (int j=0; j<2; j++) {
        if (digitalRead(buttonPin[j] == HIGH)) {
          playerSeq[i] = j;
          pressedButton = true;
          break;
        }
      }
    }
    if (playerSeq[i] != seq[i]) {
      return false;
    }
  }
  return true;
}

// lights up green led when user wins the game
void indicateWin() {
  digitalWrite(WIN_LED_PIN, HIGH);  
  delay(2000);                      
  digitalWrite(WIN_LED_PIN, LOW);   
}

// lights up red led when user loses the game
void indicateLoss() {
  digitalWrite(LOSS_LED_PIN, HIGH); 
  delay(2000);                      
  digitalWrite(LOSS_LED_PIN, LOW);  
}

// resets the game after player wins or loses
void resetGame() {
  sequenceLength = 0;
  delay(1000);
}
