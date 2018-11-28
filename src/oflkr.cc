#include <unistd.h>
#include <libgen.h>

#include <iostream>
#include <exception>
#include <stdexcept>

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

#include "RESTsvr.h"


int main(int argc, char* argv[])
{
    const char*  argv0 = basename(argv[0]);
    const char*  l = nullptr;

    int c;
    while ( (c=getopt(argc, argv, "l:")) != EOF) 
    {
        switch (c) {
	    case 'l':  l = optarg;  break;

	    default:
		;
	}
    }
    if (l == nullptr) {
        std::cerr << argv0 << ": no log4cpp config file\n";
	return -1;
    }

    log4cpp::Category&  log = log4cpp::Category::getRoot();
    try
    {
        log4cpp::PropertyConfigurator::configure(l);
    }
    catch (const std::exception& ex)
    {
        std::cerr << argv0 << ": failed to parse log4cpp config file - " << ex.what() << "\n";
	return -1;
    }
    LOG_NOTICE(log) << "server started pid=" << getpid();

    try
    {
        RESTsvr  svr;
        svr.start();
    }
    catch (const std::exception& ex)
    {
        LOG_ERROR(log) << "caught - " << ex.what();
    }
    LOG_NOTICE(log) << "server stopped";
    return 0;
}
