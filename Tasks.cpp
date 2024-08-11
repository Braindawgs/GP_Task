#include "Tasks.h"

void ProcessA::taskASend(msgQueue& queue)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> distInt(1, 100);

    while(!terminationFlag)
    {
        try
        {      
            int random = distInt(mt);
            Message msg = {.type = 1, .number = random};

            queue.send(msg);
            
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            break;
        }
    }
}

void ProcessA::taskAReceive(msgQueue& queue)
{
    while(!terminationFlag)
    {
        try
        {
            Message msg = queue.receive(2);
            std::cout << "Ack(" << msg.type << ")" << std::endl;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            break;
        }
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
    while(!terminationFlag)
    {
        try
        {
            std::unique_lock<std::mutex> lock(_mtx);
            _cv.wait(lock, [this]{return getTaskDone() || terminationFlag;});

            if (terminationFlag)
            {
                break;
            }

            //send ACK (type 2) and dummy
            Message msg = {.type = 2, .number = 0};
            queue.send(msg);
            setTaskDone(false);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            break;
        }
    }
}

void ProcessB::taskBReceive(msgQueue& queue)
{
    while(!terminationFlag)
    {
        //std::this_thread::sleep_for(std::chrono::seconds(5));
        try
        {
            static int sum = 0;
            Message msg = queue.receive(1);
            sum += msg.number;
            std::cout << "Current sum is: " << sum << std::endl;
            setTaskDone(true);
            _cv.notify_one();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            break;
        }
    }
}

void ProcessB::notifyAll() 
{
    std::unique_lock<std::mutex> lock(_mtx);
    _cv.notify_all();
}

void ProcessB::runProcessB(msgQueue& queue)
{
    std::thread t1(&ProcessB::taskBReceive, this, std::ref(queue));
    std::thread t2(&ProcessB::taskBSend, this, std::ref(queue));

    t1.join();
    t2.join();
}