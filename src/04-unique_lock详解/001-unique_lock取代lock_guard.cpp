//
// Created by 冯欣怡 on 2024/9/3.
//


#include <iostream>
#include <string>
#include <thread>
#include <list>

using namespace std;

/**
 * 1、unique_lock 是个类模板，工作中，一般还是使用 lock_guard
 *      但 unique_lock比lock_guard 灵活很多，但效率差一点、内存占得多一点
 *
 * 2、unique_lock 的第二个参数：
 *      （1）adopt_lock：和其在 lock_guard 中的作用相同。【必须先 lock】
 *      （2）try_to_lock：尝试用 mutex 的 lock() 去锁住它，但如果没用锁定成功，就会立即执行后面语句，并不会阻塞在那里
 *                      因此 try_to_lock 和 lock 不能连着用，，不然毫无意义了。【不能先 lock】
 *      （3）defer_lock：初始化一个没有加锁的 mutex。【不能先 lock】
 *
 *
 * */


class A {
public:

    void inMsgRecvList() {
        for (int i = 0; i < 100000; ++i) {


            // try_to_lock 的使用
            unique_lock<mutex> unique(my_mutex, try_to_lock);

            if (unique.owns_lock()) {
                // 拿到了锁
                msgRecvList.push_back(i);
                cout << "inMsgRecvList()执行，插入元素" << i << endl;
            } else {
                // 没拿到锁
                cout << "inMsgRecvList()执行，但没有拿到锁，只能干点别的事情" << i << endl;
            }


        }
    }

    void outMsgRecvList() {
        for (int i = 0; i < 100000; ++i) {

            // adopt_lock 的使用
            // unique_lock<mutex> unique(my_mutex, adopt_lock); // 错误，还没被 lock，不能使用 adopt_lock
            // unique_lock<mutex> unique(my_mutex); // 正确

            // 让线程休息 2 秒
//            chrono::milliseconds dura(100);
//            this_thread::sleep_for(dura);


            // defer_lock 的使用
            /** 体现 unique_lock 的灵活性 */
            unique_lock<mutex> unique(my_mutex, defer_lock); // 不要立即锁定，而是稍后手动锁定
            unique.lock(); // 手动上锁（注意这里的 lock() 是 unique_lock 的成员函数哦，不是 mutex 的成员函数！）
            // 之后一般不需要手动 unlock 了，等 unique 析构时自动 unlock 就行
            // 然而在特殊情况下，要处理一些非共享代码，也可以手动 unlock，处理完了再 lock 就行
            unique.unlock();
            // ... 处理一些非共享代码 ...
            unique.lock();


            // try_lock() 的使用。
            // 作用和 try_to_lock 一样.
            if (unique.try_lock() == true) {
                // 上锁成功
                // ...
            }

            if (!msgRecvList.empty()) {
                // 消息不为空
                int command = msgRecvList.front();
                msgRecvList.pop_front(); // 移除

                // 处理数据...

                cout << "out执行，取出" << command << endl;
            } else {
                cout << "outMsgRecvList()执行，但目前列表为空" << endl;
            }

            // release() 的使用
            // 返回它所管理的 mutex 对象指针，并释放所有权。也就是说，该 unique 和该 mutex 不再有关系
            // 需要确保把 mutex 解锁
            unique.release()->unlock();

            // 转移 unique 的所有权
            // 如果 unique 管理着一个 mutex，那可以使 unique 不管理任何 mutex，让 unique2 来管理 mutex
            // unique_lock<mutex> unique2(unique); // 错误，不能拷贝
            unique_lock<mutex> unique2(std::move(unique)); // 正确，使用移动语义

        }
    }

private:
    list<int> msgRecvList;
    mutex my_mutex;
};

int main() {

    A myA;
    thread inThread(&A::inMsgRecvList, &myA);
    thread outThread(&A::outMsgRecvList, &myA);
    inThread.join();
    outThread.join();

    cout << "主线程结束" << endl;
    return 0;
}