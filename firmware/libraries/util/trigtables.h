#ifndef __TRIGTABLES_H
#define __TRIGTABLES_H

    // Exported functions 

    #ifdef __cplusplus
        extern "C"
        {
            float atan2_table(float y,float x);
            void sincos_table(float theta, float *sth, float *cth);
        }
    #else
        extern float atan2_table(float y,float x);
        extern void sincos_table(float theta, float *sth, float *cth);
    #endif


#endif