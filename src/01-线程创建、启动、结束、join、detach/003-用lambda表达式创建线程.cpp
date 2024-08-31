//
// Created by 冯欣怡 on 2024/8/30.
//


#include <iostream>
#include <string>
#include <thread>

using namespace std;


int main() {

    // 用lambda表达式创建子线程
    auto myLam=[]{
        cout<<"我的lambda线程"<<endl;
        cout<<"我的lambda线程"<<endl;
        cout<<"我的lambda线程"<<endl;
        cout<<"我的lambda线程"<<endl;
    };

    thread myThread(myLam);
    myThread.join();

    cout<<"主线程"<<endl;
    cout<<"主线程"<<endl;
    cout<<"主线程"<<endl;
    cout<<"主线程"<<endl;
    return 0;
}