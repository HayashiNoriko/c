//
// Created by 冯欣怡 on 2024/9/10.
//


#include <iostream>
#include <string>
#include <thread>
#include <future>

using namespace std;

int myPrint(int i) {
    cout << "myPrint 开始执行" << endl;
    this_thread::sleep_for(chrono::seconds(3));
    return 5;
}

int main() {

    cout << "main() start." << endl;

    packaged_task<int(int)> mypt(myPrint);

    thread t1(ref(mypt), 1);
    t1.join();

    future<int> result = mypt.get_future();
    bool isValide = result.valid();
    cout << "result 是否 valid？" << isValide << endl; // result 是否 valid？1

    // shared_future 只接受右值
    shared_future<int> s_result(std::move(result));
    // shared_future<int> s_result = result.share(); // 与上式等价

    // 此时 result 已空
    isValide = result.valid();
    cout << "result 是否 valid？" << isValide << endl; // result 是否 valid？0

    // s_result.get() 可执行多次
    cout<<"s_result.get() = "<<s_result.get()<<endl;
    cout<<"s_result.get() = "<<s_result.get()<<endl;
    cout<<"s_result.get() = "<<s_result.get()<<endl;
    cout<<"s_result.get() = "<<s_result.get()<<endl;

    cout << "main() end" << endl;

    return 0;
}