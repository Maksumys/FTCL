//
//  dispatcherMessage.hpp
//  diplom
//
//  Created by Максим Кузин on 05.05.16.
//  Copyright © 2016 Максим Кузин. All rights reserved.
//

#ifndef dispatcherMessage_hpp
#define dispatcherMessage_hpp

#include "../message/message.hpp"
#include "../queue/mutexqueue_oneLock.hpp"
#include "../solution.hpp"
#include "../console.hpp"
#include "../checkpoint/checkpoint.hpp"

#include <mpi.h>
#include <cstdio>

namespace ftcl {
    
    class CheckPoint;
    
    class DispatcherMessage {
    public:
        using QueueFreeNode = thread_queue_oneLock<std::size_t, std::mutex>;
        using QueueMessage = thread_queue_oneLock<Message*, std::mutex>;
        using QueueFinal = thread_queue_oneLock<OutArgs*, std::mutex>;
        
        DispatcherMessage(bool isCheckPoint);
        
        template<class T>
        void sendMessage(T *t, std::size_t numberTask, std::size_t typeMessage, std::size_t number) {
            std::string buff;
            switch (typeMessage) {
                case MESSAGE_READY:
                {
                    MessageReady *message = new MessageReady(typeMessage, _rank, true);
                    buff = message -> convertToString();
                    break;
                }
                case MESSAGE_COMPLETE_TASK:
                {
                    MessageCompleteTask* message = new MessageCompleteTask((OutArgs*)t, numberTask, typeMessage, _rank);
                    buff = message -> convertToString();
                    break;
                }
                case MESSAGE_TASK:
                {
                    MessageTask *message = new MessageTask((Task*)t, numberTask, typeMessage, _rank);
                    buff = message -> convertToString();
                    break;
                }
                case MESSAGE_CLOSE_NODE:
                {
                    MessageCloseNode *message = new MessageCloseNode(typeMessage, _rank);
                    buff = message -> convertToString();
                    break;
                }
                case MESSAGE_SECURE:
                {
                    MessageSecure *message = new MessageSecure(typeMessage, _rank);
                    buff = message -> convertToString();
                    break;
                }
                default:
                    return;
            }
            MPI::COMM_WORLD.Send(buff.c_str(), (int)buff.length(), MPI::CHAR, (int)number, (int)typeMessage);
        }
        
        Message *receiveMessage();
        std::size_t analyseHostMessage();
        void addMessage(Message *message);
        void addFinalTask(OutArgs *outArgs);
        
        std::size_t getFreeNode();
        std::size_t getSizeQueueFinal() const;
        std::size_t getSizeQueueMessage() const;
        std::size_t getReserveSizeQueueFinal() const;
        bool getClose() const;
        OutArgs *getSolution();
        
        void setSizeTask (const std::size_t size);
        void setClose(const bool close);
        void setCheckPoint(const bool isCheckPoint);
        
        
    private:
        QueueMessage _queueMessage;
        QueueFreeNode _freeNode;
        QueueFinal _final;
        Console _console;
        CheckPoint *_checkPoint;
        
        std::size_t _rank;
        std::size_t _size;
        bool _close;
        bool _isCheckPoint;
    };
    
}

#endif /* dispatcherMessage_hpp */
