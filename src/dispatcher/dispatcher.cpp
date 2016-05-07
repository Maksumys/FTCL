//
//  dispatcher.cpp
//  diplom
//
//  Created by Максим Кузин on 05.05.16.
//  Copyright © 2016 Максим Кузин. All rights reserved.
//

#include "dispatcher.hpp"
#include "../checkpoint/checkpoint.hpp"

#include <thread>
#include <fstream>

namespace ftcl {
    
    Dispatcher::Dispatcher(int *argc, char ***argv) {
        MPI::Init(*argc, *argv);
        MPI::COMM_WORLD.Set_errhandler(MPI::ERRORS_THROW_EXCEPTIONS);
        _rank = (size_t) MPI::COMM_WORLD.Get_rank();
        _maxRank = (size_t) MPI::COMM_WORLD.Get_size();
        if (_rank == 0) {
            _dispatcherNode = new DispatcherNode(_rank, DEFAULT_HOST_SECURE_TIME_SECONDS);
        }
        else {
            _dispatcherNode = nullptr;
        }
        _checkPoint = nullptr;
        _dispatcherMessage = new DispatcherMessage(false);
        _isCheakPointRestore = false;
        _isCheckPoint = false;
    }
    
    Dispatcher::Dispatcher(int *argc, char ***argv, TaskBuilder *taskBuilder,
                           const std::size_t size, const std::size_t secondsSecure,
                           const bool isCheakPointRestore, const bool isCheckPoint)
    {
        MPI::Init(*argc, *argv);
        _rank = (size_t) MPI::COMM_WORLD.Get_rank();
        _maxRank = (size_t) MPI::COMM_WORLD.Get_size();
        if (_rank == 0) {
            _dispatcherNode = new DispatcherNode(_rank, secondsSecure);
            _dispatcherNode -> setTypeBuilder(taskBuilder);
            _dispatcherNode -> setSecondsSecure(secondsSecure);
        }
        else {
            _dispatcherNode = nullptr;
        }
        _dispatcherMessage = new DispatcherMessage(isCheckPoint);
        _dispatcherMessage -> setSizeTask(size);
        
        _isCheakPointRestore = isCheakPointRestore;
        _isCheckPoint = isCheckPoint;
        
        if(_isCheakPointRestore) {
            _checkPoint = new CheckPoint();
        }
        else {
            _checkPoint = nullptr;
        }
    }
    
    void Dispatcher::run(EntryArgs *args) {
        if (_rank == 0) {
            runNodeFirst(args);
        }
        else if (_rank == 1) {
            runNodeSecond();
        }
        else if (_rank > 1) {
            runNodeOther();
        }
    }
    
    void Dispatcher::runNodeFirst(EntryArgs *args) {
        //поток создания задач, контроля и рассылки задач на узлы
        std::thread thread1([&] {
            _dispatcherNode -> buildTaks(args);
            _dispatcherMessage -> setSizeTask(_dispatcherNode -> getSizeTask());
            taskSendAndControl();
        });
        //поток принятия сообщения и отправка их в очередь
        std::thread thread2(&Dispatcher::messageToQueue, this);
        //поток обработки принятых сообщений
        std::thread thread3(&Dispatcher::messageAnalyseHost, this);
        thread1.join();
        thread2.join();
        thread3.join();
    }
    
    void Dispatcher::runNodeSecond() { }
    
    void Dispatcher::runNodeOther() {
        //Отправляем готовность на главный узел
        _dispatcherMessage->sendMessage((Message *) nullptr, WITHOUT_TASK, MESSAGE_READY, 0);
        //поток принятия сообщений и их анализа
        std::thread thread2(&Dispatcher::messageAnalyseNode, this);
        thread2.join();
    }
    
    void Dispatcher::setTypeTaskBuilder(TaskBuilder *taskBuilder) {
        if (_rank == 0)
            _dispatcherNode->setTypeBuilder(taskBuilder);
    }
    
    void Dispatcher::setSizeTask(const std::size_t size) {
        _dispatcherMessage->setSizeTask(size);
    }
    
    void Dispatcher::setSecondsSecure(const std::size_t secondsSecure) {
        _dispatcherNode -> setSecondsSecure(secondsSecure);
    }
    
    void Dispatcher::setCheckPoint(const bool isCheckPoint) {
        _isCheckPoint = isCheckPoint;
        _dispatcherMessage -> setCheckPoint(isCheckPoint);
    }
    
    void Dispatcher::setCheckPointRestore(const bool isCheckPointRestore) {
        _isCheakPointRestore = isCheckPointRestore;
        if ((_isCheakPointRestore) && (_checkPoint == nullptr)) {
            _checkPoint = new CheckPoint();
        }
        else if(!_isCheakPointRestore) {
            _checkPoint = nullptr;
        }
    }
    
    std::size_t Dispatcher::getSizeQueueFinal() const {
        if (_rank == 0) {
            return _dispatcherMessage->getSizeQueueFinal();
        }
        else {
            return 0;
        }
    }
    
    ftcl::OutArgs *Dispatcher::getSolution() const {
        if (_rank == 0) {
            return _dispatcherMessage->getSolution();
        }
        else {
            return nullptr;
        }
    }
    
    bool Dispatcher::getCheckPoint() const {
        return _isCheckPoint;
    }
    
    bool Dispatcher::getCheakPointRestore() const {
        return _isCheakPointRestore;
    }
    
    void Dispatcher::closeNode() const {
        if (_rank == 0) {
            for (std::size_t i = 2; i < _maxRank; i++) {
                _dispatcherMessage->sendMessage((Message *) nullptr, WITHOUT_TASK, MESSAGE_CLOSE_NODE, i);
            }
        }
    }
    
    bool Dispatcher::isHost() const {
        return _rank == 0;
    }
    
    Dispatcher::~Dispatcher() {
        delete _dispatcherMessage;
        if (_rank == 0) {
            delete _dispatcherNode;
            delete _checkPoint;
        }
        MPI::Finalize();
    }
    
    
    void Dispatcher::messageToQueue() {
        while ((_dispatcherMessage->getSizeQueueFinal() < _dispatcherNode->getSizeTask()) ||
               (_dispatcherNode->getSizeTask() == 0)) {
            Message *message = _dispatcherMessage->receiveMessage();
            if (message != nullptr) {
                _dispatcherMessage->addMessage(message);
            }
        }
    }
    
    void Dispatcher::messageAnalyseHost() {
        while (((_dispatcherMessage->getSizeQueueFinal() < _dispatcherNode->getSizeTask()) ||
                (_dispatcherNode->getSizeTask() == 0))) {
            std::size_t rank = _dispatcherMessage -> analyseHostMessage();
            //if(rank > 0) {
            //    _dispatcherNode->setTimeNode(rank);
            //}
        }
    }
    
    void Dispatcher::messageAnalyseNode() {
        OutArgs *outArgs;
        while (!(_dispatcherMessage->getClose())) {
            Message *message = _dispatcherMessage->receiveMessage();
            if (message != nullptr) {
                switch (message->getTypeMessage()) {
                    case MESSAGE_TASK:
                    {
                        Task *task = message -> getTask();
                        std::size_t number = message -> getNumberTask();
                        std::thread thread3(
                            [&] {
                                outArgs = task->solution();
                                _dispatcherMessage->sendMessage(outArgs, number, MESSAGE_COMPLETE_TASK, 0);
                            });
                        thread3.detach();
                        break;
                    }
                    case MESSAGE_CLOSE_NODE:
                    {
                        _dispatcherMessage->setClose(true);
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    }
    
    
    void Dispatcher::taskSendAndControl() {
        if(_isCheakPointRestore) {
            CheckPoint checkPoint;
            checkPoint.load(_dispatcherMessage, _dispatcherNode);
        }
        
        while ((_dispatcherMessage -> getSizeQueueFinal() < _dispatcherNode -> getSizeTask()) ||
               (_dispatcherNode -> getSizeTask() == 0)) {
            //берем свободную задачу
            //console.ConsoleOut("1");
            Task *task = nullptr;
            std::size_t i = 0;
            //возвращает задачу и ее номер
            std::tie(task, i) = _dispatcherNode -> getFreeTask();
            std::size_t number = 0;
            if (task != nullptr) {
                //console.ConsoleOut("2");
                //если свободная задача нашлась, ищем свободный узел
                while (number == 0) {
                    //номер свободного узла(процесса)
                    number = _dispatcherMessage -> getFreeNode();
                    //говорим что задача с номером i отправляется на узел number
                    _dispatcherNode -> setNumberNode(i, number);
                }
                _dispatcherMessage -> sendMessage(task, i, MESSAGE_TASK, number);
            }
        }
    }
    
}
