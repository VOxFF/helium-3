#include "testing/PredictedTest.h"
#include <iostream>



int main(int argc, char** argv) 
{
    std::cout << "Hello from Helium-3 mining!" << std::endl << std::endl;

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
