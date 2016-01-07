#ifndef CURL_EXT_UTILS_THREADPOOL_H
#define CURL_EXT_UTILS_THREADPOOL_H

#include <unistd.h>

#include "am-vector.h"
#include "sm_queue.h"
#include "am-thread-utils.h"


#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);   \
  void operator=(const TypeName&)  


class IWork
{
 public:
  virtual ~IWork() {  
  }

  virtual void Execute() = 0;
};

class ThreadPool
{
 public:
	explicit ThreadPool(const size_t& kWorkersNum) {  
		// cond_ = new ke::ConditionVariable();
    routine_ = new Routine(this);

    for (size_t i = 0; i < kWorkersNum; ++i) {  
      workers_.append(new ke::Thread(routine_));
    }
	}
	~ThreadPool() {  
		//AddWork(NULL);

    for (size_t i = 0; i < workers_.length(); ++i) {  
      AddWork(NULL);
			workers_[i]->Join();
			delete workers_[i];
    }

    // delete cond_;
    delete routine_;
	}

	void AddWork(IWork* work) {  
		cond_.Lock();

    work_queue_.push(work);
    cond_.Notify();

    cond_.Unlock();
	}

 private:

	class Routine : public ke::IRunnable
	{
	 public:
		explicit Routine(ThreadPool* thread_pool) : thread_pool_(thread_pool) {  
		}

		virtual void Run() {  
			IWork* work = NULL;

	    for (;;) { 
        thread_pool_->cond_.Lock();

        while (thread_pool_->work_queue_.empty()) { 
        	thread_pool_->cond_.Wait();
        }

        //dec atomic free workers

        work = thread_pool_->work_queue_.first();
        thread_pool_->work_queue_.pop();

        thread_pool_->cond_.Unlock();

        if (work == NULL) { 
          // thread_pool_->AddWork(NULL);
          break;
        }

        work->Execute();
        delete work;
        //inc atomic free workers
			}
		}

   private:
		ThreadPool* thread_pool_;
	};

  void AddWorker() { 

  }

  void RemoveWorker() { 

  }

	Routine* routine_;
	ke::ConditionVariable cond_;
	Queue<IWork*> work_queue_;
	ke::Vector<ke::Thread*> workers_;

  size_t free_workers_count_;
  size_t undone_work_count_;

  friend Routine;

  DISALLOW_COPY_AND_ASSIGN(ThreadPool);
};


#endif // CURL_EXT_UTILS_THREADPOOL_H
