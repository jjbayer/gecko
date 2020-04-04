#include "output.hpp"


Output & getOutput()
{
    static Output output;
    return output;
}
