#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  

#include <iostream>

#include "TestScurt.h"
#include "TestExtins.h"

int main() {

    {
       testAll();
       testAllExtins();
        std::cout << "Finished Tests!" << std::endl;
    }

    _CrtDumpMemoryLeaks();
    return 0;
}