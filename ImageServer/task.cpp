
#include "task.h"

Task::Task()
{
}

void Task::run()
{
    // Task here

    qDebug() << "Task Start";

    int iNumber = 0;
    for(int i = 0; i < 1000; i++)
    {
        iNumber += i;
    }

    qDebug() << "Task Done";

    emit Result(iNumber);
}
