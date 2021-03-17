/*
 * @Author: your name
 * @Date: 2021-03-04 18:59:52
 * @LastEditTime: 2021-03-11 15:34:59
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: /muduo/muduo/net/inspect/tests/Inspector_test.cc
 */
#include "muduo/net/inspect/Inspector.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/EventLoopThread.h"

using namespace muduo;
using namespace muduo::net;

int main()
{
  EventLoop loop; // 
  EventLoopThread t;
  Inspector ins(t.startLoop(), InetAddress(12345), "test");
  loop.loop();
}

