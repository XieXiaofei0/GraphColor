/******************************
*@author:zjluestc@outlook.com
*@created date:2019/5/16
******************************/
#pragma once
#ifndef ZJL_UTILITY_H
#define ZJL_UTILITY_H
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "Macros.h"

namespace zjl_utility {

class Timer {
public:
    // �������Ϊ��ʱ����������ʱʱ���
    Timer(long interval) :start_time(clock()), end_time(start_time + interval) {};
    // �ж��Ƿ�ʱ
    bool isTimeOut() const { return clock() > end_time; }
    // ʹ��clock()������ʱ��ʱ�䵥λΪms
    long usedTime() const { return clock() - start_time; }
    // ʹ��clock()������ʱ��ʱ�䵥λΪms
    long restTime() const { return end_time - clock(); }
private:
    std::clock_t start_time;
    std::clock_t end_time;
};

class Date {
public:
    // ���ر�ʾ���ڸ�ʽ���ַ�����Ϊ����������
    static const std::string shortDateStr();
    // ���ر�ʾ���ڸ�ʽ���ַ�����Ϊ�׶����ַ�
    static const std::string humanDataStr();
};

class Random {
public:
    Random(unsigned int seed = 0);
    // ������������ӣ�ֻ����ԭʼseedΪ0�ҵ�һ������ʱ��Ч
    void setSeed(unsigned int seed);
    // ��ȡ���������ֵ
    unsigned int getSeed() const { return seed_; }
    // ������ΧΪ [lb, ub] ���������
    int gen(int ub = INT_MAX, int lb = 0) const;
    // ������ΧΪ [lb, ub] �����������
    double gen(double ub = 1.0, double lb = 0.0) const;
private:
    static bool set_seed_;         // ֻ������һ����������ӡ�
    static unsigned int seed_;     // ����Random�������������Ӷ�����ͬ�ġ�
};

struct LogSwitch {
    bool msg_on;        // �Ƿ��ӡ������־
    bool time_on;       // �Ƿ��ӡʱ���
    std::string name;   // ��Ϣ�����ȼ�����
    LogSwitch(bool msg_on_ = false, bool time_on_ = false, std::string name_ = "") :
        msg_on(msg_on_), time_on(time_on_), name(name_) {}
};

class Log {
public:
    Log(std::string dir_path = "");
    ~Log();

    // ����log�ľ������ݡ�
    template<typename T>
    Log& operator<<(const T &msg) {
        #ifndef CLOSE_ALL_LOGS
        oss << msg;
        #endif
        return *this;
    }

    // ���ݴ���Ŀ�����Ϣˢ��log���൱��std::endl��
    Log& operator<<=(const LogSwitch &info);

private:
    size_t nb_msg;              // ��¼һ���ж�������־��Ϣ��
    std::ofstream ofs;          // �ļ��������
    std::ostringstream oss;     // ���ڻ���һ��log�����ݵ��ַ�������
};

extern Log mylog;       // ��Ŀ��Ψһ��log��ӡ����

extern LogSwitch logsw_debug;
extern LogSwitch logsw_info;
extern LogSwitch logsw_warn;
extern LogSwitch logsw_error;
extern LogSwitch logsw_critical;

extern Random myrand;

}

#endif // !ZJL_UTILITY_H
