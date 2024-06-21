#ifndef RADIO_CONTROLLER_H
#define RADIO_CONTROLLER_H

class RadioController {
public:
    RadioController(int pin);
    ~RadioController();
    int GetValue();
    void SetValue(int value);
    void SetIsr(void (*ptr)());
    int GetPin();
    long GetRiseTime();
    void SetRiseTime(long time);
    int GetRangeMin();
    void SetRangeMin(int min);
    int GetRangeMax();
    void SetRangeMax(int max);

    static void HandleIsr(RadioController* controller);

private:
    int pin_;
    int value_;
    int max_;
    int min_;
    long t_start_;
    bool isr_attached_;
};

#endif