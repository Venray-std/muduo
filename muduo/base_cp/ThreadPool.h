#ifdef  MUDUO_BASE_CP_THREADPOOL_H
#define MUDUO_BASE_CP_THREADPOOL_H

#include "muduo/base/COndition.h"
#include "muduo/base/Mutex.h"
#include "muduo/base/Thread.h"
#include "muduo/base/Types.h"

#include <deque>
#include <vector>

namespace muduo
{

class ThreadPool : noncopyable
{
public:
    typedef std::function<void ()> Task;

    explicit ThreadPool(const string& nameArg = string("ThreadPool"));
    ~ThreadPool();

    void setMaxQueueSize(int maxSize) { maxQueueSize_ = maxSize; }

    void setThreadInitCallback(const Task& cb)
    { threadInitCallback_ = cb; }

    void start(int numThreads);
    void stop();

    const string& name() const
    { return name_; }

    size_t queueSize() const;
    void run(Task f);

private:
    bool isFull() const REQUIRES(mutex_);
    void runInThread();
    Task take();

    mutable MutexLock mutex_;
    Condition notEmpty_ GUARDED_BY(mutex_);
    Condition notFull_ GUARDED_BY(mutex_);
    string name_;
    Task threadInitCallback_;
    std::vector<std::unique_ptr<muduo::Thread>> threads_;
    std::deque<Task> queue_ GUARDED_BY(mutex);
    size_t maxQueueSize_;
    bool running_;
};
}

#endif  //MUDUO_BASE_CP_THREADPOOL_H

/**
explicit ThreadPool(const string& nameArg = string("ThreadPool"));
使用了explicit不能被隐式构造
一般的隐式转换一般通过拷贝初始化，例如A a1 = 1, 显示初始化必须为A a1(1);
const string& 传递引用，且限定只读。一般同时使用，因为既然限定只读了就没道理不传引用

REQUIRES 类型需求
GUARDED_BY 表示变量被某个锁保护
**/
