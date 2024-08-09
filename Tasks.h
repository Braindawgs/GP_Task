#pragma once

#include "messageQueueMgmt.h"

#include <iostream>
#include <thread>
#include <mutex>
#include <random>
#include <chrono>
#include <semaphore>
#include <condition_variable>

/**
 * @brief ProcessA class with methods corresponding to 2 defined threads functions
 * 
 */
class ProcessA
{
public:
    
    /**
     * @brief Creates 2 threads for both `taskASend` and `taskAReceive` methods.
     * 
     * @param queue Message queue for IPC.
     */
    void runProcessA(msgQueue& queue);

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
};

/**
 * @brief ProcessB class with methods corresponding to 2 defined threads functions
 * 
 */
class ProcessB
{
public:
    /**
     * @brief Creates 2 threads for both `taskBSend` and `taskBReceive` methods.
     * 
     * @param queue Message queue for IPC.
     */
    void runProcessB(msgQueue& queue);

    /**
     * @brief Set the isTaskDone member to val.
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

    bool isTaskDone = false;
    std::mutex _mtx;
    std::condition_variable _cv;
};