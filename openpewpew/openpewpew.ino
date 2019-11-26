struct AutofireConfig {
  int frames_active;
  int frames_inactive;
  int input_pin;
  int output_pin;
  int autofire_state;
};

AutofireConfig button1 = {
  /*frames_active=*/1,
  /*frames_inactive=*/2,
  /*input_pin=*/2,
  /*output_pin=*/12,
  /*autofire_state=*/0  
};

AutofireConfig button2 = {
  /*frames_active=*/1,
  /*frames_inactive=*/2,
  /*input_pin=*/2,
  /*output_pin=*/12,
  /*autofire_state=*/0  
};

void press_button(int pin) {
  digitalWrite(pin, LOW);
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void release_button(int pin) {
  digitalWrite (12, LOW) ;
  pinMode(12, INPUT_PULLUP);
}

void led_on() { digitalWrite (LED_BUILTIN, HIGH); }
void led_off() { digitalWrite (LED_BUILTIN, LOW); }

void handle_button(AutofireConfig* button) {
  boolean should_fire = !digitalRead(button->input_pin);
  if (should_fire) {
    if (button->autofire_state++ < button->frames_active) {
        press_button(button->output_pin);
        led_on();
    } else {
        release_button(button->output_pin);
        led_off();
    }
    if (button->autofire_state > (button->frames_active + button->frames_inactive)) {
      button->autofire_state = 0;
    }
  } else {
    button->autofire_state = 0;
    release_button(button->output_pin);
  }
}

ISR(TIMER1_COMPA_vect) {
  handle_button(&button1);
  handle_button(&button2);
}

void setup() {
  pinMode(button1.input_pin, INPUT_PULLUP);
  pinMode(button2.input_pin, INPUT_PULLUP);
  release_button(button1.output_pin);
  release_button(button2.output_pin);
  
  pinMode(LED_BUILTIN, OUTPUT);
  led_off();

  // Setup timer.
  cli();
  TCCR1A = 0x00; // Timer 1 in normal mode.
  TCCR1B = 0x02; // 16MHz clock with prescaler, TCNT1 increments every .5 uS (cs11 bit set)
  TCNT1  = 0; // Initialize counter to 0.
  OCR1A = 33333; // = 16666 microseconds (each count is .5 us)
  TIMSK1 |= (1 << OCIE1A); // Timer compare interrupt enabled.
  sei();
}

void loop() {}
