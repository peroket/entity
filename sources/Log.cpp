
#include "Log.h"

Log::stdoutLevel Log::s_outLevel = Log::STDOUT_ALL;
Log::stderrLevel Log::s_errLevel = Log::STDERR_ALL;

std::ostream * Log::s_out = &std::cout;
std::ostream * Log::s_err = &std::cerr;

bool Log::s_immediateFlush = true;

