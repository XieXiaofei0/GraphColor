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
    // 输入参数为计时器最大溢出计时时间段
    Timer(long interval) :start_time(clock()), end_time(start_time + interval) {};
    // 判断是否超时
    bool isTimeOut() const { return clock() > end_time; }
    // 使用clock()函数计时，时间单位为ms
    long usedTime() const { return clock() - start_time; }
    // 使用clock()函数计时，时间单位为ms
    long restTime() const { return end_time - clock(); }
private:
    std::clock_t start_time;
    std::clock_t end_time;
};

class Date {
public:
    // 返回表示日期格式的字符串，为连续的数字
    static const std::string shortDateStr();
    // 返回表示日期格式的字符串，为易读的字符
    static const std::string humanDataStr();
};

class Random {
public:
    Random(unsigned int seed = 0);
    // 设置随机数种子，只有在原始seed为0且第一次设置时有效
    void setSeed(unsigned int seed);
    // 获取随机数种子值
    unsigned int getSeed() const { return seed_; }
    // 产生范围为 [lb, ub] 的随机整数
    int gen(int ub = INT_MAX, int lb = 0) const;
    // 产生范围为 [lb, ub] 的随机浮点数
    double gen(double ub = 1.0, double lb = 0.0) const;
private:
    static bool set_seed_;         // 只能设置一次随机数种子。
    static unsigned int seed_;     // 所有Random对象的随机数种子都是相同的。
};

struct LogSwitch {
    bool msg_on;        // 是否打印该条日志
    bool time_on;       // 是否打印时间戳
    std::string name;   // 信息所处等级名称
    LogSwitch(bool msg_on_ = false, bool time_on_ = false, std::string name_ = "") :
        msg_on(msg_on_), time_on(time_on_), name(name_) {}
};

class Log {
public:
    Log(std::string dir_path = "");
    ~Log();

    // 传入log的具体内容。
    template<typename T>
    Log& operator<<(const T &msg) {
        #ifndef CLOSE_ALL_LOGS
        oss << msg;
        #endif
        return *this;
    }

    // 根据传入的开关信息刷新log，相当于std::endl。
    Log& operator<<=(const LogSwitch &info);

private:
    size_t nb_msg;              // 记录一共有多少条日志信息。
    std::ofstream ofs;          // 文件输出流。
    std::ostringstream oss;     // 用于缓存一条log中数据的字符串流。
};

extern Log mylog;       // 项目中唯一的log打印对象。

extern LogSwitch logsw_debug;
extern LogSwitch logsw_info;
extern LogSwitch logsw_warn;
extern LogSwitch logsw_error;
extern LogSwitch logsw_critical;

extern Random myrand;

}

#endif // !ZJL_UTILITY_H
