//
// Created by 冯欣怡 on 2024/9/17.
//


#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

/**
 * timed_mutex 允许线程在尝试锁定时指定一个超时时间，这样线程不会无限期地等待获取锁。
 *
 * lock()               和 std::mutex 类似，阻塞当前线程直到成功获取互斥锁。
 * try_lock_for()       尝试在给定的时间范围内获取锁。如果在给定的时间段内无法获取锁，则返回 false。
 * try_lock_until()     尝试在给定的时间点之前获取锁。如果在指定的时间点到来时仍未获取锁，则返回 false。
 * try_lock()           立即尝试锁定，若无法锁定，立即返回 false，不会阻塞线程。
 * unlock()             释放锁，与 lock() 或其他获取锁的函数配对使用。
 * */

timed_mutex mtx;

void try_to_get_lock(int id, int wait_time) {
    // 尝试在 wait_time 毫秒内获取锁
    if (mtx.try_lock_for(chrono::milliseconds(wait_time))) {
        cout << "Thread " << id << " acquired the lock." << endl;
        this_thread::sleep_for(100ms); // 持有锁一段时间
        mtx.unlock();  // 释放锁
    } else {
        cout << "Thread " << id << " could not acquire the lock within " << wait_time << " milliseconds." << endl;
    }
}

int main() {
    thread t1(try_to_get_lock, 1, 50);   // 线程1 尝试获取锁，等待50毫秒
    thread t2(try_to_get_lock, 2, 100);  // 线程2 尝试获取锁，等待100毫秒

    t1.join();
    t2.join();

    return 0;
}