//
// Created by 冯欣怡 on 2024/9/11.
//

/**
 * 原子操作问题引入
 * */

#include <iostream>
#include <string>
#include <thread>
#include <mutex>

using namespace std;

int g_mycount = 0; // 定义一个区局变量

mutex g_mutex; // 定义一个全局的互斥量

// 写线程的入口函数
void myWrite() {
    for (int i = 0; i < 1000000; ++i) {
        // lock_guard<mutex> lock(g_mutex); // 解决办法：使用互斥量，上锁。
        g_mycount++;
    }
}

int main() {
    thread w_thread1(myWrite);
    thread w_thread2(myWrite);
    w_thread1.join();
    w_thread2.join();

    cout << "最终的 g_myaount值为：" << g_mycount << endl; // 最终的 g_myaount值为：1003237

    /**
     * 结果和预想中（2000000）不符，原因分析：
            数据竞争： 由于 g_mycount++ 不是一个原子操作，它实际上被分解为以下三步：
                1.读取 g_mycount 的当前值。
                2.计算 g_mycount 的值加 1。
                3.将计算结果写回到 g_mycount。
            当两个线程同时执行 g_mycount++ 时，可能会发生如下的情况：
            线程 1 读取了 g_mycount，并准备增加它的值。在此期间，线程 2 也读取了相同的 g_mycount，并准备增加它的值。
            线程 1 完成加 1 操作并将值写回 g_mycount。线程 2 也完成加 1 操作并将值写回 g_mycount，但它覆盖了线程 1 的结果。
            这种情况下，两个线程的加法操作会互相覆盖，导致某些加法操作丢失，最终导致 g_mycount 的值小于预期值（2,000,000）。
     * */
    return 0;
}