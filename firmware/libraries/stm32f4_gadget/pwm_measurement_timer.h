#ifndef PWM_MEASUREMENT_TIMER_H_INCLUDED
#define PWM_MEASUREMENT_TIMER_H_INCLUDED

// global vars set by interrupt service routine
extern volatile uint16_t pwm_measurement_period;
extern volatile uint16_t pwm_measurement_pulse;
extern volatile uint32_t pwm_measurement_clkfreq;

// Implement PWM measurement timer on (TIM4).
class PwmMeasurementTimer
{
  public: // methods

    // Constructor
    PwmMeasurementTimer(uint8_t interrupt_priority);

  private: // fields

};

#endif
