// OpenPewPew, active autofire using Arduino Nano
// buffi - 2019
//

// Button configs below.
// Four autofire buttons with different frequencies are allowed.
// When "input_pin" of a button is connected to GND, "output_pin" will start auto-firing.
// Each pulse is "frames_active" frames long, with "frames_inactive" frames between each pulse.

struct AutofireConfig {
  int frames_active, frames_inactive, input_pin, output_pin, autofire_state;
};

AutofireConfig BUTTON_1 = {
  /*frames_active=*/1,
  /*frames_inactive=*/2,
  /*input_pin=*/8,
  /*output_pin=*/7,
  /*autofire_state=*/0  
};

AutofireConfig BUTTON_2 = {
  /*frames_active=*/3,
  /*frames_inactive=*/3,
  /*input_pin=*/6,
  /*output_pin=*/5,
  /*autofire_state=*/0  
};

AutofireConfig BUTTON_3 = {
  /*frames_active=*/15,
  /*frames_inactive=*/15,
  /*input_pin=*/4,
  /*output_pin=*/3,
  /*autofire_state=*/0  
};

AutofireConfig *BUTTONS[] = {&BUTTON_1, &BUTTON_2, &BUTTON_3};

// Code below.
boolean handle_button(AutofireConfig* button) {
  boolean should_fire = !digitalRead(button->input_pin);
  boolean firing = false;
  if (should_fire) {
    if (button->autofire_state++ < button->frames_active) {
        digitalWrite(button->output_pin, LOW);
        pinMode(button->output_pin, OUTPUT);
        digitalWrite(button->output_pin, LOW);
        firing = true;
    } else {
        digitalWrite (button->output_pin, LOW) ;
        pinMode(button->output_pin, INPUT);
    }
    if (button->autofire_state > (button->frames_active + button->frames_inactive)) {
      button->autofire_state = 0;
    }
  } else {
    button->autofire_state = 0;
    digitalWrite (button->output_pin, LOW) ;
    pinMode(button->output_pin, INPUT);
  }
  return firing;
}

ISR(TIMER1_COMPA_vect) {
  boolean firing = false;
  for (int i=0; i<3; i++)
    firing |= handle_button(BUTTONS[i]);
  if (firing)
    digitalWrite (LED_BUILTIN, HIGH);
  else
    digitalWrite (LED_BUILTIN, LOW);
}

void setup() {
  for (int i=0; i<3; i++) {
    pinMode(BUTTONS[i]->input_pin, INPUT_PULLUP);
    digitalWrite (BUTTONS[i]->output_pin, LOW) ;
    pinMode(BUTTONS[i]->output_pin, INPUT);
  }

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite (LED_BUILTIN, LOW);

  // Setup timer.
  cli();
  TCCR1A = 0x00; // Timer 1 in normal mode
  TCCR1B = 0x02; // 16MHz clock with prescaler, TCNT1 increments every .5 uS
  TCNT1  = 0;    // Initialize counter to 0
  OCR1A = 33333; // = 16666 microseconds (each count is .5 us)
  TIMSK1 |= (1 << OCIE1A); // Enable compare interrupt
  sei();
}

void loop() { delay(1); }
