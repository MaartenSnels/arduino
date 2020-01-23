int encoder0PinALast = LOW;

#define DIM_STEP 5
void handleEncoder0() {
  char n = digitalRead(ENCODER_0_PIN_A);
  if ((encoder0PinALast == LOW) && (n == HIGH)) {
    if (digitalRead(ENCODER_0_PIN_B) == LOW) {
      if (dim > DIM_STEP) {
        dim -= DIM_STEP;
      }
    } else {
      if (dim < 95) {
        dim += DIM_STEP;
      }
    }
  }
  encoder0PinALast = n;
  Serial.print("and the delay is now: ");
  Serial.println(_delay);
}
