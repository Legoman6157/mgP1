#define BUTTON1 A0
#define BUTTON2 A1
#define BUTTON3 A2
#define BUTTON4 A4

const int clockPin = 8;
const int dataPin = 11;
const int latchPin = 12;

int xpos = 1;
int ypos = 6;

int board[] = {
  0b11111111,
  0b10000101,
  0b10010001,
  0b10101111,
  0b10100001,
  0b10101101,
  0b10000101,
  0b11111111,
};

int winBoard[] = {
  0b10101000,
  0b10101000,
  0b11111010,
  0b00000010,
  0b10010010,
  0b11010000,
  0b10110000,
  0b10010000
};

const int numStates = sizeof(board)/sizeof(int);

int rows[] = {
  4,
  5,
  6,
  7,
  9,
  10,
  2,
  3
};

int lastb;

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  Serial.begin(9600);
  lastb = 0;
}

void printNewPos() {
  Serial.println("New position:");
  Serial.print("    ypos: ");
  Serial.println(ypos);
  Serial.print("    xpos: ");
  Serial.println(xpos);
}

void move_left(int* board) {
  Serial.println("Button 1 pressed!");
  if (!((board[ypos] >> (xpos+1)) & 1)) {
    board[ypos] = board[ypos] ^ (1 << xpos);
    ++xpos;
    board[ypos] = board[ypos] | (1 << xpos);
  }
  printNewPos();
}

void move_down(int* board) {
  Serial.println("Button 2 pressed!");
  if (!((board[ypos+1] >> xpos) & 1)) {
    board[ypos] = board[ypos] ^ (1 << xpos);
    ++ypos;
    board[ypos] = board[ypos] | (1 << xpos);
  }
  printNewPos();
}

void move_up(int* board) {
  Serial.println("Button 3 pressed!");
  if (!((board[ypos-1] >> xpos) & 1)) {
    board[ypos] = board[ypos] ^ (1 << xpos);
    --ypos;
    board[ypos] = board[ypos] | (1 << xpos);
  }
  printNewPos();
}

void move_right(int* board) {
  Serial.println("Button 4 pressed!");
  if (!((board[ypos] >> (xpos-1)) & 1)) {
    board[ypos] = board[ypos] ^ (1 << xpos);
    --xpos;
    board[ypos] = board[ypos] | (1 << xpos);
  }
  printNewPos();
}

void loop() {
  int* modifiedBoard = board;
  modifiedBoard[ypos] = modifiedBoard[ypos] | (1 << xpos);

  if ((millis() - lastb) >= 225) {
    if (digitalRead(BUTTON1) == HIGH)
      move_left(modifiedBoard);
    if (digitalRead(BUTTON2) == HIGH)
      move_down(modifiedBoard);
    if (digitalRead(BUTTON3) == HIGH)
      move_up(modifiedBoard);
    if (digitalRead(BUTTON4) == HIGH)
      move_right(modifiedBoard);
    lastb = millis();
  }
  
  for (int i = 0; i < numStates; ++i) {
    digitalWrite(rows[i], HIGH);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, ~modifiedBoard[i]);
    digitalWrite(latchPin, HIGH);
    delay(1);
    digitalWrite(rows[i], LOW);
  }

  if ((ypos == 1) && (xpos == 1)) {
    while(true) {
      for (int i = 0; i < numStates; ++i) {
        digitalWrite(rows[i], HIGH);
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, LSBFIRST, ~winBoard[i]);
        digitalWrite(latchPin, HIGH);
        delay(1);
        digitalWrite(rows[i], LOW);
      }
    }
  }
}
