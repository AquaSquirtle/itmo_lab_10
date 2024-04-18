#pragma once
#include "memory"
#include "iostream"
#include "vector"
#include "unordered_map"


class AnyFunction {
public:
    struct Base {
        virtual const char* call () = 0;
    };

    template<typename Functor>
    struct ZeroArgs : public Base {
        ZeroArgs(const Functor& func) : f(func) {}
        const char* call () override {
            std::cout << 1;
            auto tmp = f();
            return reinterpret_cast<const char*&>(tmp);
        }
        Functor f;
    };

    template<typename Functor, typename T1>
    struct OneArg : public Base {
        OneArg(const Functor& func, T1 first) : f(func), first_(first) {}
        const char* call () override {
            std::cout << 1;
            auto tmp = f(first_);
            return reinterpret_cast<const char*&>(tmp);
        }
        Functor f;
        T1 first_;
    };

    template<typename Functor, typename T1, typename T2>
    struct TwoArgs : public Base {
        TwoArgs(const Functor& func, T1 first, T2 second) : f(func), first_(first), second_(second) {}
        const char* call () override {
            std::cout << 1;
            auto tmp = f(first_, second_);
            return reinterpret_cast<const char*&>(tmp);
        }
        Functor f;
        T1 first_;
        T2 second_;
    };

    template<typename Functor>
    AnyFunction(const Functor& func) {
        ptr_ = std::make_shared<ZeroArgs<Functor>>(ZeroArgs(func));
    }

    template<typename Functor, typename T1>
    AnyFunction(const Functor& func, T1 first) {
        ptr_ = std::make_shared<OneArg<Functor, T1>>(OneArg(func, first));
    }

    template<typename Functor, typename T1, typename T2>
    AnyFunction(const Functor& func, T1 first, T2 second) {
        ptr_ = std::make_shared<TwoArgs<Functor, T1, T2>>(TwoArgs(func, first, second));
    }

    AnyFunction(const AnyFunction& other) {
        ptr_ = other.ptr_;
    }


public:
    std::shared_ptr<Base> ptr_;

};


template <typename T>
class FutureResult {
public:
    FutureResult(size_t id, std::vector<const char*>& results, std::vector<AnyFunction>& container): id_(id), results_(results), container_(container){};
    operator const T&() const {
        if (results_[id_] == nullptr) {
            results_[id_] = container_[id_].ptr_->call();
        }
        return *reinterpret_cast<const T*>(&results_[id_]);
    }
private:
    std::vector<AnyFunction>& container_;
    std::vector<const char*>& results_;
    size_t id_;
};




class TTaskScheduler {
public:
    template<typename Function>
    size_t add (const Function& func) {
        results.resize(sz + 1, nullptr);
        AnyFunction tmp(func);
        container.push_back(tmp);
        return sz++;
    }

    template<typename Function, typename T1>
    size_t add (const Function& func, T1 first) {
        results.resize(sz + 1, nullptr);
        AnyFunction tmp(func, first);
        container.push_back(tmp);
        return sz++;
    }

    template<typename Function, typename T1, typename T2>
    size_t add (const Function& func, T1 first, T2 second) {
        results.resize(sz + 1, nullptr);
        AnyFunction tmp(func, first, second);
        container.push_back(tmp);
        return sz++;
    }

    template<typename T>
    T getResult (size_t id) {
        if (results[id] == nullptr) {
            results[id] = container[id].ptr_->call();
        }
        return *reinterpret_cast<const T*>(&results[id]);
    }

    template<typename T>
     FutureResult<T> getFutureResult (size_t id) {
        return FutureResult<T>(id, results, container);
     }

    void executeAll () {
        for (int i = 0; i < container.size(); ++i) {
            if (results[i] == nullptr) {
                results[i] = container[i].ptr_->call();
            }
        }
    }
private:
    std::vector<AnyFunction> container;
    std::vector<const char*> results;
    size_t sz = 0;
};

