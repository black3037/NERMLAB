#ifndef MOTOR_COMMUTATION_CALIBRATION_H_INCLUDED
#define MOTOR_COMMUTATION_CALIBRATION_H_INCLUDED

#include <cstdint>

// State machine used to perform commutaion calibration of a brushless DC motor.
// It assumes an unobstructed motor that will settle to equilibrium for each
// command_angle returned.  runStateMachine() should be called repeatedly until
// it returns true.  The state will change once each second.
// Result should be:
//      elec_angle = direction_*num_pole_pairs(mech_angle + offet_)


class MotorCommutationCalibration
{
  public:

    // Constructor
    MotorCommutationCalibration(uint8_t num_pole_pairs);

    // Steps through angles to fit a calibration
    // returns true if finished
    bool runStateMachine(double time,            // input time
                         float mech_angle,       // current measured angle
                         float *command_angle    // electrical angle to command 
                         );
    
    float getOffset(){ return offset_; };
    float getDirection(){ return direction_; };
    
  private:
      
    enum
    {
        INITIALIZING,
        GETTING_INITIAL_ANGLE,
        FINDING_DIRECTION,
        GETTING_DATA_FOR_AVERAGE
    } cal_commutation_state_;
    
    uint8_t num_pole_pairs_;
    int8_t direction_;
    uint8_t data_index_;
    float offset_;
    double previous_time_;
    float command_angle_;
    float last_motor_angle_;
    float fit_angle_;
    float sum_;
    bool use_2PI_wrap_;
};

#endif
