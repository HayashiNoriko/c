//
// Created by 冯欣怡 on 2024/9/11.
//
/**
 * 原子操作：在多线程中不会被打断的程序执行片段
 * 可以理解为一种不需要用到互斥量（即无锁）的多线程并发编程方式
 * 是“不可分割的操作”，要么是完成的，要么是未完成的，不能是半完成的
 *
 * 效率比互斥量加锁好
 * 互斥量针对的是一段代码，原子操作针对的是一个变量
 *
 * */

#include <iostream>
#include <string>
#include <thread>

using namespace std;

atomic<int> g_mycount = 0; // 定义一个原子全局量

void myWrite() {
    for (int i = 0; i < 1000000; ++i) {
        // g_mycount++; // 是原子操作
        g_mycount.fetch_add(1); // 是原子操作

        // g_mycount = g_mycount + 1; // 不是原子操作，运行结果不对。它由多个非原子步骤组成，在多线程环境中存在数据竞争
    }
}

int main() {
    thread w_thread1(myWrite);
    thread w_thread2(myWrite);
    w_thread1.join();
    w_thread2.join();

    cout << "最终的 g_myaount值为：" << g_mycount << endl; // 最终的 g_myaount值为：2000000

    return 0;
}