// Define types of all globs. Instances are defined in globs.cpp
// To use globs include globs.h.
// To define new objects you must modify this file and globs.h

#ifndef GLOB_TYPES_H_INCLUDED
#define GLOB_TYPES_H_INCLUDED

// Includes
#include <cstdint>
#include "glob_constants.h"

//******************************************************************************
// Desired linear/angular velocity of robot.
typedef struct
{
     float linear_velocity;  // [m/s]
     float angular_velocity; // [rad/s] yaw rate

} glob_motion_commands_t;

//******************************************************************************
// Voltages reported by ADC.
typedef struct
{
    float voltages[9];     // [volts]
    float battery_voltage; // [volts]

} glob_analog_t;

//******************************************************************************
// PID parameters used to sync/edit gains from GUI.
typedef struct
{
    float kp;               // Proportional gain
    float ki;               // Integral gain
    float kd;               // Derivative gain
    float integral_lolimit; // Lower saturation limit on integral state
    float integral_hilimit; // Upper saturation limit on integral state
    float lolimit;          // Lower saturation limit on controller output
    float hilimit;          // Upper saturation limit on controller output

} glob_pid_params_t;

//******************************************************************************
// Data that is transmitted when a capture command is received.
// Variable names are kept generic since what's being sent back changes frequently.
typedef struct
{
    float time; // seconds
    float d1;
    float d2;
    float d3;
    float d4;
    float d5;
    float d6;
    float d7;
    float d8;

} glob_capture_data_t;

//******************************************************************************
// Command that is received from user for starting/stopping data logging.
// This is also sent back to user when data logging completes.
typedef struct
{
    uint8_t is_start;         // False (0) if should stop sending data.
    uint8_t paused;           // Collection won't start until this is set to false.
    uint16_t frequency;       // Rate that data is recorded [Hz]
    uint32_t desired_samples; // How many samples to collect before stopping.
    uint32_t total_samples;   // Used to notify UI samples are done being sent and how many there should be.

} glob_capture_command_t;

//******************************************************************************
// For sending low rate periodic status data for user feedback.
typedef struct
{
    float battery; // [volts]
    float roll;    // [rad]
    float pitch;   // [rad]
    float yaw;     // [rad] this comes from odometry, not filter

    // See glo_modes_t (not used directly to keep globs independent)
    uint8_t main_mode;
    uint8_t sub_mode;
    uint8_t state;
    uint8_t error_codes;

    float left_linear_position;   // [meters]
    float right_linear_position;  // [meters]
    float left_angular_position;  // [radians]
    float right_angular_position; // [radians]
    float left_linear_velocity;   // [meters / sec]
    float right_linear_velocity;  // [meters / sec]
    float left_angular_velocity;  // [radians / sec]
    float right_angular_velocity; // [radians / sec]

    float left_pwm;      // from -1 to 1
    float right_pwm;     // from -1 to 1

    int32_t firmware_version; // Firmware version stored in robot settings.

    uint8_t processor_id[12]; // Unique processor ID read from ROM.

} glob_status_data_t;

//******************************************************************************
// Experiment wave input
typedef struct
{
    glob_wave_type_t type;   // type of wave (e.g sine)
    glob_wave_state_t state; // current state of wave (e.g. started / stopped)
    uint8_t pad[2];         // pad for alignment
    float value;            // current value of wave
    float magnitude;        // size of wave
    float frequency;        // Hz
    float duration;         // seconds
    float offset;           // constant wave offset
    float time;             // current time (in sec) of wave period.
    float total_time;       // how long (in sec) the wave has been running.
    uint8_t run_continuous; // if non-zero then is equivalent to duration being infinity
    uint8_t pad2[3];        // second set of pad bytes

    // Trapezoid parameters
    float vmax;                // maximum velocity
    float amax;                // maximum acceleration
    float dx;                  // change in X
    float t1, t2, t3;          // time splits for 3 parabolic splines
    float c1[3], c2[3], c3[3]; // coefficients for 3 parabolic splines

} glob_wave_t;

//******************************************************************************
// Request a glob to be sent back.
typedef struct
{
    uint8_t requested_id; // Glob ID that's being requested.

} glob_request_t;

#endif // GLOB_TYPES_H_INCLUDED
