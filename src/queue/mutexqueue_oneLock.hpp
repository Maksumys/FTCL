//
//  mutexqueue_oneLock.hpp
//  diplom
//
//  Created by Максим Кузин on 05.05.16.
//  Copyright © 2016 Максим Кузин. All rights reserved.
//

#ifndef mutexqueue_oneLock_hpp
#define mutexqueue_oneLock_hpp

#include "queue.hpp"
#include <iostream>
#include <mutex>
#include <condition_variable>

//потокобезопасная очередь с одним мьютексом
template<class type_value, class mutex_type>
class thread_queue_oneLock : public queue<type_value> {
    mutex_type mutex_lock;
    std::condition_variable_any condition_head;
    std::condition_variable_any condition_tail;
public:
    thread_queue_oneLock() {}
    explicit thread_queue_oneLock(const std::size_t n) : queue<type_value>(n) {}
    
    void push_without_lock( const type_value elem ) {
        std::unique_lock<mutex_type> tail_lock(mutex_lock);
        if (this -> distance > 0) {
            this -> mas_queue[this -> tail] = elem;
            (this -> tail == 0) ? this -> tail = this -> length - 1 : this -> tail--;
            this -> distance--;
        }
        else {
            
        }
    }
    
    virtual void push( const type_value elem ) {
        std::unique_lock<mutex_type> tail_lock(mutex_lock);
        condition_head.wait(tail_lock, [&] {
            if(this -> distance > 0) {
                return true;
            }
            condition_tail.notify_one();
            return false;
        });
        this -> mas_queue[this -> tail] = elem;
        (this -> tail == 0) ? this -> tail = this -> length - 1 : this -> tail--;
        this -> distance--;
        condition_tail.notify_one();
    }
    bool try_push( const type_value elem ) {
        if (mutex_lock.try_lock()) {
            if (this -> distance > 0) {
                this -> mas_queue[this -> tail] = elem;
                (this -> tail == 0) ? this -> tail = this -> length - 1 : this -> tail--;
                this -> distance--;
                mutex_lock.unlock();
                return true;
            }
            mutex_lock.unlock();
        }
        return false;
    }
    
    virtual void pop() {
        std::unique_lock<mutex_type> head_lock(mutex_lock);
        condition_tail.wait(head_lock, [&] {
            if(this -> distance != this -> length) {
                return true;
            }
            condition_head.notify_one();
            return false;
        });
        (this -> head == 0) ? this -> head = this -> length - 1 : this -> head--;
        this -> distance++;
        condition_head.notify_one();
    }
    bool try_pop() {
        if (mutex_lock.try_lock()) {
            if (this -> distance != this -> length) {
                (this -> head == 0) ? this -> head = this -> length - 1 : this -> head--;
                this -> distance++;
                mutex_lock.unlock();
                return true;
            }
        }
        mutex_lock.unlock();
        return false;
    }
    virtual type_value front() {
        std::lock_guard<mutex_type> lock(mutex_lock);
        return this -> mas_queue[this -> head];
    }
    virtual type_value back() {
        std::lock_guard<mutex_type> lock(mutex_lock);
        return this -> mas_queue[this -> tail];
    }
    virtual std::size_t getDistance() {
        std::lock_guard<mutex_type> lock(mutex_lock);
        return this -> distance;
    }
};


#endif /* mutexqueue_oneLock_hpp */
