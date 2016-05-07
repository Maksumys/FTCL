//
//  checkpoint.hpp
//  diplom
//
//  Created by Максим Кузин on 05.05.16.
//  Copyright © 2016 Максим Кузин. All rights reserved.
//

#ifndef checkpoint_hpp
#define checkpoint_hpp

#include "../dispatcher/dispatcherMessage.hpp"
#include "../solution.hpp"
#include "../dispatcher/dispatcherNode.hpp"
#include "../message/message.hpp"

#include <vector>

namespace ftcl {
    class DispatcherMessage;
    
    class Elem {
    public:
        Elem() = default;
        Elem(OutArgs* outArgs, std::size_t numberInVectorTask);
        
        std::size_t getNumberInVectorTask() const;
        OutArgs *getOutArgs() const;
        ~Elem() = default;
        
    private:
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version) {
            ar & _numberInVectorTask;
            ar & _outArgs;
        }
        
        std::size_t _numberInVectorTask;
        OutArgs* _outArgs;
    };
    
    
    class CheckPoint {
    public:
        CheckPoint() = default;
        void save(Message *message);
        void load(DispatcherMessage *dispMsg, DispatcherNode *dispNode);
    private:
        Console console;
    };
}

#endif /* checkpoint_hpp */
