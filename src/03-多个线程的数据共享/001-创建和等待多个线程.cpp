//
// Created by 冯欣怡 on 2024/9/2.
//


#include <iostream>
#include <string>
#include <thread>


using namespace std;


// 线程入口函数
void myPrint(int i) {
    cout << "myPrint开始了，i=" << i << endl;
    cout << "myPrint结束了，i=" << i << endl;
    return;
}


int main() {

    vector<thread> myThreads;

    // 创建 10 个线程，入口函数统一使用myPrint
    // 多个线程执行顺序是乱的，跟操作系统对线程的调度有关
    for (int i = 0; i < 10; ++i) {
        myThreads.push_back(thread(myPrint, i));
    }
    for (auto &t: myThreads) {
        t.join();
    }

    cout << "主线程结束" << endl;
    return 0;
}