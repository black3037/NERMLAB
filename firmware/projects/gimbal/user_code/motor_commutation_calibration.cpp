// Includes
#include "motor_commutation_calibration.h"
#include "math_util.h"

//*****************************************************************************
MotorCommutationCalibration::MotorCommutationCalibration(uint8_t num_pole_pairs):
      num_pole_pairs_(num_pole_pairs),
      cal_commutation_state_(INITIALIZING)
{
}

//*****************************************************************************
bool MotorCommutationCalibration::runStateMachine(
                         double time,
                         float mech_angle,
                         float *command_angle
                         )
{
    switch (cal_commutation_state_)
    {
    case INITIALIZING:
    {
        // allow 3 seconds for first position to settle
        previous_time_ = time + 2.0;

        command_angle_ = PI/6;

        data_index_ = 0;
        sum_ = 0.0f;

        cal_commutation_state_ = GETTING_INITIAL_ANGLE;
    }    
    case GETTING_INITIAL_ANGLE:
    {
        // look for the first angle after setting the command
        // need the first one and another to determine direction
        if (time-previous_time_ > 1.0)
        {
            last_motor_angle_ = mech_angle;
            cal_commutation_state_ = FINDING_DIRECTION;
            command_angle_ += PI/3;
            previous_time_ = time;
        }
    }
    case FINDING_DIRECTION:
    {
        // look for the second angle to determine direction
        if (time-previous_time_ > 1.0)
        {
            float delta_angle = mech_angle - last_motor_angle_;
            if ( ( delta_angle > 0.0f )&&( delta_angle < PI/2/num_pole_pairs_ ))
            {
                direction_ = 1;
            }
            else
            {
                direction_ = -1;
            }
            // equilibrium angle is offset 90 elec degrees from command
            fit_angle_ = command_angle_ + direction_*PI/2;
            
            // decide if offset will be close to the 0 / 2PI boundary
            float offset = direction_*fit_angle_/num_pole_pairs_ - mech_angle;
            offset = wrap_angle_0_to_2PI(offset);
            if ((offset > PI/6)&&(offset < 11*PI/6))
            {
                use_2PI_wrap_ = true;
            }
            else
            {
                use_2PI_wrap_ = false;
            }
                
            cal_commutation_state_ = GETTING_DATA_FOR_AVERAGE;
        }
    }
    case GETTING_DATA_FOR_AVERAGE:
    {
        if (time-previous_time_ > 1.0)
        {
            // find offset between actual and expected mechanicanical angle
            float offset = direction_*fit_angle_/num_pole_pairs_ - mech_angle;
            if (use_2PI_wrap_)
            {
                offset = wrap_angle_0_to_2PI(offset);
            }
            else
            {
                offset = wrap_angle_negPI_to_PI(offset);
            }
            
            sum_ += offset;
            command_angle_ += PI/3;
            fit_angle_ += PI/3;
            previous_time_ = time;
            data_index_++;
        }
        
        if (data_index_ >= 6*num_pole_pairs_)
        {
            // calculate least squares fit
            offset_ = sum_/6.0f/num_pole_pairs_;
            
            // move offset to within one electrical rotation
            float elec_range = PI/num_pole_pairs_;
            while (offset_ <= -elec_range)
            {
                offset_ += 2*elec_range;
            }
            while (offset_ > elec_range)
            {
                offset_ -= 2*elec_range;
            }

            // finished with calibration
            cal_commutation_state_ = INITIALIZING;
            return true;
        }
    }
    }
    *command_angle = wrap_angle_0_to_2PI(command_angle_);
    return false;
}    
