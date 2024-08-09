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

class msgQueue
{
public:

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

    void send(Message& message);
    Message receive(long type);
    void destroyQueue();

private:

    key_t _key;
    int _msgid;
    std::mutex _writemtx;
    std::mutex _readmtx;
};