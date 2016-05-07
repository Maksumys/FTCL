//
//  dispatcherNode.hpp
//  diplom
//
//  Created by Максим Кузин on 05.05.16.
//  Copyright © 2016 Максим Кузин. All rights reserved.
//

#ifndef dispatcherNode_hpp
#define dispatcherNode_hpp

#include "solution.hpp"
#include "console.hpp"

namespace ftcl {
    
    class DispatcherNode {
    public:
        DispatcherNode(const std::size_t rank, const std::size_t secondsSecure);
        DispatcherNode(const DispatcherNode& o) = delete;
        DispatcherNode(const DispatcherNode&& o) = delete;
        
        void buildTaks(EntryArgs* args);
        void checkTimeNode();
        void setTimeNode(std::size_t rank);
        void setTypeBuilder(TaskBuilder* builder);
        void setSecondsSecure(const std::size_t millisecondsSecure);
        void setNumberNode(std::size_t number, std::size_t rank);
        void setTrues(std::size_t numberTask);
        
        std::size_t getSizeTask() const;
        std::tuple<Task*, std::size_t> getFreeTask();
        
        ~DispatcherNode();
    private:
        std::vector<MessageNode*> _infoNode;
        TaskBuilder *_builder;
        std::size_t _rank;
        Console _console;
        std::size_t _secondsSecure;
    };
}

#endif /* dispatcherNode_hpp */
