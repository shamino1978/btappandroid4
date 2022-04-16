#ifndef SAMPLINGTHREAD_H
#define SAMPLINGTHREAD_H

#include <vector>
#include <QMutex>
#include <QThread>
#include <QElapsedTimer>
#include <QTimer>

using namespace std;

typedef uint16_t WORD;

extern QMutex osc_mutex;

extern QElapsedTimer d_clock;
extern volatile long int data_index_counter;
extern volatile long int data_index_counter2;
extern volatile long int data_index_counter3;

extern std::vector<double> osc_data_vec1;
extern std::vector<double> osc_data_vec2;
extern std::vector<double> osc_data_vec3;

extern volatile WORD* osc_data_vec1b;
extern volatile WORD* osc_data_vec2b;
extern volatile WORD* osc_data_vec3b;
extern volatile uint32_t osc_data_vec1b_size;
extern volatile uint32_t osc_data_vec2b_size;
extern volatile uint32_t osc_data_vec3b_size;

extern std::vector<double> osc_trigger_vec1;
extern std::vector<double> osc_trigger_vec2;
extern std::vector<double> osc_trigger_vec3;

extern volatile WORD* osc_trigger_vec1b;
extern volatile WORD* osc_trigger_vec2b;
extern volatile WORD* osc_trigger_vec3b;
extern volatile uint32_t osc_trigger_vec1b_size;
extern volatile uint32_t osc_trigger_vec2b_size;
extern volatile uint32_t osc_trigger_vec3b_size;

extern volatile bool triggered;
extern volatile bool trigger;
extern volatile bool runit;
extern volatile bool trigger_fall;
extern volatile bool trigger_rise;

extern volatile int trigger_voltage;
extern volatile int trigger_channel;
extern volatile int trigger_record_times;

extern volatile double osc_lower_thresh_hit;
extern volatile double osc_upper_thresh_hit;
extern volatile double osc_abs_lowest;
extern volatile double osc_abs_highest;
extern volatile long int osc_gettimes;

extern volatile double osc_lower_thresh;
extern volatile double osc_upper_thresh;

extern volatile double osc_lower_per;
extern volatile double osc_upper_per;

extern int triggered_index;

extern volatile bool recordthis;
extern vector<vector<double>> ch1_recordvec;
extern vector<vector<double>> ch2_recordvec;
extern vector<vector<double>> ch3_recordvec;


#endif
