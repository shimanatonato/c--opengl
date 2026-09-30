#pragma once
#include <random>

// ランダム関数
namespace random{
    // 整数乱数を返す
    inline int RandInt(const int a, const int b){
        static std::mt19937 random_engine{std::random_device{}()};  // 乱数生成器
        const int lower = std::min(a, b);
        const int upper = std::max(a, b);
        return std::uniform_int_distribution<int>(lower,upper)(random_engine);
    };
}