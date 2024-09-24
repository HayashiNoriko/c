//
// Created by 冯欣怡 on 2024/9/24.
//


#include <iostream>
#include <string>
#include <thread>
#include <shared_mutex>

using namespace std;

/**
 * 两个用于实现读写锁的类：
 *
 *  1、shared_mutex（C++17）
 *      允许多个读取者同时访问共享资源，但写入者独占访问。当有写入者时，所有读取者和其他写入者都被阻塞。
 *      适用于读操作远多于写操作的场景，能有效提高性能。
 *
 *  2、shared_timed_mutex（C++14）
 *      与shared_mutex类似，但增加了时间限制，可以设置超时时间进行锁定。这样允许在获取锁时进行更灵活的控制。
 *      适用于需要控制锁等待时间的情况，可以避免线程长时间阻塞。
 *      比 shared_mutex多两个功能：
 *          try_lock_shared_for()：尝试在特定时间内获取共享锁。
 *          try_lock_for()：尝试在特定时间内获取独占锁。
 *
 *
 * 上面两个是 mutex 的类型，可以使用它们自身的lock、unlock 函数，也可以搭配RAII 类：shared_lock（读）和unique_lock/lock_guard（写）来使用
 * 使用shared_lock可以确保在有写入者时，所有读取者都会被阻塞，直到写入操作完成，从而保证数据的完整性。
 *
 * 本程序只演示shared_mutex
 * */


shared_mutex s_mutex;
int shared_data = 0;

/**
 * 1、使用自身的 lock、unlock
 * */
void read1(){
    s_mutex.lock_shared(); // 用于读取者线程，获取共享锁
    cout << "Reading: " << shared_data << endl;
    s_mutex.unlock_shared(); // 释放共享锁
}

void write1(int value){
    s_mutex.lock(); // 用于写入者线程，获取独占锁。
    shared_data = value;
    cout << "Writing: " << shared_data << endl;
    s_mutex.unlock(); // 释放独占锁。
}



/**
 * 2、使用 RAII 类
 * */
void read2() {
    shared_lock<shared_mutex> lock(s_mutex);
    cout << "Reading: " << shared_data << endl;
}

void write2(int value) {
    unique_lock<shared_mutex> lock(s_mutex);
    shared_data = value;
    cout << "Writing: " << shared_data << endl;
}

int main() {
    thread t1(read1);
    thread t2(write1, 42);
    thread t3(read1);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}