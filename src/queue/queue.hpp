//
//  queue.hpp
//  diplom
//
//  Created by Максим Кузин on 05.05.16.
//  Copyright © 2016 Максим Кузин. All rights reserved.
//

#ifndef queue_hpp
#define queue_hpp

#include <memory>

template <class type_value>
class queue {
protected:
    std::unique_ptr<type_value[]> mas_queue;
    std::size_t length;
    std::size_t head;
    std::size_t tail;
    std::size_t distance;
public:
    queue()  {
        length = 0;
        head = 0;
        tail = 0;
        distance = 0;
    }
    
    explicit queue(const std::size_t n) : mas_queue(new type_value[n]) {
        mas_queue.reset(new type_value[n]);
        length = n;
        head = n - 1;
        tail = n - 1;
        distance = n;
    }
    
    void reserveLength(const std::size_t n) {
        mas_queue.reset(new type_value[n]);
        length = n;
        head = n - 1;
        tail = n - 1;
        distance = n;
    }
    
    bool isEmpty() {
        if(length == distance) {
            return true;
        }
        return false;
    }
    
    explicit queue(const queue &o) {
        mas_queue.reset(new type_value[o.length]);
        for (std::size_t i = 0; i < o.length; i ++) {
            mas_queue[i] = o.mas_queue[i];
        }
        length = o.length;
        head = o.head;
        tail = o.tail;
        distance = o.distance;
    }
    
    //Не правильно работает если очередь полна
    virtual void push(const type_value elem) {
        if (distance > 0) {
            mas_queue[tail] = elem;
            (tail == 0) ? tail = length - 1 : tail--;
            distance--;
        }
    }
    //Не правильно работает если очередь пуста
    virtual void pop() {
        if(distance != length) {
            (head == 0) ? head = length - 1 : head--;
            distance++;
        }
    }
    //Не правильно работает если очередь пуста
    virtual type_value back() const {
        return mas_queue[tail];
    }
    
    virtual type_value front() const {
        return mas_queue[head];
    }
    
    virtual std::size_t getLength()const {
        return length;
    }
    
    virtual std::size_t size() const {
        return length - distance;
    }
    
    virtual std::size_t getDistance() const {
        return distance;
    }
};


#endif /* queue_hpp */
