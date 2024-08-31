//
// Created by 冯欣怡 on 2024/8/31.
//


#include <iostream>
#include <string>
#include <thread>
#include <memory>

using namespace std;

void fun(unique_ptr<int> pzn) {
    cout << "指针地址：" << pzn << " 子线程id：" << std::this_thread::get_id() << endl;
}

int main() {
    // 创建一个独占式智能指针
    unique_ptr<int> myp(new int(100));

    cout << "指针地址：" << myp << " 主线程id：" << std::this_thread::get_id() << endl;

    // thread myThread(fun, myp); // 错误，unique_ptr没有拷贝构造函数。只能move不能copy
    thread myThread(fun, std::move(myp));
    myThread.join();

    cout << "指针地址：" << myp << " 主线程id：" << std::this_thread::get_id() << endl;

    /**
        指针地址：0x600002dbc030 主线程id：0x1ec5b6080
        指针地址：0x600002dbc030 子线程id：0x16d1af000
        指针地址：0x0 主线程id：0x1ec5b6080
     */

    return 0;
}