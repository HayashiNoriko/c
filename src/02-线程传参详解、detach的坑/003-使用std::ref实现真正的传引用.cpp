//
// Created by 冯欣怡 on 2024/8/31.
//


#include <iostream>
#include <string>
#include <thread>

/**
 * 前文已经多次证明，创建线程传递进去的参数都会进行拷贝，即使使用了 &
 * 若我执意想要修改主线程中的变量，需要传递引用，应该使用 std::ref
 * */

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

    cout << "主线程id：" << std::this_thread::get_id() << endl;

    int i = 666;

    A a(i);
    // 使用 ref() 函数，实现真正的引用
    thread myThread(fun, ref(a));
    myThread.join();
    cout << "主线程 end" << endl;
    /**
        主线程id：0x1ec5b6080
        值构造：0x16d09f0c8 线程id：0x1ec5b6080
        调用fun，0x16d09f0c8 子线程id：0x16d127000
        主线程 end
        析构：0x16d09f0c8 线程id：0x1ec5b6080
     */
    // 可以看到，完完全全是引用了。从头到尾只有第一次的值构造。
    return 0;
}