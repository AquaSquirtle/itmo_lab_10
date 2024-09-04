#include <lib/TTaskScheduler.hpp>
#include <gtest/gtest.h>
#include "iostream"
#include "cmath"


TEST(TTaskSchedulerTest, BaseTest) {
    float a = 1;
    float b = -2;
    float c = 0;

    TTaskScheduler scheduler;

    auto id1 = scheduler.add([](float a, float c){return -4 * a * c;}, a, c);

    auto id2 = scheduler.add([](float b, float v){return b * b + v;}, b, scheduler.getFutureResult<float>(id1));

    auto id3 = scheduler.add([](float b, float d){return -b + std::sqrt(d);}, b, scheduler.getFutureResult<float>(id2));

    auto id4 = scheduler.add([](float b, float d){return -b - std::sqrt(d);}, b, scheduler.getFutureResult<float>(id2));

    auto id5 = scheduler.add([](float a, float v){return v/(2*a);}, a, scheduler.getFutureResult<float>(id3));

    auto id6 = scheduler.add([](float a, float v){return v/(2*a);},a, scheduler.getFutureResult<float>(id4));

    EXPECT_FLOAT_EQ(scheduler.getResult<float>(id5), 2);
    EXPECT_FLOAT_EQ(scheduler.getResult<float>(id6), 0);
}

TEST(TaskSchedulerTest, ZeroArgs) {
    TTaskScheduler scheduler;
    auto id = scheduler.add([]()->float {return 1 * 1; });
    EXPECT_FLOAT_EQ(scheduler.getResult<float>(id), 1);
}


TEST(TaskSchedulerTest, WithOneArg) {
    TTaskScheduler scheduler;
    float arg = 120;
    auto id = scheduler.add([](float x) { return x * x; }, arg);
    EXPECT_FLOAT_EQ(scheduler.getResult<float>(id), arg * arg);
}


TEST (TaskSchedulerTest, WithTwoArgs) {
    TTaskScheduler scheduler;
    float arg1 = 100;
    float arg2 = 220;
    auto id = scheduler.add([](float x, float y) { return x * y; }, arg1, arg2);
    EXPECT_FLOAT_EQ(scheduler.getResult<float>(id), arg1 * arg2);
}

TEST (TaskSchedulerTest, FutureResultTest) {
    TTaskScheduler scheduler;
    int a = 10;
    int b = 20;
    int c = 40;
    auto id = scheduler.add([](int x, int b) {return b/x;}, a, b);
    auto id2 = scheduler.add([](int a, int c){return a * c;}, scheduler.getFutureResult<int>(id), c);
    EXPECT_EQ(scheduler.getResult<int>(id2), (b/a)*c);
}


TEST(TaskSchedulerTest, NegativeArgs) {
    TTaskScheduler scheduler;
    float arg = -5;
    auto id = scheduler.add([](float x) { return x * x; }, arg);
    EXPECT_FLOAT_EQ(scheduler.getResult<float>(id), arg * arg);
}

TEST(TaskSchedulerTest, LargeArgs) {
    TTaskScheduler scheduler;
    float arg = 1e6;
    auto id = scheduler.add([](float x) { return x * x; }, arg);
    EXPECT_FLOAT_EQ(scheduler.getResult<float>(id), arg * arg);
}

TEST(TaskSchedulerTest, ZeroResult) {
    TTaskScheduler scheduler;
    float arg = 0;
    auto id = scheduler.add([](float x) { return x * x; }, arg);
    EXPECT_FLOAT_EQ(scheduler.getResult<float>(id), arg * arg);
}

TEST(TaskSchedulerTest, MultipleTasks) {
    TTaskScheduler scheduler;
    float arg = 10;
    auto id1 = scheduler.add([](float x) { return x * x; }, arg);
    auto id2 = scheduler.add([](float x) { return x + x; }, scheduler.getFutureResult<float>(id1));
    EXPECT_FLOAT_EQ(scheduler.getResult<float>(id2), (arg * arg) + (arg * arg));
}

TEST(TaskSchedulerTest, DoubleArgs) {
    TTaskScheduler scheduler;
    double arg1 = 3.14;
    double arg2 = 2.718;
    auto id = scheduler.add([](double x, double y) { return x * y; }, arg1, arg2);
    EXPECT_DOUBLE_EQ(scheduler.getResult<double>(id), arg1 * arg2);
}

TEST(TaskSchedulerTest, StringArgs) {
    TTaskScheduler scheduler;
    std::string arg1 = "Hello, ";
    std::string arg2 = "world!";
    auto id = scheduler.add([](const std::string& x, const std::string& y) { return x + y; }, arg1, arg2);
    EXPECT_EQ(scheduler.getResult<const char*>(id), arg1 + arg2);
}

TEST(TaskSchedulerTest, CharArgs) {
    TTaskScheduler scheduler;
    char arg1 = 'a';
    char arg2 = 'b';
    auto id = scheduler.add([](char x, char y) { return x + y; }, arg1, arg2);
    EXPECT_EQ(scheduler.getResult<int>(id), arg1 + arg2);
}

TEST(TaskSchedulerTest, BoolArgs) {
    TTaskScheduler scheduler;
    bool arg1 = true;
    bool arg2 = false;
    auto id = scheduler.add([](bool x, bool y) { return x && y; }, arg1, arg2);
    EXPECT_EQ(scheduler.getResult<bool>(id), arg1 && arg2);
}

TEST(TaskSchedulerTest, MixedTypes) {
    TTaskScheduler scheduler;
    int arg1 = 5;
    double arg2 = 2.5;
    auto id = scheduler.add([](int x, double y) { return x * y; }, arg1, arg2);
    EXPECT_DOUBLE_EQ(scheduler.getResult<double>(id), arg1 * arg2);
}

TEST(TaskSchedulerTest, EmptyArgs) {
    TTaskScheduler scheduler;
    auto id = scheduler.add([]() { return 42; });
    EXPECT_EQ(scheduler.getResult<int>(id), 42);
}


TEST(TaskSchedulerTest, IntegerFunctions) {
    TTaskScheduler scheduler;
    int a = 10;
    int b = 5;

    auto id1 = scheduler.add([](int x, int y) { return x + y; }, a, b);
    auto id2 = scheduler.add([](int x, int y) { return x - y; }, a, b);
    auto id3 = scheduler.add([](int x, int y) { return x * y; }, a, b);
    auto id4 = scheduler.add([](int x, int y) { return x / y; }, a, b);
    auto id5 = scheduler.add([](int x, int y) { return x % y; }, a, b);
    auto id6 = scheduler.add([](int x, int y) { return x * x - y * y; }, a, b);

    EXPECT_EQ(scheduler.getResult<int>(id1), a + b);
    EXPECT_EQ(scheduler.getResult<int>(id2), a - b);
    EXPECT_EQ(scheduler.getResult<int>(id3), a * b);
    EXPECT_EQ(scheduler.getResult<int>(id4), a / b);
    EXPECT_EQ(scheduler.getResult<int>(id5), a % b);
    EXPECT_EQ(scheduler.getResult<int>(id6), a * a - b * b);
}

TEST(TaskSchedulerTest, DoubleFunctions) {
    TTaskScheduler scheduler;
    double a = 3.14;
    double b = 2.718;

    auto id1 = scheduler.add([](double x, double y) { return x + y; }, a, b);
    auto id2 = scheduler.add([](double x, double y) { return x - y; }, a, b);
    auto id3 = scheduler.add([](double x, double y) { return x * y; }, a, b);
    auto id4 = scheduler.add([](double x, double y) { return x / y; }, a, b);
    auto id5 = scheduler.add([](double x, double y) { return std::pow(x, y); }, a, b);
    auto id6 = scheduler.add([](double x, double y) { return std::sqrt(x) + std::sqrt(y); }, a, b);

    EXPECT_DOUBLE_EQ(scheduler.getResult<double>(id1), a + b);
    EXPECT_DOUBLE_EQ(scheduler.getResult<double>(id2), a - b);
    EXPECT_DOUBLE_EQ(scheduler.getResult<double>(id3), a * b);
    EXPECT_DOUBLE_EQ(scheduler.getResult<double>(id4), a / b);
    EXPECT_DOUBLE_EQ(scheduler.getResult<double>(id5), std::pow(a, b));
    EXPECT_DOUBLE_EQ(scheduler.getResult<double>(id6), std::sqrt(a) + std::sqrt(b));
}


TEST(TaskSchedulerTest, FutureResultUsage) {
    TTaskScheduler scheduler;
    int a = 10;
    int b = 5;
    int c = 2;

    auto id1 = scheduler.add([](int x, int y) { return x + y; }, a, b);
    auto id2 = scheduler.add([](int x, int y) { return x * y; }, b, c);
    auto id3 = scheduler.add([](int x, int y) { return x - y; }, scheduler.getFutureResult<int>(id1), scheduler.getFutureResult<int>(id2));
    auto id4 = scheduler.add([](int x, int y) { return x / y; }, scheduler.getFutureResult<int>(id2), scheduler.getFutureResult<int>(id1));
    auto id5 = scheduler.add([](int x, int y) { return x % y; }, scheduler.getFutureResult<int>(id1), scheduler.getFutureResult<int>(id2));
    auto id6 = scheduler.add([](int x, int y) { return x * y; }, scheduler.getFutureResult<int>(id3), scheduler.getFutureResult<int>(id5));

    EXPECT_EQ(scheduler.getResult<int>(id3), (a + b) - (b * c));
    EXPECT_EQ(scheduler.getResult<int>(id4), (b * c) / (a + b));
    EXPECT_EQ(scheduler.getResult<int>(id5), (a + b) % (b * c));
    EXPECT_EQ(scheduler.getResult<int>(id6), ((a + b) - (b * c)) * ((a + b) % (b * c)));
}


TEST(TaskSchedulerTest, ComplexFutureResultUsage) {
    TTaskScheduler scheduler;
    double a = 3.0;
    double b = 2.0;
    int c = 4;
    int d = 3;

    auto id1 = scheduler.add([](double x, double y) { return std::pow(x, y); }, a, b);
    auto id2 = scheduler.add([](int x, int y) { return x + y; }, c, d);
    auto id3 = scheduler.add([](double x, int y) { return std::sqrt(x) * y; }, scheduler.getFutureResult<double>(id1), scheduler.getFutureResult<int>(id2));
    auto id4 = scheduler.add([](double x, int y) { return std::pow(x, y); }, scheduler.getFutureResult<double>(id1), scheduler.getFutureResult<int>(id2));
    auto id5 = scheduler.add([](double x, double y) { return std::cos(x) + std::sin(y); }, scheduler.getFutureResult<double>(id1), scheduler.getFutureResult<double>(id4));
    auto id6 = scheduler.add([](double x, double y) { return std::log(x) * std::exp(y); }, scheduler.getFutureResult<double>(id1), scheduler.getFutureResult<double>(id5));

    EXPECT_DOUBLE_EQ(scheduler.getResult<double>(id3), std::sqrt(std::pow(a, b)) * (c + d));
    EXPECT_DOUBLE_EQ(scheduler.getResult<double>(id4), std::pow(std::pow(a, b), (c + d)));
    EXPECT_DOUBLE_EQ(scheduler.getResult<double>(id5), std::cos(std::pow(a, b)) + std::sin(std::pow(std::pow(a, b), (c + d))));
    EXPECT_DOUBLE_EQ(scheduler.getResult<double>(id6), std::log(std::pow(a, b)) * std::exp(std::cos(std::pow(a, b)) + std::sin(std::pow(std::pow(a, b), (c + d)))));
}
