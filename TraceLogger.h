#ifndef TRACE_LOGGER_H
#define TRACE_LOGGER_H
#include <iostream>
#include <string>
class TraceLogger
{
    std::string mFunctionName;
    std::string mFileName;
public:
    explicit TraceLogger(std::string fileName, std::string functionName, int lineNum):
        mFunctionName(functionName),
        mFileName(fileName)
        {
            std::cout << "ENTERING: " << mFileName << ":" << mFunctionName << ":" << lineNum << std::endl;
        }

    ~TraceLogger()
        {
            std::cout << "LEAVING: " << mFileName << ":" << mFunctionName << std::endl;
        }
};

#define TRACE TraceLogger myLogger( __FILE__,__FUNCTION__,__LINE__);
#endif 
