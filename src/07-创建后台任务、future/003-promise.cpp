//
// Created by 冯欣怡 on 2024/9/5.
//


#include <iostream>
#include <string>
#include <thread>
#include <future>

using namespace std;

void set_value(promise<int>& prom) {
    cout<<"进入set_value函数"<<endl;
    this_thread::sleep_for(chrono::seconds(1));
    prom.set_value(10);  // 设置 promise 的值
    this_thread::sleep_for(chrono::seconds(1));
    cout<<"即将离开set_value函数~~~"<<endl;
}

int main() {
    // 创建一个 promise 和它对应的 future
    promise<int> prom;
    future<int> result = prom.get_future();
    /**
     * promise 的使用注意：
        std::promise 是一个单值的同步原语，设计上只能被一个线程设置值。是一次性的。
        一旦调用 promise.set_value() 或 promise.set_exception()，与该 promise 相关联的 std::future 就会获得结果，并且这个 promise 就完成了它的任务。
        std::promise 本身并不是线程安全的，因此如果多个线程试图同时操作同一个 promise，会导致数据竞争或未定义行为。
        为了让不同的线程设置不同的值，通常需要每个线程拥有独立的 promise 实例。
     * */

    // 启动一个线程设置 promise 的值
    thread t(set_value, ref(prom));

    // 获取 future 的结果
    cout << "Waiting for result..." << endl;
    int value = result.get();
    cout << "Result: " << value << endl;

    t.join();  // 等待线程结束
    /**
     * 即使 future.get() 等待了子线程计算的完成，我们仍然需要 t.join()，原因有二：
     *
     * 第一，
     * std::thread 对象管理的是线程的句柄，而不是线程的运行状态。
     * 当一个线程启动后，必须通过 join() 或 detach() 来处理它的生命周期。
     * join() 会阻塞主线程，直到子线程结束，并确保资源被正确回收。
     * 如果我们不调用 join() ，线程对象在 main() 函数结束时析构，
     * 系统会认为它仍然是一个“可联结的”（joinable）线程，导致未被回收的线程资源，从而触发异常。
     *
     * 第二，
     * future.get() 只保证了我们拿到了子线程计算的结果，
     * 但 promise.set_value() 之后还有一些代码没有运行完。
     * 这就需要使用 t.join() 来等待子线程的全部执行都完成后才继续。
     * */


    // 控制台输出：
    /**
        Waiting for result...
        进入set_value函数
        Result: 10
        即将离开set_value函数~~~
     */
    return 0;
}