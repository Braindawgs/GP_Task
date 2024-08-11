#include "messageQueueMgmt.h"

void msgQueue::send(Message& message)
{
    if (-1 != msgsnd(_msgid, &message, sizeof(message), 0))
    {
        std::cout << "Message:" << message.number << " sent" << std::endl;
    }
    else
    {
        throw std::runtime_error("Failed to send message"); 
    }
}

Message msgQueue::receive(long type)
{
    Message message;

    if (-1 != msgrcv(_msgid, &message, sizeof(message), type, 0))
    {
        std::cout << "Message received:" << message.number << std::endl;
    }
    else
    {
        throw std::runtime_error("Failed to receive message"); 
    }

    return message;
}

void msgQueue::destroyQueue()
{
    msgctl(_msgid, IPC_RMID, NULL); 
    system("rm GPqueue.txt");
}