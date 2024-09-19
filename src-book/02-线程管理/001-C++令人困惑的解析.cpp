//
// Created by 冯欣怡 on 2024/9/17.
//


#include <iostream>
#include <string>
#include <thread>

using namespace std;

// 可调用对象
class Task {
public:
    void operator()() const {
        cout << "task()运行中..." << endl;
    }
};

int main() {

    // 正常的创建方式
    Task task;
    thread t(task);

    // 错误的创建方式
    // 下面这行代码会被解析为一个函数声明。thread 为返回类型，tt 为函数名，形参是一个函数指针（返回类型为 task、无形参）。
    thread tt(Task()); // Parentheses were disambiguated as a function declaration

    // 解决方法一：使用额外的括号
    thread t1((Task()));
    // 解决方法二：使用新的统一初始化语法
    thread t2{Task()};

    t.join();
    t1.join();
    t2.join();


    return 0;
}