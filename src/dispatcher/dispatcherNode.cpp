//
//  dispatcherNode.cpp
//  diplom
//
//  Created by Максим Кузин on 05.05.16.
//  Copyright © 2016 Максим Кузин. All rights reserved.
//

#include "dispatcherNode.hpp"

#include <vector>
#include <chrono>
#include <string>
#include <tuple>

namespace ftcl {
    
    DispatcherNode::DispatcherNode(const std::size_t rank, const std::size_t secondsSecure) {
        _rank = rank;
        _secondsSecure = secondsSecure;
        _builder = nullptr;
    }
    
    void DispatcherNode::buildTaks(EntryArgs *args) {
        _builder -> create(args);
        delete args;
        _infoNode = _builder -> getTasks();
        
        delete _builder;
        _builder = nullptr;
    }
    
    void DispatcherNode::checkTimeNode() {
        for(auto elem : _infoNode) {
            if((elem -> getNumberNode() != 0) && (elem -> getTrues() == false)
               && (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - elem -> getTime()).count() > _secondsSecure)) {
                elem -> setNumberNode(0);
            }
        }
    }
    
    void DispatcherNode::setTimeNode(std::size_t rank) {
        for(auto elem : _infoNode) {
            if(elem->getNumberNode() == rank) {
                elem -> setTime(std::chrono::system_clock::now());
            }
        }
    }
    
    void DispatcherNode::setTypeBuilder(TaskBuilder *builder) {
        if (_builder != nullptr) {
            delete _builder;
        }
        _builder = builder;
    }
    
    void DispatcherNode::setTrues(std::size_t numberTask) {
        _infoNode[numberTask]->setTrues(true);
    }
    
    void DispatcherNode::setSecondsSecure(std::size_t secondsSecure) {
        _secondsSecure = secondsSecure;
    }
    
    std::size_t DispatcherNode::getSizeTask() const {
        return _infoNode.size();
    }
    
    std::tuple<Task*, std::size_t> DispatcherNode::getFreeTask() {
        for (std::size_t i = 0; i < _infoNode.size(); i++) {
            if (((_infoNode[i] -> getNumberNode()) == 0) && (_infoNode[i] -> getTrues() == false)) {
                //Костыль//
                _infoNode[i] -> setNumberNode(1);
                ///////////
                return std::make_tuple(_infoNode[i] -> getTask(), i);
            }
        }
        return std::make_tuple(nullptr, 0);
    }
    
    void DispatcherNode::setNumberNode(std::size_t number, std::size_t rank) {
        _infoNode[number] -> setNumberNode(rank);
    }
    
    DispatcherNode::~DispatcherNode() {
        if (_builder != nullptr) {
            delete _builder;
        }
        
        for (std::size_t i = 0; i < _infoNode.size(); i++) {
            delete _infoNode[i];
        }
    }
    
}