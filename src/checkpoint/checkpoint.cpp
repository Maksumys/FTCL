//
//  checkpoint.cpp
//  diplom
//
//  Created by Максим Кузин on 05.05.16.
//  Copyright © 2016 Максим Кузин. All rights reserved.
//

#include "checkpoint.hpp"

#include <boost/serialization/vector.hpp>
#include <fstream>

namespace ftcl {
    
    Elem::Elem(OutArgs* outArgs, std::size_t numberInVectorTask) {
        _outArgs = outArgs;
        _numberInVectorTask = numberInVectorTask;
    }
    
    std::size_t Elem::getNumberInVectorTask() const {
        return _numberInVectorTask;
    }
    
    OutArgs *Elem::getOutArgs() const {
        return _outArgs;
    }

    void CheckPoint::save(Message *message) {
        std::ofstream out("check.txt", std::ios::app);
        console.ConsoleOut("Сделана задача: " + std::to_string(message -> getNumberTask()));
        if(out) {
            //out << "#" << std::endl;
            boost::archive::text_oarchive out2(out);
            Elem check(message -> getOutArgs(), message -> getNumberTask());
            out2 << check;
            out.flush();
        }
        out.close();
    }

    void CheckPoint::load(DispatcherMessage *dispMsg, DispatcherNode *dispNode) {
        console.ConsoleOut("Восстановление из контрольной точки включено");
        std::fstream check("check.txt");
        std::size_t count = 0;
        if(check) {
            console.ConsoleOut("Начинаю восстановление из глобальной контрольной точки");
            console.ConsoleOut("Всего задач: " + std::to_string(dispMsg -> getReserveSizeQueueFinal()));
            while (!check.eof()) {
                Elem out;
                try {
                    boost::archive::text_iarchive ia(check);
                    ia >> out;
                }
                catch(...) {
                    break;
                }
                console.ConsoleOut("Восстанавливаю задачу: " + std::to_string(out.getNumberInVectorTask()));
                count++;
                dispMsg -> addFinalTask(out.getOutArgs());
                dispNode -> setTrues(out.getNumberInVectorTask());
            }
            console.ConsoleOut("Восстановлено из контрольной точки " + std::to_string(count) + "/" + std::to_string(dispMsg -> getReserveSizeQueueFinal()));
        }
        else {
            console.ConsoleOut("Файл контрольной точки не найден");
        }
        check.close();
    }
    
}
