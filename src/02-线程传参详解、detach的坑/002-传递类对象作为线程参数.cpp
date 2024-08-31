//
// Created by 冯欣怡 on 2024/8/31.
//


#include <iostream>
#include <string>
#include <thread>

using namespace std;

class A {
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
};


void fun(const A &a) {
    cout << "调用fun，" << &a << " 子线程id：" << std::this_thread::get_id() << endl;
}

int main() {

    // 线程id的获取
    cout << "主线程id：" << std::this_thread::get_id() << endl;

    int i = 666;

    // 隐式转换
    // thread myThread(fun,i);
    /**
        主线程id：0x1ec5b6080
        值构造：0x16f42af3c 线程id：0x16f42b000
        调用fun，0x16f42af3c 子线程id：0x16f42b000
        析构：0x16f42af3c 线程id：0x16f42b000
        主线程 end
     * */
    // 只有一个值构造（隐式转换），以及其析构，都发生在子线程里


    // 显示转换
    thread myThread(fun, A(i));
    /**
        主线程id：0x1ec5b6080
        值构造：0x16ef830bc 线程id：0x1ec5b6080
        拷贝构造：0x6000021751f0 线程id：0x1ec5b6080
        析构：0x16ef830bc 线程id：0x1ec5b6080
        调用fun，0x6000021751f0 子线程id：0x16f00b000
        析构：0x6000021751f0 线程id：0x16f00b000
        主线程 end
     * */
    // 一个值构造（显示转换），以及其析构，都在主线程
    // 另一个拷贝构造在主线程，但析构在子线程
    myThread.join();

    cout << "主线程 end" << endl;
    return 0;
}