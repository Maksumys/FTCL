//
//  dispatcher.hpp
//  diplom
//
//  Created by Максим Кузин on 05.05.16.
//  Copyright © 2016 Максим Кузин. All rights reserved.
//

#ifndef dispatcher_hpp
#define dispatcher_hpp

#include "../solution.hpp"
#include "dispatcherNode.hpp"
#include "dispatcherMessage.hpp"
#include "../checkpoint/checkpoint.hpp"

#include <cstdio>

namespace ftcl {
    
    constexpr std::size_t DEFAULT_HOST_SECURE_TIME_SECONDS = 30;
    constexpr std::size_t WITHOUT_TASK = 0;
    
    class Dispatcher {
    public:
        Dispatcher() = delete;
        Dispatcher(const Dispatcher &o) = delete;
        Dispatcher(const Dispatcher &&o) = delete;
        Dispatcher(int *argc, char ***argv);
        Dispatcher(int *argc, char ***argv, TaskBuilder *taskBuilder,
                   const std::size_t size, const std::size_t secondsSecure,
                   const bool isCheckPointRestore, const bool isCheckPoint);
        
        void run(EntryArgs *args);
        void runNodeFirst(EntryArgs *args);
        void runNodeSecond();
        void runNodeOther();
        
        void setTypeTaskBuilder(TaskBuilder *taskBuilder);
        void setSizeTask(const std::size_t size);
        void setSecondsSecure(const std::size_t secondsSecure);
        void setCheckPoint(const bool isCheakPoint);
        void setCheckPointRestore(const bool isCheakPointRestore);
        
        std::size_t getSizeQueueFinal() const;
        OutArgs *getSolution() const;
        bool getCheckPoint() const;
        bool getCheakPointRestore() const;
        bool isHost() const;
        
        void closeNode() const;
        
        ~Dispatcher();
    private:
        void messageToQueue();
        void messageAnalyseHost();
        void messageAnalyseNode();
        void taskSendAndControl();
        void checkPointRestore();
        
        DispatcherNode *_dispatcherNode;
        DispatcherMessage *_dispatcherMessage;
        Console _console;
        CheckPoint *_checkPoint;
        
        std::size_t _rank;
        std::size_t _maxRank;
        bool _isCheakPointRestore;
        bool _isCheckPoint;
    };
}

#endif /* dispatcher_hpp */
