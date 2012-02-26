#include "calc.h"
#include <QDebug>

Calc::Calc()
{
    operations.insert(new OperationAnd());
    operations.insert(new OperationOr());
    operations.insert(new OperationLess());
    operations.insert(new OperationMore());
    operations.insert(new OperationLessEq());
    operations.insert(new OperationMoreEq());
    operations.insert(new OperationEq());
    operations.insert(new OperationNotEq());
    operations.insert(new OperationMul());
    operations.insert(new OperationDiv());
    operations.insert(new OperationMod());
    operations.insert(new OperationAdd());
    operations.insert(new OperationSub());
}

Calc::Calc(Calc&)
{
}

Calc::~Calc()
{
    std::for_each(operations.begin(), operations.end(), [](Operation* op){
            delete op;
            op = nullptr;
    });
    operations.clear();
}

int Calc::evaluate(const QString &eq, bool &ok)
{
    if (eq.isEmpty())
        return true;

    int pos = -1;
    Operation* op = NULL;
    int last_p = (*operations.begin())->getPriority();
    for (auto It = operations.begin(); It != operations.end(); ++It)
    {
        if (pos > -1 && (last_p != (*It)->getPriority()))
            break;

        last_p = (*It)->getPriority();

        QString n_eq;
        n_eq.reserve(eq.length());
        int level = 0;
        for (int i = 0; i < eq.length(); ++i)
        {
            if (level == 0)
                n_eq[i] = eq.at(i);
            else
                n_eq[i] = '0';
            if (eq.at(i) == '(')
                ++level;
            if (eq.at(i) == ')')
                --level;
        }
        int tPos = n_eq.lastIndexOf((*It)->str());
        if (tPos > pos)
            if (!op || (op && op->getPriority() == (*It)->getPriority()) )
            {
                pos = tPos;
                op = *It;
            }
    }

    if (pos == -1)
    {
        if (eq.at(0) == '(')
        {
            if (eq.at(eq.length()-1) == ')')
                return evaluate(eq.mid(1, eq.length()-2), ok);
            else
            {
                ok = false;
                return 1;
            }
        }
        return eq.toInt(&ok);
    }
    int left = evaluate(eq.left(pos), ok);
    int res;
    if (op->fast(left, res))
        return res;
    int right = evaluate(eq.right(eq.length() - pos - op->str().length()), ok);
    return (*op)(left, right);
}

Calc & Calc::Instance()
{
    static Calc inst;
    return inst;
}
