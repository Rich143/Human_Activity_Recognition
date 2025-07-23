#ifndef IMU_TEST_DATA_H
#define IMU_TEST_DATA_H

#define IMU_SIGNAL_LENGTH 1992

extern float unproc_x[IMU_SIGNAL_LENGTH];
extern float unproc_y[IMU_SIGNAL_LENGTH];
extern float unproc_z[IMU_SIGNAL_LENGTH];

extern float proc_x[IMU_SIGNAL_LENGTH];
extern float proc_y[IMU_SIGNAL_LENGTH];
extern float proc_z[IMU_SIGNAL_LENGTH];

#endif // IMU_TEST_DATA_H
