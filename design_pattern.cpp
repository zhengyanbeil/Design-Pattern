#include "design_pattern.h"

using namespace std;
/*
 * 单例模式注意事项：
 * 1. 构造函数、拷贝构造、复制构造等等特殊函数需要私有化；
 * 2. 由于单例，故定义自身类类型的数据成员
 * 3. 对外提供getInstance的唯一类接口，用来获取单例
 * 4. getInstance需要注意多线程情况，故使用双重锁定。
 */
class Singleton
{
    private:
        Singleton() = default;
        Singleton(const Singleton&) = delete;
        Singleton operator ==(const Singleton&) = delete;
        static mutex mtx;

    public:
        static Singleton* a;

        static Singleton* getInstance()
        {
            if (nullptr == a)
            {
                mtx.lock();
                if (nullptr == a)
                {
                    a = new Singleton();
                    cout << "1..." << endl;
                }
                mtx.unlock();
            }
            return a;
        }
};

Singleton* Singleton::a = NULL;
mutex Singleton::mtx;
