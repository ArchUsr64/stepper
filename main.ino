#define driving_mode 0
#define step_angle 1.8

#define a true
#define pos 1
#define coil_a_0 2
#define coil_a_1 3
#define coil_a_v 5

#define b false
#define neg 2
#define coil_b_0 6
#define coil_b_1 7
#define coil_b_v 9

const int u_step_frac = 4;

int full_drive_iter = 3;
int wave_drive_iter = 0;
int half_step_iter = 0;
int u_step_iter = 0;

int coil_a_speed = 255;
int coil_b_speed = 255;

void setup() {
  pinMode(coil_a_0, OUTPUT);
  pinMode(coil_a_1, OUTPUT);
  pinMode(coil_a_v, OUTPUT);

  pinMode(coil_b_0, OUTPUT);
  pinMode(coil_b_1, OUTPUT);
  pinMode(coil_b_v, OUTPUT);
}

void loop() {
  switch (driving_mode) {
    //wave drive aka single phase
    case 0:

      break;
    //full-step drive aka double phase
    case 1:
      break;
    //half-step
    case 2:
      break;
    //micro-step
    default:
      break;
  }
}

void set_current_value(bool coil_index, int speed) {
  if (coil_index == a) {
    analogWrite(coil_a_v, speed);
  }
  if (coil_index == b) {
    analogWrite(coil_b_v, speed);
  }
}

void set_current_direction(bool coil_index, bool current_direction) {
  if (current_direction == pos) {
    if (coil_index == a) {
      digitalWrite(coil_a_0, HIGH);
      digitalWrite(coil_a_1, LOW);
    }
    if (coil_index == b) {
      digitalWrite(coil_b_0, HIGH);
      digitalWrite(coil_b_1, LOW);
    }
  }
  if (current_direction == neg) {
    if (coil_index == a) {
      digitalWrite(coil_a_0, LOW);
      digitalWrite(coil_a_1, HIGH);
    }
    if (coil_index == b) {
      digitalWrite(coil_b_0, LOW);
      digitalWrite(coil_b_1, HIGH);
    }
  }
}

void kill_coil(bool coil_index) {
  if (coil_index == a) {
    digitalWrite(coil_a_0, HIGH);
    digitalWrite(coil_a_1, HIGH);
  }
  if (coil_index == b) {
    digitalWrite(coil_b_0, HIGH);
    digitalWrite(coil_b_1, HIGH);
  }
}

void wave_drive() {
  switch (wave_drive_iter % 4) {
    case 0:
      kill_coil(a);
      set_current_direction(b, pos);
      break;
    case 1:
      set_current_direction(a, neg);
      kill_coil(b);
      break;
    case 2:
      kill_coil(a);
      set_current_direction(b, neg);
      break;
    case 3:
      set_current_direction(a, pos);
      kill_coil(b);
      break;
  }
  wave_drive_iter++;
  wave_drive_iter %= 4;
}

void full_step() {
  switch (full_drive_iter % 4) {
    case 0:
      set_current_direction(b, neg);
      set_current_direction(b, pos);
      break;
    case 1:
      set_current_direction(b, neg);
      set_current_direction(b, neg);
      break;
    case 2:
      set_current_direction(b, pos);
      set_current_direction(b, neg);
      break;
    case 3:
      set_current_direction(b, pos);
      set_current_direction(b, pos);
      break;
  }
  full_drive_iter++;
  full_drive_iter %= 4;
}

void half_step() {
  switch (half_step_iter % 2) {
    case 0:
      full_step();
      break;
    case 1:
      wave_drive();
      break;
  }
  half_step_iter++;
  half_step_iter %= 8;
}

void u_step() {
  int current_a = 255 * cos((1.8*u_step_iter)/u_step_frac);
  int current_b = 255 * sin((1.8*u_step_iter)/u_step_frac);
  set_current_value(a, abs(current_a));
  set_current_value(b, abs(current_b));
  if (current_a > 0)
    set_current_direction(a, pos);
  else
    set_current_direction(a, neg);
  if (current_b > 0)
    set_current_direction(b, pos);
  else
    set_current_direction(b, neg);
  u_step_iter++;
  u_step_iter %= u_step_frac * 4;
}
