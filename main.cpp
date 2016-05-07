//
//  main.cpp
//  diplom
//
//  Created by Максим Кузин on 05.05.16.
//  Copyright © 2016 Максим Кузин. All rights reserved.
//

#include "src/dispatcher/dispatcher.hpp"
#include <boost/serialization/export.hpp>

class ConcreteEntryArgs : public ftcl::EntryArgs {
public:
    ConcreteEntryArgs() : EntryArgs() {
        _n = 0;
        _nOnUzel = 0;
    }
    
    ConcreteEntryArgs(const ConcreteEntryArgs& o) {
        _n = o._n;
        _nOnUzel = o._nOnUzel;
    }
    
    ConcreteEntryArgs(std::size_t n, std::size_t nOnUzel) : EntryArgs() {
        _n = n;
        _nOnUzel = nOnUzel;
    }
    
    std::size_t _n;
    std::size_t _nOnUzel;
    ~ConcreteEntryArgs() {}
};

class ConcreteOutArgs : public ftcl::OutArgs {
public:
    ConcreteOutArgs() {
        _sum = 0;
    }
    
    ConcreteOutArgs(double sum) {
        _sum = sum;
    }
    
    ConcreteOutArgs(const ConcreteOutArgs& o) {
        _sum = o._sum;
    }
    
    double getSum() {
        return _sum;
    }
    
    virtual void consoleOut() override {
        std::cout << "СУММА НА УЗЛЕ " << _sum << std::endl;
    }
    
    ~ConcreteOutArgs() override {}
private:
    
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & boost::serialization::base_object<OutArgs>(*this);
        ar & _sum;
    }
    double _sum;
};

class ConcreteTask : public ftcl::Task {
public:
    ConcreteTask() : Task() {
        _a = 0;
        _b = 0;
        _h = 0;
        _n = 0;
    }
    
    ConcreteTask(std::size_t a, std::size_t b, double h, std::size_t n) : Task() {
        _a = a;
        _b = b;
        _h = h;
        _n = n;
    }
    
    ConcreteTask(const ConcreteTask &o) {
        _a = o._a;
        _b = o._b;
        _h = o._h;
        _n = o._n;
    }
    
    virtual ftcl::OutArgs* solution() override {
        double x;
        double sum = 0;
        for(std::size_t i = _a; i < _b; i++) {
            x = ( i + 0.5 ) * _h;
            sum += 4.0 / ( 1.0 + x * x );
        }
        sum = sum * _h;
        return (new ConcreteOutArgs(sum));
    }
    
    virtual ~ConcreteTask(){}
    
    
private:
    
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & boost::serialization::base_object<Task>(*this);
        ar & _a;
        ar & _b;
        ar & _h;
        ar & _n;
    }
    
    std::size_t _a;
    std::size_t _b;
    double _h;
    std::size_t _n;
};

class ConcreteTaskBuilder : public ftcl::TaskBuilder {
public:
    ConcreteTaskBuilder() = default;
    virtual void create(ftcl::EntryArgs* args) override {
        ///////НЕ БАГ, А ФИЧА///////
        auto args2 = reinterpret_cast<ConcreteEntryArgs*>(args);
        ////////////////////////////
        double h = 1. / (args2 -> _n);
        std::size_t numberNode = args2 -> _n / args2 -> _nOnUzel;
        for(std::size_t i = 0; i < numberNode; i++) {
            std::size_t left  = i * args2 -> _nOnUzel;
            std::size_t right = (i + 1) * args2 -> _nOnUzel;
            auto message = new ftcl::MessageNode(new ConcreteTask(left, right, h, args2 -> _nOnUzel), i);
            _tasks.push_back(message);
        }
    }
    virtual ~ConcreteTaskBuilder(){}
};

BOOST_CLASS_EXPORT(ConcreteTask);
BOOST_CLASS_EXPORT(ConcreteOutArgs);

constexpr auto count       = 10000000000;
constexpr auto countOnNode = 10000000;


int main(int argc, char* argv[]) {
    try {
        ftcl::Dispatcher _disp(&argc, &argv);
        _disp.setTypeTaskBuilder( new ConcreteTaskBuilder());
        _disp.setCheckPoint(true);
        _disp.setCheckPointRestore(true);
        _disp.run(new ConcreteEntryArgs(count, countOnNode));
        _disp.closeNode();
        
        if ( _disp.isHost() ) {
            double sum = 0;
            while(_disp.getSizeQueueFinal() > 0) {
                ConcreteOutArgs* test = reinterpret_cast<ConcreteOutArgs*>(_disp.getSolution());
                sum += test -> getSum();
            }
            std::cout << std::setprecision(20) <<sum << std::endl;
        }
    } catch(...) {
        std::cout << "Поймал EXCEPTION\n";
    }
    return 0;
}
