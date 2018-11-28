#ifndef OFLKR_LOGGER_H
#define OKFLR_LOGGER_H

#include <log4cpp/Priority.hh>
#include <log4cpp/Category.hh>


#define LOG_EMERG(__log4cpp_log_wrapper) __log4cpp_log_wrapper << log4cpp::Priority::EMERG
#define LOG_ALERT(__log4cpp_log_wrapper) __log4cpp_log_wrapper << log4cpp::Priority::ALERT
#define LOG_CRIT(__log4cpp_log_wrapper) __log4cpp_log_wrapper << log4cpp::Priority::CRIT
#define LOG_ERROR(__log4cpp_log_wrapper) __log4cpp_log_wrapper << log4cpp::Priority::ERROR
#define LOG_WARN(__log4cpp_log_wrapper) __log4cpp_log_wrapper << log4cpp::Priority::WARN
#define LOG_NOTICE(__log4cpp_log_wrapper) __log4cpp_log_wrapper << log4cpp::Priority::NOTICE
#define LOG_INFO(__log4cpp_log_wrapper) __log4cpp_log_wrapper << log4cpp::Priority::INFO
#define LOG_DEBUG(__log4cpp_log_wrapper) __log4cpp_log_wrapper << log4cpp::Priority::DEBUG

#endif
