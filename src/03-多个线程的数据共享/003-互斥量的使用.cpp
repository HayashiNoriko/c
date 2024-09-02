//
// Created by 冯欣怡 on 2024/9/2.
//


#include <iostream>
#include <string>
#include <thread>
#include <list>
#include <mutex>

/**
 * 1、mutex.lock() 和 mutex.unlock() 必须成对使用
 *
 * 2、lock_guard 类模板，直接取代 lock() 和 unlock()，避免忘记写 unlock
 *
 * */
using namespace std;

class A {
public:

    void inMsgRecvList() {
        for (int i = 0; i < 100000; ++i) {


            // 第一种写法：lock 和 unlock 搭配使用
            // my_mutex.lock();
            // msgRecvList.push_back(i);
            // my_mutex.unlock();

            // 第二种写法：lock_guard。原理：
            // lock_guard 构造时执行了 lock()，析构时执行了 unlock()
            // 若想提前结束其生命周期，可以用大括号把要 lock 的部分括起来，离开大括号时 guard 就析构了
            {
                lock_guard<mutex> guard(my_mutex);
                msgRecvList.push_back(i);
            }

            cout << "inMsgRecvList()执行，插入元素" << i << endl;
        }
    }

    void outMsgRecvList() {
        for (int i = 0; i < 100000; ++i) {

            my_mutex.lock();

            if (!msgRecvList.empty()) {
                // 消息不为空
                int command = msgRecvList.front();
                msgRecvList.pop_front(); // 移除

                // 处理数据...

                cout << "out执行，取出" << command << endl;
            } else {
                cout << "outMsgRecvList()执行，但目前列表为空" << endl;
            }

            my_mutex.unlock();
        }
    }

private:
    list<int> msgRecvList;
    mutex my_mutex; // 创建了一个互斥量
};

int main() {

    A myA;
    thread outThread(&A::outMsgRecvList, &myA);
    thread inThread(&A::inMsgRecvList, &myA);
    inThread.join();
    outThread.join();

    cout << "主线程结束" << endl;
    return 0;
}