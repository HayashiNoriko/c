//
// Created by 冯欣怡 on 2024/8/31.
//

/**
 * 两个陷阱
 * */
#include <iostream>
#include <string>
#include <thread>

using namespace std;

/**
 * 陷阱 1
 * */
void myPrint(const int &i, char *pmybuf) {
    /**
     * const int &i 实际上传递的是一个临时对象的引用（创建thread的机制如此），
     * 这个临时对象的生命周期会被延长到该引用的生命周期结束。
     * 所以detach之后，子线程中使用i仍是安全的
     * 可以观察到子线程中打印的 i 的地址和主线程打印的 a 的地址不同。
     * 并且，这里由于使用了 &，则必须使用 const，因为要绑定一个临时对象，只能绑定到const的引用上
     * 若只使用 int i 则没问题，可 const，也可不 const
     *
     *
     * char *pmybuf 这个指针本身并不是一个临时对象，而是一个具体的地址。
     * 指针传递不涉及临时对象的问题。
     * 主线程中的myBuf 是一个在栈上分配的数组。
     * 主线程结束时，myBuf 所在的栈帧会被销毁，myBuf 的内存会被释放。
     * 此时子线程若继续使用 pmybuf，那么 pmybuf 将会指向一块已经被释放的内存。
     * 这可能导致子线程在访问 pmybuf 时发生未定义行为，比如崩溃或者打印出垃圾值。
     *
     * */

    cout << "&i = " << &i << endl;
    cout << "i = " << i << endl;
    cout << "char = " << pmybuf << endl;
    return;
}

/**
 * 陷阱 2
 * */
void myPrint2(int i, const string &pmybuf) {
    cout << pmybuf << endl;
    return;
}

int main() {

    int a = 1;
    cout << "&a = " << &a << endl;
    char myBuf[] = "this is a test";

    // 陷阱一
    // thread myThread(myPrint, a, myBuf);
    // myThread.detach();

    // 陷阱二
    // thread myThread2(myPrint2, a, myBuf);
    /**
     * myBuf 在进入子线程之后，才会发生隐式类型转换（由char数组转换为string对象）
     * 而不是在进入子线程之前就转换。
     * 因此，可能发生主线程已经结束、myBuf内存已经被回收了，myPrint2才开始使用 myBuf
     * 后果可想而知。
     * */
    // 安全的做法
    thread myThread2(myPrint2, a, string(myBuf)); // 手动显示转换成string
    myThread2.detach();

    /**
     * 总结：
     * 当类型简单（如int）时，传值，可避免忘记const。
     * 当类型复杂（如自定义类型）时，传引用（听说效率高，可减少一次拷贝构造），但要记得加上const，且在传参之前就构造好，避免隐式构造
     *
     * */
    cout << "主线程" << endl;

    return 0;
}