#include <QDebug>
#include <QCoreApplication>
#include <QThread>

#include "appveyor.h"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    AppVeyor::BuildWorkerApiClient appveyor; 
    AppVeyor::Test test;

    test.name = "First test";
    test.fileName = "first.cpp";
    test.framework = "CppUnit";
    test.outcome = AppVeyor::TestOutcome::Running;
    appveyor.addTest(test);
    QThread::sleep(2);
    test.duration = 1985;
    test.outcome = AppVeyor::TestOutcome::Passed;
    appveyor.updateTest(test);

    QThread::sleep(1);
    
    test.name = "Second test";
    test.fileName = "second.cpp";
    test.framework = "CppUnit";
    test.outcome = AppVeyor::TestOutcome::Running;
    appveyor.addTest(test);
    QThread::sleep(2);
    test.duration = 2025;
    test.outcome = AppVeyor::TestOutcome::Failed;
    test.errorMessage = "FooBar!";
    test.errorStackMessage = "second.cpp:17\nmain.cpp:42";
    test.stdOut = "Ble ble ble";
    test.stdErr = "Initializing bulbulator\nReading foobars... done\nZapping caches."; 
    appveyor.updateTest(test);
    return 0;
}
