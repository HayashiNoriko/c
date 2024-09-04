//
// Created by 冯欣怡 on 2024/9/4.
//


#include <iostream>
#include <string>
#include <thread>

using namespace std;

/**
 * 然而在实际编程中我们还是可能需要在子线程创建 MyCAS 这个单例类的对象，且是在不止 1 个线程
 * 所以 getInstance() 成员函数要互斥
 *
 * 可以使用双重锁定
 * */


// 互斥量
mutex myMutex;

// 一个单例类
class MyCAS {
public:
    static MyCAS *getInstance() {

        // 双重锁定（双重检查），提高效率
        // 若不加外层 if，则除了第一次 new 时要锁，以后每次获取对象时也要锁上一会儿
        if (mInstance == nullptr) {

            unique_lock<mutex> myUnique(myMutex); // 自动加锁。防止被不同线程多次 new

            if (mInstance == nullptr) {

                mInstance = new MyCAS();
                static CG cg;
            }
        }

        return mInstance;
    }

    class CG {
    public:
        ~CG() {
            if (mInstance) {
                delete mInstance;
                mInstance = nullptr;
            }
        }
    };

    void func() {
        cout << "测试" << endl;
    }

private:
    MyCAS() {}

    static MyCAS *mInstance;
};

MyCAS *MyCAS::mInstance = nullptr;

// 线程入口函数
void myPrint() {
    cout << "我的线程开始执行了" << endl;
    MyCAS *mc = MyCAS::getInstance();
    cout << "我的线程开始执行完毕了" << endl;
}


int main() {


    thread myThread1(myPrint);
    thread myThread2(myPrint);
    myThread1.join();
    myThread2.join();
    return 0;
}