#include <unistd.h>
#include <libgen.h>

#include <iostream>
#include <exception>
#include <stdexcept>

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

#include <libconfig.h++>

#include "RESTsvr.h"


int main(int argc, char* argv[])
{
    const char*  argv0 = basename(argv[0]);
    const char*  l = nullptr;
    int  port = -1;
    const char*  cfgname = nullptr;

    int c;
    while ( (c=getopt(argc, argv, "c:p:")) != EOF) 
    {
        switch (c) {
	    case 'c':  cfgname = optarg;  break;
	    case 'p':  port = atoi(optarg);  break;

	    default:
		;
	}
    }
    if (cfgname == nullptr) {
        std::cerr << argv0 << ": no config file\n";
	return -1;
    }

    libconfig::Config  cfg;
    try
    {
        cfg.readFile(cfgname);
    }
    catch (const std::exception& ex)
    {
        std::cerr << argv0 << ": failed to process config file - " << ex.what() << "\n";
	return -1;
    }
    const libconfig::Setting&  cfgroot = cfg.getRoot();


    log4cpp::Category&  log = log4cpp::Category::getRoot();
    try
    {
	const libconfig::Setting&  logcfg = cfgroot["logging"];
        std::string  where;
        if (!logcfg.lookupValue("config", where)) {
            throw std::invalid_argument("no logging.config value defined");
        }
        log4cpp::PropertyConfigurator::configure(where);
    }
    catch (const std::exception& ex)
    {
        std::cerr << argv0 << ": failed to parse log4cpp config file - " << ex.what() << "\n";
	return -1;
    }
    LOG_NOTICE(log) << "server started pid=" << getpid();

    try
    {

	const libconfig::Setting&  restcfg = cfgroot["rest_svr"];
	int  workers;
        if ( (port < 0 && !restcfg.lookupValue("port", port)) || !restcfg.lookupValue("worker_limit", workers)) {
	    throw std::invalid_argument("incomplete REST svr cfg");
	}

        RESTsvr  svr(port, workers);
        svr.start();
    }
    catch (const std::exception& ex)
    {
        LOG_ERROR(log) << "failed to start REST svc - " << ex.what();
    }
    LOG_NOTICE(log) << "server stopped";
    return 0;
}
