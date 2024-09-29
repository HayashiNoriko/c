//
// Created by 冯欣怡 on 2024/9/27.
//


#include <iostream>
#include <string>
#include <thread>
#include <future>
#include <latch>
#include <barrier>


using namespace std;

/**
 * 在 C++20 并发编程中，latch 和 barrier 是两个用于控制多个线程同步的工具。
 * 它们的基本功能是协调线程的启动和等待，从而确保程序中某些线程在某些点上同步完成操作。
 *
 * 需要包含头文件 <latch>、<barrier>
 * */

/**
 * 1、latch
 *
 * latch 是一个单次使用的同步工具，它允许线程在某个同步点之前等待，直到计数减为0。
 * 它不能被重用，但可以用于倒计时，常用于一组线程在执行某个操作前进行同步。
 * */
int main1() {
    // latch 的构造函数需要一个正整数，表示需要多少个线程调用 count_down() 之后，才能继续运行等待的线程。
    latch mylatch(3); // 初始化 latch，计数值为 3
    thread t1([&]() {
        cout << "thread 1 running" << endl;
        mylatch.count_down(); // 计数器减 1
    });
    thread t2([&]() {
        cout << "thread 2 running" << endl;
        mylatch.count_down(); // 计数器减 1
    });
    thread t3([&]() {
        cout << "thread 3 running" << endl;
        mylatch.count_down(); // 计数器减 1
    });

    mylatch.wait(); // 等待计数值变为 0，才继续执行后面的内容

    cout << "all threads finished" << endl;

    t1.join();
    t2.join();
    t3.join();


    return 0;
}

/**
 * 2、barrier
 * barrier 是一个可重用的同步工具，线程可以反复使用 barrier 进行同步。
 * 与 latch 不同，barrier 的同步点可以重复多次。它通常用于一组线程需要在某个点上反复进行同步的场景。
 * */
int main() {

    // barrier 的构造函数也需要一个正整数，表示需要多少个线程到达同步点时，才可以继续执行。
    // barrier 还支持一个可选的完成函数，在所有线程到达同步点时执行。
    barrier sync_point(3, [] {
        cout << "all threads reached barrier" << endl;
    }); // 创建一个 barrier，3 个线程同步点，并指定完成函数

    thread t1([&]() {
        cout << "thread 1 running" << endl;
        sync_point.arrive_and_wait(); // 到达同步点，等待其他线程
        cout<<"thread 1 after barrier"<<endl;
    });
    thread t2([&]() {
        cout << "thread 2 running" << endl;
        sync_point.arrive_and_wait(); // 到达同步点，等待其他线程
        cout<<"thread 2 after barrier"<<endl;
    });
    thread t3([&]() {
        cout << "thread 3 running" << endl;
        sync_point.arrive_and_wait(); // 到达同步点，等待其他线程
        cout<<"thread 3 after barrier"<<endl;
    });
    t1.join();
    t2.join();
    t3.join();
    return 0;
}

/**
 * Latch 不关心哪个线程减少计数器，
 * 而 Barrier 则确保每个线程在同时到达同步点
 *
 * barrier还可以设置多个同步点，使得每个线程在每个周期同时到达这些同步点
 * */

