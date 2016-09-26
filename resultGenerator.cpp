#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;

void writeBuffer(string writePath)
{
    ostringstream buffer;

    buffer << "Some lines of text" << endl;
    buffer << "Could be randomized to prevent caching" << endl;
    buffer << "In November 1907, Swann was elected a judge of the Court of General Sessions, " << endl;
    buffer << "and took office on January 1, 1908. He was New York County District Attorney " << endl;
    buffer << "from 1916 to 1921, elected in a special election in November 1915 defeating " << endl;
    buffer << "the incumbent Charles A. Perkins, and re-elected in November 1917 to a full term. " << endl;
    buffer << "Swann was accused several times by reform organizations of misconduct and malfeasance in " << endl;
    buffer << "office, urging his removal. Since the precedent of 1900, when Asa Bird Gardiner was removed by Gov. " << endl;
    buffer << "Theodore Roosevelt, this had become a constant move in New York City politics, but neither Republican " << endl;
    buffer << "Charles S. Whitman nor Democrat Al Smith saw reason enough to act." << endl;

    ofstream file(writePath);

    file << buffer.str();

    file.close();
}

void writeNoBuffer(string writePath)
{
    ofstream file(writePath);

    file << "Some lines of text" << endl;
    file << "Could be randomized to prevent caching" << endl;
    file << "In November 1907, Swann was elected a judge of the Court of General Sessions, " << endl;
    file << "and took office on January 1, 1908. He was New York County District Attorney " << endl;
    file << "from 1916 to 1921, elected in a special election in November 1915 defeating " << endl;
    file << "the incumbent Charles A. Perkins, and re-elected in November 1917 to a full term. " << endl;
    file << "Swann was accused several times by reform organizations of misconduct and malfeasance in " << endl;
    file << "office, urging his removal. Since the precedent of 1900, when Asa Bird Gardiner was removed by Gov. " << endl;
    file << "Theodore Roosevelt, this had become a constant move in New York City politics, but neither Republican " << endl;
    file << "Charles S. Whitman nor Democrat Al Smith saw reason enough to act." << endl;

    file.close();
}

void writeFile(string writePath, bool useBuffer)
{
    if (useBuffer)
        writeBuffer(writePath);
    else
        writeNoBuffer(writePath);
}


void generateResults(string writePath, string logPath, int passes, ulong sleep, ulong sleepStep, bool useBuffer)
{
    const ulong MAX_SLEEP = 899999999;

    struct timespec timeOut,remains;
    timeOut.tv_sec = 0;
    timeOut.tv_nsec = sleep;

    struct timespec timeOutMax,remainsMax;
    timeOut.tv_sec = 0;
    timeOut.tv_nsec = MAX_SLEEP;

    ulong extraSleeps = 0;

    while (sleep > MAX_SLEEP)
    {
        sleep -= MAX_SLEEP;
        extraSleeps++;
    }
    timeOut.tv_nsec = sleep;

    ostringstream output;

    // CSV Format
    // pass, pass_time, average_time, microseconds_since_last_write, sleep

    chrono::steady_clock::time_point overallStart;
    chrono::steady_clock::time_point passStart;
    chrono::steady_clock::time_point passEnd;
    chrono::steady_clock::time_point lastWrite;

    // us = microseconds
    ulong usPassDuration = 0;
    ulong usAvgPassTime = 0;

    ulong usTotalTime = 0;
    ulong usTimeSinceLastWrite = 0;

    overallStart = chrono::steady_clock::now();
    passEnd = chrono::steady_clock::now();

    for (int i = 1; i < passes + 1; i++)
    {
        usTimeSinceLastWrite = chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - passEnd).count();

        passStart = chrono::steady_clock::now();
        writeFile(writePath, useBuffer);
        passEnd = chrono::steady_clock::now();

        usPassDuration = chrono::duration_cast<chrono::microseconds>(passEnd- passStart).count();

        usTotalTime += usPassDuration;

        usAvgPassTime = usTotalTime / i;

        output << i << ", " << usPassDuration << ", " << usAvgPassTime << ", " << usTimeSinceLastWrite << ", " << (sleep + extraSleeps * MAX_SLEEP) / 1000 << "\n";

        //cout << "SLEEP FOR " << sleep << " ns\n";

        nanosleep(&timeOut, &remains);

        for (ulong j = 0; j < extraSleeps; j++)
        {
            //cout << "SLEEP FOR " << MAX_SLEEP << " ns\n";
            nanosleep(&timeOutMax, &remainsMax);
        }


        // Get difference between the new sleep and the max sleep
        sleep += sleepStep;
        while (sleep > MAX_SLEEP)
        {
            sleep -= MAX_SLEEP;
            extraSleeps++;
        }
        timeOut.tv_nsec = sleep;

    }

    cout << "Passes: " << passes << endl;
    cout << "Average time: " << usAvgPassTime << " microseconds" << endl;

    ofstream file(logPath);
    file << output.str();
    file.close();
}

void printHelp()
{
    cout << "Usage: ./resultGenerator.out <OPTION> <VALUE>\n\n";

    cout << "OPTIONS:\n";
    cout << " -w, --write-path \t The path to write the test file.\n";
    cout << " -l, --log-path \t The path to write the results as a csv\n";
    cout << " -p, --passes \t\t Number of times to write the test file and record times\n";
    cout << " -s, --sleep \t\t Microseconds to wait between passes\n";
    cout << " -t, --sleep-step \t Microseconds to increase the sleep after each pass\n";
    cout << " -b, --use-buffer\t [0|1] Use the buffer method of writing to disk?\n";
    cout << " -h, --help \t\t Print this help text\n";

}

int main(int argc, char *argv[])
{

    map<string,int> flagMap;

    string logPath = "results.csv";
    flagMap.insert( pair<string, int>("--log-path", 0));
    flagMap.insert ( pair<string,int>("-l",0) );

    int passes = 0;
    flagMap.insert( pair<string, int>("--passes", 1));
    flagMap.insert( pair<string, int>("-p", 1));

    ulong sleep = 0;
    flagMap.insert( pair<string, int>("--sleep", 2));
    flagMap.insert( pair<string, int>("-s", 2));

    ulong sleepStep = 0;
    flagMap.insert( pair<string, int>("--sleep-step", 3));
    flagMap.insert( pair<string, int>("-t", 3));

    string writePath = "testFile.txt";
    flagMap.insert( pair<string, int>("--write-path", 4));
    flagMap.insert( pair<string, int>("-w", 4));

    bool useBuffer = 0;
    flagMap.insert( pair<string, int>("--use-buffer", 5));
    flagMap.insert( pair<string, int>("-b", 5));

    flagMap.insert( pair<string, int>("--help", 6));
    flagMap.insert( pair<string, int>("-h", 6));

    std::map<string,int>::iterator it;

    for (int i = 1; i < argc; i = i + 2)
    {
        string flag = argv[i];
        string value = "EMPTY";

        if (i + 1 < argc)
            value = argv[i + 1];

        it = flagMap.find(flag);

        if (it != flagMap.end())
        {
            switch(flagMap.find(flag)->second)
            {
                case(0):
                    logPath = argv[i];
                    break;

                case(1):
                    passes = stoi(value);
                    break;

                case(2):
                    sleep = stol(value);
                    sleep = sleep * 1000;
                    break;

                case(3):
                    sleepStep = stol(value);
                    sleepStep = sleepStep * 1000;
                    break;

                case(4):
                    writePath = value;
                    break;

                case(5):
                    useBuffer = stoi(value);
                    break;

                case(6):
                    printHelp();
                    return 0;
                    break;
            }
        }
        else
        {
            cout << "Unkown switch: " << flag << endl;
        }
    }

    generateResults(writePath, logPath, passes, sleep, sleepStep, useBuffer);

    return 0;
}
