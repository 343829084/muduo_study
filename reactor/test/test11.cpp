//
// Created by jxq on 19-9-1.
//

#include <stdio.h>
#include <sys/uio.h>
#include <cstring>
#include <iostream>
#include <sys/timerfd.h>
#include <time.h>
#include "../../base/Timestamp.h"
#include "../EventLoop.h"
#include "../Channel.h"

using namespace std;

muduo::EventLoop* g_loop;

void timeout(muduo::Timestamp receiveTime)
{
    printf("%s Timestamp\n", receiveTime.toFormattedString().c_str());
    g_loop->quit();
}

int main()
{
    printf("%s started\n", muduo::Timestamp::now().toFormattedString().c_str());
    muduo::EventLoop loop;
    g_loop = &loop;

    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    muduo::Channel channel(&loop, timerfd);
    channel.setReadCallback(timeout);
    channel.enableReading();

    struct itimerspec howlong;
    bzero(&howlong, sizeof(howlong));
    howlong.it_value.tv_sec = 5;
    ::timerfd_settime(timerfd, 0, &howlong, NULL);

    loop.loop();

    ::close(timerfd);
    return 0;
}