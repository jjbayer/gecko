#pragma once
#include <vector>


namespace obj {


class Allocated
{
public:

    virtual const std::vector<const Allocated *> & children() const
    {
        return mChildren;
    }

    virtual ~Allocated() {}

private:
    std::vector<const Allocated *> mChildren;
};

} // namespace obj
