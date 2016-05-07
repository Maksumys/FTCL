//
//  solution.hpp
//  diplom
//
//  Created by Максим Кузин on 05.05.16.
//  Copyright © 2016 Максим Кузин. All rights reserved.
//

#ifndef solution_hpp
#define solution_hpp

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace ftcl {
    
    class EntryArgs {
    public:
        EntryArgs() = default;
        virtual ~EntryArgs() = default;
    };
    
    class OutArgs {
    public:
        OutArgs() = default;
        virtual void consoleOut();
        virtual ~OutArgs();
        
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version) {}
    };
    
    class Task {
    public:
        Task() = default;
        virtual OutArgs *solution();
        virtual ~Task() = default;
    private:
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version) {}
    };
    
    class MessageNode {
    public:
        MessageNode() = default;
        MessageNode(Task* task, std::size_t numberTask);
        void setNumberTask(const std::size_t numberTask);
        void setNumberNode(const std::size_t numberNode);
        void setTrues(const bool trues);
        void setTime(std::chrono::time_point<std::chrono::system_clock> lastSecure);
        Task* getTask();
        std::size_t getNumberTask();
        std::size_t getNumberNode();
        bool getTrues();
        std::chrono::time_point<std::chrono::system_clock> getTime();
        ~MessageNode();
    private:
        Task* _task;
        std::size_t _numberTask;
        std::size_t _numberNode;
        bool _trues;
        std::chrono::time_point<std::chrono::system_clock> _lastSecure;
    };
    
    class TaskBuilder {
    public:
        TaskBuilder() = default;
        virtual void create(EntryArgs *args);
        virtual std::vector<MessageNode*> getTasks()const;
        virtual ~TaskBuilder() = default;
    protected:
        std::vector<MessageNode*> _tasks;
    };
}

#endif /* solution_hpp */
