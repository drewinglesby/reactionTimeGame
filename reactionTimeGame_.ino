//REACTION TIME GAME

#include <Adafruit_CircuitPlayground.h>

bool gameOn = false;
bool gameEnded = false;
int score = 0;
int rounds = 5;
unsigned long startTime;
bool waitingForReaction = false;

void setup() {
  Serial.begin(9600);
  while (!Serial) { // Wait for serial to begin
    delay(10);
  }
  Serial.println("Flip the switch to start the game!");
  Serial.println("");
  CircuitPlayground.begin();
}

void loop() {
  // Check if the switch is on to start the game
  if (CircuitPlayground.slideSwitch() && !gameOn) {
    Serial.println("Starting game...");
    startGame();
  }

  // Check if the game is on and if waiting for the reaction
  if (gameOn && waitingForReaction) {
    if (CircuitPlayground.leftButton() || CircuitPlayground.rightButton()) {
      buttonPress();
    }
  }

  if (gameEnded) {
    party();

    return; // Skip the rest of the loop if the game has ended
  }

  // Continue game logic only if game is on
  if (gameOn && !waitingForReaction) {
    delay(random(1000, 5000));
    CircuitPlayground.clearPixels(); // Make sure LEDs are off
    for (int i = 0; i < 10; i++) {
      CircuitPlayground.setPixelColor(i, 255, 255, 255); // Turn on all LEDs white
    }
    startTime = millis();
    waitingForReaction = true;
  }
}

void startGame() {
  gameOn = true;
  score = 0;
  delay(2000);
  Serial.println("Game Started!");
  Serial.println("");
  playRound();
}

void playRound() {
  for (int i = 0; i < rounds; i++) {
    waitingForReaction = false;
    delay(random(1000, 5000)); // Wait for a random time between 1-5 seconds
    CircuitPlayground.clearPixels(); // Make sure LEDs are off
    for (int j = 0; j < 10; j++) {
      CircuitPlayground.setPixelColor(j, 255, 255, 255); // Turn on all LEDs white
    }
    startTime = millis();
    waitingForReaction = true;

    while (waitingForReaction) {
      if (CircuitPlayground.leftButton() || CircuitPlayground.rightButton()) {
        buttonPress();
        break;
      }
    }
    delay(1000); // Wait 1 second before next round
  }

  endGame();
}

void buttonPress() {
  unsigned long reactionTime = millis() - startTime;
  waitingForReaction = false;

  if (reactionTime <= 150) {

    score += 3;
    setColor(0, 255, 0); //GREEN
    CircuitPlayground.playTone(1000, 100);
    Serial.print("Reaction time: ");
    Serial.println(reactionTime);
    Serial.println("");

  } else if (reactionTime <= 175) {

    score += 2;
    setColor(255, 255, 0); //YELLOW
    CircuitPlayground.playTone(800, 100);
    Serial.print("Reaction time: ");
    Serial.println(reactionTime);
    Serial.println("");

  } else if (reactionTime <= 200) {

    score += 1;
    setColor(255, 75, 0); //ORANGE
    CircuitPlayground.playTone(600, 100);
    Serial.print("Reaction time: ");
    Serial.println(reactionTime);
    Serial.println("");

  } else {

    score -= 1;
    setColor(255, 0, 0); //RED
    CircuitPlayground.playTone(400, 100);
    Serial.print("Reaction time: ");
    Serial.println(reactionTime);
    Serial.println("");

  }

  delay(1000);
  CircuitPlayground.clearPixels();
}

void setColor(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, r, g, b);
  }
}

void endGame() {
  gameOn = false;
  gameEnded = true;
  CircuitPlayground.clearPixels();
  Serial.println("Game Over!");
  Serial.print("Your score is: ");
  Serial.println(score);
  Serial.println("Flip the switch to play again!");
  Serial.println("");

  CircuitPlayground.playTone(600, 75);
  CircuitPlayground.playTone(700, 75);
  delay(300);
  CircuitPlayground.playTone(400, 180);
  delay(250);
  CircuitPlayground.playTone(600, 75);
  CircuitPlayground.playTone(700, 75);
  delay(300);
  CircuitPlayground.playTone(400, 180);
}


//ADDED FROM OTHER ASSIGNMENTS

void party() {
  int led = random(10);
  uint32_t color = CircuitPlayground.colorWheel(random(256));

  CircuitPlayground.setPixelColor(led, color);
  delay(80);

  CircuitPlayground.clearPixels();
}

