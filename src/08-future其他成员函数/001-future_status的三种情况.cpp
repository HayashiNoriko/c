//
// Created by 冯欣怡 on 2024/9/10.
//


#include <iostream>
#include <string>
#include <thread>
#include <future>

using namespace std;

// 线程执行需要 3 秒
int myPrint() {
    cout << "myPrint() start. thread id = " << this_thread::get_id() << endl;
    this_thread::sleep_for(chrono::seconds(3));
    cout << "myPrint() end. thread id = " << this_thread::get_id() << endl;
    return 5;
}

int main() {
    /**
     * status 的三种情况
     * */

    cout << "main() start. thread id =" << this_thread::get_id() << endl;

    // future<int> result = async(launch::deferred,myPrint); // -----第三种情况，status 是 deferred 的
    future<int> result = async(myPrint);

    // 若前面的 async 已经设置了 deferred，那么 wait_for 不会等待，直接返回一个 deferred
    // future_status status = result.wait_for(chrono::seconds(1)); // 等到 1 秒，此时线程还没执行完-----第一种情况，status 是 timeout 的
    future_status status = result.wait_for(chrono::seconds(4)); // 等到 4 秒，此时线程已经执行完了-----第二种情况，status 是 ready 的

    if (status == future_status::timeout) {
        cout << "超时，线程还没执行完" << endl;
    } else if (status == future_status::ready) {
        cout << "时间够了！线程成功返回了" << endl;
        cout << "result.get() = " << result.get() << endl;
    } else if (status == future_status::deferred) {
        cout << "async 是 deferred 的，线程并没有执行" << endl;
    }

    cout << "main() end" << endl;

    return 0;
}