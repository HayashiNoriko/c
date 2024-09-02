//
// Created by 冯欣怡 on 2024/9/2.
//


#include <iostream>
#include <string>
#include <thread>
#include <list>

using namespace std;

/**
 * 1、死锁的产生：
 * 至少两把锁（两个互斥量 / mutex）才能产生
 *
 * 2、死锁的一般解锁方案：
 * （1）上锁顺序一致
 * （2）使用std::lock() 函数模板
 *      一次锁住两个以上的互斥量
 *      要么把多个互斥量都锁上，要么都不锁（就在那一直等着）
 *
 * 3、lock_guard的拓展：std::adopt_lock
 * */

class A {
public:

    void inMsgRecvList() {
        for (int i = 0; i < 100000; ++i) {


            lockA.lock();
            lockB.lock();

            // lock(lockA,lockB); // 一次性锁两个锁，若失败则把两个都释放，然后在原地等待。预防死锁

            msgRecvList.push_back(i);

            // 代替 unlock() 的一种方式
            // adopt_lock 表示如果 mutax 已经被 lock，就不再次 lock 了，只等 guard 析构时 unlock
            // adopt_lock 是一个结构体对象，起一个标记作用
            // lock_guard<mutex> guardA(lockA, adopt_lock);
            // lock_guard<mutex> guardB(lockB, adopt_lock);

            lockB.unlock();
            lockA.unlock();

            cout << "inMsgRecvList()执行，插入元素" << i << endl;
        }
    }

    void outMsgRecvList() {
        for (int i = 0; i < 100000; ++i) {

            lockB.lock();
            lockA.lock();

            if (!msgRecvList.empty()) {
                // 消息不为空
                int command = msgRecvList.front();
                msgRecvList.pop_front(); // 移除

                // 处理数据...

                cout << "out执行，取出" << command << endl;
            } else {
                cout << "outMsgRecvList()执行，但目前列表为空" << endl;
            }

            lockA.unlock();
            lockB.unlock();
        }
    }

private:
    list<int> msgRecvList;
    mutex lockA;
    mutex lockB;
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