#include "samplingthread.h"
#include "signaldata.h"

#include <math.h>
#include <QMessageBox>
#include <QString>
#include <QMutex>
#include "plot.h"


using namespace std;

QMutex osc_mutex;
QElapsedTimer d_clock;
vector<double> osc_data_vec1;
std::vector<double> osc_data_vec2;
std::vector<double> osc_data_vec3;

volatile WORD *osc_data_vec1b;
volatile WORD *osc_data_vec2b;
volatile WORD *osc_data_vec3b;
volatile uint32_t osc_data_vec1b_size=0;
volatile uint32_t osc_data_vec2b_size=0;
volatile uint32_t osc_data_vec3b_size=0;



std::vector<double> osc_trigger_vec1;
std::vector<double> osc_trigger_vec2;
std::vector<double> osc_trigger_vec3;

volatile WORD *osc_trigger_vec1b;
volatile WORD *osc_trigger_vec2b;
volatile  WORD *osc_trigger_vec3b;
volatile  uint32_t osc_trigger_vec1b_size=0;
volatile  uint32_t osc_trigger_vec2b_size=0;
volatile  uint32_t osc_trigger_vec3b_size=0;

volatile bool triggered;
volatile bool trigger;
volatile bool runit;
volatile bool trigger_fall;
volatile bool trigger_rise;

volatile int trigger_channel{1};
volatile int trigger_record_times{0};

volatile long int data_index_counter=0;
volatile long int data_index_counter2=0;
volatile long int data_index_counter3=0;

int triggered_index{0};


volatile bool recordthis{false};
vector<vector<double>> ch1_recordvec;
vector<vector<double>> ch2_recordvec;
vector<vector<double>> ch3_recordvec;
