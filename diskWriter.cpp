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


void generateResults(string writePath, string logPath, int passes, unsigned long long int nsSleep, unsigned long long int  nsSleepStep, bool useBuffer, bool zeroSleep, float maxDev, bool displayOutliers)
{
    // Given as us
    nsSleep = nsSleep * 1000;
    nsSleepStep = nsSleepStep * 1000;

    const ulong MAX_SLEEP = 999999998;

    struct timespec timeOut,remains;
    timeOut.tv_sec = 0;
    timeOut.tv_nsec = nsSleep;

    struct timespec timeOutMax,remainsMax;
    timeOutMax.tv_sec = 0;
    timeOutMax.tv_nsec = MAX_SLEEP;

    ulong extraSleeps = 0;

    while (nsSleep >= MAX_SLEEP)
    {
        cout << "nsSleep = " << nsSleep << endl;
        nsSleep -= MAX_SLEEP;
        extraSleeps++;

        cout << "extrasleeps ++: " << extraSleeps << "\n";
        cout << "nsSleep = " << nsSleep << endl;
    }
    timeOut.tv_nsec = nsSleep;

    ostringstream output;

    // CSV Format
    output << "pass_number, pass_time, average_time, microseconds_since_last_write, nsSleep_time\n";

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
        //cout << "Time since last write = " << usTimeSinceLastWrite << endl;

        passStart = chrono::steady_clock::now();
        writeFile(writePath, useBuffer);
        passEnd = chrono::steady_clock::now();

        usPassDuration = chrono::duration_cast<chrono::microseconds>(passEnd- passStart).count();
        usTotalTime += usPassDuration;
        usAvgPassTime = usTotalTime / i;

        // Check if we get wacky results
        if (usPassDuration > (usAvgPassTime * maxDev))
        {
            usTotalTime -= usPassDuration;
            usAvgPassTime = usTotalTime / (i - 1);

            if (displayOutliers)
            {
                cout << "Not adding to avg:\n";
                cout << "Outlier: " << usPassDuration << " microseconds\n";
                cout << "Avg    : " << usAvgPassTime << " microseconds\n\n";

                output << i << ", " << usPassDuration << ", " << usAvgPassTime << ", " << usTimeSinceLastWrite << ", " << (nsSleep + extraSleeps * MAX_SLEEP) / 1000 << "\n";
            }
            else
            {
                i--;
                zeroSleep = true;
                cout << "Not adding to csv:\n";
                cout << "Outlier: " << usPassDuration << " microseconds\n";
                cout << "Avg    : " << usAvgPassTime << " microseconds\n\n";
            }
        }
        else
        {
            output << i << ", " << usPassDuration << ", " << usAvgPassTime << ", " << usTimeSinceLastWrite << ", " << (nsSleep + extraSleeps * MAX_SLEEP) / 1000 << "\n";
        }


        // if zeroSleep is false we do not want to skip on first pass (=true)
        // if zero sleep is false and we are not no the first pass

        // If zero sleep is true, return false if on the first pass

        // False when zeroSleep and i == 1
        // True when !zeroSleep

        if (!zeroSleep)
        {
            //cout << "Sleep for " << nsSleep << " ns\n";

            nanosleep(&timeOut, &remains);

            for (ulong j = 0; j < extraSleeps; j++)
            {
                //cout << "SLEEP FOR " << MAX_SLEEP << " ns\n";
                nanosleep(&timeOutMax, &remainsMax);
            }

            // Get difference between the new nsSleep and the max nsSleep
            nsSleep += nsSleepStep;
            while (nsSleep >= MAX_SLEEP)
            {
                //cout << "nsSleep = " << nsSleep << endl;
                nsSleep -= MAX_SLEEP;
                extraSleeps++;

                //cout << "extrasleeps ++: " << extraSleeps << "\n";
                //cout << "nsSleep = " << nsSleep << endl;
            }
            timeOut.tv_nsec = nsSleep;
        }

        if (zeroSleep && i != 1)
            zeroSleep = !zeroSleep;

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
    cout << " -b, --use-buffer=1\t [0|1] Use the buffer method of writing to disk?\n";
    cout << " -z, --initial-zero-sleep=0\t [0|1] Use 0 sleep for second pass?\n";
    cout << " -d, --max-deviation=20\t Acceptable deviation from avg \n";
    cout << " -u, --display-outliers=1\t [0|1] Include the passes caught by -d in the .csv (Does not affect average) \n";
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

    unsigned long long int sleep = 0;
    flagMap.insert( pair<string, int>("--sleep", 2));
    flagMap.insert( pair<string, int>("-s", 2));

    unsigned long long int sleepStep = 0;
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

    bool zeroSleep = 0;
    flagMap.insert( pair<string, int>("--initial-zero-sleep", 7));
    flagMap.insert( pair<string, int>("-z", 7));

    float maxDev = 20;
    flagMap.insert( pair<string, int>("--max-deviation", 8));
    flagMap.insert( pair<string, int>("-d", 8));

    bool displayOutliers = 1;
    flagMap.insert( pair<string, int>("--display-outliers", 9));
    flagMap.insert( pair<string, int>("-u", 9));

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
                    logPath = value;
                    break;

                case(1):
                    passes = stoi(value);
                    break;

                case(2):
                    sleep = stoull(value);
                    //cout << value << endl;
                    //sleep = sleep;
                    break;

                case(3):
                    sleepStep = stoull(value);
                    //sleepStep = sleepStep;
                    break;

                case(4):
                    writePath = value;
                    break;

                case(5):
                    useBuffer = stoi(value);
                    break;

                case(7):
                    zeroSleep = stoi(value);
                    break;

                case(8):
                    maxDev = stof(value);
                    break;

                case(9):
                    displayOutliers = stoi(value);
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


    generateResults(writePath, logPath, passes, sleep, sleepStep, useBuffer, zeroSleep, maxDev, displayOutliers);

    return 0;
}
