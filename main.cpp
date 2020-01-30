#include <iostream>
#include <vector>
#include <iomanip>
#include <unistd.h>
#include <algorithm>
#include "Decrypter.h"


using namespace std;

int main(){

    string line;
    ifstream file;
    file.open("100rand_words.txt");
    vector<string> pswToCrack;
    while(getline(file, line)) {
        pswToCrack.push_back(line);
    }
    file.close();

    //vector<string> pswToCrack = { "1212MARI"};
    vector<int> nThreads = {2, 3, 4, 5, 6, 7, 8, 9, 10, 50, 100, 500, 1000,3000, 5000, 7000, 10000};
    int runs = 1;
    vector<long> sequentialTimes, parallelTimes;
    vector<float> speedupsVector;
    vector<float> singleWordSpeedUp;
    string saltpsw = "username";
    Decrypter Decrypter(pswToCrack, saltpsw);
    sequentialTimes = Decrypter.sequentialDecryption(1);
    for(int threads : nThreads) {

        parallelTimes = Decrypter.parallelDecryption(runs, threads);

        speedupsVector = Decrypter.calcSpeedup(sequentialTimes, parallelTimes);
        cout << "*************************** Results with # thread : " << threads << " ***************************"<< endl;

        for(int i=0; i < pswToCrack.size(); i++ ){

            cout << "Password to decrypt : " << pswToCrack[i];
            cout << "  sequential time : " << sequentialTimes[i] << " μs "<< "  parallel time : " << parallelTimes[i] << " μs "<< endl;
            cout << "Number of runs : " << runs << endl ;
            cout << "SPEEDUP : " << fixed << setprecision(5) << speedupsVector[i] << endl;
            cout << "-------------------------------------------------------------------------------"<< endl;
        }
        usleep(5000);
        //singleWordSpeedUp.push_back(speedupsVector[0]);
        singleWordSpeedUp.push_back(Decrypter.getMedian(speedupsVector));
    }



    for(int i=0; i < singleWordSpeedUp.size(); i++){
        cout << singleWordSpeedUp[i] << endl;
    }

}