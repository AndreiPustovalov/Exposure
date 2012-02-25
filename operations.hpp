#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP
#include <QString>

class Operation
{
protected:
    QString m_str;
public:
    virtual int getPriority() = 0;
    virtual bool fast(int left, int& result) = 0;
    virtual int operator()(int left, int right) = 0;
    const QString& str() const { return m_str; }
};

class OperationAnd : public Operation
{
public:
    OperationAnd() { m_str = "&&"; }
    virtual int getPriority() { return 0; }
    virtual bool fast(int left, int& result)
    {
        if (left == 0)
        {
            result = 0;
            return true;
        }
        return false;
    }

    virtual int operator()(int left, int right)
    {
        return left && right;
    }
};

class OperationOr : public Operation
{
public:
    OperationOr() { m_str = "||"; }
    virtual int getPriority() { return 0; }
    virtual bool fast(int left, int& result)
    {
        if (left > 0)
        {
            result = 1;
            return true;
        }
        return false;
    }

    virtual int operator()(int left, int right)
    {
        return left || right;
    }
};

class OperationLess : public Operation
{
public:
    OperationLess() { m_str = "<"; }
    virtual int getPriority() { return 1; }
    virtual bool fast(int left, int& result)
    {
        return false;
    }

    virtual int operator()(int left, int right)
    {
        return left < right;
    }
};

class OperationMore : public Operation
{
public:
    OperationMore() { m_str = ">"; }
    virtual int getPriority() { return 1; }
    virtual bool fast(int left, int& result)
    {
        return false;
    }

    virtual int operator()(int left, int right)
    {
        return left > right;
    }
};

class OperationLessEq : public Operation
{
public:
    OperationLessEq() { m_str = "<="; }
    virtual int getPriority() { return 1; }
    virtual bool fast(int left, int& result)
    {
        return false;
    }

    virtual int operator()(int left, int right)
    {
        return left <= right;
    }
};

class OperationMoreEq : public Operation
{
public:
    OperationMoreEq() { m_str = ">="; }
    virtual int getPriority() { return 1; }
    virtual bool fast(int left, int& result)
    {
        return false;
    }

    virtual int operator()(int left, int right)
    {
        return left >= right;
    }
};

class OperationEq : public Operation
{
public:
    OperationEq() { m_str = "=="; }
    virtual int getPriority() { return 1; }
    virtual bool fast(int left, int& result)
    {
        return false;
    }

    virtual int operator()(int left, int right)
    {
        return left == right;
    }
};

class OperationNotEq : public Operation
{
public:
    OperationNotEq() { m_str = "!="; }
    virtual int getPriority() { return 1; }
    virtual bool fast(int left, int& result)
    {
        return false;
    }

    virtual int operator()(int left, int right)
    {
        return left != right;
    }
};

#endif // OPERATIONS_HPP
