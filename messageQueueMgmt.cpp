#include "messageQueueMgmt.h"

void msgQueue::send(Message& message)
{
    std::lock_guard<std::mutex> lock(_writemtx);
    if (-1 != msgsnd(_msgid, &message, sizeof(message), 0))
    {
        std::cout << "Message:" << message.number << " sent" << std::endl;
    }
    else
    {
        std::cerr << "Failed to send message" << std::endl;
    }
}

Message msgQueue::receive(long type)
{
    Message message;
    std::lock_guard<std::mutex> lock(_readmtx);
    if (-1 != msgrcv(_msgid, &message, sizeof(message), type, 0))
    {
        std::cout << "Message received:" << message.number << std::endl;
    }
    else
    {
        std::cout << "Failed to receive" << std::endl;
    }

    return message;
}

void msgQueue::destroyQueue()
{
    msgctl(_msgid, IPC_RMID, NULL); 
    system("rm GPqueue.txt");
}