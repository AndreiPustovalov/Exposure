#ifndef CALC_H
#define CALC_H

#include <QString>
#include "operations.hpp"
#include <set>

class priorityComp
{
public:
    bool operator()(Operation* a, Operation* b)
    {
        return a->getPriority() < b->getPriority();
    }
};

class Calc
{
private:
    std::multiset<Operation*, priorityComp> operations;
    Calc();
    Calc(Calc&);
public:
    int evaluate(const QString& eq, bool& ok);
    static Calc& Instance();
    ~Calc();
};

#endif // CALC_H
