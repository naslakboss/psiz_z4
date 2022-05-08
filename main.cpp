#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <vector>
#include <chrono>
#include <sstream>

using namespace std;
using namespace chrono;

fstream logFile;
string logFileName = "log.txt";

string getCurrentTimeStr(){
    auto t = time(nullptr);
    auto tm = *localtime(&t);
    ostringstream oss;
    oss << put_time(&tm, "%d-%m-%Y %H:%M:%S");
    return oss.str();
}

void log(string msg) {
    string dateString = getCurrentTimeStr();
    string finalMsg = dateString + " --- " + msg + "\n";
    logFile << finalMsg;
    cout << finalMsg;
}

uint8_t hammingDistance(uint8_t n1, uint8_t n2) {
    uint8_t x = n1 ^ n2; // XOR
    uint8_t setBits = 0;
    while (x > 0) {
        setBits += x & 1;
        x >>= 1;
    }
    return setBits;
}

bool isFileExists(const char *fileName) {
    ifstream infile(fileName);
    return infile.good();
}

int main(int argc, char * argv[]) {
    logFile.open(logFileName, ios_base::app);
    if (!logFile.good()) {
        cout << "Failed to open " << logFileName << endl;
        return 1;
    } else {
        log(logFileName + " has been opened successfully");
    }

    unsigned long BER = 0;

    if(!isFileExists(argv[1]) || !isFileExists(argv[2])){
        log("Invalid file names. Program terminated");
        return 1;
    }
    log("Compared files have been opened successfully");

    ifstream firstFile(argv[1], ios::binary | ios::in);
    ifstream secondFile(argv[2], ios::binary | ios::in); 
    unsigned long counterBits = 0;
    char a{}, b{};
    auto begin = high_resolution_clock::now();
    while (!firstFile.eof()) {
			firstFile >> a;
			secondFile >> b;

			counterBits += 8;
			BER += hammingDistance(a, b);

		}
    auto end = high_resolution_clock::now();
    log("Files: " +  static_cast<string>(argv[1]) + " and " + static_cast<string>(argv[2]) + " have been compared successfully");
    auto elapsed = duration_cast<milliseconds>(end - begin);


    log("Number of compared bits: " + to_string(counterBits));
    log("Number of different bits: " + to_string(BER));
    log("Computation time (ms): " + to_string(elapsed.count()));

    firstFile.close();
    secondFile.close();
    log("Compared files have been closed");

    logFile.close();
    log(logFileName + " has been closed");
    return 0;
}