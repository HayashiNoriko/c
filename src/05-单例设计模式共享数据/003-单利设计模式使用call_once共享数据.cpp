//
// Created by 冯欣怡 on 2024/9/4.
//


#include <iostream>
#include <string>
#include <thread>

using namespace std;

/**
 * 使用 call_once 保证只调用一次（即使用多个线程）
 * */

// 一个系统定义的标记
once_flag g_flag;

// 一个单例类
class MyCAS {
public:
    static MyCAS *getInstance() {

        // createInstance 函数只会被调用一次。
        // g_flag 有点像一把锁的意思
        // 假如现在 createInstance 还没有被执行过，即 g_flag 还表示未执行的状态，
        // 突然两个线程同时执行到这里，线程 2 先进去执行 createInstance 了，并上锁。
        // 这时线程 1 只能乖乖等线程 2 执行 createInstance 完毕。
        // 完毕后，线程 2 将 g_flag 标记为已执行，线程 1 看见了，便不再执行
        // 之后的线程路过这里，看到 g_flag 为已执行，便不再执行 createInstance。差不多是这个意思
        call_once(g_flag, createInstance);

        return mInstance;
    }

    class CG {
    public:
        ~CG() {
            if (mInstance) {
                delete mInstance;
                mInstance = nullptr;
            }
        }
    };

    void func() {
        cout << "测试" << endl;
    }

private:
    MyCAS() {}

    static MyCAS *mInstance;

    // 只被调用一次
    static void createInstance() {
        cout << "createInstance 调用" << endl;
        mInstance = new MyCAS();
        static CG cg;
    }
};

MyCAS *MyCAS::mInstance = nullptr;

// 线程入口函数
void myPrint() {
    cout << "我的线程开始执行了" << endl;
    MyCAS *mc = MyCAS::getInstance();
    cout << "我的线程开始执行完毕了" << endl;
}


int main() {


    thread myThread1(myPrint);
    thread myThread2(myPrint);
    myThread1.join();
    myThread2.join();

    // 控制台输出：
    /**
        我的线程开始执行了
        我的线程开始执行了
        createInstance 调用
        我的线程开始执行完毕了
        我的线程开始执行完毕了
     */
    return 0;
}