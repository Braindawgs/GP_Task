#pragma once

#include "messageQueueMgmt.h"

#include <iostream>
#include <thread>
#include <mutex>
#include <random>
#include <chrono>
#include <semaphore>
#include <condition_variable>


void runProcessA(msgQueue& queue);
void taskASend(msgQueue& queue);
void taskAReceive(msgQueue& queue);



void runProcessB(msgQueue& queue);
void taskBSend(msgQueue& queue);
void taskBReceive(msgQueue& queue);
