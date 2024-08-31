//
// Created by 冯欣怡 on 2024/8/30.
//

#include <iostream>
#include "string"
#include <thread>

using namespace std;

/**
 * 主线程从main函数开始执行，我们自己创建的线程也需要从一个函数开始运行（初始函数）
 * 一旦这个函数运行完毕，就代表我们这个线程结束
 *
 * 整个进程是否执行完毕的标志是：主线程是否执行完
 * 若那时其他子线程还未执行完毕，那么也会被强行终止
 * 因此，为了让保持子线程的运行状态，必须让主线程也一直保持运行
 * */

// 初始函数
void myPrint(){
    cout<<"我的线程 1"<<endl;
    cout<<"我的线程 2"<<endl;
    cout<<"我的线程 3"<<endl;
    cout<<"我的线程 4"<<endl;
    cout<<"我的线程 5"<<endl;
}

int main() {

    thread myThread(myPrint); // 创建子线程，并开始运行

    // 判断是否可join或detach
    cout<<myThread.joinable()<<endl; // 1

    // 正常做法，把子线程加到主线程中
    // myThread.join(); // 主线程会等待它。如果不使用join，主线程会先结束，导致程序异常

    // 不常用做法，把子线程从主线程解除，不能再join回来。
    // 子线程从此由c++运行时库接管，程序不会报错但是输出会混乱，且主线程结束后我们再也看不到子线程的输出（在后台偷偷运行完毕）
    myThread.detach();

    cout<<myThread.joinable()<<endl; // 0

    cout<<"主线程1"<<endl;
    cout<<"主线程2"<<endl;
    cout<<"主线程3"<<endl;
    cout<<"主线程4"<<endl;
    cout<<"主线程5"<<endl;

    return 0;
}