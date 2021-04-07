# MD5Hashing
Used MD5Hashing algorithm to hashind data files by blocks. Used threads for optimisation (token ring idiom)

1) How to use

After compiling call .exe file without arguments:
 
Used for creation a signature of file. Use: 
   -i= to set url of readed file
   -o= to set url of outed file
   -s= to set blocksize in Mb (default - 1 Mb)
   -d to enable debug printing 
   Examples: 
"FileEncypringVS1.exe" -i="b:\Films\open_lesson_small.mp4" -o="b:\projects\MD5Hashing\data\TestOutput1.txt" -s=5
"FileEncypringVS1.exe" -i="b:\Films\open_lesson_small.mp4" -o="b:\projects\MD5Hashing\data\TestOutput1.txt"

2) MD5Hashing 

Create MD5Hashing and call cypredMessage to returning hashing block of message

See https://www.youtube.com/watch?v=EafNW4UYuhY&t=129s to understanding how algorithm is working

3) ThreadedEncypred

Create ThreadedEncypred with params: input file url, output file url, blocksize (in mb) and debug log flag

Call start() to starting hashing procedure. Numbers of threads have compared with cores of the computer. If you need other number - define __MANUALTHREADCONTROL and change threadNumbers variable.

Token ring idiom.
Each thread read parth of the input file in size of block, hash it and then write result. Before reading/writing thread wait on semaphore when previosly thread post it. After that thread complete reading and hashing or writing procedure and then post the next semaphore. It procedure repeated for every threads and looped by ring: the last one thread posting semaphore of the first.  
As size of block is bigger as hashing work longer that reading.