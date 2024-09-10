//
// Created by 冯欣怡 on 2024/9/4.
//


#include <iostream>
#include <string>
#include <thread>

using namespace std;

// 一个单例类
class MyCAS {
public:
    static MyCAS *getInstance() {
        if (mInstance == nullptr) {
            mInstance = new MyCAS();

            // cg 是静态对象，当程序退出时会被自动销毁，所以可以在 CG 类的析构函数中释放 mInstance
            static CG cg;
        }
        return mInstance;
    }

    // 类中套类，用来释放对象
    class CG {
    public:
        ~CG() {
            if (mInstance) {
                cout << mInstance << "被delete了" << endl;
                delete mInstance;
                mInstance = nullptr;
            }
        }
    };

    void func() {
        cout << "测试" << endl;
    }

private:
    MyCAS() {} // 私有化了构造函数
    static MyCAS *mInstance; // 静态成员变量
};

// 类静态变量初始化
MyCAS *MyCAS::mInstance = NULL;

int main() {
    MyCAS *mc = MyCAS::getInstance(); // 创建一个对象
    MyCAS *mc2 = MyCAS::getInstance(); // 再创建一个，和第一个一样
    cout << mc << " " << mc2 << endl; // 0x600003a64030 0x600003a64030

    // MyCAS myCas; // 错误，构造函数是私有的

    /**
     * 单例类一般是只读的
     * 强烈建议在主线程中，在创建其他子线程之前，就把单例类初始化了，该做的操作都做好。
     * 以后在其他线程例只读，不写。
     * */

    return 0;
}