#include "messageQueueMgmt.h"
#include "Tasks.h"

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid = fork();
    msgQueue queue;

    if (0 > pid)
    {
        std::cerr << "Fork failed" << std::endl;
        return -1;
    }

    if (0 == pid)
    {
        sleep(1); // Ensure the parent sends the message first
        runProcessB(queue);
    }
    else
    {
        runProcessA(queue);
        wait(NULL);
    }

    return 0;
}

