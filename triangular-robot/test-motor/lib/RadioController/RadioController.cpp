#include <Arduino.h>
#include <RadioController.h>
#include <utility>

RadioController::RadioController(int pin) {
    pin_ = pin;
    // Set default values to avoid jumps at start
    value_ = 1500;

    // Min max autocal range
    max_ = 1900;
    min_ = 1100;
}

RadioController::~RadioController() {
    if (isr_attached_) {
        detachInterrupt(pin_);
    }
}

void RadioController::SetIsr(void (*ptr)()) {
    pinMode(pin_, INPUT);
    attachInterrupt(digitalPinToInterrupt(pin_), ptr, CHANGE);
    isr_attached_ = true;
}

int RadioController::GetPin() {
    return pin_;
}

int RadioController::GetValue() {
    return value_;
}

void RadioController::SetValue(int value) {
    value_ = value;
}

long RadioController::GetRiseTime() {
    return t_start_;
}
void RadioController::SetRiseTime(long time) {
    t_start_ = time;
}

int RadioController::GetRangeMin() {
    return min_;
}

void RadioController::SetRangeMin(int min) {
    min_ = min;
}

int RadioController::GetRangeMax() {
    return max_;
}

void RadioController::SetRangeMax(int max) {
    max_ = max;
}

void RadioController::HandleIsr(RadioController* controller)
{
    if (digitalRead(controller->GetPin())) {
        controller->SetRiseTime(micros());
        return;
    }

    int delta = micros() - controller->GetRiseTime();
    if(delta >= 900 && delta <= 2100) {
        controller->SetValue(delta);
    }
}