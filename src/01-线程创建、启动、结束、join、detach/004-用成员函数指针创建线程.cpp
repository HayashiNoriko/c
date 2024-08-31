//
// Created by 冯欣怡 on 2024/8/31.
//


#include <iostream>
#include <string>
#include <thread>

using namespace std;

class A{
public:
    int mi;

    A(int i) : mi(i) {
        cout << "值构造：" << this << " 线程id：" << std::this_thread::get_id() << endl;
    }

    A(const A &a) {
        mi = a.mi;
        cout << "拷贝构造：" << this << " 线程id：" << std::this_thread::get_id() << endl;
    }

    ~A() {
        cout << "析构：" << this << " 线程id：" << std::this_thread::get_id() << endl;
    }

    void fun(int i){
        cout << "子线程执行。对象：" << this << " 线程id：" << std::this_thread::get_id() << endl;
    }
};
int main() {
    cout << "主线程执行。线程id：" << std::this_thread::get_id() << endl;

    A a(10);
    thread myThread(&A::fun, a, 666);
    myThread.join();

    /**
        主线程执行。线程id：0x1ec5b6080
        值构造：0x16d7170cc 线程id：0x1ec5b6080
        拷贝构造：0x600001ce11f8 线程id：0x1ec5b6080
        子线程执行。对象：0x600001ce11f8 线程id：0x16d79f000
        析构：0x600001ce11f8 线程id：0x16d79f000
        析构：0x16d7170cc 线程id：0x1ec5b6080
     */

    // 和 02-002-传递类对象作为线程参数 差不多

    return 0;
}