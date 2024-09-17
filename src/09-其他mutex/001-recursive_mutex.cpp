//
// Created by 冯欣怡 on 2024/9/17.
//


#include <iostream>
#include <string>
#include <thread>
#include <mutex>

using namespace std;

/**
 * std::mutex 不能多次锁定，否则会造成死锁
 * 然而，有时我们在编程中会遇到这样的情况：一个函数已经获得了锁，但这个函数又调用了另一个也会尝试锁定同一个互斥体的函数。
 * 为了避免死锁，我们可以使用 std::recursive_mutex。
 *
 *
 * std::recursive_mutex 的特点
 *      递归锁定：允许同一个线程多次锁定同一个互斥体，而不会导致死锁。
 *      解锁机制：每次锁定需要对应的解锁，同一个线程必须将其锁定次数与解锁次数保持一致，互斥体才会真正解锁。
 *      性能影响：由于 recursive_mutex 需要额外的逻辑来跟踪锁定的次数，性能上可能比 std::mutex 略有降低。
 *              因此，只有在确实需要递归锁定的情况下才使用 recursive_mutex。
 *      次数有限：允许的递归次数是有限的，例如我这边测试递归 20000 次就报错了。
 * */

recursive_mutex mtx;

void recursive_function(int count) {
    if (count <= 0) return;

    // 锁定互斥体
    mtx.lock();
    cout << "Lock acquired, count = " << count << endl;

    // 递归调用
    recursive_function(count - 1);

    // 解锁互斥体
    mtx.unlock();
}

int main() {
    thread t1(recursive_function, 2000);

    t1.join();
    return 0;
}