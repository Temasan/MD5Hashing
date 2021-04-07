#include "threadedencypred.h"
#include <iostream>
#include <fstream>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include "md5hashing.h"

std::vector<std::unique_ptr<boost::interprocess::interprocess_semaphore>> ThreadedEncypred::sequensedBinSemaph_;
boost::mutex ThreadedEncypred::messageMutex;
std::vector<boost::thread> ThreadedEncypred::threads_;
std::ofstream ThreadedEncypred::outputFile_;
std::ifstream ThreadedEncypred::inputFile_;
bool ThreadedEncypred::debugLogFlag_;
uint64_t ThreadedEncypred::blockSizeBytes_;
bool ThreadedEncypred::invalidResult = false;


unsigned long long int fileLength = 0;

unsigned long long int threadIterations = 0;


ThreadedEncypred::ThreadedEncypred(const std::string& inputFilename,
                                   const std::string& outputFilename,
                                   const uint64_t& blocksize,
                                   const bool debugLogFlag)
    : inputFilename_ (inputFilename)
    , outputFilename_ (outputFilename)
{
    debugLogFlag_ = debugLogFlag;
    blockSizeBytes_ = blocksize*1024*1024;
}

ThreadedEncypred::~ThreadedEncypred() = default;

int ThreadedEncypred::start(){
    remove(outputFilename_.c_str());
    invalidResult = false;
    int success = 0;
    fileLength = 0;
    threadIterations = 0;
    // reinit semaphores 
    sequensedBinSemaph_.clear();
    for (size_t i = 0; i < threadNumbers; ++i) {
        // \note for the first semaphore setting 1 number for it immediately working after initialisztion 
        sequensedBinSemaph_.push_back(std::make_unique<boost::interprocess::interprocess_semaphore>(i == 0 ? 2 : 0));
    }
    try{
        if(!blockSizeBytes_) throw std::logic_error("Zero block size exception");

        inputFile_.open(inputFilename_, std::ios::binary);
        outputFile_.open(outputFilename_, std::ios::app | std::ios::binary);


        if(!inputFile_.is_open()) throw std::ios_base::failure("Input file cannot be opened");
        if(!outputFile_.is_open()) throw std::ios_base::failure("Output file cannot be opened");

        inputFile_.sync_with_stdio(false);

        // calculate fileLength and threadIterations
        inputFile_.seekg(0, inputFile_.end);
        fileLength = inputFile_.tellg();
        inputFile_.seekg(0, inputFile_.beg);

        threadIterations = std::ceil(fileLength / blockSizeBytes_);
        for (size_t i = 0; i < threadNumbers; ++i) {
            initNewThread(i);
        }

        for (auto & it: threads_) {
            it.join();
        }

        if (!invalidResult) {
            std::cout << "Successful " << std::endl;
        }
        else {
            throw std::runtime_error("Thread working exception catched");
        }

    }  catch (std::exception& e) {
        std::cout << e.what() ;

        for (auto & i : threads_) {
            i.interrupt();
        }
        remove(outputFilename_.c_str());
        success = -1;
    }  catch (...) {
        std::cout << "Critical exception catched"<< std::endl;
        remove(outputFilename_.c_str());
        success = -1;
    }

    inputFile_.close();
    outputFile_.close();
    return success;
}

void ThreadedEncypred::initNewThread(const size_t threadNumber) {
    printDebugMessage("Thread " + std::to_string(threadNumber) + " init new the work\n");

    threads_.push_back(boost::thread(threadCycle,
        threadNumber));
}

void ThreadedEncypred::threadCycle(const size_t number){
    std::unique_ptr<MD5Hashing> hashent = std::make_unique<MD5Hashing>();
    char* buffer = new char[blockSizeBytes_+1];
    size_t nextThreadNumber = number == threadNumbers - 1 ? 0 : number + 1;
    uint64_t iterations = 0;

    auto* streambuf = inputFile_.rdbuf();
    sequensedBinSemaph_[number]->wait();
    try{
        while (!inputFile_.eof()) {
            memset(&(buffer[0]), '\0', blockSizeBytes_ + 1);
            uint64_t index = (threadNumbers * iterations + number);
            inputFile_.seekg(index * blockSizeBytes_, std::ios::beg);
            
            ++iterations;

            streambuf->sgetn(buffer, blockSizeBytes_);
            inputFile_.peek();
            
            printDebugMessage("Thread " + std::to_string(number) + " read file\n");

            std::string str(buffer, blockSizeBytes_);

            sequensedBinSemaph_[nextThreadNumber]->post();

            std::string result = hashent->cypredMessage(str);
            sequensedBinSemaph_[number]->wait();
            writeOut(result);

            printDebugMessage("Thread " + std::to_string(number) + " writed result\n");
            sequensedBinSemaph_[nextThreadNumber]->post();

            if (invalidResult)
                throw std::runtime_error("Broke the threads\n");
            if (inputFile_.eof()) break;
            sequensedBinSemaph_[number]->wait();
        }
        printDebugMessage("Thread "+ std::to_string(number) + " completed the work\n");

    } catch(std::exception e){
        std::cout <<e.what() << std::endl;
        printDebugMessage("Thread "+ std::to_string(number) + " reset the work\n");
        invalidResult = true;
    }

    sequensedBinSemaph_[nextThreadNumber]->post();
    delete [] buffer;
}

void ThreadedEncypred::writeOut(const std::string & str){
    outputFile_.write(str.c_str(),str.size());
}

void ThreadedEncypred::printDebugMessage(const std::string message) {
    if (debugLogFlag_) {
        boost::mutex::scoped_lock lock(messageMutex);
        std::cout << message;
    }
}