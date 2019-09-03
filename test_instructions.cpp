#include "instructions.hpp"

#define BOOST_TEST_MAIN
#if !defined( WIN32 )
    #define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_instructions)
{



    std::vector<Instruction> instructions {
        setInt(0, 666),
        setInt(1, 123),
        addInt(0, 1, 2),
        printInt(2),
        setInt(3, 0),
        setInt(4, 10),
        intGte(3, 4, 5),
        jumpIf(5, 12),
            printInt(3),
            setInt(6, 1),
            addInt(3, 6, 3),
        jump(6),
        setInt(12, 1337),
        printInt(12),
    };

    std::vector<Object> data(100);
    for(auto i = 0; i < instructions.size(); i++) {
        instructions[i](data, i);
    }
}
