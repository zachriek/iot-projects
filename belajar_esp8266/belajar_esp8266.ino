#define LED_DO D1      // Do (C4)
#define LED_RE D2      // Re (D4)
#define LED_MI D3      // Mi (E4)
#define LED_FA D4      // Fa (F4)
#define LED_SOL D5     // Sol (G4)
#define LED_LA D6      // La (A4)
#define LED_SI D7      // Si (B4)
#define LED_DO_HIGH D8 // Do' (C5)
#define BUZZER D0      // Buzzer

#define NOTE_DO  262   // C4
#define NOTE_RE  294   // D4
#define NOTE_MI  330   // E4
#define NOTE_FA  349   // F4
#define NOTE_SOL 392   // G4
#define NOTE_LA  440   // A4
#define NOTE_SI  494   // B4
#define NOTE_DO_HIGH 523 // C5

int melody[] = {
  NOTE_DO, NOTE_DO, NOTE_SOL, NOTE_SOL, NOTE_LA, NOTE_LA, NOTE_SOL,
  NOTE_FA, NOTE_FA, NOTE_MI, NOTE_MI, NOTE_RE, NOTE_RE, NOTE_DO,
  NOTE_DO, NOTE_DO, NOTE_SOL, NOTE_SOL, NOTE_LA, NOTE_LA, NOTE_SOL,
  NOTE_FA, NOTE_FA, NOTE_MI, NOTE_MI, NOTE_RE, NOTE_RE, NOTE_DO
};

int durations[] = {
  500, 500, 500, 500, 500, 500, 1000,
  500, 500, 500, 500, 500, 500, 1000,
  500, 500, 500, 500, 500, 500, 1000,
  500, 500, 500, 500, 500, 500, 1000
};

int songLength = sizeof(melody) / sizeof(melody[0]);

void setup() {
  pinMode(LED_DO, OUTPUT);
  pinMode(LED_RE, OUTPUT);
  pinMode(LED_MI, OUTPUT);
  pinMode(LED_FA, OUTPUT);
  pinMode(LED_SOL, OUTPUT);
  pinMode(LED_LA, OUTPUT);
  pinMode(LED_SI, OUTPUT);
  pinMode(LED_DO_HIGH, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  turnOffAll();

  Serial.begin(115200);
}

void loop() {
  playSong();
}

void playSong() {
  for (int i = 0; i < songLength; i++) {
    int note = melody[i];
    int duration = durations[i];

    lightLED(note);

    tone(BUZZER, note, duration);

    int pause = duration * 0.3;
    delay(duration + pause);

    turnOffAll();
    noTone(BUZZER);
  }
}

void lightLED(int note) {
  turnOffAll();
  switch (note) {
    case NOTE_DO:
      digitalWrite(LED_DO, HIGH);
      break;
    case NOTE_RE:
      digitalWrite(LED_RE, HIGH);
      break;
    case NOTE_MI:
      digitalWrite(LED_MI, HIGH);
      break;
    case NOTE_FA:
      digitalWrite(LED_FA, HIGH);
      break;
    case NOTE_SOL:
      digitalWrite(LED_SOL, HIGH);
      break;
    case NOTE_LA:
      digitalWrite(LED_LA, HIGH);
      break;
    case NOTE_SI:
      digitalWrite(LED_SI, HIGH);
      break;
    case NOTE_DO_HIGH:
      digitalWrite(LED_DO_HIGH, HIGH);
      break;
  }
}

void turnOffAll() {
  digitalWrite(LED_DO, LOW);
  digitalWrite(LED_RE, LOW);
  digitalWrite(LED_MI, LOW);
  digitalWrite(LED_FA, LOW);
  digitalWrite(LED_SOL, LOW);
  digitalWrite(LED_LA, LOW);
  digitalWrite(LED_SI, LOW);
  digitalWrite(LED_DO_HIGH, LOW);
}