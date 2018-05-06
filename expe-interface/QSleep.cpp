#include <QString>
#include <QThread>
#include <QSleep.h>
#include <thread>
#include <chrono>

#ifdef Q_OS_WIN
#include <windows.h> // for Sleep
#endif

void qSleep(int ms)
{
#ifdef Q_OS_WIN
    Sleep(uint(ms));
#else
     // struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
     // nanosleep(&ts, NULL);
     std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#endif
}
