//
// Created by 冯欣怡 on 2024/9/24.
//


#include <iostream>
#include <string>
#include <thread>
#include <future>

using namespace std;

/**
 * 1、async
 * 当通过std::async调用的函数抛出异常时，该异常会被捕获并存储在与该任务关联的std::future对象中。
 * 调用get()时，如果任务抛出了异常，会重新抛出该异常。
 * */
void throwError() {
    throw std::runtime_error("Error!!!!!");
}

int main1() {
    auto future = std::async(throwError);
    try {
        future.get();  // 这里会重新抛出异常
    } catch (const std::exception &e) {
        std::cout << "Caught exception in async: " << e.what() << std::endl;
    }
    return 0;
}


/**
 * 2、packaged_task
 *
 * packaged_task的行为与std::async类似。
 * 当packaged_task中存储的函数抛出异常时，该异常会被捕获并存储在其关联的std::future中。
 * 使用get()时，如果发生异常，会重新抛出。
 * */
int main2() {
    packaged_task<void()> task(throwError);
    future<void> ft = task.get_future();

    task();
    try {
        ft.get();
    } catch (exception &e) {
        cout << "exception in packaged_task:" << e.what() << endl;
    }
    return 0;
}


/**
 * 3、promise
 * 使用std::promise，你可以显式地存储异常。
 * 通过调用set_exception()，你可以将捕获的异常填充到promise中，从而使相关的future对象可以访问。
 * */
void fun(promise<void> prom) {
    try {
        throw runtime_error("error in promise!!!!!!!!");
    } catch (exception &e) {
        prom.set_exception(current_exception()); // 存储异常
    }
}

int main() {
    promise<void> prom;
    future<void> ft = prom.get_future();
    thread t(fun, std::move(prom));

    try{
        ft.get();
    } catch (exception& e) {
        cout<<e.what()<<endl;
    }

    t.join();
    return 0;
}