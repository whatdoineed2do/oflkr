#ifndef OFLKR_LOGGER_H
#define OFLKR_LOGGER_H

#include <string>
#include <iostream>

#include <log4cpp/Priority.hh>
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/Layout.hh>
#include <log4cpp/PatternLayout.hh>



#define LOG_EMERG(__log4cpp_log_wrapper) __log4cpp_log_wrapper << log4cpp::Priority::EMERG
#define LOG_ALERT(__log4cpp_log_wrapper) __log4cpp_log_wrapper << log4cpp::Priority::ALERT
#define LOG_CRIT(__log4cpp_log_wrapper) __log4cpp_log_wrapper << log4cpp::Priority::CRIT
#define LOG_ERROR(__log4cpp_log_wrapper) __log4cpp_log_wrapper << log4cpp::Priority::ERROR
#define LOG_WARN(__log4cpp_log_wrapper) __log4cpp_log_wrapper << log4cpp::Priority::WARN
#define LOG_NOTICE(__log4cpp_log_wrapper) __log4cpp_log_wrapper << log4cpp::Priority::NOTICE
#define LOG_INFO(__log4cpp_log_wrapper) __log4cpp_log_wrapper << log4cpp::Priority::INFO
#define LOG_DEBUG(__log4cpp_log_wrapper) __log4cpp_log_wrapper << log4cpp::Priority::DEBUG

namespace oflkr {
    using Logger = log4cpp::Category;
}

inline oflkr::Logger&  LOG_ROOT(const std::string& where_ = "")
{
    log4cpp::Category&  log = log4cpp::Category::getRoot();
    bool  console = true;
    try
    {
        if (where_.size() > 0) {
            console = false;
            log4cpp::PropertyConfigurator::configure(where_);
        }
    }
    catch (const std::exception& ex)
    {
        console = true;
        std::cerr << "failed to configure logger via " << where_ << ", using stdout\n";
    }

    if (console) {
        log4cpp::Appender*  a = new log4cpp::OstreamAppender("console", &std::cout);
        log4cpp::PatternLayout*  l = new log4cpp::PatternLayout();
        l->setConversionPattern("%m%n");
        a->setLayout(l);
        log.setPriority(log4cpp::Priority::NOTICE);
        log.addAppender(a);
    }
    return log;
}

#define LOG_INSTANCE(__log4cpp_log_instance_) log4cpp::Category::getInstance(__log4cpp_log_instance_)

#endif
