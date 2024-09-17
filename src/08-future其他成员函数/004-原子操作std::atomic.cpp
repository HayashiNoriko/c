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
        // 自增
        g_mycount++; // 是原子操作
        // g_mycount.fetch_add(1); // 是原子操作
        // g_mycount = g_mycount + 1; // 不是原子操作，运行结果不对。它由多个非原子步骤组成，在多线程环境中存在数据竞争

        // 赋值
        // atomic<int> g_mycount2 = g_mycount; // 错误，不能使用拷贝赋值运算符，这不是原子操作了
        // atomic<int> g_mycount2(g_mycount); // 错误，不能使用拷贝构造
        // atomic<int> g_mycount2 = g_mycount.load(); // 正确，可以使用 load 执行一个原子操作，提取值
        atomic<int> g_mycount2(g_mycount.load()); // 正确，可以使用 load 执行一个原子操作，提取值
        g_mycount2.store(12); // 正确，使用 store 来赋值，是原子操作
        g_mycount2 = 13; // 正确，是原子操作
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