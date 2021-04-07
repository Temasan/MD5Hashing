#include <iostream>
#include <algorithm>
#include "boost/filesystem/operations.hpp"
#include "src/threadedencypred.h"

//\brief disable to manual controlling threads count 
//#define __MANUALTHREADCONTROL


static constexpr char help[] =
"Used for creation a signature of file. Use: \n"
"   -i= to set url of readed file \n"
"   -o= to set url of outed file \n"
"   -s= to set blocksize in Mb (default - 1 Mb) \n"
"   -d to enable debug printing \n"
"   Examples: \n"
"\"FileEncypringVS1.exe\" -i=\"b:\\Films\\open_lesson_small.mp4\" -o=\"b:\\projects\\MD5Hashing\\data\\TestOutput1.txt\" -s=5\n"
"\"FileEncypringVS1.exe\" -i=\"b:\\Films\\open_lesson_small.mp4\" -o=\"b:\\projects\\MD5Hashing\\data\\TestOutput1.txt\"\n";

/*
* example:
* -i=b:\projects\FileEncypringVS1\FileEncypringVS1\FileEncypringVS1\data\TestInput0.txt
* -o=b:\projects\FileEncypringVS1\FileEncypringVS1\FileEncypringVS1\data\TestOutput1.txt
* -s=800
*/

enum CommandArgs {
    inputFile = 0x1,
    outputFile,
    blocksize,
    debugLog
};

CommandArgs argToCommand(const char* argv) {
    if (argv[0] != '-')
        throw std::invalid_argument("Syntax error. First letter of argument must be \'-\'  instead of " + argv[0]);

    if (argv[1] == 'd') return debugLog;
    if (argv[2] != '=')
        throw std::invalid_argument("Syntax error. Third letter of argument must be \'=\'  instead of " + argv[2]);
    if (argv[1] == 'i') return inputFile;
    if (argv[1] == 'o') return outputFile;
    if (argv[1] == 's') return blocksize;
    throw std::invalid_argument("Unkown argument used: " + argv[1]);
}

template <typename T> 
void initPropChechOnFilled(T &data, const std::string& value, const std::string& arg, const std::string exceptionTxt) {
    if (!data.empty()) throw std::invalid_argument(exceptionTxt + arg);
    if (value.empty()) throw std::invalid_argument("Empty argument value -" + arg);
    data = value;
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");

    try {
        boost::filesystem::path readedFile = "";
        boost::filesystem::path writedFile = "";
        std::string blockSize = "";
        bool debugLogFlag = false;
        for (int i = 1; i < argc; ++i) {
            std::cout << "Argument: " << argv[i] << std::endl;
            CommandArgs argRes = argToCommand(argv[i]);
            std::string arg = argv[i], value = argv[i];
            value.replace(0, 3, "");
            switch (argRes) {
            case inputFile:
                initPropChechOnFilled(readedFile, value, arg, "Ambiguous using readed filepath");
                break;
            case outputFile:
                initPropChechOnFilled(writedFile, value, arg, "Ambiguous using readed filepath ");
                break;
            case blocksize:
                initPropChechOnFilled(blockSize, value, arg, "Ambiguous using blocksize ");
                break;
            case debugLog:
                debugLogFlag = true;
                break;
            }
        }
        if (argc != 1) {
            uint64_t blockSizeI = std::atoi(blockSize.c_str());
            blockSize = blockSize.empty() ? "1" : blockSize;
            return ThreadedEncypred(readedFile.string(), writedFile.string(), std::atoi(blockSize.c_str()), debugLogFlag).start();
        }
        else {
            std::cout << help << std::endl;
        }
    }
    catch (const std::invalid_argument e) {
        std::cout << e.what() << std::endl;
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
