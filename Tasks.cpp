#include "Tasks.h"

void taskASend(msgQueue& queue)
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

void taskAReceive(msgQueue& queue)
{
    while(1)
    {
        Message msg = queue.receive(2);
        std::cout << "Ack(" << msg.type << ")" << std::endl;
    }
}

void runProcessA(msgQueue& queue)
{
    std::thread t1(taskASend, std::ref(queue));
    std::thread t2(taskAReceive, std::ref(queue));

    t1.join();
    t2.join();
}


bool isTaskDone = false;
std::mutex mtx;
std::condition_variable cv;

void taskBSend(msgQueue& queue)
{
    while(1)
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, []{ return isTaskDone;});

        //send ACK (type 2) and dummy
        Message msg = {.type = 2, .number = 0};
        queue.send(msg);
        isTaskDone = false;
    }
}

void taskBReceive(msgQueue& queue)
{
    while(1)
    {
        static int sum = 0;
        Message msg = queue.receive(1);
        sum += msg.number;
        std::cout << "Current sum is: " << sum << std::endl;
        isTaskDone = true;
        cv.notify_one();
    }
}

void runProcessB(msgQueue& queue)
{
    std::thread t1(taskBReceive, std::ref(queue));
    std::thread t2(taskBSend, std::ref(queue));

    t1.join();
    t2.join();
}