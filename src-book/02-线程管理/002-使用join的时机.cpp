//
// Created by 冯欣怡 on 2024/9/17.
//


#include <iostream>
#include <string>
#include <thread>
#include <exception>
#include <vector>

using namespace std;

/**
 * 使用 join 的时机过早，主线程可能会阻塞等待子线程完成，影响并行性
 * 使用 join 的时机过晚，若在 join 之前程序抛出异常，直接退出，那时子线程还没有被 join，也会报错
 *
 * 如何决定合适的 join 时机？
 * 方法 1：使用 try/catch
 * 方法 2：使用 RAII
 * */

// 线程入口函数
void task() {
    cout << "task()开始..." << endl;
    this_thread::sleep_for(3s); // 模拟执行任务
    cout << "task()结束" << endl;
}
// 方法 1：使用 try/catch
// 对 join 之前的所有代码进行异常捕获。比较繁琐。
int main1() {
    thread t1(task);
    try {
        // 抛出异常
        vector<int> v;
        v.at(1);
    } catch (...) {
        t1.join();
        throw;
    }
    t1.join();
    return 0;
}

// 方法 2：使用 RAII
// (lock_guard就是一种 raii)
class thread_guard {
    thread &t; // 引用一个线程
public:
    explicit thread_guard(thread &t_) : t(t_) {}

    ~thread_guard() {
        if (t.joinable()) {
            t.join();
            cout << "子线程被 join 了" << endl;
        }
    }

    thread_guard(const thread_guard &) = delete;

    thread_guard &operator=(const thread_guard &) = delete;
};

int main(){
    // 在退出作用域之前将 t2 join，从而保证安全退出。
    thread t2(task);
    thread_guard g(t2);

    /**
     * 不过这个例子举的不好，方式 1 主张在 join 之前 try-catch 所有代码，以解决因异常退出而没有 join 的问题
     * 而方式 2 并没有体现如何应对异常。
     * 问了下 gpt 还是得用 try-catch
     * */

    return 0;
}