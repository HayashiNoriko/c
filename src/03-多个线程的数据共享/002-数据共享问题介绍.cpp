//
// Created by 冯欣怡 on 2024/9/2.
//


#include <iostream>
#include <string>
#include <thread>
#include <list>

using namespace std;

class A {
public:
    // 把收到的消息（玩家命令）乳到一个队列的线程
    void inMsgRecvList() {
        for (int i = 0; i < 100000; ++i) {
            cout << "inMsgRecvList()执行，插入元素" << i << endl;
            msgRecvList.push_back(i); // 假设i是收到的命令
        }
    }

    // 把数据从消息列表取出
    void outMsgRecvList() {
        for (int i = 0; i < 100000; ++i) {
            if (!msgRecvList.empty()) {
                // 消息不为空
                int command = msgRecvList.front();
                msgRecvList.pop_front(); // 移除

                // 处理数据...

            } else {
                cout << "outMsgRecvList()执行，但目前列表为空" << i << endl;
            }
        }
    }

private:
    list<int> msgRecvList; // 用于代表玩家给咱们发送过来的命令
};

int main() {

    A myA;
    thread outThread(&A::outMsgRecvList, ref(myA));
    thread inThread(&A::inMsgRecvList,ref(myA));
    outThread.join();
    inThread.join();

    // 运行结果：初始过程中程序出错
    // 因为在同一时刻对列表又读又写
    // 因此需要使用互斥

    cout << "主线程结束" << endl;
    return 0;
}