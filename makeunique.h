//
// Created by 刘松洋 on 2018/6/21.
//

#ifndef UNTITLED_MAKEUNIQUE_H
#define UNTITLED_MAKEUNIQUE_H
#include <iostream>
#include <memory>

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

#endif //UNTITLED_MAKEUNIQUE_H
