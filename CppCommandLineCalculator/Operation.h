#pragma once
#include <cmath>
#include <iostream>

template <typename T>
class Operation
{
public:
	virtual ~Operation() = default;
    virtual T Calculate(T a, T b) const = 0;
};

template <typename T>
class Addition : public Operation<T> 
{
public:
    T Calculate(T a, T b) const override
    {
        return a + b;
    }
};

template <typename T>
class Subtraction : public Operation<T> 
{
public:
    T Calculate(T a, T b) const override
    {
        return a - b;
    }
};

template <typename T>
class Multiplication : public Operation<T> 
{
public:
    T Calculate(T a, T b) const override
    {
        return a * b;
    }
};

template <typename T>
class Division : public Operation<T> 
{
public:
    T Calculate(T a, T b) const override
    {
        if (b == 0)
            throw std::runtime_error("Cannot divide by zero!");
        return a / b;
    }
};

template <typename T>
class Exponentiation : public Operation<T> 
{
public:
    T Calculate(T a, T b) const override
    {
        return std::pow(a, b);
    }
};

template <typename T>
class SquareRoot : public Operation<T>
{
public:
    T Calculate(T a, [[maybe_unused]] T b) const override
    {
        return sqrt(a);
    }
};


template <typename T>
class Negation : public Operation<T>
{
public:
    T Calculate(T a, [[maybe_unused]] T b) const override
    {
        return -a;
    }
};
