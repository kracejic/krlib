#include <chrono>
#include <cstdarg>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

namespace kr
{

class Logger
{
  public:
    enum class LogType
    {
        trace,
        error,
        warn,
        info
    };

    // Life-cycle
    Logger(const std::string& _filename = "", bool toConsole = true)
        : filename(_filename)
        , printToStdout(toConsole)
    {
        info("----------------START--------------------");
    };
    ~Logger()
    {
        info("-----------------END---------------------");
    }

    // Interface
    void log(LogType logtype, const char* Format, ...)
    {
        va_list args;
        va_start(args, Format);
        log(logtype, Format, args);
        va_end(args);
    };
    void info(const char* Format, ...)
    {
        va_list args;
        va_start(args, Format);
        log(LogType::info, Format, args);
        va_end(args);
    };
    void trace(const char* Format, ...)
    {
        va_list args;
        va_start(args, Format);
        log(LogType::trace, Format, args);
        va_end(args);
    };
    void warn(const char* Format, ...)
    {
        va_list args;
        va_start(args, Format);
        log(LogType::warn, Format, args);
        va_end(args);
    };
    void error(const char* Format, ...)
    {
        va_list args;
        va_start(args, Format);
        log(LogType::error, Format, args);
        va_end(args);
    };


  private:
    std::string filename = "";
    bool printToStdout = true;

    std::string NowTime()
    {
        auto tt = std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now());
        std::tm tm;
        char buf[32];
        gmtime_r(&tt, &tm);
        strftime(buf, 31, "%H:%M:%S", &tm);
        return buf;
    };
    void log(LogType logtype, const char* Format, va_list args)
    {
        char Buffer[2048];
        vsprintf(Buffer, Format, args);
        std::string tempStr(NowTime());
        switch (logtype)
        {
            case LogType::trace: tempStr += " Trace "; break;
            case LogType::info: tempStr += " Info  "; break;
            case LogType::warn: tempStr += " Warn  "; break;
            case LogType::error: tempStr += " Error "; break;
        }
        tempStr += Buffer;

        if (filename != "")
        {
            std::ofstream logfile(filename, std::ios::app);
            logfile << tempStr << std::endl;
            logfile.close();
        }
        std::cout << tempStr << std::endl;
    };
};

} /* kr */
