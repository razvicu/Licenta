//
//  ThreadSafeVector.hpp
//  Licenta
//
//  Created by Razvan on 6/2/20.
//  Copyright © 2020 Razvan. All rights reserved.
//

#ifndef ThreadSafeVector_hpp
#define ThreadSafeVector_hpp

#include <vector>
#include <mutex>

template <class T>
class ThreadSafeVector {
public:
    ThreadSafeVector() {}
    void setVector(std::vector<T>);
    void clear();
    void push_back(T);
    std::vector<T> get();
private:
    std::mutex mtx;
    std::vector<T> vec;
};

template <class T>
void ThreadSafeVector<T>::push_back(T val) {
    std::lock_guard<std::mutex> lk(mtx);
    vec.push_back(val);
}

template <class T>
std::vector<T> ThreadSafeVector<T>::get() {
    std::lock_guard<std::mutex> lk(mtx);
    return vec;
}

template <class T>
void ThreadSafeVector<T>::clear() {
    std::lock_guard<std::mutex> lk(mtx);
    vec.clear();
}

template <class T>
void ThreadSafeVector<T>::setVector(std::vector<T> vec_) {
    std::lock_guard<std::mutex> lk(mtx);
    vec = vec_;
}

#endif /* ThreadSafeVector_hpp */
