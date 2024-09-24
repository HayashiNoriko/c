//
// Created by 冯欣怡 on 2024/9/19.
//


#include <iostream>
#include <string>
#include <thread>
#include <list>
#include <algorithm>

using namespace std;

list<int> mylist;
mutex mymutex;


// 将新值添加到列表中
void add_to_list(int val) {
    /**
     * C++17 支持类模板参数推导，对于简单类模板，模板参数列表通常可以省略
     * */
    // lock_guard<mutex> guard(mymutex); // 原写法，需要显示指定mutex类型
    lock_guard guard(mymutex); // C++17 新特性
    mylist.push_back(val);
}

// 检查列表中是否包含指定的值
bool list_contains(int val) {
    lock_guard guard(mymutex);
    return find(mylist.begin(), mylist.end(), val) != mylist.end();
}


int main() {

    return 0;
}