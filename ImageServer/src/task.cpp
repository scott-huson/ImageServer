
#include "task.h"

Task::Task(CameraModel *PassedCamera)
{
    Camera = PassedCamera;
}

void Task::run()
{
    emit Result(Camera->getFrame());
}
