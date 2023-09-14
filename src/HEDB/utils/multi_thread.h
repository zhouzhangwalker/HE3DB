#pragma once

#include "ThreadPool.h"

namespace HEDB
{
    template <typename... Args>
    void multi_thread(void (*func)(Args...), int num_threads, int task_size, Args... args)
    {
        ThreadPool pool(num_threads);
        const size_t work_load = (task_size + num_threads - 1) / num_threads;
        for (int w = 0; w < num_threads; ++w)
        {
            size_t start = w * work_load;
            size_t end = std::min<size_t>(start + work_load, task_size);

                if (end > start)
                {
                    pool.enqueue([&](size_t s, size_t e)
                    {
                        for (size_t j = s; j < e; ++j)
                        {
                            func(args...);
                        }
                        
                    }, 
                    start, end);
                }
        }
        pool.wait_until_empty();
        pool.wait_until_nothing_in_flight();
    }
} // namespace HEDB

