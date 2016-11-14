#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H

#include <cerrno>
#include <system_error>
#include <string>

/**
 * error constructors for sensors
 * used to create i2c errors
 * these can later be replaced with any logging stuff we want 
 */

static inline std::system_error sensors_error()
{
    return std::system_error(errno, std::system_category());
}

static inline std::system_error sensors_error(const char * what)
{
    return std::system_error(errno, std::system_category(), what);
}

static inline std::system_error sensors_error(const std::string & what)
{
    return std::system_error(errno, std::system_category(), what);
}

#endif
