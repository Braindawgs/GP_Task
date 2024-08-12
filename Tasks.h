#pragma once

#include "messageQueueMgmt.h"

#include <iostream>
#include <thread>
#include <mutex>
#include <random>
#include <chrono>
#include <semaphore>
#include <atomic>
#include <condition_variable>

/**
 * @brief ProcessA class with methods corresponding to 2 defined threads functions
 * 
 */
class ProcessA
{
public:
    
    ~ProcessA()
    {
        setTerminationFlag();

        if (t1.joinable())
        {
            std::cout << "t1 of processA joined" << std::endl;
            t1.join();
        }

        if (t2.joinable())
        {
            std::cout << "t2 of processA joined" << std::endl;
            t2.join();
        }
    }

    /**
     * @brief Creates 2 threads for both `taskASend` and `taskAReceive` methods.
     * 
     * @param queue Message queue for IPC.
     */
    void runProcessA(msgQueue& queue);

    void setTerminationFlag()
    {
        terminationFlag = true;
    }

private:
    /**
     * @brief Generates a random number and sends it to ProcessB.
     * 
     * @param queue Message queue for IPC.
     */
    void taskASend(msgQueue& queue);

    /**
     * @brief Receives ACK from ProcessB
     * 
     * @param queue Message queue for IPC.
     */
    void taskAReceive(msgQueue& queue);

    std::thread t1;
    std::thread t2;

    std::atomic<bool> terminationFlag = false;
};

/**
 * @brief ProcessB class with methods corresponding to 2 defined threads functions
 * 
 */
class ProcessB
{
public:

    ~ProcessB()
    {
        setTerminationFlag();

        if (t1.joinable())
        {
            std::cout << "t1 of processB joined" << std::endl;
            t1.join();
        }

        if (t2.joinable())
        {
            std::cout << "t2 of processB joined" << std::endl;
            t2.join();
        }
    }

    /**
     * @brief Creates 2 threads for both `taskBSend` and `taskBReceive` methods.
     * 
     * @param queue Message queue for IPC.
     */
    void runProcessB(msgQueue& queue);

    /**
     * @brief Set the isTaskDone member to val.
     * It's used to notify ACK sending task.
     * 
     * @param val Bool value to set isTaskDone to it.
     */
    void setTaskDone(bool val)
    {
        isTaskDone = val;
    }

    /**
     * @brief Get the isTaskDone object.
     * 
     * @return isTaskDone object.
     */
    bool getTaskDone()
    {
        return isTaskDone;
    }

    /**
     * @brief Set the Termination Flag, used for task termination.
     * 
     */
    void setTerminationFlag()
    {
        terminationFlag = true;
    }

    /**
     * @brief Notifies task.
     * 
     */
    void notifyAll();

private:
    /**
     * @brief Receives random number from ProcessA and sums them.
     * 
     * @param queue Message queue for IPC.
     */
    void taskBSend(msgQueue& queue);

    /**
     * @brief After `taskBSend` finishes it's computation, sends ACK to processA.
     * 
     * @param queue Message queue for IPC.
     */
    void taskBReceive(msgQueue& queue);

    std::thread t1;
    std::thread t2;

    bool isTaskDone = false;
    std::mutex _mtx;
    std::condition_variable _cv;
    std::atomic<bool> terminationFlag = false;
};