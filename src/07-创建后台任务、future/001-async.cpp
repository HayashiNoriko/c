//
// Created by 冯欣怡 on 2024/9/5.
//


#include <iostream>
#include <string>
#include <future>
#include <chrono>

using namespace std;

/**
 * std::async 是用来启动异步任务的。
 * 它会启动一个新的线程来执行任务，返回一个 std::future 对象，用来获取异步任务的结果
 * 不需要我们手动创建一个新的 thread*/



// 要异步执行的任务
int compute() {
    cout << "compute 开始执行" << endl;
    this_thread::sleep_for(chrono::seconds(2)); // 模拟计算过程
    return 42;
}

int main() {
    // 启动异步任务，立即执行 compute 函数
    future<int> result = async(launch::async, compute);
    /**
     * async() 的第一个参数：
     * std::launch::deferred    表示线程入口函数调用被延迟到 future 的 wait()或者 get()函数调用时才执行，且是在主线程执行的
     *                          若此情况下未调用 wait()或 get()，那么子线程不会执行，甚至不会创建！（maybe 是被编译器优化了）
     *
     * std::launch::async       默认。立即创建并执行子线程
     * */

    // 可以在这里做其他的事情
    this_thread::sleep_for(chrono::seconds(3)); // 模拟做其他事情
    cout << "Doing some other work..." << endl;

    // 获取异步任务的结果，阻塞直到结果计算完成
    int value = result.get();
    // value = result.get(); // 错误，get() 函数只能调用一次。也可以用 wait()，但是只会等，不能拿到值

    /**
     * 一个神奇的现象：如果既不调用 get，又不调用 wait，主线程也会在 return 之前，等子线程运行完毕。
     * 因为 std::future 的析构函数会确保与之关联的异步任务完成后才销毁对象。
     * 这是 std::async 的一种设计特性，旨在确保程序的安全性，避免子线程任务在主线程退出前未完成而引发潜在的问题。
     */

    cout << "Result: " << value << endl;

    // 控制台打印：
    /**
        compute 开始执行
        Doing some other work...
        Result: 42
     */


    return 0;
}