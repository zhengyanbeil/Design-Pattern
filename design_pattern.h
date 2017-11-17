#ifndef DESIGN_PATTERN_H
#define DESIGN_PATTERN_H

#include <thread>
#include <mutex>

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
                    cout << "The singleton has been created." <<endl;
                }
                mtx.unlock();
            }
            return a;
        }
};
Singleton* Singleton::a = NULL;
mutex Singleton::mtx;


/*
 * 简单工厂模式注意事项：
 * 1. 抽奖出子类（产品）
 * 2. 提供一个工厂（工厂类）和一个负责上述产品生产工作的管理员（对应的createXXXProduct函数）
 * 3. 为了使管理员清楚即将生产的产品类型，管理员的上级（调用者）需要告知管理员（通过传入参数）
 * 4. 管理员通过多态的思想生产产品
 */
class SensorType
{
    public:
        SensorType(): name("") {}
        SensorType(string name)
        {
            /*
             * Optimize!
             * TWO PROBLEM!
             * How to check the parameter in the constructor?
             * What does the constructor do if the parameter is unregistered?
             */
            if (name != "CO2" && name != "SO2" && name != "temperature" && name != "none")
                ;
            this->name = name;
        }
        /*
         *  Return a reference? Does this coding style look good?
         */
        string getName() const {return name;}
    private:
        string name;
};

class Sensor
{
    public:
        Sensor(): type("NONE"), dat('n') {}
        ~Sensor() = default;
        Sensor(const Sensor&) = default;
        Sensor& operator=(const Sensor&) = default;
        virtual unsigned char getData() const {cout << "Sensor" << endl; return dat;}
    protected:
        string type;
        unsigned char dat;
};

class CarbonDioxideSensor: public Sensor
{
    public:
        CarbonDioxideSensor()  {type = "CO2"; dat = 'c';}
//        CarbonDioxideSensor& operator =(const CarbonDioxideSensor&)=default;
        unsigned char getData()const
        {
            cout << "CarbonDioxideSensor" << endl;
            return dat;
        }
};

class SulfurDioxideSensor: public Sensor
{
    public:
        SulfurDioxideSensor()  {type = "SO2"; dat = 's';}
        unsigned char getData()const
        {
            cout << "SulfurDioxideSensor" << endl;
            return dat;
        }
};

class TemperatureSensor: public Sensor
{
    public:
        TemperatureSensor() {type = "temperature"; dat = 't';}
        unsigned char getData() const
        {
            cout << "TemperatureSensor" << endl;
            return dat;
        }
};

class SensorFactory
{
    public:
        static Sensor* createSensor(const SensorType& type)
        {
            string type_str = type.getName();
            if (type_str == "CO2")
            {
                cout << "create CarbonDioxideSensor" << endl;
                return new CarbonDioxideSensor;
            }
            else if (type_str == "SO2")
            {
                cout << "create SulfurDioxideSensor" << endl;
                return new SulfurDioxideSensor;
            }
            else if (type_str == "temperature")
            {
                cout << "create TemperatureSensor" << endl;
                return new TemperatureSensor;
            }
            else
            {
                cout << "create Sensor" << endl;
                return new Sensor;
            }
        }
};

class Client
{
    public:
        static void singletonClient()
        {
            thread arr[100000];
            for (thread& t : arr)
                t = thread(Singleton::getInstance);
            for (thread& t : arr)
                t.join();
        }
        static void factoryClient()
        {
            Sensor* s1 = SensorFactory::createSensor(SensorType("CO2"));
            Sensor* s2 = SensorFactory::createSensor(SensorType("SO2"));
            Sensor* s3 = SensorFactory::createSensor(SensorType("TEMP"));
            cout << s1->getData() << endl;
            cout << s2->getData() << endl;
            cout << s3->getData() << endl;
        }
};

#endif // DESIGN_PATTERN_H
