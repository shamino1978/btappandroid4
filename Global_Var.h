#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H



#include <fstream>
#include <sstream>
#include <vector>

extern char* data_array_bluetooth;

extern uint8_t cpuident_bluetooth ; // 0 for intel 1 for amd
extern  uint8_t model_bluetooth; // 0 for adl/rpl, 0 for vmr, 1 for raph
extern     uint8_t numrealcores_bluetooth; // full cores for amd
extern     uint8_t numatom_bluetooth ;
 extern  uint8_t numccx_bluetooth ;
   extern  uint8_t htt_bluetooth;
    extern uint8_t element_type_bluetooth ;
    extern uint8_t rows_bluetooth ;
  extern   uint8_t cols_bluetooth ;

    using namespace std;

    template <class T>
    void alignit(std::stringstream& ss, T val) {

        if (val >= 10000) {
            ss << showpoint << fixed << setprecision(0);
        }
        else if (val >= 1000) {
            ss << showpoint << fixed << setprecision(1);
        }
        else if (val >= 100) {
            ss << showpoint << fixed << setprecision(2);
        }
        else if (val >= 10) {
            ss << showpoint << fixed << setprecision(3);
        }

        else {
            ss << showpoint << fixed << setprecision(4);
        }


    }






#endif // GLOBAL_VAR_H
