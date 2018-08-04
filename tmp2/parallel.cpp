//http://www.alecjacobson.com/weblog/?p=4544
#include <vector>
#include <thread>
#include <iostream>

int main(int argc, char *argv[])
{
  const size_t nloop = 11;

  // Serial version
  {
    // Pre loop
    std::cout<<"serial:"<<std::endl;
    // loop over all items
    for(int i = 0;i<nloop;i++)
    {
      // inner loop
      {
        const int j = i*i;
        std::cout<<j<<std::endl;
      }
    }
    // Post loop
    std::cout<<std::endl;
  }

  // Parallel version
  // number of threads
  const size_t nthreads = std::thread::hardware_concurrency();
  {
    // Pre loop
    std::cout<<"parallel ("<<nthreads<<" threads):"<<std::endl;
    std::vector<std::thread> threads(nthreads);
    std::mutex critical;
    for(int t = 0;t<nthreads;t++)
    {
      threads[t] = std::thread(std::bind(
        [&](const int bi, const int ei, const int t)
        {
          // loop over all items
          for(int i = bi;i<ei;i++)
          {
            // inner loop
            {
              const int j = i*i;
              // (optional) make output critical
              std::lock_guard<std::mutex> lock(critical);
              std::cout<<j<<std::endl;
            }
          }
        },t*nloop/nthreads,(t+1)==nthreads?nloop:(t+1)*nloop/nthreads,t));
    }
    std::for_each(threads.begin(),threads.end(),[](std::thread& x){x.join();});
    // Post loop
    std::cout<<std::endl;
  }
}
