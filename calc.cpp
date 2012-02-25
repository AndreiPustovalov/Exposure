#include "calc.h"

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
}

Calc::Calc(Calc&)
{
}

Calc::~Calc()
{

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

        int tPos = eq.lastIndexOf((*It)->str());
        if (tPos > pos)
            if (!op || (op && op->getPriority() == (*It)->getPriority()) )
            {
                pos = tPos;
                op = *It;
            }
    }

    if (pos == -1)
        return eq.toInt(&ok);

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
