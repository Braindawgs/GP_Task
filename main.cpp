#include "messageQueueMgmt.h"
#include "Tasks.h"

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <csignal>

std::shared_ptr<msgQueue> messageQueue = nullptr;
std::shared_ptr<ProcessA> procA = nullptr;
std::shared_ptr<ProcessB> procB = nullptr;
pid_t pid = -1;

/**
 * @brief Signal handler, for gracefull shutdown with Ctrl+C
 * 
 * @param sig Triggered signal
 */
void signalHandler(int sig)
{
    std::cout << "Signal " << sig << " , Clean up invoked!" << std::endl;

    if (procA)
    {
        procA->setTerminationFlag();
    }

    if (procB)
    {
        procB->setTerminationFlag();
        procB->notifyAll();
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    if (pid > 0) 
    {
        std::cout << "Cleaning fork" << std::endl;
        kill(pid, SIGTERM);
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) 
        {
            std::cout << "Child process exited with: " << WEXITSTATUS(status) << std::endl;
        } 
        else if (WIFSIGNALED(status)) 
        {
            std::cout << "Child process was terminated by signal: " << WTERMSIG(status) << std::endl;
        }

        if (messageQueue)
        {
            messageQueue->destroyQueue();
        }

    }

    exit(sig);
}

int main()
{
    // Register signals
    signal(SIGTERM, signalHandler);
    signal(SIGINT, signalHandler);

    pid = fork();

    if (0 > pid)
    {
        std::cerr << "Fork failed" << std::endl;
        return -1;
    }

    try
    {
        messageQueue = std::make_shared<msgQueue>();
    }
    catch(const std::exception& e)
    {
        std::cerr << "Memory queue allocation failed with:" << e.what() << std::endl;
        return -1;
    }
    

    if (0 == pid)
    {
        // Child process
        try
        {
            // Sleep, just for the parent process to execute first.
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            procB = std::make_shared<ProcessB>();
            procB->runProcessB(*messageQueue);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            exit(-1);
        }
    }
    else
    {
        // Parent process
        try
        {
            procA = std::make_shared<ProcessA>();
            procA->runProcessA(*messageQueue);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            kill(pid, SIGTERM);
            waitpid(pid, nullptr, 0);
            if (messageQueue) 
            {
                messageQueue->destroyQueue();
            }

            return -1;
        }
    }

    std::cout << "Press Enter to terminate..." << std::endl;
    std::cin.get(); 
    if (pid > 0)
    {
        //Graceful shutdown if Ctrl+C is not used
        kill(pid, SIGTERM);
        waitpid(pid, nullptr, 0);
        if (messageQueue) 
        {
            messageQueue->destroyQueue();
        }
    }
    return 0;
}

