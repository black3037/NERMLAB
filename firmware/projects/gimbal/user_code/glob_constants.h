// Define constants used by globs.  Named enums are typedef to guarantee size.

#ifndef GLOB_CONSTANTS_H_INCLUDED
#define GLOB_CONSTANTS_H_INCLUDED

//******************************************************************************
typedef uint8_t glob_operating_state_t;
enum
{
    STATE_STOPPED,
    STATE_INITIALIZING,
    STATE_NORMAL
};

//******************************************************************************
typedef uint8_t glob_modes_t;
enum
{
    MODE_CALIBRATE_COMMUTATION,
    MODE_OPEN_LOOP,
    MODE_SPEED_CONTROL,
    MODE_POSITION_CONTROL,
    MODE_CUSTOM,

    NUM_MAIN_MODES
};

//******************************************************************************
typedef uint8_t glob_wave_type_t;
enum
{
    WAVE_SINE,
    WAVE_SQUARE,
    WAVE_TRIANGLE,
    WAVE_TRAPEZOIDAL,
    WAVE_CONSTANT
};

//******************************************************************************
typedef uint8_t glob_wave_state_t;
enum
{
    WAVE_STOPPED,      // Either no valid wave or wave is finished running.
    WAVE_READY_TO_RUN, // New wave received, but isn't running yet.
    WAVE_STARTING_UP,  // Brief time before wave actually starts where wave value is a constant 0.
    WAVE_STARTED,      // Wave is running normally.
};

//******************************************************************************
typedef uint8_t glob_pid_id_t;
enum
{
    PID_ID_POSITION_CONTROLLER,
    PID_ID_SPEED_CONTROLLER,
    PID_ID_CURRENT_CONTROLLER,

    NUM_PID_CONTROLLERS
};

#endif // GLOB_CONSTANTS_H_INCLUDED
