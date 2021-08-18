//
// Created by echo-djx on 2021/8/18.
//
//any类设计要点
//1.any类不能是一个模板类形如int i;any<int>anyValue=i;无意义，还不如直接写int anyValue =i;
//2.any必须提供模板构造函数（赋值操作不必是模板的），才能完成如下操作:
//  int i; long j; struct X; X x;any anyValue(i); anyValue=j; anyValue =x;
//3.必须提供某些有关它所保存的对象型别的信息。
//4.它必须能保存数值且提供某种方法将它保存的数值“取出来”。
//5.数据不能放在any类里，这会使any类成为模板类，不符合1的要求。
//  数据应该动态存放，即动态分配一个数据的容器来存放数据，而any类中则保存指向这个容器的指针。
//  明确地说，是指向这个容器的基类的指针，这是因为容器本身必须为模板，而any类中的指针成员又必须不是泛型的。
//  因为any不能是泛型的，所以any中所有数据成员都不能是泛型的。
//  所以，结论是：为容器准备一个非泛型的基类，而让指针指向该基类。
#pragma once
#include <memory>
#include <typeindex>
#include <exception>
#include <iostream>
#include <typeindex>
using namespace std;
/*
	类似于boost Any类的实现
*/
class Any {
public:
    Any(void) :m_tpIndex(type_index(typeid(void))) {}
    Any(Any& that) :m_ptr(that.Clone()), m_tpIndex(that.m_tpIndex) {}
    Any(Any&& that) :m_ptr(move(that.m_ptr)), m_tpIndex(that.m_tpIndex) {}
    // 创建智能指针时，对于一般的类型，通过decay来移除引用和CV符，从而获取原始类型
    template<typename U, class = typename enable_if<!is_same<typename decay<U>::type, Any>::value,U>::type>
            Any(U &&value):m_ptr(new Derived<typename decay<U>::type>(forward<U>(value))), m_tpIndex(type_index(typeid(typename decay<U>::type))) {}

            bool IsNull() const { return !bool(m_ptr); }
            template<class U>bool Is() const {
        return m_tpIndex == type_index(typeid(U));
    }

    // 将Any转换成实际的类型
    template<class U>
            U& AnyCast()
            {
        if (!Is<U>())
        {
            cout << "can not cast " << typeid(U).name() << " to " << m_tpIndex.name() << endl;
            throw bad_cast();
        }
        auto derived = dynamic_cast<Derived<U>*> (m_ptr.get());
        return derived->m_value;
            }

            Any operator=(const Any& a)
                    {
        if (m_ptr == a.m_ptr)
            return *this;
        m_ptr = a.Clone();
        m_tpIndex = a.m_tpIndex;
        return *this;
                    }
private:
    struct Base;
    typedef unique_ptr<Base> BasePtr;
    struct Base
            {
        virtual ~Base() {}
        virtual BasePtr Clone() const = 0;
            };

    template<typename T>
    struct Derived :Base
            {
        template<typename U>
        Derived(U&& value) :m_value(forward<U>(value)) {}
        BasePtr Clone() const {
            return BasePtr(new Derived<T>(m_value));
        }
        T m_value;
            };

    BasePtr Clone() const
    {
        if (m_ptr != nullptr)
        {
            return m_ptr->Clone();
        }
        return nullptr;
    }

    BasePtr m_ptr;
    type_index m_tpIndex;
};
