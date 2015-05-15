
#pragma once
#ifndef ENTITY_LOG_H_
#define ENTITY_LOG_H_

class Log {

public:
    enum stdoutLevel : unsigned int {
        STDOUT_NONE = 0,
        STDOUT,
        STDOUT_APP,
        STDOUT_DEV,
        STDOUT_ALL
    };

    enum stderrLevel : unsigned int {
        STDERR_NONE = 0,
        STDERR,
        STDERR_APP,
        STDERR_DEV,
        STDERR_ALL
    };

private:
    static stdoutLevel s_outLevel; // = STDOUT_ALL
    static stderrLevel s_errLevel; // = STDERR_ALL

    static std::ostream * s_out; // = &std::cout
    static std::ostream * s_err; // = &std::cerr

    static bool s_immediateFlush; // = true

public:
    Log() = delete;

    static void setStdout(std::ostream & out);
    static void setStderr(std::ostream & err);
    static void setImmediateFlush(bool value);

    template <typename... Args>
    static void out(stdoutLevel level, fmt::StringRef string, Args &&... args);

    template <typename... Args>
    static void out(fmt::StringRef string, Args &&... args);

    template <typename... Args>
    static void dev_out(fmt::StringRef string, Args &&... args);

    template <typename... Args>
    static void app_out(fmt::StringRef string, Args &&... args);


    template <typename... Args>
    static void err(stderrLevel level, fmt::StringRef string, Args &&... args);

    template <typename... Args>
    static void err(fmt::StringRef string, Args &&... args);

    template <typename... Args>
    static void dev_err(fmt::StringRef string, Args &&... args);

    template <typename... Args>
    static void app_err(fmt::StringRef string, Args &&... args);

};

#ifdef NDEBUG
#   define DEBUG_OUT(...)
#   define DEBUG_ERR(...)
#else
#   define DEBUG_OUT(...) Log::out(__VA_ARGS__)
#   define DEBUG_ERR(...) Log::err(__VA_ARGS__)
#endif

/**************************************************************/
/**** Inline functions ****************************************/
/**************************************************************/

//// Public ////////////////////////////////////////////////////

inline void Log::setStdout(std::ostream & out) {

    s_out = &out;

}

inline void Log::setStderr(std::ostream & err) {

    s_err = &err;

}

inline void Log::setImmediateFlush(bool value) {

    s_immediateFlush = value;

}

template <typename... Args>
void Log::out(stdoutLevel level, fmt::StringRef string, Args &&... args) {

    if (level <= s_outLevel)
        fmt::print(*s_out, string, std::forward<Args>(args)...);
    if (s_immediateFlush)
        (*s_out) << std::endl;

}

template <typename... Args>
void Log::out(fmt::StringRef string, Args &&... args) {

    out(STDOUT, string, std::forward<Args>(args)...);

}

template <typename... Args>
void Log::dev_out(fmt::StringRef string, Args &&... args) {

    out(STDOUT_DEV, string, std::forward<Args>(args)...);

}

template <typename... Args>
void Log::app_out(fmt::StringRef string, Args &&... args) {

    out(STDOUT_APP, string, std::forward<Args>(args)...);

}

template <typename... Args>
void Log::err(stderrLevel level, fmt::StringRef string, Args &&... args) {

    if (level <= s_errLevel)
        fmt::print(*s_err, string, std::forward<Args>(args)...);
    if (s_immediateFlush)
        (*s_err) << std::endl;

}

template <typename... Args>
void Log::err(fmt::StringRef string, Args &&... args) {

    err(STDERR, string, std::forward<Args>(args)...);

}

template <typename... Args>
void Log::dev_err(fmt::StringRef string, Args &&... args) {

    err(STDERR_DEV, string, std::forward<Args>(args)...);

}

template <typename... Args>
void Log::app_err(fmt::StringRef string, Args &&... args) {

    err(STDERR_APP, string, std::forward<Args>(args)...);

}

//// Protected /////////////////////////////////////////////////

//// Private ///////////////////////////////////////////////////

#endif // ENTITY_LOG_H_

