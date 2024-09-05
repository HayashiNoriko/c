//
// Created by 冯欣怡 on 2024/9/5.
//


#include <iostream>
#include <string>
#include <thread>
#include <future>

using namespace std;

// 要异步执行的任务
int compute() {
    cout<<"compute 开始执行"<<endl;
    this_thread::sleep_for(chrono::seconds(2)); // 模拟计算过程
    cout<<"compute 执行完毕"<<endl;
    return 42;
}

int main() {
    // packaged_task 将 compute 函数包装进任务中
    // 其模板参数是一个函数签名，这里为 int()，而不是单纯的 int
    // 其目的是封装一个可调用对象（函数、lambda 表达式、函数对象等），并将其与一个 std::future 绑定
    packaged_task<int()> task(compute);

    // 获取 future 用来获取任务结果
    future<int> result = task.get_future();

    // 新建一个线程，执行 task
    thread t(std::move(task));

    /**
     * ps：packaged_task本身也是一个可调用对象，也就是我们可以这样来调用 computer 函数：
     * task();
     * 但这就不是多线程了，虽然后续也可以通过 result 来拿到返回值，但与多线程无关，就是普通的函数调用
     * */

    // 在这里可以执行其他任务
    cout << "其他任务开始" << endl;
    this_thread::sleep_for(chrono::seconds(3)); // 模拟做其他事情
    cout << "其他任务结束" << endl;

    // 获取任务结果
    int value = result.get();
    cout << "Result: " << value << endl;

    t.join();  // 等待线程结束

    /**
        其他任务开始
        compute 开始执行
        compute 执行完毕
        其他任务结束
        Result: 42
     */
    return 0;
}