//
//  message.cpp
//  diplom
//
//  Created by Максим Кузин on 05.05.16.
//  Copyright © 2016 Максим Кузин. All rights reserved.
//

#include "message.hpp"

#include <sstream>

namespace ftcl {
    Message::Message() {
        _typeMessage = 0;
        _rank = 0;
    }
    
    Message::Message( std::size_t typeMessage, std::size_t rank) : _typeMessage(typeMessage), _rank(rank) {}
    
    std::size_t Message::getTypeMessage() {
        return _typeMessage;
    }
    
    std::size_t Message::getRank() {
        return _rank;
    }
    
    Task* Message::getTask() {
        return nullptr;
    }
    
    std::size_t Message::getNumberTask() {
        return 0;
    }
    
    OutArgs* Message::getOutArgs() {
        return nullptr;
    }
    
    
    
    MessageReady::MessageReady() {
        _ready = false;
    }
    
    MessageReady::MessageReady(std::size_t typeMessage, std::size_t rank, bool ready) : Message(typeMessage, rank), _ready(ready) {}
    
    std::string MessageReady::convertToString() {
        std::ostringstream file;
        {
            boost::archive::text_oarchive oa(file);
            oa << this;
        }
        return file.str();
    }
    
    MessageReady* MessageReady::convertToObject( std::string byte ) {
        MessageReady* b;
        std::istringstream file(byte.c_str());
        boost::archive::text_iarchive as(file);
        as & b;
        return b;
    }
    
    bool MessageReady::getReady() {
        return _ready;
    }
    
    
    
    MessageTask::MessageTask() {
        _task =nullptr;
    }
    
    MessageTask::MessageTask( Task* task, std::size_t numberTask, std::size_t typeMessage, std::size_t rank) : Message( typeMessage, rank) {
        _task = task;
        _numberTask = numberTask;
    }
    
    std::string MessageTask::convertToString() const {
        std::ostringstream file;
        {
            boost::archive::text_oarchive oa(file);
            oa << this;
        }
        return file.str();
    }
    
    Task* MessageTask::getTask() {
        return _task;
    }
    
    std::size_t MessageTask::getNumberTask() {
        return _numberTask;
    }
    
    MessageTask* MessageTask::convertToObject( const std::string byte ) {
        MessageTask* b;
        std::istringstream file(byte.c_str());
        boost::archive::text_iarchive as(file);
        as & b;
        return b;
    }
    
    MessageTask::~MessageTask() {
        delete _task;
    }
    
    
    
    MessageCompleteTask::MessageCompleteTask() : Message() {
        _outArgs = nullptr;
    }
    
    MessageCompleteTask::MessageCompleteTask( OutArgs* outArgs, std::size_t numberTask, std::size_t typeMessage, std::size_t rank) : Message(typeMessage, rank) {
        _outArgs = outArgs;
        _numberTask = numberTask;
    }
    
    std::string MessageCompleteTask::convertToString() const {
        std::ostringstream file;
        {
            boost::archive::text_oarchive oa(file);
            oa << this;
        }
        return file.str();
    }
    
    OutArgs* MessageCompleteTask::getOutArgs() {
        return _outArgs;
    }
    
    std::size_t MessageCompleteTask::getNumberTask() {
        return _numberTask;
    }
    
    MessageCompleteTask* MessageCompleteTask::convertToObject(const std::string byte) {
        MessageCompleteTask* b;
        std::istringstream file(byte.c_str());
        boost::archive::text_iarchive as(file);
        as & b;
        return b;
    }
    
    
    
    MessageCloseNode::MessageCloseNode() : Message() {}
    
    MessageCloseNode::MessageCloseNode(std::size_t typeMessage, std::size_t rank) : Message(typeMessage, rank) {}
    
    std::string MessageCloseNode::convertToString() const {
        std::ostringstream file;
        {
            boost::archive::text_oarchive oa(file);
            oa << this;
        }
        return file.str();
    }
    
    MessageCloseNode* MessageCloseNode::convertToObject(const std::string byte) {
        MessageCloseNode* b;
        std::istringstream file(byte.c_str());
        boost::archive::text_iarchive as(file);
        as & b;
        return b;
    }

    
    
    MessageSecure::MessageSecure(std::size_t typeMessage, std::size_t rank) : Message(typeMessage, rank) {}
    
    std::string MessageSecure::convertToString() const {
        std::ostringstream file;
        {
            boost::archive::text_oarchive oa(file);
            oa << this;
        }
        return file.str();
    }
    
    MessageSecure* MessageSecure::convertToObject(const std::string byte) {
        MessageSecure* b;
        std::istringstream file(byte.c_str());
        boost::archive::text_iarchive as(file);
        as & b;
        return b;
    }

}