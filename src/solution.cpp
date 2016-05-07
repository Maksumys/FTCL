//
//  solution.cpp
//  diplom
//
//  Created by Максим Кузин on 05.05.16.
//  Copyright © 2016 Максим Кузин. All rights reserved.
//

#include "solution.hpp"

namespace ftcl {
    
    void OutArgs::consoleOut() {}
    OutArgs::~OutArgs() {}
    
    
    
    
    OutArgs *Task::solution() {
        return nullptr;
    }
    
    
    
    
    MessageNode::MessageNode(Task* task, std::size_t numberTask) : _task(task), _numberTask(numberTask),
                                                                   _numberNode(0), _trues(false)
    {
        _lastSecure = std::chrono::system_clock::now();
    }
    
    void MessageNode::setNumberTask(const std::size_t numberTask) {
        _numberTask = numberTask;
    }
    
    void MessageNode::setNumberNode(const std::size_t numberNode) {
        _numberNode = numberNode;
    }
    
    void MessageNode::setTrues(const bool trues) {
        _trues = trues;
    }
    
    void MessageNode::setTime(std::chrono::time_point<std::chrono::system_clock> lastSecure) {
        _lastSecure = lastSecure;
    }
    
    Task* MessageNode::getTask() {
        return _task;
    }
    
    std::size_t MessageNode::getNumberTask() {
        return _numberTask;
    }
    
    std::size_t MessageNode::getNumberNode() {
        return _numberNode;
    }
    
    bool MessageNode::getTrues() {
        return _trues;
    }
    
    std::chrono::time_point<std::chrono::system_clock> MessageNode::getTime() {
        return _lastSecure;
    }
    
    MessageNode::~MessageNode() {
        delete _task;
    }
    
    
    
    void TaskBuilder::create(EntryArgs *args) {}
    
    std::vector<MessageNode*> TaskBuilder::getTasks() const {
        return _tasks;
    }

}