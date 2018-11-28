#include <iostream>
#include <exception>
#include <stdexcept>

#include "RESTsvr.h"


int main()
{
    try
    {
        RESTsvr  svr;
        svr.start();
    }
    catch (const std::exception& ex)
    {
        std::cerr << "caught - " << ex.what() << '\n';
    }
       
}
