//
//  message.hpp
//  diplom
//
//  Created by Максим Кузин on 05.05.16.
//  Copyright © 2016 Максим Кузин. All rights reserved.
//

#ifndef message_hpp
#define message_hpp

#include <cstdio>
#include "../solution.hpp"

namespace ftcl {
    
    constexpr std::size_t MESSAGE_READY = 1;
    constexpr std::size_t MESSAGE_TASK = 2;
    constexpr std::size_t MESSAGE_COMPLETE_TASK = 3;
    constexpr std::size_t MESSAGE_CLOSE_NODE = 4;
    constexpr std::size_t MESSAGE_SECURE = 5;
    
    class Message {
    public:
        Message();
        Message( std::size_t typeMessage, std::size_t rank);
        std::size_t getTypeMessage();
        std::size_t getRank();
        virtual Task* getTask();
        virtual std::size_t getNumberTask();
        virtual OutArgs* getOutArgs();
        virtual ~Message() = default;
    private:
        friend class boost::serialization::access;
        template< class Archive >
        void serialize( Archive & ar, const unsigned int version )
        {
            ar & _typeMessage;
            ar & _rank;
        }
        std::size_t _typeMessage;
        std::size_t _rank;
    };
    
    class MessageReady : public Message {
    public:
        MessageReady();
        MessageReady(std::size_t typeMessage, std::size_t rank, bool ready);
        std::string convertToString();
        static MessageReady* convertToObject( std::string byte );
        bool getReady();
        virtual ~MessageReady() override = default;
    private:
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version) {
            ar & boost::serialization::base_object<Message>(*this);
            ar & _ready;
        }
        bool _ready;
    };
    
    class MessageTask : public Message {
    public:
        MessageTask();
        MessageTask(Task* task, std::size_t numberTask, std::size_t typeMessage, std::size_t rank);
        std::string convertToString() const;
        virtual Task* getTask() override;
        virtual std::size_t getNumberTask() override;
        static MessageTask* convertToObject(const std::string byte);
        virtual ~MessageTask() override;
    private:
        friend class boost::serialization::access;
        template< class Archive >
        void serialize( Archive & ar, const unsigned int version )
        {
            ar & boost::serialization::base_object<Message>(*this);
            ar & _task;
            ar & _numberTask;
        }
        Task* _task;
        std::size_t _numberTask;
    };
    
    class MessageCompleteTask : public Message {
    public:
        MessageCompleteTask();
        MessageCompleteTask( OutArgs* outArgs, std::size_t numberTask, std::size_t typeMessage, std::size_t rank);
        std::string convertToString() const;
        virtual OutArgs* getOutArgs() override;
        virtual std::size_t getNumberTask() override;
        static MessageCompleteTask* convertToObject(const std::string byte);
    private:
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version)
        {
            ar & boost::serialization::base_object<Message>(*this);
            ar & _outArgs;
            ar & _numberTask;
        }
        
        OutArgs* _outArgs;
        std::size_t _numberTask;
    };
    
    class MessageCloseNode : public Message {
    public:
        MessageCloseNode();
        MessageCloseNode(std::size_t typeMessage, std::size_t rank);
        static MessageCloseNode* convertToObject(const std::string byte);
        std::string convertToString() const;
    private:
        friend class boost::serialization::access;
        template< class Archive >
        void serialize( Archive & ar, const unsigned int version ) {
            ar & boost::serialization::base_object<Message>(*this);
        }
    };
    
    
    class MessageSecure : public Message {
    public:
        MessageSecure() = default;
        MessageSecure(std::size_t typeMessage, std::size_t rank);
        std::string convertToString() const;
        static MessageSecure* convertToObject(const std::string byte);
    private:
        friend class boost::serialization::access;
        template< class Archive >
        void serialize( Archive & ar, const unsigned int version ) {
            ar & boost::serialization::base_object<Message>(*this);
        }
    };
    
}

#endif /* message_hpp */
