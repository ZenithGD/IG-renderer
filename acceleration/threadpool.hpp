#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

/**
 * @brief Generic task queue.
 * 
 * @tparam TaskType The task type.
 * @tparam ResultType The result type.
 * @tparam N The number of threads
 */
template <typename TaskType, typename ResultType>
class Threadpool {
public:

    /**
     * @brief Construct a new Threadpool object
     * Initializes the pool of threads and runs the task loop
     */
    Threadpool(unsigned int n) : shouldStop(false) {
        for ( unsigned int i = 0; i < n; i++ ) {
            #ifdef DEBUG
                cout << "Creating thread " << i << endl;
            #endif
            threads.push_back(std::thread(&Threadpool::runTaskLoop, this, i));
        }
    }

    /**
     * @brief Enqueue a task and notify any free thread.
     * This operation is mutually exclusive with any other operation that
     * reads/modifies the task queue.
     * 
     * @param task The task to enqueue.
     */
    void enqueueTask(const TaskType& task) {
        std::unique_lock<std::mutex> lck(taskMtx);
        taskQueue.push(task);
        waitTaskQueue.notify_one();
    }

    /**
     * @brief Pop a result from the result queue.
     * If the queue is empty, this operation is blocking until a result is added
     * into the queue.
     * 
     * @return ResultType 
     */
    ResultType getResult() {
        std::unique_lock<std::mutex> lck(resultMtx);
        // wait until the result queue isn't empty
        waitResultQueue.wait(lck, [this] {
            return !resultQueue.empty();
        });

        auto val = resultQueue.front();
        resultQueue.pop();
        return val;
    }

    /**
     * @brief Destroy the Threadpool object
     * This will instruct every thread to stop.
     */
    ~Threadpool() {
        {
            std::unique_lock<std::mutex> lck(taskMtx);
            shouldStop = true;
            
            // notify every thread
            waitTaskQueue.notify_all();
        }

        for ( std::thread& active_thread : threads ) {
            // controlled termination
            active_thread.join();
        }
    }


private:
    std::queue<TaskType> taskQueue;
    std::queue<ResultType> resultQueue;

    /**
     * @brief Run the task loop.
     * This operation runs on every worker thread, and waits for tasks to be enqueued
     * in the task queue.
     * 
     * The task will run in parallel, and its result will be enqueued to the result queue.
     * @param i The thread id (for debug purposes)
     */
    void runTaskLoop(int i) {

        for ( ;; ) {
            // Pop task from task queue or stop
            TaskType task;
            {
                std::unique_lock<std::mutex> lck(taskMtx);

                // wait until the task queue isn't empty or if the task loop should stop
                waitTaskQueue.wait(lck, [this] {
                    return !taskQueue.empty() || shouldStop;
                });

                if ( shouldStop ) {
                    #ifdef DEBUG
                        cout << "thread " << i << " stopped" << endl;
                    #endif
                    return;
                }

                task = taskQueue.front();
                taskQueue.pop();

            }

            // perform task
            ResultType result = task();

            {
                std::unique_lock<std::mutex> lck(resultMtx);
                resultQueue.push(result);
                
                // notify result in result queue
                waitResultQueue.notify_one();
            }
        }
    };

    // Thread array
    std::vector<std::thread> threads;

    bool shouldStop;
    std::condition_variable waitTaskQueue, waitResultQueue;
    std::mutex taskMtx, resultMtx;
};