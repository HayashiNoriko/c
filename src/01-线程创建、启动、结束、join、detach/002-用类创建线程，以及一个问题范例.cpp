//
// Created by 冯欣怡 on 2024/8/30.
//


#include <iostream>
#include "string"
#include <thread>

using namespace std;

class TA {
public:
    void operator()() {
        cout << "mi=" << mi << endl;
        cout << "mi=" << mi << endl;
        cout << "mi=" << mi << endl;
        cout << "mi=" << mi << endl;
        cout << "mi=" << mi << endl;
    }

    int &mi;

    TA(int &i) : mi(i) {
        cout << "TA(int &i)构造函数执行，this = " << this << endl;
    }

    TA(const TA &ta) : mi(ta.mi) {
        cout << "TA拷贝构造函数执行，this = " << this << endl;
    }

    ~TA() {
        cout << "~TA析构函数执行，this = " << this << endl;
    }
};

int main() {

    // 用类创建，以及一个问题范例
    int i = 666;
    TA ta(i);
    thread myThread(ta);

    // 先使用join观察正常情况下，TA对象的创建。
    // myThread.join();
    // 控制台输出如下：
    /**
        TA(int &i)构造函数执行，this = 0x16fca30d0
        TA拷贝构造函数执行，this = 0x600002e4c048
        mi=666
        mi=666
        mi=666
        mi=666
        mi=666
        ~TA析构函数执行，this = 0x600002e4c048
        主线程1
        主线程2
        主线程3
        主线程4
        主线程5
        ~TA析构函数执行，this = 0x16fca30d0
     */
    // 证明创建线程时，是用的拷贝构造


    // 然后，使用detach观察一下
    myThread.detach();
    // 遇到了一种情况：
    /**
        TA(int &i)构造函数执行，this = 0x16f9bb0d0
        TA拷贝构造函数执行，this = 0x600000a7c048
        主线程1
        主线程2
        主线程3
        主线程4
        主线程5
        ~TA析构函数执行，this = 0x16f9bb0d0
        mi=-1856764588
     */
    // 当主线程的ta析构后，子线程的ta还存在，只是其中保存的成员变量mi被释放了，因为它是引用
    // 因此在多线程环境下，可调用对象的成员变量不要使用引用，很危险

    cout << "主线程1" << endl;
    cout << "主线程2" << endl;
    cout << "主线程3" << endl;
    cout << "主线程4" << endl;
    cout << "主线程5" << endl;
    return 0;
}