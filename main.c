#include <avr/io.h>
#include <avr/interrupt.h>

// Pin assignments
int redPin = 4;
int yellowPin = 5;
int greenPin = 2;
int greenarrowPin = 3;
int buttonPin = 12;
int buzzerPin = 6;
int ledPin = 13;

// Traffic light states
int redState = 1;
int yellowState = 2;
int greenState = 3;

// Global variables
volatile int currentState = redState;  // Current state of the traffic light
volatile int toggleFlag = 0;  // Flag for toggling the LED

void setup() {
  // Pin mode setup
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(arrowPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  // Set up timer interrupt
  cli();  // Disable interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 15624;  // 1Hz at 1024 prescaler
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);  // 1024 prescaler
  TIMSK1 |= (1 << OCIE1A);  // Enable timer compare interrupt
  sei();  // Enable interrupts
}

// Timer1's interrupt service routine (ISR)
ISR(TIMER1_COMPA_vect) {
  toggleFlag = !toggleFlag;  // Toggle the interrupt flag
}

void loop() {
  // Perform traffic light logic based on currentState
  switch (currentState) {
    case redState:
      // Red state: Only turn on the red LED, turn off others
      digitalWrite(redPin, HIGH);
      digitalWrite(yellowPin, LOW);
      digitalWrite(greenPin, LOW);
      digitalWrite(arrowPin, LOW);
      break;

    case yellowState:
      // Yellow state: Only turn on the yellow LED, turn off others
      digitalWrite(redPin, LOW);
      digitalWrite(yellowPin, HIGH);
      digitalWrite(greenPin, LOW);
      digitalWrite(arrowPin, LOW);
      break;

    case greenState:
      // Green state: Only turn on the green LED, turn off others
      digitalWrite(redPin, LOW);
      digitalWrite(yellowPin, LOW);
      digitalWrite(greenPin, HIGH);
      digitalWrite(arrowPin, LOW);
      break;
  }

  // Check button press
  if (digitalRead(buttonPin) == LOW) {
    // Switch to the next state in order: red -> yellow -> green -> red
    currentState = (currentState % 3) + 1;
    delay(1000);  // Debounce delay for demonstration purposes
  }

  // Check the interrupt flag for the LED and the buzzer
  if (toggleFlag) {
    // Toggle the LED on and off with each timer interrupt
    digitalWrite(ledPin, HIGH);

    // Toggle the buzzer on and off with each timer interrupt
    digitalWrite(buzzerPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
  }
}

