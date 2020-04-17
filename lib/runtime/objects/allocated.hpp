#pragma once
#include <vector>


namespace obj {


class Allocated
{
public:

    // TODO: return iterator, not vector
    virtual std::vector<const Allocated *> children() const = 0;

    virtual ~Allocated() {}
};


class Childless: public Allocated
{
public:
    std::vector<const Allocated *> children() const override { return {}; }
};

} // namespace obj
