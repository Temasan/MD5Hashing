"x64/Debug/FileEncypringVS1.exe"

SET fileCounts=4
SET testFile1="b:\Films\open_lesson_small.mp4"
SET testfile2="b:\projects\FileEncypringVS1_save\FileEncypringVS1\FileEncypringVS1\data\TestInput2.txt"
SET testfile3="b:\projects\FileEncypringVS1_save\FileEncypringVS1\FileEncypringVS1\data\TestInput1.txt"
SET testfile3_3="b:\projects\FileEncypringVS1_save\FileEncypringVS1\FileEncypringVS1\data\TestInput1_1.txt"
SET testfile4="b:\projects\FileEncypringVS1_save\FileEncypringVS1\FileEncypringVS1\data\TestInput3.txt"
SET testFile5="b:\Films\Avatar_1080p.mkv"
SET testFile6="b:\projects\FileEncypringVS1_save\FileEncypringVS1\FileEncypringVS1\data\lk1.pdf"
SET testFile7="b:\Загрузки\android-studio-ide-201.6858069-windows.exe"


SET outPref=b:\projects\MD5Hashing\MD5Hashing\FileEncypringVS1\data

REM "x64/Debug/FileEncypringVS1.exe" -i=%testfile3% -o=%outPref%\TestOutput1_1.txt -d -s=100
REM "x64/Debug/FileEncypringVS1.exe" -i=%testfile3_3% -o=%outPref%\TestOutput1_1_1.txt -d -s=100
REM fc /b %outPref%\TestOutput1_1.txt %outPref%\TestOutput1_1_1.txt


FOR /L %%i IN (0,1,%fileCounts%) DO "x64/Debug/FileEncypringVS1.exe" -i=%testFile1% -o=%outPref%\TestOutput%%i.txt -s=2
ECHO "Work's is completed, try to compare";
FOR /L %%k IN (0,1,%fileCounts%) DO (
	FOR /L %%j IN (0,1,%fileCounts%) DO (
		fc /b %outPref%\TestOutput%%k.txt %outPref%\TestOutput%%j.txt
	);
);