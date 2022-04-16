/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtBluetooth module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "chat.h"
//#include "remoteselector.h"

#include "chatclient.h"
#include "chatserver.h"

#include <QtCore/qdebug.h>

#include <QtBluetooth/qbluetoothdeviceinfo.h>
#include <QtBluetooth/qbluetoothlocaldevice.h>
#include <QtBluetooth/qbluetoothuuid.h>

#ifdef Q_OS_ANDROID
#include <QtAndroidExtras/QtAndroid>
#endif

//static const QLatin1String serviceUuid("e8e10f95-1a70-4b27-9ccf-02010264e9c8");
#ifdef Q_OS_ANDROID
//static const QLatin1String reverseUuid("c8e96402-0102-cf9c-274b-701a950fe1e8");
#endif

#include "Global_Var.h"


#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

#include <QMessageBox>
#include <QAbstractItemView>

#include "samplingthread.h"

char* data_array_bluetooth=NULL;

uint8_t cpuident_bluetooth = 0; // 0 for intel 1 for amd
uint8_t model_bluetooth = 0; // 0 for adl/rpl, 0 for vmr, 1 for raph
 uint8_t numrealcores_bluetooth = 0; // full cores for amd
 uint8_t numatom_bluetooth = 0;
 uint8_t numccx_bluetooth = 0;
   uint8_t htt_bluetooth = 0;
   uint8_t element_type_bluetooth = 0;
  uint8_t rows_bluetooth = 0;
 uint8_t cols_bluetooth = 0;

    using namespace std;


 void aligncore(stringstream &ss, int num){
     int justsize=13;
     if(num>9)
         justsize=12;
     if(num>99)
         justsize=11;
     ss <<  left << setw(justsize)<<num;
 }

 void alignmid(stringstream &ss,int num){
     int justsize=14;

     if(num>9)
         justsize=13;
     if(num>99)
         justsize=12;
     if(num>999)
         justsize=11;
     if(num>9999)
         justsize=10;
     if(num>99999)
         justsize=9;

     ss << right << setw(justsize) <<num<< setw(justsize);
 }
 void alignend(stringstream &ss,int num){
     int justsize=14;

     if(num>9)
         justsize=13;
     if(num>99)
         justsize=12;
     if(num>999)
         justsize=11;
     if(num>9999)
         justsize=10;
     if(num>99999)
         justsize=9;

     ss << right << setw(justsize) <<num;
 }




#pragma pack(push)
#pragma pack(1)
struct BT_DATA_CMD_STRUCT {
    uint8_t code = 77;
    uint8_t page = 0; //page menu
    uint8_t cmd = 0; //
    uint8_t type = 0;
    uint8_t rows = 0;
    uint8_t cols = 0;
};
#pragma pack(pop)


    #pragma pack(push)
    #pragma pack(1)
    struct BT_DATA_ANDROID_STRUCT {
        uint8_t pass = 123;
        uint8_t cpuident = 0; // 0 for intel 1 for amd
        uint8_t model = 0; // 0 for adl/rpl, 0 for vmr, 1 for raph
        uint8_t numrealcores = 0; // full cores for amd
        uint8_t numatom = 0;
        uint8_t numccx = 0;
        uint8_t htt = 0;
        uint8_t type = 0;
        uint8_t rows = 0;
        uint8_t cols = 0;
           // freq
            //vid
            //temp
    };
    #pragma pack(pop)

    class BlueToothAP_Android_Cmd{
    public:
        struct BT_DATA_CMD_STRUCT bts;
        uint8_t sizeoftype = 1;
        int* int_ptr=NULL;
        float* float_ptr = NULL;
        double* double_ptr = NULL;
        long double* long_double_ptr = NULL;
        char* char_ptr = NULL;

        int MAX_LEN = 50000;
        int data_index = 0;
        vector<vector<int>> intvec2d;
        vector<int> intvec1d;
        vector<vector<double>> doubvec2d;
        vector<double> doubvec1d;
        char* data1 = NULL;
        vector<vector<long double>> longdoubvec2d;
        vector<long double> longdoubvec1d;

        vector<vector<float>> floatvec2d;
        vector<float> floatvec1d;

        vector<vector<char>> charvec2d;
        vector<char> charvec1d;

        template <class T>
        BlueToothAP_Android_Cmd(vector<T>& data) {
            int t1;
            double t2;
            float t3;
            long double t4;

            bts.rows = 1;
            bts.cols = data.size();
            data_index = 0;
            if (typeid(t1) == typeid(T)) {
                bts.type = 0;
                sizeoftype = sizeof(int);
                int_ptr = new int[bts.rows * bts.cols];
                for (int i = 0; i < data.size(); i++) {
                    int_ptr[i] = data.at(i);
                    data_index++;
                }
            }
            else if (typeid(t2) == typeid(T)) {
                bts.type = 1;
                sizeoftype = sizeof(double);
                double_ptr = new double[bts.rows * bts.cols];
                for (int i = 0; i < data.size(); i++) {
                    double_ptr[i] = data.at(i);
                    data_index++;
                }
            }
            else if (typeid(t3) == typeid(T)) {
                bts.type = 2;
                sizeoftype = sizeof(float);
                float_ptr = new float[bts.rows * bts.cols];
                for (int i = 0; i < data.size(); i++) {
                    float_ptr[i] = data.at(i);
                    data_index++;
                }
            }
            else if (typeid(t4) == typeid(T)) {
                bts.type = 3;
                sizeoftype = sizeof(long double);
                long_double_ptr = new long double[bts.rows * bts.cols];
                for (int i = 0; i < data.size(); i++) {
                    long_double_ptr[i] = data.at(i);
                    data_index++;
                }
            }
            else {
                bts.type = 4;
                sizeoftype = sizeof(char);
                char_ptr = new char[bts.rows * bts.cols];
                for (int i = 0; i < data.size(); i++) {
                    char_ptr[i] = data.at(i);
                    data_index++;
                }
            }

        };

        template <class T>
        BlueToothAP_Android_Cmd(vector<vector<T>>& data) {
            int t1;
            double t2;
            float t3;
            long double t4;
            bts.rows = data.size();
            if (data.size() > 0)
                bts.cols = data.at(0).size();
            data_index = 0;
            if (typeid(t1) == typeid(T)) {
                bts.type = 0;
                sizeoftype = sizeof(int);
                int_ptr = new int[bts.rows * bts.cols];
                for (int i = 0; i < bts.rows; i++) {
                    try {
                        for (int j = 0; j < bts.cols; j++) {
                            int_ptr[data_index] = data.at(i).at(j);
                            data_index++;

                        }
                    }
                    catch (...) {

                    }

                }
            }
            else if (typeid(t2) == typeid(T)) {
                bts.type = 1;
                sizeoftype = sizeof(double);
                double_ptr = new double[bts.rows * bts.cols];
                for (int i = 0; i < bts.rows; i++) {
                    try {
                        for (int j = 0; j < bts.cols; j++) {
                            double_ptr[data_index] = data.at(i).at(j);
                            data_index++;

                        }
                    }
                    catch (...) {

                    }

                }
            }
            else if (typeid(t3) == typeid(T)) {
                bts.type = 2;
                sizeoftype = sizeof(float);
                float_ptr = new float[bts.rows * bts.cols];
                for (int i = 0; i < bts.rows; i++) {
                    try {
                        for (int j = 0; j < bts.cols; j++) {
                            float_ptr[data_index] = data.at(i).at(j);
                            data_index++;

                        }
                    }
                    catch (...) {

                    }

                }
            }
            else if (typeid(t4) == typeid(T)) {
                bts.type = 3;
                sizeoftype = sizeof(long double);
                long_double_ptr = new long double[bts.rows * bts.cols];
                for (int i = 0; i < bts.rows; i++) {
                    try {
                        for (int j = 0; j < bts.cols; j++) {
                            long_double_ptr[data_index] = data.at(i).at(j);
                            data_index++;

                        }
                    }
                    catch (...) {

                    }

                }
            }
            else {
                bts.type = 4;
                sizeoftype = sizeof(char);
                char_ptr = new char[bts.rows * bts.cols];
                for (int i = 0; i < bts.rows; i++) {
                    try {
                        for (int j = 0; j < bts.cols; j++) {
                            char_ptr[data_index] = data.at(i).at(j);
                            data_index++;

                        }
                    }
                    catch (...) {

                    }

                }
            }
         };

        vector<char> get_data() {
            struct BT_DATA_CMD_STRUCT* ptr;
            ptr = &bts;
            char* st_ptr;
            st_ptr = (char*)ptr;
            vector<char> retvec;
            for (int i = 0; i < sizeof(struct BT_DATA_CMD_STRUCT); i++)
            {
                retvec.push_back((char)st_ptr[i]);
            }
            char* st_ptr2;
            if (bts.type == 0) {
                st_ptr2 = (char*)int_ptr;
            }
            else if (bts.type == 1) {
                st_ptr2 = (char*)double_ptr;
            }
            else if (bts.type == 2) {
                st_ptr2 = (char*)float_ptr;
            }
            else if (bts.type == 3) {
                st_ptr2 = (char*)long_double_ptr;
            }
            else {
                st_ptr2 = (char*)char_ptr;
            }

            for (int i = 0; i < (data_index * sizeoftype); i++)
            {
                retvec.push_back((char)st_ptr2[i]);

            }
            return retvec;
        }

        BlueToothAP_Android_Cmd(char* data)
        {
            if (1) {

                bts.code = data[0];
                bts.page = data[1];
                bts.cmd = data[2];
                bts.type = data[3];
                bts.rows = data[4];
                bts.cols = data[5];

                size_t typeoffset = offsetof(BT_DATA_CMD_STRUCT, type);
                bts.type = data[typeoffset];
                switch (bts.type) {
                case 0:
                    sizeoftype = sizeof(int);
                    break;
                case 1:
                    sizeoftype = sizeof(double);
                    break;
                case 2:
                    sizeoftype = sizeof(float);
                    break;
                case 3:
                    sizeoftype = sizeof(long double);
                    break;
                case 4:
                    sizeoftype = sizeof(char);
                    break;
                default:
                    sizeoftype = sizeof(char);
                    break;
                }

                uint64_t* rptr = (uint64_t*)(&data[offsetof(BT_DATA_CMD_STRUCT, rows)]);

                bts.rows = *rptr;
                rptr = (uint64_t*)(&data[offsetof(BT_DATA_CMD_STRUCT, cols)]);
                bts.cols = *rptr;
                int start_index = sizeof(struct BT_DATA_CMD_STRUCT);
                data1=&data[start_index];
                start_index = 0;
                if (bts.type == 0) {
                    if (bts.rows > 1) {

                        for (int i = 0; i < bts.rows; i++) {
                            vector<int> tempvec;
                            for (int j = 0; j < bts.cols; j++) {
                                int* ptr = (int*)(&data1[start_index]);

                                tempvec.push_back(*ptr);
                                start_index += sizeof(int);
                            }
                            intvec2d.push_back(tempvec);
                        }

                    }
                    else {

                        for (int j = 0; j < bts.cols; j++) {
                            int* ptr = (int*)(&data1[start_index]);

                            intvec1d.push_back(*ptr);
                            start_index += sizeof(int);
                        }

                    }
                }
                else if (bts.type == 1) {
                    if (bts.rows > 1) {

                        for (int i = 0; i < bts.rows; i++) {
                            vector<double> tempvec;
                            for (int j = 0; j < bts.cols; j++) {
                                double* val = (double*)(&data1[start_index]);
                                tempvec.push_back(*val);
                                start_index += sizeof(double);

                            }
                            doubvec2d.push_back(tempvec);

                        }

                    }
                    else {

                        for (int j = 0; j < bts.cols; j++) {
                            double* val = (double*)(&data1[start_index]);

                            doubvec1d.push_back(*val);
                            start_index += sizeof(double);
                        }

                    }
                }

                else if (bts.type == 2) {
                    if (bts.rows > 1) {

                        for (int i = 0; i < bts.rows; i++) {
                            vector<float> tempvec;
                            for (int j = 0; j < bts.cols; j++) {
                                float* val = (float*)(&data1[start_index]);

                                tempvec.push_back(*val);
                                start_index += sizeof(float);
                            }
                            floatvec2d.push_back(tempvec);
                        }

                    }
                    else {

                        for (int j = 0; j < bts.cols; j++) {
                            float* val = (float*)(&data1[start_index]);

                            floatvec1d.push_back(*val);
                            start_index += sizeof(float);
                        }

                    }
                }
                else if (bts.type == 3) {
                    if (bts.rows > 1) {

                        for (int i = 0; i < bts.rows; i++) {
                            vector<long double> tempvec;
                            for (int j = 0; j < bts.cols; j++) {
                                long double* val = (long double*)(&data1[start_index]);

                                tempvec.push_back(*val);
                                start_index += sizeof(long double);
                            }
                            longdoubvec2d.push_back(tempvec);
                        }

                    }
                    else {

                        for (int j = 0; j < bts.cols; j++) {
                            long double* val = (long double*)(&data1[start_index]);

                            longdoubvec1d.push_back(*val);
                            start_index += sizeof(long double);
                        }

                    }
                }
                else {
                    if (bts.rows > 1) {

                        for (int i = 0; i < bts.rows; i++) {
                            vector<char> tempvec;
                            for (int j = 0; j < bts.cols; j++) {
                                tempvec.push_back((char)data1[start_index]);
                                start_index += sizeof(char);
                            }
                            charvec2d.push_back(tempvec);
                        }

                    }
                    else {

                        for (int j = 0; j < bts.cols; j++) {
                            charvec1d.push_back((char)data1[start_index]);
                            start_index += sizeof(char);
                        }

                    }
                }
            }

        }

        ~BlueToothAP_Android_Cmd() {

            delete int_ptr;
            delete float_ptr;
            delete double_ptr;
            delete long_double_ptr;
            delete char_ptr;
        };

    };



    class BlueToothAP_Android {
    public:
        struct BT_DATA_ANDROID_STRUCT bts;
        uint8_t sizeoftype = 1;
        int* int_ptr=NULL;
        float* float_ptr = NULL;
        double* double_ptr = NULL;
        long double* long_double_ptr = NULL;
        char* char_ptr = NULL;

        int MAX_LEN = 50000;
        int data_index = 0;
        vector<vector<int>> intvec2d;
        vector<int> intvec1d;
        vector<vector<double>> doubvec2d;
        vector<double> doubvec1d;
        char* data1 = NULL;
        vector<vector<long double>> longdoubvec2d;
        vector<long double> longdoubvec1d;

        vector<vector<float>> floatvec2d;
        vector<float> floatvec1d;

        vector<vector<char>> charvec2d;
        vector<char> charvec1d;

        template <class T>
        BlueToothAP_Android(vector<T>& data) {
            int t1;
            double t2;
            float t3;
            long double t4;

            bts.rows = 1;
            bts.cols = data.size();
            data_index = 0;
            if (typeid(t1) == typeid(T)) {
                bts.type = 0;
                sizeoftype = sizeof(int);
                int_ptr = new int[bts.rows * bts.cols];
                for (int i = 0; i < data.size(); i++) {
                    int_ptr[i] = data.at(i);
                    data_index++;
                }
            }
            else if (typeid(t2) == typeid(T)) {
                bts.type = 1;
                sizeoftype = sizeof(double);
                double_ptr = new double[bts.rows * bts.cols];
                for (int i = 0; i < data.size(); i++) {
                    double_ptr[i] = data.at(i);
                    data_index++;
                }
            }
            else if (typeid(t3) == typeid(T)) {
                bts.type = 2;
                sizeoftype = sizeof(float);
                float_ptr = new float[bts.rows * bts.cols];
                for (int i = 0; i < data.size(); i++) {
                    float_ptr[i] = data.at(i);
                    data_index++;
                }
            }
            else if (typeid(t4) == typeid(T)) {
                bts.type = 3;
                sizeoftype = sizeof(long double);
                long_double_ptr = new long double[bts.rows * bts.cols];
                for (int i = 0; i < data.size(); i++) {
                    long_double_ptr[i] = data.at(i);
                    data_index++;
                }
            }
            else {
                bts.type = 4;
                sizeoftype = sizeof(char);
                char_ptr = new char[bts.rows * bts.cols];
                for (int i = 0; i < data.size(); i++) {
                    char_ptr[i] = data.at(i);
                    data_index++;
                }
            }

        };

        template <class T>
        BlueToothAP_Android(vector<vector<T>>& data) {
            int t1;
            double t2;
            float t3;
            long double t4;
            bts.rows = data.size();
            if (data.size() > 0)
                bts.cols = data.at(0).size();
            data_index = 0;
            if (typeid(t1) == typeid(T)) {
                bts.type = 0;
                sizeoftype = sizeof(int);
                int_ptr = new int[bts.rows * bts.cols];
                for (int i = 0; i < bts.rows; i++) {
                    try {
                        for (int j = 0; j < bts.cols; j++) {
                            int_ptr[data_index] = data.at(i).at(j);
                            data_index++;

                        }
                    }
                    catch (...) {

                    }

                }
            }
            else if (typeid(t2) == typeid(T)) {
                bts.type = 1;
                sizeoftype = sizeof(double);
                double_ptr = new double[bts.rows * bts.cols];
                for (int i = 0; i < bts.rows; i++) {
                    try {
                        for (int j = 0; j < bts.cols; j++) {
                            double_ptr[data_index] = data.at(i).at(j);
                            data_index++;

                        }
                    }
                    catch (...) {

                    }

                }
            }
            else if (typeid(t3) == typeid(T)) {
                bts.type = 2;
                sizeoftype = sizeof(float);
                float_ptr = new float[bts.rows * bts.cols];
                for (int i = 0; i < bts.rows; i++) {
                    try {
                        for (int j = 0; j < bts.cols; j++) {
                            float_ptr[data_index] = data.at(i).at(j);
                            data_index++;

                        }
                    }
                    catch (...) {

                    }

                }
            }
            else if (typeid(t4) == typeid(T)) {
                bts.type = 3;
                sizeoftype = sizeof(long double);
                long_double_ptr = new long double[bts.rows * bts.cols];
                for (int i = 0; i < bts.rows; i++) {
                    try {
                        for (int j = 0; j < bts.cols; j++) {
                            long_double_ptr[data_index] = data.at(i).at(j);
                            data_index++;

                        }
                    }
                    catch (...) {

                    }

                }
            }
            else {
                bts.type = 4;
                sizeoftype = sizeof(char);
                char_ptr = new char[bts.rows * bts.cols];
                for (int i = 0; i < bts.rows; i++) {
                    try {
                        for (int j = 0; j < bts.cols; j++) {
                            char_ptr[data_index] = data.at(i).at(j);
                            data_index++;

                        }
                    }
                    catch (...) {

                    }

                }
            }
         };

        vector<char> get_data() {
            struct BT_DATA_ANDROID_STRUCT* ptr;
            ptr = &bts;
            char* st_ptr;
            st_ptr = (char*)ptr;
            vector<char> retvec;
            for (int i = 0; i < sizeof(struct BT_DATA_ANDROID_STRUCT); i++)
            {
                retvec.push_back((char)st_ptr[i]);
            }
            char* st_ptr2;
            if (bts.type == 0) {
                st_ptr2 = (char*)int_ptr;
            }
            else if (bts.type == 1) {
                st_ptr2 = (char*)double_ptr;
            }
            else if (bts.type == 2) {
                st_ptr2 = (char*)float_ptr;
            }
            else if (bts.type == 3) {
                st_ptr2 = (char*)long_double_ptr;
            }
            else {
                st_ptr2 = (char*)char_ptr;
            }

            for (int i = 0; i < (data_index * sizeoftype); i++)
            {
                retvec.push_back((char)st_ptr2[i]);

            }
            return retvec;
        }

        BlueToothAP_Android(char* data)
        {
            if (1) {

                bts.pass = data[0];
                bts.cpuident = data[1];
                bts.model = data[2];
                bts.numrealcores = data[3];
                bts.numatom = data[4];
                bts.numccx = data[5];
                bts.htt = data[6];
                bts.type = data[7];

                size_t typeoffset = offsetof(BT_DATA_ANDROID_STRUCT, type);
                bts.type = data[typeoffset];
                switch (bts.type) {
                case 0:
                    sizeoftype = sizeof(int);
                    break;
                case 1:
                    sizeoftype = sizeof(double);
                    break;
                case 2:
                    sizeoftype = sizeof(float);
                    break;
                case 3:
                    sizeoftype = sizeof(long double);
                    break;
                case 4:
                    sizeoftype = sizeof(char);
                    break;
                default:
                    sizeoftype = sizeof(char);
                    break;
                }

                uint64_t* rptr = (uint64_t*)(&data[offsetof(BT_DATA_ANDROID_STRUCT, rows)]);

                bts.rows = *rptr;
                rptr = (uint64_t*)(&data[offsetof(BT_DATA_ANDROID_STRUCT, cols)]);
                bts.cols = *rptr;
                int start_index = sizeof(struct BT_DATA_ANDROID_STRUCT);
                data1=&data[start_index];
                start_index = 0;
                if (bts.type == 0) {
                    if (bts.rows > 1) {

                        for (int i = 0; i < bts.rows; i++) {
                            vector<int> tempvec;
                            for (int j = 0; j < bts.cols; j++) {
                                int* ptr = (int*)(&data1[start_index]);

                                tempvec.push_back(*ptr);
                                start_index += sizeof(int);
                            }
                            intvec2d.push_back(tempvec);
                        }

                    }
                    else {

                        for (int j = 0; j < bts.cols; j++) {
                            int* ptr = (int*)(&data1[start_index]);

                            intvec1d.push_back(*ptr);
                            start_index += sizeof(int);
                        }

                    }
                }
                else if (bts.type == 1) {
                    if (bts.rows > 1) {

                        for (int i = 0; i < bts.rows; i++) {
                            vector<double> tempvec;
                            for (int j = 0; j < bts.cols; j++) {
                                double* val = (double*)(&data1[start_index]);
                                tempvec.push_back(*val);
                                start_index += sizeof(double);

                            }
                            doubvec2d.push_back(tempvec);

                        }

                    }
                    else {

                        for (int j = 0; j < bts.cols; j++) {
                            double* val = (double*)(&data1[start_index]);

                            doubvec1d.push_back(*val);
                            start_index += sizeof(double);
                        }

                    }
                }

                else if (bts.type == 2) {
                    if (bts.rows > 1) {

                        for (int i = 0; i < bts.rows; i++) {
                            vector<float> tempvec;
                            for (int j = 0; j < bts.cols; j++) {
                                float* val = (float*)(&data1[start_index]);

                                tempvec.push_back(*val);
                                start_index += sizeof(float);
                            }
                            floatvec2d.push_back(tempvec);
                        }

                    }
                    else {

                        for (int j = 0; j < bts.cols; j++) {
                            float* val = (float*)(&data1[start_index]);

                            floatvec1d.push_back(*val);
                            start_index += sizeof(float);
                        }

                    }
                }
                else if (bts.type == 3) {
                    if (bts.rows > 1) {

                        for (int i = 0; i < bts.rows; i++) {
                            vector<long double> tempvec;
                            for (int j = 0; j < bts.cols; j++) {
                                long double* val = (long double*)(&data1[start_index]);

                                tempvec.push_back(*val);
                                start_index += sizeof(long double);
                            }
                            longdoubvec2d.push_back(tempvec);
                        }

                    }
                    else {

                        for (int j = 0; j < bts.cols; j++) {
                            long double* val = (long double*)(&data1[start_index]);

                            longdoubvec1d.push_back(*val);
                            start_index += sizeof(long double);
                        }

                    }
                }
                else {
                    if (bts.rows > 1) {

                        for (int i = 0; i < bts.rows; i++) {
                            vector<char> tempvec;
                            for (int j = 0; j < bts.cols; j++) {
                                tempvec.push_back((char)data1[start_index]);
                                start_index += sizeof(char);
                            }
                            charvec2d.push_back(tempvec);
                        }

                    }
                    else {

                        for (int j = 0; j < bts.cols; j++) {
                            charvec1d.push_back((char)data1[start_index]);
                            start_index += sizeof(char);
                        }

                    }
                }
            }

        }

        ~BlueToothAP_Android() {

            delete int_ptr;
            delete float_ptr;
            delete double_ptr;
            delete long_double_ptr;
            delete char_ptr;
        };

    };


    intel1::intel1( QWidget *parent)
        : QWidget(parent)
    {
        QSpacerItem* seperator=new QSpacerItem(30,100);

       vrm_voltage_label = new QLabel(tr("VRM Voltage"));
       vrm_loadline_label = new QLabel(tr("VRM Loadline"));

       vrm_voltage_lineedit = new QLineEdit();
       vrm_loadline_lineedit =new QLineEdit();
       apply_vrm_button=new  QPushButton("Apply");
      vbox1=new QVBoxLayout;
       vbox2=new QVBoxLayout;
       hbox1=new QHBoxLayout;
        spacer1=new QSpacerItem(800,20);

        line1 = new QFrame();
         line1->setFrameShape(QFrame::HLine);
        line1->setFrameShadow(QFrame::Sunken);
      vbox1->addWidget(vrm_voltage_label);
        vbox1->addWidget(vrm_voltage_lineedit);

        vbox2->addWidget(vrm_loadline_label);
        vbox2->addWidget(vrm_loadline_lineedit);

        hbox1->addItem(vbox1);
        hbox1->addItem(seperator);
        hbox1->addItem(vbox2);
         hbox1->addItem(seperator);
        hbox1->addWidget(apply_vrm_button);

        QPalette pal = apply_vrm_button->palette();
        pal.setColor(QPalette::Button, QColor(Qt::lightGray));
        apply_vrm_button->setAutoFillBackground(true);
        apply_vrm_button->setPalette(pal);
        apply_vrm_button->update();



         vbox3=new QVBoxLayout;
        vbox3->addItem(hbox1);
       vbox3->addItem(spacer1);

          vbox3->addWidget(line1);//firstbox


     spacer2=new QSpacerItem(800,10);
   svid_voltage_label = new QLabel(tr("SVID Voltage"));
  domain_label = new QLabel(tr("Domain"));
  svid_voltage_lineedit =  new QLineEdit();
  domain_menu= new QComboBox();
  vbox4=new QVBoxLayout;
  vbox4->addWidget(svid_voltage_label);
vbox4->addWidget(svid_voltage_lineedit);
vbox4->addWidget(domain_label);
vbox4->addWidget(domain_menu);



  radiogroup1 =  new QButtonGroup;
  adaptive_radio=  new QRadioButton("Adaptive", this);
  manual_radio= new QRadioButton("Manual", this);
  offset_radio= new QRadioButton("Offset", this);
  radiogroup1 ->addButton(adaptive_radio);
   radiogroup1 ->addButton(manual_radio);
    radiogroup1 ->addButton(offset_radio);

  vbox5=new QVBoxLayout;
  vbox5->addWidget(adaptive_radio);
vbox5->addWidget(manual_radio);
vbox5->addWidget(offset_radio);

  apply_svid_button=new  QPushButton("Apply Voltage");
  maxout_button=new  QPushButton("Max Limits");
  apply_svid_button->setAutoFillBackground(true);
  apply_svid_button->setPalette(pal);
  apply_svid_button->update();

  maxout_button->setAutoFillBackground(true);
  maxout_button->setPalette(pal);
  maxout_button->update();

   vbox6=new QVBoxLayout;
vbox6->addWidget(apply_svid_button);
vbox6->addWidget(maxout_button);

      hbox2=new QHBoxLayout;
      spacer3=new QSpacerItem(800,10);

    line2 = new QFrame();
     line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Sunken);

    vbox7=new QVBoxLayout;

hbox2->addItem(vbox4);
QSpacerItem* spacer4=new QSpacerItem(10,100);
hbox2->addItem(spacer4);
hbox2->addItem(vbox5);
hbox2->addItem(spacer4);
hbox2->addItem(vbox6);
//hbox2->addItem(spacer2);
//hbox2->addWidget(line2);

//

ratio_limit_label = new QLabel(tr("Ratio\nLimit"));
act_cores_label = new QLabel(tr("Act\nCores"));
atom_ratio_label = new QLabel(tr("Atom\nRatio"));
act_atom_label = new QLabel(tr("Act\nAtom"));
QFont f( "Arial", 8);
ratio_limit_label->setFont( f);
act_cores_label->setFont( f);
atom_ratio_label->setFont( f);
act_atom_label->setFont( f);

for(int i=0;i<4;i++){
    vector<QLineEdit *> v1;
    for(int j=0;j<8;j++){
        QLineEdit* ptr =  new QLineEdit();
        v1.push_back(ptr);
    }
    ratio_limit_vec.push_back(v1);
}

apply_ratios_button=new QPushButton("Apply Ratios");
apply_ratios_button->setAutoFillBackground(true);
apply_ratios_button->setPalette(pal);
apply_ratios_button->update();
vector<QVBoxLayout*> qvboxly;

QVBoxLayout* vbox8=new QVBoxLayout;
vbox8->addWidget(ratio_limit_label);
vbox8->addWidget(act_cores_label);
vbox8->addWidget(atom_ratio_label);
vbox8->addWidget(act_atom_label);
qvboxly.push_back(vbox8);

for(int j=0;j<8;j++){
    QVBoxLayout* vboxer=new QVBoxLayout;
    for(int i=0;i<4;i++){
        vboxer->addWidget(ratio_limit_vec.at(i).at(j));
        if(i!=3){
           QSpacerItem* spacerer=new QSpacerItem(50,5);
           vboxer->addItem(spacerer);
        }
    }

    qvboxly.push_back(vboxer);
}
QHBoxLayout* hbox4=new QHBoxLayout;
for(int i=0;i<qvboxly.size();i++)
{

    hbox4->addItem(qvboxly.at(i));
    if(i!=(qvboxly.size()-1)&& i!=0){
         QSpacerItem* spacerer=new QSpacerItem(5,100);
        hbox4->addItem(spacerer);
    }
}
QVBoxLayout* vbox9=new QVBoxLayout;
vbox9->addItem(hbox4);
vbox9->addWidget(apply_ratios_button);


vbox7->addItem(vbox3);
vbox7->addItem(spacer2);
vbox7->addItem(hbox2);
vbox7->addItem(spacer2);

vbox7->addItem(vbox9);
vbox7->addItem(spacer2);
vbox7->addWidget(line2);
vbox7->addItem(spacer2);

synch_allcores_label = new QLabel(tr("Synch Cores"));
synch_allatom_label = new QLabel(tr("Synch Atom"));
fixed_ring_label = new QLabel(tr("Fixed Ring"));
QVBoxLayout* vbox10=new QVBoxLayout;
vbox10->addWidget(synch_allcores_label );
vbox10->addWidget(synch_allatom_label );
vbox10->addWidget(fixed_ring_label );
synch_allcores_lineedit =  new QLineEdit();
synch_allatom_lineedit =  new QLineEdit();
fixed_ring_lineedit =  new QLineEdit();

apply_core_button=new QPushButton("Apply Core");

apply_ring_button=new QPushButton("Apply Ring");
apply_core_button->setAutoFillBackground(true);
apply_core_button->setPalette(pal);
apply_core_button->update();

apply_ring_button->setAutoFillBackground(true);
apply_ring_button->setPalette(pal);
apply_ring_button->update();


QVBoxLayout* vbox11=new QVBoxLayout;
vbox11->addWidget(synch_allcores_lineedit );
vbox11->addWidget(synch_allatom_lineedit );
vbox11->addWidget(fixed_ring_lineedit );

QHBoxLayout* hbox5=new QHBoxLayout;
hbox5->addItem(vbox10);
hbox5->addItem(vbox11);

QVBoxLayout* vbox12=new QVBoxLayout;
vbox12->addWidget(apply_core_button );
vbox12->addWidget(apply_ring_button );
QHBoxLayout* hbox6=new QHBoxLayout;
hbox6->addItem(vbox12);


bclk_label =  new QLabel(tr("BCLK"));
pcie_label =  new QLabel(tr("PCIE"));

apply_bclk_button=new QPushButton("Apply BCLK");
apply_bclk_button->setAutoFillBackground(true);
apply_bclk_button->setPalette(pal);
apply_bclk_button->update();

bclk_lineedit =  new QLineEdit();
pcie_lineedit =  new QLineEdit();

QHBoxLayout* hbox7=new QHBoxLayout;
hbox7->addWidget(bclk_label);
hbox7->addWidget(bclk_lineedit);
QSpacerItem* spacerer2=new QSpacerItem(30,30);
hbox7->addItem(spacerer2);

QHBoxLayout* hbox8=new QHBoxLayout;
hbox7->addWidget(pcie_label);
hbox7->addWidget(pcie_lineedit);

QVBoxLayout* vbox13=new QVBoxLayout;
vbox13->addItem(hbox7);
vbox13->addWidget(apply_bclk_button);
vbox13->addItem(hbox8);

QHBoxLayout* hbox9=new QHBoxLayout;
hbox9->addItem(hbox5);
 QSpacerItem* spacerer=new QSpacerItem(5,100);
 hbox9->addItem(spacerer);
hbox9->addItem(hbox6);

vbox7->addItem(hbox9);
vbox7->addItem(spacer2);
vbox7->addWidget(line2);
vbox7->addItem(spacer2);

//vbox7->addItem(vbox13);
vbox7->addItem(hbox7);
vbox7->addItem(spacer2);
vbox7->addWidget(apply_bclk_button);
        vbox7->addStretch(1);
        setLayout(vbox7);
       domain_menu->addItem("0 IA Core");
       domain_menu->addItem("1 GT");
       domain_menu->addItem("2 Ring");
       domain_menu->addItem("3 GT Media");
      domain_menu->addItem("4 System Agent");
      domain_menu->addItem("5 L2 Atom");
      adaptive_radio->setChecked(true);

connect(this->apply_vrm_button, &QPushButton::clicked, this, &intel1::apply_vrm_button_pressed);
connect(this->apply_svid_button, &QPushButton::clicked, this, &intel1::apply_svid_button_pressed);
connect(this->maxout_button, &QPushButton::clicked, this, &intel1::maxout_button_pressed);
connect(this->apply_ratios_button, &QPushButton::clicked, this, &intel1::apply_ratios_button_pressed);
connect(this->apply_core_button, &QPushButton::clicked, this, &intel1::apply_core_button_pressed);
connect(this->apply_ring_button, &QPushButton::clicked, this, &intel1::apply_ring_button_pressed);
connect(this->apply_bclk_button, &QPushButton::clicked, this, &intel1::apply_bclk_button_pressed);

    }

 void   intel1:: apply_vrm_button_pressed()
 {
     vector<int> vrm;
     if(!vrm_voltage_lineedit->text().isEmpty()){
         vrm.push_back(vrm_voltage_lineedit->text().toUInt());
     }else{
          vrm.push_back(0);
     }

     if(!vrm_loadline_lineedit->text().isEmpty()){
         vrm.push_back(vrm_loadline_lineedit->text().toUInt());
     }else{
          vrm.push_back(0);
     }


     BlueToothAP_Android_Cmd cmd(vrm);
     cmd.bts.code=88; //88=wr
     cmd.bts.page=1;
     cmd.bts.cmd=0;
     vector<char> to_send=cmd.get_data();
     to_send.push_back(123);
emit  send_cmd(to_send);
     QMessageBox msgbox;
     msgbox.setBaseSize(QSize(600, 600));
     msgbox.information(this, "Write","Applied!");
 }

 void  intel1::apply_svid_button_pressed(){
     if(!svid_voltage_lineedit->text().isEmpty()){
         vector<int> vrm;
         int voltage_type=0;

         if(manual_radio->isChecked()){
             voltage_type=1;
         }else if(offset_radio->isChecked()){
             voltage_type=2;
         }
         vrm.push_back(voltage_type);
         int domain=domain_menu->currentIndex();
         vrm.push_back(domain);
         vrm.push_back(svid_voltage_lineedit->text().toInt());

         BlueToothAP_Android_Cmd cmd(vrm);
         cmd.bts.code=88; //88=wr
         cmd.bts.page=1;
         cmd.bts.cmd=1;
         vector<char> to_send=cmd.get_data();
         to_send.push_back(123);
    emit  send_cmd(to_send);
         QMessageBox msgbox;
         msgbox.setBaseSize(QSize(600, 600));
         msgbox.information(this, "Write","Applied!");
     }else{
         QMessageBox msgbox;
         msgbox.setBaseSize(QSize(600, 600));
         msgbox.warning(this, "Error","Nothing to Write!");
     }

 }

 void  intel1::maxout_button_pressed(){
     vector<int> vrm;

     vrm.push_back(1);

     BlueToothAP_Android_Cmd cmd(vrm);
     cmd.bts.code=88; //88=wr
     cmd.bts.page=1;
     cmd.bts.cmd=2;
     vector<char> to_send=cmd.get_data();
     to_send.push_back(123);
emit  send_cmd(to_send);
     QMessageBox msgbox;
     msgbox.setBaseSize(QSize(600, 600));
     msgbox.information(this, "Write","Applied!");
 }

 void  intel1::apply_ratios_button_pressed(){
      vector< QLineEdit *> oneadvec= ratio_limit_vec.at(0);
       vector<vector<int>> vrm;

       for(int i=0;i<ratio_limit_vec.size();i++)
       {
           vector<int> vrm1={0,0,0,0,0,0,0,0};
           for(int j=0;j<ratio_limit_vec.at(i).size();j++){
               if(!ratio_limit_vec.at(i).at(j)->text().isEmpty()){
                   vrm1.at(j)=(ratio_limit_vec.at(i).at(j)->text().toUInt());
               }
           }
           vrm.push_back(vrm1);

       }
       BlueToothAP_Android_Cmd cmd(vrm);
       cmd.bts.code=88; //88=wr
       cmd.bts.page=1;
       cmd.bts.cmd=3;
       vector<char> to_send=cmd.get_data();
       to_send.push_back(123);
  emit  send_cmd(to_send);
       QMessageBox msgbox;
       msgbox.setBaseSize(QSize(600, 600));
       msgbox.information(this, "Write","Applied!");

 }
 void  intel1::apply_core_button_pressed(){

    if(synch_allcores_lineedit->text().isEmpty()&&synch_allatom_lineedit->text().isEmpty()){
        QMessageBox msgbox;
        msgbox.setBaseSize(QSize(600, 600));
        msgbox.warning(this, "Error","Nothing to Write!");
    }else{
        vector<int> vrm={0,0};
        if(!synch_allcores_lineedit->text().isEmpty()){
            vrm.at(0)=synch_allcores_lineedit->text().toUInt();
        }
        if(!synch_allatom_lineedit->text().isEmpty()){
            vrm.at(1)=synch_allatom_lineedit->text().toUInt();
        }
        BlueToothAP_Android_Cmd cmd(vrm);
        cmd.bts.code=88; //88=wr
        cmd.bts.page=1;
        cmd.bts.cmd=4;
        vector<char> to_send=cmd.get_data();
        to_send.push_back(123);
   emit  send_cmd(to_send);

        QMessageBox msgbox;
        msgbox.setBaseSize(QSize(600, 600));
        msgbox.information(this, "Write","Applied!");
    }
 }
 void  intel1::apply_ring_button_pressed(){

     if(fixed_ring_lineedit->text().isEmpty()){
         QMessageBox msgbox;
         msgbox.setBaseSize(QSize(600, 600));
         msgbox.warning(this, "Error","Nothing to Write!");
     }else{
         vector<int> vrm;
         vrm.push_back(fixed_ring_lineedit->text().toUInt());

         BlueToothAP_Android_Cmd cmd(vrm);
         cmd.bts.code=88; //88=wr
         cmd.bts.page=1;
         cmd.bts.cmd=5;
         vector<char> to_send=cmd.get_data();
         to_send.push_back(123);
    emit  send_cmd(to_send);

         QMessageBox msgbox;
         msgbox.setBaseSize(QSize(600, 600));
         msgbox.information(this, "Write","Applied!");
     }
 }
 void   intel1::apply_bclk_button_pressed(){

     if(bclk_lineedit->text().isEmpty()&&pcie_lineedit->text().isEmpty()){
         QMessageBox msgbox;
         msgbox.setBaseSize(QSize(600, 600));
         msgbox.warning(this, "Error","Nothing to Write!");
     }else{
         vector<double> vrm={0,0};
         if(!bclk_lineedit->text().isEmpty()){
             vrm.at(0)=bclk_lineedit->text().toDouble();
         }
         if(!pcie_lineedit->text().isEmpty()){
             vrm.at(1)=pcie_lineedit->text().toDouble();
         }
         BlueToothAP_Android_Cmd cmd(vrm);
         cmd.bts.code=88; //88=wr
         cmd.bts.page=1;
         cmd.bts.cmd=6;
         vector<char> to_send=cmd.get_data();
         to_send.push_back(123);
    emit  send_cmd(to_send);

         QMessageBox msgbox;
         msgbox.setBaseSize(QSize(600, 600));
         msgbox.information(this, "Write","Applied!");
     }
 }

intel1::~intel1(){


    delete vrm_voltage_label ;
      delete  vrm_loadline_label ;

      delete vrm_voltage_lineedit ;
     delete vrm_loadline_lineedit ;
      delete apply_vrm_button;
     delete  vbox1;
      delete  vbox2;
       delete  hbox1;
       delete  spacer1;
      delete line1;
        delete  vbox3;
    delete spacer2;
     delete  svid_voltage_label ;
    delete domain_label ;
    delete svid_voltage_lineedit ;
    delete domain_menu;
    delete  vbox4;
    delete radiogroup1 ;
    delete adaptive_radio;
    delete manual_radio;
    delete offset_radio;
     delete vbox5;

    delete apply_svid_button;
     delete maxout_button;
      delete  vbox6;

    delete  hbox2;
      delete  spacer3;
     delete line2;
      delete  vbox7;
    delete ratio_limit_label ;
      delete act_cores_label ;
      delete atom_ratio_label ;
       delete act_atom_label ;

    delete apply_ratios_button;

      delete synch_allcores_label ;
     delete synch_allatom_label ;
      delete fixed_ring_label ;
      delete apply_core_button;
    delete apply_ring_button;
    delete bclk_label ;
      delete pcie_label ;
      delete apply_bclk_button;
      delete synch_allcores_lineedit ;
       delete synch_allatom_lineedit ;
       delete fixed_ring_lineedit ;
      delete bclk_lineedit ;
      delete pcie_lineedit ;

}

 intel2::intel2( QWidget *parent)
     : QWidget(parent)
 {

     //scrollArea = new QScrollArea(this);
  // scrollArea->setWidgetResizable(true);
  // scrollArea->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred );
    seperator=new QSpacerItem(20,20);
spco= new QLabel(tr("Specific Cores"));
 ovc= new QLabel(tr("OC Volt"));
ovr_mode= new QLabel(tr("Override\nMode"));
 offsetv= new QLabel(tr("Offset"));
QHBoxLayout* hbox1=new QHBoxLayout;
 hbox1->addWidget(spco,5);
 hbox1->addItem(seperator);
 hbox1->addWidget(ovc,3);
 hbox1->addItem(seperator);
 hbox1->addWidget(ovr_mode,1);
 hbox1->addItem(seperator);
 hbox1->addWidget(offsetv,1);
spacer1=new QSpacerItem(800,10);
//mainvbox=new QVBoxLayout(scrollArea);
mainvbox=new QVBoxLayout;
mainvbox->addLayout(hbox1);
mainvbox->addItem(spacer1);

apply=new QPushButton("Apply");
synchall=new QPushButton("Synch All Override");
bycore=new QCheckBox("+By Core Use");
        QPalette pal =apply->palette();
        pal.setColor(QPalette::Button, QColor(Qt::lightGray));
        apply->setAutoFillBackground(true);
        apply->setPalette(pal);
        apply->update();

        synchall->setAutoFillBackground(true);
        synchall->setPalette(pal);
        synchall->update();
        connect(this->synchall, &QPushButton::clicked, this, &intel2::synch_all);
        connect(this->apply, &QPushButton::clicked, this, &intel2::apply_this);


/*
connect(this->apply_vrm_button, &QPushButton::clicked, this, &intel1::apply_vrm_button_pressed);
connect(this->apply_svid_button, &QPushButton::clicked, this, &intel1::apply_svid_button_pressed);
connect(this->maxout_button, &QPushButton::clicked, this, &intel1::maxout_button_pressed);
connect(this->apply_ratios_button, &QPushButton::clicked, this, &intel1::apply_ratios_button_pressed);
connect(this->apply_core_button, &QPushButton::clicked, this, &intel1::apply_core_button_pressed);
connect(this->apply_ring_button, &QPushButton::clicked, this, &intel1::apply_ring_button_pressed);
connect(this->apply_bclk_button, &QPushButton::clicked, this, &intel1::apply_bclk_button_pressed);
*/
 }

  intel2::~intel2()
  {

   delete seperator;
   delete spacer1;
   delete mainvbox;
   delete scrollArea;
   delete m_scrollArea;
  delete spco;
 delete ovc;
   delete ovr_mode;
   delete offsetv;
   delete apply;
  delete synchall;
   delete bycore;

  }
 void intel2::synch_all(){
     for(int i=0;i<override_vec.size();i++){
         override_vec.at(i)->setChecked(true);
     }
 }


 void intel2::apply_this(){
     vector<vector<int>> sendvec;

     vector<int>core_ra;
     for(int i=0;i<core_ratio_vec.size();i++){
         core_ra.push_back(core_ratio_vec.at(i)->text().toUInt());
     }
     sendvec.push_back(core_ra);

     vector<int>core_vo;
     for(int i=0;i<core_volt_vec.size();i++){
         core_vo.push_back(core_volt_vec.at(i)->text().toUInt());
     }
     sendvec.push_back(core_vo);


     vector<int>override;
     for(int i=0;i<override_vec.size();i++){
         if(override_vec.at(i)->isChecked()){
              override.push_back(1);
         }else{
             override.push_back(0);
         }

     }
     sendvec.push_back(override);

     vector<int>offset;
     for(int i=0;i<offset_vec.size();i++){
         offset.push_back(offset_vec.at(i)->text().toInt());
     }
     sendvec.push_back(offset);
     int bycorer=(int)bycore->isChecked();
     vector<int> bco;
     for(int i=0;i<offset_vec.size();i++){
         bco.push_back(bycorer);
     }
      sendvec.push_back(bco);
     BlueToothAP_Android_Cmd cmd(sendvec);
     cmd.bts.code=88; //88=wr
     cmd.bts.page=2;
     cmd.bts.cmd=0;
     vector<char> to_send=cmd.get_data();
     to_send.push_back(123);
emit  send_cmd(to_send);
     QMessageBox msgbox;
     msgbox.setBaseSize(QSize(600, 600));
     msgbox.information(this, "Write","Applied!");
 }


 amd1::amd1( QWidget *parent)
     : QWidget(parent)
 {

     vrm_voltage_label = new QLabel(tr("VRM Voltage"));
     vrm_loadline_label = new QLabel(tr("VRM Loadline"));

     vrm_voltage_lineedit = new QLineEdit();
     vrm_loadline_lineedit =new QLineEdit();
     apply_vrm_button=new  QPushButton("Apply");
      QSpacerItem* endseperator=new QSpacerItem(20,20);
       seperator=new QSpacerItem(20,20);
spacer1=new QSpacerItem(800,20);
QVBoxLayout* vbox1=new QVBoxLayout;
QVBoxLayout* vbox2=new QVBoxLayout;
QHBoxLayout* hbox1=new QHBoxLayout;

 line1 = new QFrame();
   line1->setFrameShape(QFrame::HLine);
  line1->setFrameShadow(QFrame::Sunken);
vbox1->addWidget(vrm_voltage_label);
  vbox1->addWidget(vrm_voltage_lineedit);

  vbox2->addWidget(vrm_loadline_label);
  vbox2->addWidget(vrm_loadline_lineedit);

  hbox1->addItem(vbox1);
  hbox1->addItem(seperator);
  hbox1->addItem(vbox2);
   hbox1->addItem(seperator);
  hbox1->addWidget(apply_vrm_button);
  hbox1->addItem(endseperator);

  QPalette pal = apply_vrm_button->palette();
  pal.setColor(QPalette::Button, QColor(Qt::lightGray));
  apply_vrm_button->setAutoFillBackground(true);
  apply_vrm_button->setPalette(pal);
  apply_vrm_button->update();



 QVBoxLayout*  vbox3=new QVBoxLayout;
  vbox3->addItem(hbox1);
 vbox3->addItem(spacer1);

    vbox3->addWidget(line1);//firstbox
 vbox3->addItem(spacer1);
allcore_freq_lineedit = new QLineEdit();
 vid_lineedit  = new QLineEdit();
allco= new QLabel(tr("All Core MHz"));
vider= new QLabel(tr("VID mv"));

QVBoxLayout* vbox4=new QVBoxLayout;
vbox4->addWidget(allco);
  vbox4->addWidget(allcore_freq_lineedit);

  QVBoxLayout* vbox5=new QVBoxLayout;
  vbox5->addWidget(vider);
  vbox5->addWidget(vid_lineedit);

  QHBoxLayout* hbox2=new QHBoxLayout;
  hbox2->addLayout(vbox4);
    hbox2->addItem(seperator);
  hbox2->addLayout(vbox5);
    hbox2->addItem(seperator);
  apply_it=new QPushButton("Apply");
  apply_it->setAutoFillBackground(true);
  apply_it->setPalette(pal);
  apply_it->update();

  hbox2->addWidget(apply_it);
    hbox2->addItem(endseperator);
vbox3->addLayout(hbox2);
  vbox3->addItem(spacer1);

     vbox3->addWidget(line1);//firstbox
  vbox3->addItem(spacer1);



mainvbox=new QVBoxLayout;
mainvbox->addLayout(vbox3);


//mainvbox->addItem(spacer1);
//mainvbox->addWidget(line1);//firstbox
//mainvbox->addItem(spacer1);


        connect(this->apply_vrm_button, &QPushButton::clicked, this, &amd1::applyvr);
        connect(this->apply_it, &QPushButton::clicked, this, &amd1::applyvid);

 }

 void amd1::applyvr(){
     vector<int> vrm;
     if(!vrm_voltage_lineedit->text().isEmpty()){
         vrm.push_back(vrm_voltage_lineedit->text().toUInt());
     }else{
          vrm.push_back(0);
     }

     if(!vrm_loadline_lineedit->text().isEmpty()){
         vrm.push_back(vrm_loadline_lineedit->text().toUInt());
     }else{
          vrm.push_back(0);
     }


     BlueToothAP_Android_Cmd cmd(vrm);
     cmd.bts.code=88; //88=wr
     cmd.bts.page=1;
     cmd.bts.cmd=0;
     vector<char> to_send=cmd.get_data();
     to_send.push_back(123);
emit  send_cmd(to_send);
     QMessageBox msgbox;
     msgbox.setBaseSize(QSize(600, 600));
     msgbox.information(this, "Write","Applied!");

 }

void amd1::applyvid(){
    vector<int> vrm;
    if(!allcore_freq_lineedit->text().isEmpty()){
        vrm.push_back(allcore_freq_lineedit->text().toUInt());
    }else{
         vrm.push_back(0);
    }

    if(!vid_lineedit->text().isEmpty()){
        vrm.push_back(vid_lineedit->text().toUInt());
    }else{
         vrm.push_back(0);
    }

    BlueToothAP_Android_Cmd cmd(vrm);
    cmd.bts.code=88; //88=wr
    cmd.bts.page=1;
    cmd.bts.cmd=1;
    vector<char> to_send=cmd.get_data();
    to_send.push_back(123);
emit  send_cmd(to_send);
    QMessageBox msgbox;
    msgbox.setBaseSize(QSize(600, 600));
    msgbox.information(this, "Write","Applied!");

}


void amd1::applyccx(){
    vector<int> vrm;
    if(!vid_lineedit->text().isEmpty()){
        vrm.push_back(vid_lineedit->text().toUInt());
    }else{
         vrm.push_back(0);
    }

    for(int i=0;i<core_ratio_vec.size();i++){
        if(!core_ratio_vec.at(i)->text().isEmpty()){
            vrm.push_back(core_ratio_vec.at(i)->text().toUInt());
        }else{
             vrm.push_back(0);
        }
    }


    BlueToothAP_Android_Cmd cmd(vrm);
    cmd.bts.code=88; //88=wr
    cmd.bts.page=1;
    cmd.bts.cmd=2;
    vector<char> to_send=cmd.get_data();
    to_send.push_back(123);
emit  send_cmd(to_send);
    QMessageBox msgbox;
    msgbox.setBaseSize(QSize(600, 600));
    msgbox.information(this, "Write","Applied!");

}

  amd1::~amd1()
  {
     delete vrm_voltage_label;
    delete vrm_loadline_label;
  delete vrm_voltage_lineedit;
    delete vrm_loadline_lineedit;
   delete apply_vrm_button;
  delete line1 ;

 delete allcore_freq_lineedit ;
 delete vid_lineedit;
 delete apply_it;
 delete apply_ccx;;

     delete seperator;
    delete spacer1;
    delete mainvbox;
   delete allco;
  delete vider;

  }




Chat::Chat(QWidget *parent)
    : QDialog(parent), ui(new Ui_Chat)
{
    //! [Construct UI]
    ui->setupUi(this);
 /*QScrollArea*  scrollArea = new QScrollArea(this);
  scrollArea->setWidgetResizable(true);

scrollArea->setWidget( ui->tabWidget);*/
ui->connectButton->setHidden(true);

   ui->radioButton_2->setChecked(true);
// ui->tabWidget->addTab(new intel1(), tr("Intel1"));
intel1tab=new intel1(this);
intel2tab=new intel2(this);
amd1tab=new amd1(this);
//test_window();
//ui->tabWidget->addTab(new intel2(), tr("Intel2"));
    data_array_bluetooth=new char[50000];

  //  ui->tabWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    // ui->tabWidget->setMinimumHeight(1000);
    // ui->tabWidget->setMaximumHeight(1000);
//ui->tabWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

     //QScroller:: grabGesture(scrollArea, QScroller::TouchGesture);
 //QScroller:: grabGesture(scrollArea, QScroller::LeftMouseButtonGesture);
     connect(ui->quitButton, &QPushButton::clicked, this, &Chat::quit_clicked);




   // connect(ui->connectButton, &QPushButton::clicked, this, &Chat::connectClicked);
   // connect(ui->sendButton, &QPushButton::clicked, this, &Chat::sendClicked);
    //! [Construct UI]

    localAdapters = QBluetoothLocalDevice::allDevices();
    //

    if(localAdapters.size()>2){
         QBluetoothLocalDevice adapter(localAdapters.at(0).address());
         adapter.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
    }

    /*mine
    QBluetoothLocalDevice local_device;
    QString local_dev_name;
    if(local_device.isValid()){
        QMessageBox msgBox;
        local_dev_name=local_device.name();

        msgBox.information(this, "Result", local_dev_name);
        local_device.powerOn();

        local_device.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
        QList <QBluetoothAddress> remotes;
        remotes=local_device.connectedDevices();
         msgBox.information(this, "connectedDevices", QString::number(remotes.size()));
         connector_widget=new RemoteSelector2(this);

         // Create a discovery agent and connect to its signals
            discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
            connect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
                    connector_widget, SLOT(deviceDiscovered(QBluetoothDeviceInfo)));

            connect(connector_widget, SIGNAL(selected_device(int,  QList<QBluetoothAddress>, QList <QBluetoothUuid>)),
                    this, SLOT(device_selected(int,  QList<QBluetoothAddress>, QList <QBluetoothUuid> )));

            // Start a discovery
            discoveryAgent->start();
            connector_widget->show();

    }
*/

    //! [Create Chat Server]
    server = new ChatServer(this);
    connect(server, QOverload<const QString &>::of(&ChatServer::clientConnected),
            this, &Chat::clientConnected);
    connect(server, QOverload<const QString &>::of(&ChatServer::clientDisconnected),
            this,  QOverload<const QString &>::of(&Chat::clientDisconnected));
    connect(server, &ChatServer::messageReceived,
            this,  &Chat::showMessage);

    connect(server, &ChatServer::messageReceived2,
             this,  &Chat::showMessage2);


  //  connect(this, &Chat::sendMessage, server, &ChatServer::sendMessage);


    connect(intel1tab,SIGNAL(send_cmd(vector<char>&)),
            this, SLOT(send_cmd(vector<char>&)));

    connect(intel2tab,SIGNAL(send_cmd(vector<char>&)),
            this, SLOT(send_cmd(vector<char>&)));

    connect(amd1tab,SIGNAL(send_cmd(vector<char>&)),
            this, SLOT(send_cmd(vector<char>&)));
    connect(ui->radioButton_2, SIGNAL( toggled(bool ) ),
           this, SLOT( radioButton_2_changed(bool ) ) );



    server->startServer();
    //! [Create Chat Server]

    //! [Get local device name]
    localName = QBluetoothLocalDevice().name();
    //! [Get local device name]
    //connectClicked();

}

Chat::~Chat()
{
  qDeleteAll(clients);
   delete server;
    delete data_array_bluetooth;
  delete intel1tab;
  delete  intel2tab;
  delete  amd1tab;

  delete  m_scrollArea;
}

void Chat::startClient()
{

   // if (socket1)
     //   return;

    // Connect to service
    socket1 = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    connect(socket1, SIGNAL(connected()),
            this, SLOT(time_to_start()));

    qDebug() << "Create socket";
    socket1->connectToService(address_vec.at(devnumber_to_connect), uuid_vec.at(devnumber_to_connect));
    qDebug() << "ConnectToService done";


   /* connect(socket1, &QBluetoothSocket::readyRead, this, &ChatClient::readSocket);
    connect(socket, &QBluetoothSocket::connected, this, QOverload<>::of(&ChatClient::connected));
    connect(socket, &QBluetoothSocket::disconnected, this, &ChatClient::disconnected);
    connect(socket, QOverload<QBluetoothSocket::SocketError>::of(&QBluetoothSocket::error),
            this, &ChatClient::onSocketErrorOccurred);
            */

}

void Chat::device_selected(int devnum, QList<QBluetoothAddress> addy, QList <QBluetoothUuid> udy)
{
  /*  disconnect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            connector_widget, SLOT(deviceDiscovered(QBluetoothDeviceInfo)));*/
  address_vec=addy;
  uuid_vec=udy;

     devnumber_to_connect=devnum;
startClient();

}

//! [clientConnected clientDisconnected]
void Chat::clientConnected(const QString &name)
{
    ui->chat->insertPlainText(QString::fromLatin1("%1 has connected.\n").arg(name));
}

void Chat::clientDisconnected(const QString &name)
{
    ui->chat->insertPlainText(QString::fromLatin1("%1 has disconnected.\n").arg(name));
}
//! [clientConnected clientDisconnected]

//! [connected]
void Chat::connected(const QString &name)
{
    ui->chat->insertPlainText(QString::fromLatin1("Connected with %1.\n").arg(name));
}
//! [connected]

void Chat::newAdapterSelected()
{
    const int newAdapterIndex = adapterFromUserSelection();
    if (currentAdapterIndex != newAdapterIndex) {
        server->stopServer();
        currentAdapterIndex = newAdapterIndex;
        const QBluetoothHostInfo info = localAdapters.at(currentAdapterIndex);
        QBluetoothLocalDevice adapter(info.address());
        adapter.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
        server->startServer(info.address());
        localName = info.name();
    }
}

int Chat::adapterFromUserSelection() const
{
    int result = 0;
    QBluetoothAddress newAdapter = localAdapters.at(0).address();
/*
    if (ui->secondAdapter->isChecked()) {
        newAdapter = localAdapters.at(1).address();
        result = 1;
    }*/
    return result;
}

void Chat::reactOnSocketError(const QString &error)
{
    ui->chat->insertPlainText(error);
}

//! [clientDisconnected]
void Chat::clientDisconnected()
{
    ChatClient *client = qobject_cast<ChatClient *>(sender());
    if (client) {
        disconnect(client, &ChatClient::messageReceived2,
                 this,  &Chat::showMessage2);
        clients.removeOne(client);
        client->deleteLater();
    }
}
//! [clientDisconnected]

//! [Connect to remote service]
void Chat::connectClicked()
{
   /* ui->connectButton->setEnabled(false);

    // scan for services
    const QBluetoothAddress adapter = localAdapters.isEmpty() ?
                                           QBluetoothAddress() :
                                           localAdapters.at(currentAdapterIndex).address();

    RemoteSelector remoteSelector(adapter);
#ifdef Q_OS_ANDROID
    if (QtAndroid::androidSdkVersion() >= 23)
        remoteSelector.startDiscovery(QBluetoothUuid(reverseUuid));
    else
        remoteSelector.startDiscovery(QBluetoothUuid(serviceUuid));
#else
    remoteSelector.startDiscovery(QBluetoothUuid(serviceUuid));
#endif
    if (remoteSelector.exec() == QDialog::Accepted) {
        QBluetoothServiceInfo service = remoteSelector.service();

        qDebug() << "Connecting to service 2" << service.serviceName()
                 << "on" << service.device().name();

        // Create client
        qDebug() << "Going to create client";
        ChatClient *client = new ChatClient(this);
qDebug() << "Connecting...";

      //  connect(client, &ChatClient::messageReceived,
           //     this, &Chat::showMessage);
        connect(client, &ChatClient::messageReceived2,
                this, &Chat::showMessage2);
        connect(client, &ChatClient::disconnected,
                this, QOverload<>::of(&Chat::clientDisconnected));
        connect(client, QOverload<const QString &>::of(&ChatClient::connected),
                this, &Chat::connected);
        connect(client, &ChatClient::socketErrorOccurred,
                this, &Chat::reactOnSocketError);
        connect(this, &Chat::sendMessage, client, &ChatClient::sendMessage);
qDebug() << "Start client";
        client->startClient(service);

        clients.append(client);
        connect(client, &ChatClient::messageReceived2,
                 this,  &Chat::showMessage2);

    }

    ui->connectButton->setEnabled(true);*/
}
//! [Connect to remote service]
//!

//! [sendClicked]
void Chat::sendClicked()
{
   // ui->sendButton->setEnabled(false);
   // ui->sendText->setEnabled(false);

   // showMessage(localName, ui->sendText->text());
   // emit sendMessage(ui->sendText->text());

//    ui->sendText->clear();

  //  ui->sendText->setEnabled(true);
    //ui->sendButton->setEnabled(true);
}
//! [sendClicked]

//! [showMessage]
void Chat::showMessage(const QString &sender, const QString &message)
{
    ui->chat->insertPlainText(QString::fromLatin1("%1: %2\n").arg(sender, message));
    ui->chat->ensureCursorVisible();
}
//! [showMessage]

void Chat::widgets_got_arch()
{
    if(this_is_intel){

          ui->tabWidget->addTab(intel1tab, tr("Intel1"));
          if(numatom_bluetooth<1){

              intel1tab->atom_ratio_label->setHidden(true);
              intel1tab->act_atom_label->setHidden(true);
              for(int i=0;i<intel1tab->ratio_limit_vec.at(2).size();i++){
                  intel1tab->ratio_limit_vec.at(2).at(i)->setHidden(true);
                  intel1tab->ratio_limit_vec.at(3).at(i)->setHidden(true);
              }
             intel1tab->synch_allatom_label->setHidden(true);

             intel1tab->synch_allatom_lineedit->setHidden(true);

          }

           ui->tabWidget->addTab(intel2tab, tr("Intel2"));
           int total_cores=numrealcores_bluetooth+numatom_bluetooth;

           for(int i=0;i<total_cores;i++){
                QFont font;

               QHBoxLayout* hbox=new QHBoxLayout;
               QLabel *l1=new QLabel("Core"+QString::number(i));
               intel2tab->qlabel_vec1.push_back(l1);
               QLineEdit *core_ratio=new QLineEdit;
               intel2tab->core_ratio_vec.push_back(core_ratio);
               QLineEdit *core_volt=new QLineEdit;

               intel2tab->core_volt_vec.push_back(core_volt);
               QCheckBox* checkb=new QCheckBox;
               intel2tab->override_vec.push_back(checkb);
               QLineEdit *offset=new QLineEdit;
               intel2tab->offset_vec.push_back(offset);
               hbox->addWidget(l1,2);
               hbox->addItem(intel2tab->seperator);
               hbox->addWidget(core_ratio,2);
               hbox->addItem(intel2tab->seperator);
               hbox->addWidget(core_volt,2);
               hbox->addItem(intel2tab->seperator);
               hbox->addWidget(checkb,1);
               hbox->addItem(intel2tab->seperator);
               hbox->addWidget(offset,1);
               if(total_cores>12){
               if(total_cores>48){

                      font.setPointSize(7);


               }else if(total_cores>24){

                       font.setPointSize(9);

               }else if(total_cores>12){
                      font.setPointSize(12);

               }
               l1->setFont(font);
               core_ratio->setFont(font);
               core_volt->setFont(font);
               offset->setFont(font);
               l1->setFont(font);
               l1->setFont(font);
               l1->setFont(font);
               intel2tab->spco->setFont(font);
                intel2tab->ovc->setFont(font);
               intel2tab->ovr_mode->setFont(font);;
                intel2tab->offsetv->setFont(font);
               }


               intel2tab->mainvbox->addLayout(hbox);
               intel2tab->mainvbox->addItem(intel2tab->spacer1);
           }

           // intel2tab->scrollArea->setBackgroundRole(QPalette::Dark);
            //intel2tab->scrollArea->setLayout(intel2tab->mainvbox);

            //intel2tab->mainvbox->setSizeConstraint(QLayout::SetMinAndMaxSize);
      /*    intel2tab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
           intel2tab->setMinimumHeight(1000);
           intel2tab->setMaximumHeight(1000);
            intel2tab->m_scrollArea=new QScrollArea(intel2tab);
            intel2tab->m_scrollArea->setWidget(intel2tab);
            intel2tab->m_scrollArea->setWidgetResizable(true);
            intel2tab->m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
             intel2tab->m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
           intel2tab->mainvbox->addWidget(m_scrollArea);*/
          // intel2tab->mainvbox->setContentsMargins(0,0,0,0);

           if((numrealcores_bluetooth+numatom_bluetooth)>48){
              QFont font;
                  font.setPointSize(8);
                  ui->chat->setFont(font);
           }else if((numrealcores_bluetooth+numatom_bluetooth)>32){
               QFont font;
                   font.setPointSize(11);
                   ui->chat->setFont(font);
           }else if((numrealcores_bluetooth+numatom_bluetooth)>24){
               QFont font;
                   font.setPointSize(14);
                   ui->chat->setFont(font);
           }
           QHBoxLayout* hboxy=new QHBoxLayout();
           hboxy->addWidget(intel2tab->bycore);
           hboxy->addWidget(intel2tab->synchall);
           hboxy->addWidget(intel2tab->apply);
           if((numrealcores_bluetooth+numatom_bluetooth)>48){
              QFont font;
                  font.setPointSize(8);
                 intel2tab->bycore->setFont(font);
                 intel2tab->synchall->setFont(font);
                 intel2tab->apply->setFont(font);
           }else if((numrealcores_bluetooth+numatom_bluetooth)>24){
               QFont font;
                   font.setPointSize(11);
                  intel2tab->bycore->setFont(font);
                   intel2tab->synchall->setFont(font);
                   intel2tab->apply->setFont(font);
           }else if((numrealcores_bluetooth+numatom_bluetooth)>12){
               QFont font;
                   font.setPointSize(14);
                  intel2tab->bycore->setFont(font);
                   intel2tab->synchall->setFont(font);
                   intel2tab->apply->setFont(font);
           }

           intel2tab->mainvbox->addLayout(hboxy);


          intel2tab->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
          intel2tab->setLayout(intel2tab->mainvbox);



        //  QAbstractItemView:: setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

          //intel2tab->setMinimumHeight(5000);

            ui->tabWidget->update();
            ui->tabWidget->setCurrentIndex(0);


}else{
           ui->tabWidget->addTab(amd1tab, tr("AMD1"));
           vector<QHBoxLayout*> hboxes;
           for(int i=0;i<numccx_bluetooth;i++){
               if(i%2==0){
                    QHBoxLayout* hbox=new QHBoxLayout;
                   hboxes.push_back(hbox);
               }
              int indexer=hboxes.size()-1;
               QLabel *l1=new QLabel("CCX"+QString::number(i));
               amd1tab->qlabel_vec1.push_back(l1);
               QLineEdit *core_ratio=new QLineEdit;
               amd1tab->core_ratio_vec.push_back(core_ratio);
                hboxes.at(indexer)->addWidget(l1);
               hboxes.at(indexer)->addItem(amd1tab->seperator);
               hboxes.at(indexer)->addWidget(core_ratio);
              hboxes.at(indexer)->addItem(amd1tab->seperator);
               if(i%2!=0){
                    hboxes.at(indexer)->addItem(amd1tab->seperator);
               }

           }
           for(int i=0;i<hboxes.size();i++){

              amd1tab->mainvbox->addLayout( hboxes.at(i));
               amd1tab->mainvbox->addItem(amd1tab->spacer1);


           }
           amd1tab->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
           amd1tab->apply_ccx=new QPushButton("Apply Per CCX Freq");
            QSpacerItem* sep=new QSpacerItem(300,300);
            QHBoxLayout* anotherh=new QHBoxLayout;
            anotherh->addItem(sep);
            anotherh->addWidget( amd1tab->apply_ccx);
             anotherh->addItem(sep);

              amd1tab->mainvbox->addLayout(anotherh);

              QFont font;
                  font.setPointSize(13);
                  if(numccx_bluetooth>4){
                       intel2tab->bycore->setFont(font);
                      amd1tab->vrm_voltage_label->setFont(font);
                      amd1tab-> vrm_loadline_label->setFont(font);
                    amd1tab-> vrm_voltage_lineedit->setFont(font);
                      amd1tab-> vrm_loadline_lineedit->setFont(font);

                       amd1tab-> allco->setFont(font);
                       amd1tab->vider->setFont(font);



                  amd1tab->allcore_freq_lineedit->setFont(font);
                   amd1tab->vid_lineedit->setFont(font);
                  for(int i=0;i<  amd1tab->qlabel_vec1.size();i++){
                        amd1tab->qlabel_vec1.at(i)->setFont(font);
                        amd1tab->core_ratio_vec.at(i)->setFont(font);
                  }

                  }


           amd1tab->setLayout(amd1tab->mainvbox);
 connect(amd1tab->apply_ccx, &QPushButton::clicked, amd1tab, &amd1::applyccx);

           ui->tabWidget->update();
           ui->tabWidget->setCurrentIndex(0);




    }
    if(got_osc){
        osc_data_vec1b=new WORD[29000];
        osc_data_vec2b=new WORD[29000];
        osc_data_vec3b=new WORD[29000];

        osc_trigger_vec1b=new WORD[29000];
        osc_trigger_vec2b=new WORD[29000];
        osc_trigger_vec3b=new WORD[29000];

        osctab=new mainwindow4(server, this);
    ui->tabWidget->addTab(osctab, tr("OSC"));
    }
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(page_changed()));


}

void Chat:: radioButton_2_changed(bool checked)
{
    if(checked){
        keep_monitor=true;
    }else{
        keep_monitor=false;
    }
}

void Chat::page_changed(){

    int page=ui->tabWidget->currentIndex();
    if(((this_is_intel && page==3) || (!this_is_intel && page==2)) && got_osc && !doing_osc){
        ui->quitButton->setHidden(true);
        vector<char>sender={77,static_cast<char>(page),1,123};
        server->sendMessage2(sender);
        keep_monitor=false;
        if(!osctab->started){
        osctab->start();
        }else{
            runit=true;
        }
         connect(osctab->d_plot->quit_button, &QPushButton::clicked, this, &Chat::quit_clicked);
         osctab->updateGeometry();
         osctab->d_plot->updateGeometry();
         osctab->d_plot->graphicsView->updateGeometry();
         doing_osc=true;

    }
    else if(page!=0){
        ui->quitButton->setHidden(false);
        vector<char>sender={77,static_cast<char>(page),123};
        server->sendMessage2(sender);
        keep_monitor=false;
        if(doing_osc){
        doing_osc=false;
        osctab->stop();
        }

    }else{
        ui->quitButton->setHidden(false);
        if(ui->radioButton_2->isChecked()){
            vector<char>sender={77,0,123};
            server->sendMessage2(sender);
            keep_monitor=true;
        }
        if(doing_osc){
        doing_osc=false;
         osctab->stop();
        }
    }
}


void Chat::send_cmd(vector<char>& vec){
    server->sendMessage2(vec);
}


//! [showMessage]
void Chat::showMessage2(int length)
{
    if(data_array_bluetooth[0]==145){
//QMessageBox msg;
//msg.information(this,"i","gotit");
        uint8_t channel=data_array_bluetooth[1];
        //length-=3;
        int actual=0;
        int indexer=2;
        while(indexer<(length)){
             WORD* wd=(WORD*)(&(data_array_bluetooth[indexer]));
             if(channel==1){
             osc_data_vec1b[actual]=*wd;
             }else if(channel==2){
                 osc_data_vec2b[actual]=*wd;
                 }else if(channel==3){
                 osc_data_vec3b[actual]=*wd;
                 }
            indexer+=2;
            actual++;
        }
        if(channel==1){

            data_index_counter=0;
            osc_data_vec1b_size=length/2;
        }if(channel==2){
            data_index_counter2=0;
            osc_data_vec2b_size=length/2;
        }if(channel==3){
            data_index_counter3=0;
            osc_data_vec3b_size=length/2;
        }
       }
    else if(length>sizeof(BT_DATA_ANDROID_STRUCT) || length>sizeof(BlueToothAP_Android_Cmd)){
        if(data_array_bluetooth[0]==123 && keep_monitor){

            string s1;
            std::stringstream ss;
            QString buff;
            cpuident_bluetooth = (data_array_bluetooth[1])&0x7F; // 0 for intel 1 for amd


              model_bluetooth = data_array_bluetooth[2]; // 0 for adl/rpl, 0 for vmr, 1 for raph
            numrealcores_bluetooth = data_array_bluetooth[3]; // full cores for amd
               numatom_bluetooth = data_array_bluetooth[4];
              numccx_bluetooth = data_array_bluetooth[5];
               htt_bluetooth = data_array_bluetooth[6];
                element_type_bluetooth = data_array_bluetooth[7];
                rows_bluetooth = data_array_bluetooth[8];
               cols_bluetooth = data_array_bluetooth[9];
               if(!got_arch){
                   if((data_array_bluetooth[1]>>7)&0x1){
                       got_osc=true;
                   }
                                got_arch=true;
                                if(cpuident_bluetooth==1){
                                    this_is_intel=false;
                                }
                                widgets_got_arch();
                            }

                             BlueToothAP_Android BT_AN=(data_array_bluetooth);
                             if(1){  ss <<  left << setw(8) << "Core" <<  right << setw(10) << "Freq MHz"  << setw(13) << right << setw(13) << "VID mv" << setw(13) << right << setw(13) << "Temp C"  << setw(13)<< endl;
                                                 if(BT_AN.bts.type==0){
                                                     if(BT_AN.bts.rows==1){

                                                         for(int i=0;i<BT_AN.intvec1d.size();i++){
                                                             if(i==0){
                                                                 aligncore(ss,BT_AN.intvec1d.at(i));
                                                            //  ss <<  left << setw(13)<<BT_AN.intvec1d.at(i);
                                                             }else if(i<(BT_AN.intvec1d.size()-1)){
                                                                 // ss << right << setw(13) <<BT_AN.intvec1d.at(i)<< setw(13);
                                                                  alignmid(ss,BT_AN.intvec1d.at(i));
                                                             }else{
                                                                  alignend(ss,BT_AN.intvec1d.at(i));
                                                             }
                                                         }
                                                         ss<<endl;
                                                     }else{
                                                         if(this_is_intel){
                                                         for(int i=0;i<BT_AN.intvec2d.size();i++){
                                                              for(int j=0;j<BT_AN.intvec2d.at(i).size();j++){
                                                                  if(j==0){
                                                             // ss <<  left << setw(13)<<BT_AN.intvec2d.at(i).at(j);
                                                                          aligncore(ss,BT_AN.intvec2d.at(i).at(j));
                                                                  }else if(j<(BT_AN.intvec2d.at(i).size()-1)){
                                                                       //ss << right << setw(13) <<BT_AN.intvec2d.at(i).at(j) << setw(13);
                                                                       alignmid(ss,BT_AN.intvec2d.at(i).at(j));
                                                                  }else{
                                                                       alignend(ss,BT_AN.intvec2d.at(i).at(j));
                                                                  }
                                                              }
                                                              ss<<endl;
                                                         }
                                                     }else{

                                                             for(int i=0;i<(BT_AN.intvec2d.size());i++){
                                                                  for(int j=0;j<BT_AN.intvec2d.at(i).size();j++){
                                                                      if(j==0){
                                                                 // ss <<  left << setw(13)<<BT_AN.intvec2d.at(i).at(j);
                                                                              aligncore(ss,BT_AN.intvec2d.at(i).at(j));
                                                                      }else if(j<(BT_AN.intvec2d.at(i).size()-1)){
                                                                           //ss << right << setw(13) <<BT_AN.intvec2d.at(i).at(j) << setw(13);
                                                                           alignmid(ss,BT_AN.intvec2d.at(i).at(j));
                                                                      }else{
                                                                           alignend(ss,BT_AN.intvec2d.at(i).at(j));
                                                                      }
                                                                  }
                                                                  ss<<endl;
                                                         }
                                                            // if(   BT_AN.intvec2d.at(BT_AN.intvec2d.size()-1).at(0)==1){
                                                              //      ss<<endl;
                                                                //    ss<<"Clk Stretching Detected!"<<endl;
                                                            // }


                                                     }
                                                 }
                                                 }

                                                 else  if(BT_AN.bts.type==1){
                                                     if(BT_AN.bts.rows==1){

                                                         for(int i=0;i<BT_AN.doubvec1d.size();i++){
                                                              if(i==0){
                                                             ss << left << setw(13);
                                                             alignit(ss, BT_AN.doubvec1d.at(i));
                                                             ss <<  BT_AN.doubvec1d.at(i);
                                                              }else{
                                                                  ss << right << setw(13) ;
                                                                   alignit(ss, BT_AN.doubvec1d.at(i));
                                                                  ss << BT_AN.doubvec1d.at(i) << setw(13);
                                                              }
                                                         }
                                                         ss<<endl;
                                                     }else{
                                                         for(int i=0;i<BT_AN.doubvec2d.size();i++){
                                                              for(int j=0;j<BT_AN.doubvec2d.at(i).size();j++){
                                                                  if(j==0){
                                                                  ss << left << setw(13);
                                                                  alignit(ss, BT_AN.doubvec2d.at(i).at(j));
                                                                  ss << BT_AN.doubvec2d.at(i).at(j);
                                                                  }else{
                                                                       ss << right << setw(13) ;
                                                                        alignit(ss, BT_AN.doubvec2d.at(i).at(j));
                                                                       ss <<  BT_AN.doubvec2d.at(i).at(j) << setw(13);
                                                                  }

                                                              }
                                                              ss<<endl;
                                                         }
                                                     }
                                                 }
                                                 else  if(BT_AN.bts.type==2){
                                                     if(BT_AN.bts.rows==1){

                                                         for(int i=0;i<BT_AN.floatvec1d.size();i++){
                                                              if(i==0){
                                                             ss << left << setw(13);
                                                             alignit(ss, BT_AN.floatvec1d.at(i));
                                                             ss <<  BT_AN.floatvec1d.at(i);
                                                              }else{
                                                                  ss << right << setw(13) ;
                                                                   alignit(ss, BT_AN.floatvec1d.at(i));
                                                                  ss <<  BT_AN.floatvec1d.at(i) << setw(13);
                                                              }
                                                         }
                                                         ss<<endl;
                                                     }else{
                                                         for(int i=0;i<BT_AN.floatvec2d.size();i++){
                                                              for(int j=0;j<BT_AN.floatvec2d.at(i).size();j++){
                                                                  if(j==0){
                                                                  ss << left << setw(13);
                                                                  alignit(ss, BT_AN.floatvec2d.at(i).at(j));
                                                                  ss << BT_AN.floatvec2d.at(i).at(j);
                                                                  }else{
                                                                      ss << right << setw(13) ;
                                                                       alignit(ss, BT_AN.floatvec2d.at(i).at(j));
                                                                      ss <<  BT_AN.floatvec2d.at(i).at(j) << setw(13);
                                                                  }

                                                              }
                                                              ss<<endl;
                                                         }
                                                     }
                                                 }
                                                 else  if(BT_AN.bts.type==3){
                                                     if(BT_AN.bts.rows==1){

                                                         for(int i=0;i<BT_AN.longdoubvec1d.size();i++){
                                                             if(i==0){
                                                             ss << left << setw(13);
                                                             alignit(ss, BT_AN.longdoubvec1d.at(i));
                                                             ss <<  BT_AN.longdoubvec1d.at(i);
                                                             }else{
                                                                 ss << right << setw(13) ;
                                                                  alignit(ss, BT_AN.longdoubvec1d.at(i));
                                                                 ss <<  BT_AN.longdoubvec1d.at(i)<< setw(13);
                                                             }
                                                         }
                                                         ss<<endl;
                                                     }else{
                                                         for(int i=0;i<BT_AN.longdoubvec2d.size();i++){
                                                              for(int j=0;j<BT_AN.longdoubvec2d.at(i).size();j++){
                                                                  if(j==0){
                                                                  ss << left << setw(13);
                                                                  alignit(ss, BT_AN.longdoubvec2d.at(i).at(j));
                                                                  ss << BT_AN.longdoubvec2d.at(i).at(j);
                                                                  }else{
                                                                      ss << right << setw(13) ;
                                                                       alignit(ss, BT_AN.longdoubvec2d.at(i).at(j));
                                                                      ss <<  BT_AN.longdoubvec2d.at(i).at(j)<< setw(13);
                                                                  }

                                                              }
                                                              ss<<endl;
                                                         }
                                                     }
                                                 }
                                                 else {
                                                     if(BT_AN.bts.rows==1){

                                                         for(int i=0;i<BT_AN.charvec1d.size();i++){
                                                             if(i==0){
                                                              ss <<  left << setw(13)<<BT_AN.charvec1d.at(i);
                                                             }else{
                                                                 ss << right << setw(13) <<BT_AN.charvec1d.at(i)<< setw(13);
                                                            }
                                                         }
                                                         ss<<endl;
                                                     }else{
                                                         for(int i=0;i<BT_AN.charvec2d.size();i++){
                                                              for(int j=0;j<BT_AN.charvec2d.at(i).size();j++){
                                                                  if(j==0){

                                                              ss <<  left << setw(13)<<BT_AN.charvec2d.at(i).at(j);
                                                                  }else{
                                                                       ss << right << setw(13) <<BT_AN.charvec2d.at(i).at(j)<< setw(13);
                                                                  }
                                                              }
                                                              ss<<endl;
                                                         }
                                                     }
                                                 }



                                           }
                                            ss << endl;
                                            s1 = ss.str();
                                            buff = QString::fromStdString(s1);
                                            if((numrealcores_bluetooth+numatom_bluetooth)>48){
                                               QFont font;
                                                   font.setPointSize(8);
                                                   ui->chat->setFont(font);
                                            }else if((numrealcores_bluetooth+numatom_bluetooth)>32){
                                                QFont font;
                                                    font.setPointSize(11);
                                                    ui->chat->setFont(font);
                                            }else if((numrealcores_bluetooth+numatom_bluetooth)>24){
                                                QFont font;
                                                    font.setPointSize(14);
                                                    ui->chat->setFont(font);
                                            }


                                            ui->chat->setText(buff);
                                           ui->chat->ensureCursorVisible();
                     }

      else if(data_array_bluetooth[0]==55 || data_array_bluetooth[0]==66){
            //uint8_t code = data_array_bluetooth[0];
            uint8_t page = data_array_bluetooth[1]; //page menu
            //uint8_t cmd = data_array_bluetooth[2]; //
            //uint8_t type = data_array_bluetooth[3];
            //uint8_t rows = data_array_bluetooth[4];
            //uint8_t cols = data_array_bluetooth[5];

            if(page==ui->tabWidget->currentIndex()){
                 BlueToothAP_Android_Cmd BT_AN=(data_array_bluetooth);
                 if(this_is_intel){
                     if(page==1){
                         vector<int> settings=BT_AN.intvec1d;
                         intel1tab->vrm_voltage_lineedit->setText(QString::number(settings.at(0)));
                         intel1tab->vrm_loadline_lineedit->setText(QString::number(settings.at(1)));
                         int current_ind=2;
                         for(int i=0;i<intel1tab->ratio_limit_vec.size();i++){
                             for(int j=0;j<intel1tab->ratio_limit_vec.at(i).size();j++){
                                 intel1tab->ratio_limit_vec.at(i).at(j)->setText(QString::number(settings.at(current_ind)));
                                 current_ind++;
                             }
                         }
                         double bb=settings.at(current_ind);
                         bb/=100.0;
                        intel1tab->bclk_lineedit->setText(QString::number(bb));
                        current_ind++;
                        bb=settings.at(current_ind);
                                                 bb/=100.0;
                        intel1tab->pcie_lineedit->setText(QString::number(bb));

                          }
                     else if(page==2){
                         vector< vector<int>> settings=BT_AN.intvec2d;
                         for(int i=0;i<settings.at(0).size();i++){
                             if(settings.at(0).at(i)==1){
                                 QString fav="*Core";
                                 fav+=QString::number(i);
                                 intel2tab->qlabel_vec1.at(i)->setText(fav);
                             }

                         }
                          for(int i=0;i<settings.at(1).size();i++){
                              intel2tab->core_ratio_vec.at(i)->setText(QString::number(settings.at(1).at(i)));
                          }
                          for(int i=0;i<settings.at(2).size();i++){
                              intel2tab->core_volt_vec.at(i)->setText(QString::number(settings.at(2).at(i)));
                          }

                          for(int i=0;i<settings.at(3).size();i++){
                              if(settings.at(3).at(i)==1){
                                  intel2tab->override_vec.at(i)->setChecked(true);
                              }else{
                                   intel2tab->override_vec.at(i)->setChecked(false);
                              }

                          }
                          for(int i=0;i<settings.at(4).size();i++){
                              intel2tab->offset_vec.at(i)->setText(QString::number(settings.at(4).at(i)));
                          }



                     }
                 }else{
                       if(page==1){
                           vector<int> settings=BT_AN.intvec1d;
                           amd1tab->vrm_voltage_lineedit->setText(QString::number(settings.at(0)));
                           amd1tab->vrm_loadline_lineedit->setText(QString::number(settings.at(1)));

                       }
                 }
            }

        }



    }
}
//! [showMessage]


void Chat:: quit_clicked(){
    disconnect(server, QOverload<const QString &>::of(&ChatServer::clientConnected),
            this, &Chat::clientConnected);
    disconnect(server, QOverload<const QString &>::of(&ChatServer::clientDisconnected),
            this,  QOverload<const QString &>::of(&Chat::clientDisconnected));
    disconnect(server, &ChatServer::messageReceived,
            this,  &Chat::showMessage);

    disconnect(server, &ChatServer::messageReceived2,
             this,  &Chat::showMessage2);
    disconnect(intel1tab,SIGNAL(send_cmd(vector<char>&)),
            this, SLOT(send_cmd(vector<char>&)));

    disconnect(intel2tab,SIGNAL(send_cmd(vector<char>&)),
            this, SLOT(send_cmd(vector<char>&)));


    disconnect(ui->radioButton_2, SIGNAL( toggled(bool ) ),
           this, SLOT( radioButton_2_changed(bool ) ) );
    for(int i=0;i<clients.size();i++){
    disconnect(clients.at(i), &ChatClient::messageReceived2,
             this,  &Chat::showMessage2);
    disconnect(clients.at(i), &ChatClient::messageReceived2,
            this, &Chat::showMessage2);
    disconnect(clients.at(i), &ChatClient::disconnected,
            this, QOverload<>::of(&Chat::clientDisconnected));
    disconnect(clients.at(i), QOverload<const QString &>::of(&ChatClient::connected),
            this, &Chat::connected);
    disconnect(clients.at(i), &ChatClient::socketErrorOccurred,
            this, &Chat::reactOnSocketError);
    disconnect(this, &Chat::sendMessage, clients.at(i), &ChatClient::sendMessage);

    disconnect(clients.at(i), &ChatClient::messageReceived2,
             this,  &Chat::showMessage2);
    clients.removeOne(clients.at(i));
    clients.at(i)->deleteLater();
    }

    delete server->workerThread;


  // server->stopServer();
   for(int i=0;i<clients.size();i++)
        clients.pop_back();
   emit killme();
  //::exit(0);
  // connect(ui->quitButton, &QPushButton::clicked, this, &Chat::accept);




}


void Chat::test_window(){
  osctab=new mainwindow4(server, this);
 ui->tabWidget->addTab(osctab, tr("OSC"));
 osctab->start();
}
