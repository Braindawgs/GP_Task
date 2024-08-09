#pragma once

#include <string>
#include <iostream>
#include <mutex>
#include <sys/ipc.h>
#include <sys/msg.h>

struct Message
{
    long type;
    int number;
};

/**
 * @brief Message queue for IPC.
 * Uses `GPqueue.txt` for communication between processes.
 * 
 */
class msgQueue
{
public:

    /**
     * @brief Construct a new msg Queue object.
     * Creates message queue.
     * 
     */
    msgQueue() 
    {
        system("touch GPqueue.txt");
        _key = ftok("GPqueue.txt", 65);
        _msgid = msgget(_key, 0666 | IPC_CREAT);

        if ((-1 == _key) || (-1 == _msgid))
        {
            throw std::runtime_error("Failed to create a queue");
        }
    }

    ~msgQueue()
    {
        destroyQueue();
    }

    /**
     * @brief Send message over queue.
     * 
     * @param message 
     */
    void send(Message& message);

    /**
     * @brief Receives message over queue.
     * 
     * @param type Type of send. 1 for Data send, 2 for ACK send with dummy number.
     * @return Message Received message struct.
     */
    Message receive(long type);

    /**
     * @brief Destroys queue and GPqueue.txt file.
     * 
     */
    void destroyQueue();

private:

    key_t _key;
    int _msgid;
    std::mutex _writemtx;
    std::mutex _readmtx;
};