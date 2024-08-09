#include "Tasks.h"

void ProcessA::taskASend(msgQueue& queue)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> distInt(1, 100);

    while(1)
    {
        int random = distInt(mt);
        Message msg = {.type = 1, .number = random};

        queue.send(msg);
        
        std::this_thread::sleep_for(std::chrono::seconds(4));
    }
}

void ProcessA::taskAReceive(msgQueue& queue)
{
    while(1)
    {
        Message msg = queue.receive(2);
        std::cout << "Ack(" << msg.type << ")" << std::endl;
    }
}

void ProcessA::runProcessA(msgQueue& queue)
{
    std::thread t1(&ProcessA::taskASend, this, std::ref(queue));
    std::thread t2(&ProcessA::taskAReceive, this, std::ref(queue));

    t1.join();
    t2.join();
}


void ProcessB::taskBSend(msgQueue& queue)
{
    while(1)
    {
        std::unique_lock<std::mutex> lock(_mtx);
        _cv.wait(lock, [this]{return getTaskDone();});

        //send ACK (type 2) and dummy
        Message msg = {.type = 2, .number = 0};
        queue.send(msg);
        setTaskDone(false);
    }
}

void ProcessB::taskBReceive(msgQueue& queue)
{
    while(1)
    {
        static int sum = 0;
        Message msg = queue.receive(1);
        sum += msg.number;
        std::cout << "Current sum is: " << sum << std::endl;
        setTaskDone(true);
        _cv.notify_one();
    }
}

void ProcessB::runProcessB(msgQueue& queue)
{
    std::thread t1(&ProcessB::taskBReceive, this, std::ref(queue));
    std::thread t2(&ProcessB::taskBSend, this, std::ref(queue));

    t1.join();
    t2.join();
}