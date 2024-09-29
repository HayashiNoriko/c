//
// Created by 冯欣怡 on 2024/9/24.
//


#include <iostream>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

/**
 * 对于 C++标准库而言，时钟提供四种信息：
 *
 * (1)当前时间
 *      now()
 * (2)时间点类型
 *      特定时钟的时间点类型由该时钟类的time_point成员类型定义，即now()的返回类型是time_point
 * (3)时钟 tick 周期（时钟节拍）
 *      时钟的滴答周期以秒的分数表示，每秒滴答 25 次的时钟周期为ratio<1,25>。
 *      那么周期给以指定为平均tick周期、最小可能的tick周期，或者库编写者认为合适的其他值
 * (4)是否为稳定时钟
 *      稳定时钟是指能够以恒定速度运行且无法调整的时钟。
 *      当is_steady静态数据成员为true时，表明这个时钟就是稳定的，否则，就是不稳定的。
 *      std::chrono::system_clock：系统时钟，通常是是不稳定的，因为时钟是可调的，即是这种是完全自动适应本地账户的调节。
 *      std::chrono::steady_clock：稳定时钟
 *      std::chrono::high_resolution_clock：标准库中提供的具有最小节拍周期(因此具有最高的精度[分辨率])的时钟。
 * */

int main() {
    /**
     * 1、持续时间/时延：std::chrono::duration，是类模板
     * */
    // 1、创建时延变量（自定义时间单位）
    // ratio 是一个帮助类，用于定义时间单位的比例。它接受两个参数，表示分子(秒)和分母(自定义的单位)。
    // 用 short 值表示分钟，一分钟有 60 秒
    chrono::duration<short, ratio<60, 1>> minutes(5); // 5 分钟
    // 用 double 值表示毫秒，一毫秒是 1/1000 秒
    chrono::duration<double, ratio<1, 1000>> milliseconds(1500.5); // 1500.5 毫秒

    // 2、算数运算
    auto total = minutes + chrono::duration<short, ratio<60, 1>>(2); // 7 分钟

    // 3、类型转换
    // 标准库中预定义了多种时长别名，例如 nanoseconds、microseconds，当然还有 seconds
    auto total_seconds = chrono::duration_cast<chrono::seconds>(total); // 7 分钟转换为 7*60 秒

    //4、与 time_point 结合使用
    auto now = chrono::system_clock::now();
    auto future_time = now + minutes; // 当前时间加 5 分钟


    /**
     * 2、为了方便，C++14 引入了chrono_literals命名空间，提供了许多预定义字面量后缀，例如
     * */
    auto one_day = 24h;
    auto half_an_hour = 30min;
    auto half_a_minute = 30s;
    15ns;
    10ms;
    /**建议只使用整数字面量，如果使用浮点字面量，例如 2.5ns，那么它会创建一个具有“未指定浮点类型”的时延类型，不是很安全*/


    /**
     * 3、时间点：time_point，是类模板
     * */
    // 1、创建时间点
    auto tp1 = std::chrono::system_clock::now();

    // 2、获取自历元以来的时间
    auto duration_since_epoch = tp1.time_since_epoch();

    // 3、加减、比较时间点
    auto tp2 = tp1 + chrono::hours(5);
    tp1 < tp2;

    return 0;
}