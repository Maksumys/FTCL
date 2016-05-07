//
//  dispatcherMessage.cpp
//  diplom
//
//  Created by Максим Кузин on 05.05.16.
//  Copyright © 2016 Максим Кузин. All rights reserved.
//

#include "dispatcherMessage.hpp"
#include "../checkpoint/checkpoint.hpp"

#include <fstream>
#include <string>

namespace ftcl {
    
    DispatcherMessage::DispatcherMessage(bool isCheckPoint) {
        _rank = (size_t) MPI::COMM_WORLD.Get_rank();
        _size = (size_t) MPI::COMM_WORLD.Get_size();
        _queueMessage.reserveLength(_size * 2);
        _freeNode.reserveLength(_size + 2);
        _close = false;
        _isCheckPoint = isCheckPoint;
        if(_isCheckPoint) {
            _checkPoint = new CheckPoint();
        }
        else {
            _checkPoint = nullptr;
        }
    }
    
    Message *DispatcherMessage::receiveMessage() {
        MPI::Status status;
        if (MPI::COMM_WORLD.Iprobe(MPI::ANY_SOURCE, MPI::ANY_TAG, status)) {
            const auto sender = status.Get_source();
            const auto n_bytes = status.Get_count(MPI::CHAR);
            const auto tag = status.Get_tag();
            const auto buff = new char[n_bytes];
            MPI::COMM_WORLD.Recv(buff, n_bytes, MPI::CHAR, sender, tag);
            switch (tag) {
                case MESSAGE_TASK: {
                    Message* msg = MessageTask::convertToObject(std::string(buff, (std::size_t) n_bytes));
                    return msg;
                }
                case MESSAGE_READY: {
                    Message* msg = MessageReady::convertToObject(std::string(buff, (std::size_t) n_bytes));
                    return msg;
                }
                case MESSAGE_COMPLETE_TASK: {
                    Message* msg = MessageCompleteTask::convertToObject(std::string(buff, (std::size_t) n_bytes));
                    return msg;
                }
                case MESSAGE_CLOSE_NODE: {
                    Message* msg = MessageCloseNode::convertToObject(std::string(buff, (std::size_t) n_bytes));
                    return msg;
                }
                case MESSAGE_SECURE: {
                    Message* msg = MessageSecure::convertToObject(std::string(buff, (std::size_t) n_bytes));
                    return msg;
                }
                default:
                    return nullptr;
            }
        }
        return nullptr;
    }
    
    std::size_t DispatcherMessage::analyseHostMessage() {
        if (!_queueMessage.isEmpty()) {
            Message *message = _queueMessage.front();
            switch (message->getTypeMessage()) {
                case MESSAGE_READY: {
                    _freeNode.push(message->getRank());
                    _queueMessage.pop();
                    return 0;
                }
                case MESSAGE_COMPLETE_TASK: {
                    _final.push(message->getOutArgs());
                    _freeNode.push(message->getRank());
                    if(_isCheckPoint) {
                        _checkPoint->save(message);
                    }
                    _queueMessage.pop();
                    message->getNumberTask();
                    return 0;
                }
                case MESSAGE_SECURE: {
                    return message->getRank();
                }
                default:
                    return 0;
            }
            
        }
        return 0;
    }
    
    std::size_t DispatcherMessage::getFreeNode() {
        if (!_freeNode.isEmpty()) {
            std::size_t number = _freeNode.front();
            _freeNode.pop();
            return number;
        }
        return 0;
    }
    
    void DispatcherMessage::addMessage(Message *message) {
        _queueMessage.push(message);
    }
    
    void DispatcherMessage::addFinalTask(OutArgs *outArgs) {
        _final.push(outArgs);
    }
    
    std::size_t DispatcherMessage::getSizeQueueFinal() const {
        return _final.size();
    }
    
    std::size_t DispatcherMessage::getSizeQueueMessage() const {
        return _queueMessage.size();
    }
    
    std::size_t DispatcherMessage::getReserveSizeQueueFinal() const {
        return _final.getLength();
    }
    
    bool DispatcherMessage::getClose() const {
        return _close;
    }
    
    void DispatcherMessage::setClose(const bool close) {
        _close = close;
    }
    
    OutArgs *DispatcherMessage::getSolution() {
        OutArgs *buff = _final.front();
        _final.pop();
        return buff;
    }
    
    void DispatcherMessage::setSizeTask(const std::size_t size) {
        _final.reserveLength(size);
    }
    
    void DispatcherMessage::setCheckPoint(const bool isCheckPoint) {
        _isCheckPoint = isCheckPoint;
        if((_isCheckPoint) && (_checkPoint == nullptr)) {
            _checkPoint = new CheckPoint();
        }
        else if(!_isCheckPoint) {
            _checkPoint = nullptr;
        }
    }
    
}