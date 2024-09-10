//
// Created by 冯欣怡 on 2024/9/4.
//


#include <iostream>
#include <string>
#include <thread>
#include <list>

using namespace std;


class A {
public:

    // 生产者
    void inMsgRecvList() {
        int count=0;
        while(1){
            unique_lock<mutex> myUnique(myMutex);

            myCond.wait(myUnique,[this](){
                return msgRecvList.size()<5000; // 等待消息列表中有空位
            });

            // 生产中...
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 模拟生产过程
            cout << "inMsgRecvList()执行，插入元素" << count << endl;
            msgRecvList.push_back(count);
            count++;

            // 生产完毕
            myUnique.unlock();
            // myCond.notify_all(); // 通知所有消费者，列表有新数据了。适用于要唤醒多个子线程
            myCond.notify_one(); // 只通知一个子线程


        }

    }

    // 消费者
    void outMsgRecvList() {
        while(1){
            unique_lock<mutex> myUnique(myMutex); // 目前已上锁

            // 条件变量对象 myCond 和 myMutex（myUnique）配合使用
            /**
                 wait() 的工作机制：
                 1. 解锁：wait 函数首先会自动解锁当前线程持有的互斥锁，然后将线程置于阻塞状态，等待其他线程发出通知。
                 2. 等待条件：当线程被唤醒时，wait 函数会重新锁定互斥锁（如果不能马上锁定，则一直尝试锁定，直到成功上锁）
                            然后并检查给定的条件是否为真。
                            如果条件为真，线程继续执行；如果条件为假，线程重新进入等待状态，且解锁。
                 3. 解锁并继续：条件满足后，线程解除阻塞并继续执行。

                 wait() 不填写第二个参数会有什么问题：
                 1. 虚假唤醒：线程可能在 notify_one 调用之前被虚假唤醒（由于系统层面的原因）。
                            如果消费者在 msgRecvList 仍为空时继续执行，它将错误地认为可以消费数据，导致逻辑错误。
                 2. 繁琐且容易出错：没有条件检查的 wait 需要开发者在被唤醒后手动检查条件，
                                这增加了代码的复杂性和出错的可能性。万一忘记检查条件，可能会导致程序行为不正确。
             * */
            myCond.wait(myUnique,[this]{
                return !msgRecvList.empty();
            });

            // 消费中...
            this_thread::sleep_for(chrono::milliseconds(150)); // 模拟消费过程
            int command = msgRecvList.front();
            msgRecvList.pop_front(); // 移除
            cout << "outMsgRecvList()执行，取出" << command << endl;

            //消费完毕
            myUnique.unlock();
            myCond.notify_all();
        }
    }

private:
    list<int> msgRecvList; // 消息列表
    mutex myMutex;
    condition_variable myCond; // 生成一个条件变量对象
};

int main() {

    A myA;
    thread outThread(&A::outMsgRecvList, ref(myA));
    thread inThread(&A::inMsgRecvList, ref(myA));
    outThread.join();
    inThread.join();


    cout << "主线程结束" << endl;
    return 0;
}