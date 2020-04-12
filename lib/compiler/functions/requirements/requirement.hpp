#pragma once


namespace ct {

class FunctionKey;

class FunctionRequirement
{
public:
    virtual bool matches(const FunctionKey &) = 0;

    virtual ~FunctionRequirement();
};

} // namespace ct
