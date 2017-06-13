#ifndef TIMEDELTA_H
#define TIMEDELTA_H

#include <ctime>

class TimeDelta
{
public:
    static TimeDelta* getInstance()
    {
        static TimeDelta instance;
        return &instance;
    }

    inline time_t getDeltaTime()
    {
        time(&newSeconds);
        time_t delta = newSeconds - oldSeconds;
        oldSeconds = newSeconds;
        return delta;
    }

private:
    TimeDelta()
        : newSeconds(time(&newSeconds)),
          oldSeconds(time(&oldSeconds))
    { }

    time_t newSeconds;
    time_t oldSeconds;
};

#endif // TIMEDELTA_H
