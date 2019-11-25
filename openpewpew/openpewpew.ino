typedef struct AutofireConfig {
  int frames_active;
  int frames_inactive;
  int input_pin;
  int output_pin;
  int autofire_state;
}

AutofireConfig button1 = {
  /*frames_active=*/1,
  /*frames_inactive=*/2,
  /*input_pin=*/2,
  /*frames_active=*/12,
  /*frames_active=*/0  
};

void setup_pins() {
  pinMode(2,INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  digitalWrite (12, LOW) ;
  digitalWrite (13, LOW) ;
}

void setup_timer() {
  cli();
  TCCR1A = 0x00;      // Normal mode, => Disconnect Pin OC1  PWM Operation disabled
  TCCR1B = 0x02;      // 16MHz clock with prescaler, TCNT1 increments every .5 uS (cs11 bit set)
  TCNT1  = 0;//initialize counter value to 0
  OCR1A = 33333;   // = 16666 microseconds (each count is .5 us)
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  sei();
}

void setup() {
  setup_pins();
  setup_timer();
}

ISR(TIMER1_COMPA_vect) {
  boolean should_fire = !digitalRead(2);
  if (should_fire) {
    if (autofire_state++ < frame_active_button_1) {
        pinMode(12, OUTPUT);
        digitalWrite (12, LOW);
        digitalWrite (13, HIGH);
    } else {
        digitalWrite (12, LOW) ;
        pinMode(12, INPUT_PULLUP);
        digitalWrite (13, LOW);
    }
    if (autofire_state > (frame_active_button_1 + frame_inactive_button_1) {
      autofire_state = 0;
    }
  } else {
    autofire_state = 0;
    pinMode(12, INPUT_PULLUP);
    digitalWrite (13, LOW);
  }
}

void loop() {
    delay(10);
}
