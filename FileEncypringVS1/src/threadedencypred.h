#ifndef THREADEDENCYPRED_H
#define THREADEDENCYPRED_H
#include <exception>

#ifdef _WIN32
#define BOOST_USE_WINDOWS_H
#endif

#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/thread/thread.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/filesystem/fstream.hpp>

#ifdef __MANUALTHREADCONTROL
const size_t threadNumbers = 6;
#else
const size_t threadNumbers = std::thread::hardware_concurrency();
#endif // __MANUALTHREADCONTROL



/*
* \class ThreadedEncypred create hash of file input and write in to output file
*/
class ThreadedEncypred
{
public:/*
    * \class ThreadedEncypred
    * \param blocksize usen in Mb representation
    */
    ThreadedEncypred(const std::string& inputFilename,
                     const std::string& outputFilename,
                     const uint64_t& blocksize = 1,
                     const bool debugLogFlag = false);
    ~ThreadedEncypred();
    /**
    * \brief create hash from one file and write it to another
    * Token ring realisation - threads read/write data in deffinetly sequence and move their read/write tokens to the next  
    * 1) check filepath fo input
    * 2) calculate filesize, calculate thread quantity to working
    * 3) create new threads in threadNumbers quantity and get it their number
    * 3.1) first thread have an increasing by 1 semaphore state and started immediately 
    * 4) wait for all thread compliting
    */
    int start();
private:
    /**
    * \brief thread cycle function
    * \param number thread number to synchronisation between others
    * 1) init some variables, wait self semaphore
    * 2) after waiting check EOF file flag and start working
    * 3) start to read input file and then post semaphore the next thread
    * 4) calculate hash
    * 5) wait self semaphore
    * 6) start to write in file and then post semaphore the next thread
    * 7) check EOF file, if true - break (go to 9). Else - wait self semapore
    * 8) go to 2
    * 9) post semaphore the next thread, complete working
    */
    static void threadCycle(const size_t number);
    /**
    * \brief write hash part in the file
    */
    static void writeOut(const std::string & str);
    /**
    * \brief init new thread by part of file and number
    */
    void initNewThread(const size_t threadNumber);
    static void printDebugMessage(const std::string message);

    const std::string inputFilename_;
    const std::string outputFilename_;
    /*
    * \brief used for debuging information
    */
    static bool debugLogFlag_;
    /*
    * \brief block size in Byte
    */
    static uint64_t blockSizeBytes_;

    static std::ofstream outputFile_;
    static std::ifstream inputFile_;
    static std::vector<boost::thread> threads_;
    static boost::mutex messageMutex;
    /*
    * \brief used semaphore for multiple access
    */
    static std::vector<std::unique_ptr<boost::interprocess::interprocess_semaphore>> sequensedBinSemaph_;
    /*
    * \brief flag of critical error in thread witch reset all threads
    */
    static bool invalidResult;
};

#endif // THREADEDENCYPRED_H
