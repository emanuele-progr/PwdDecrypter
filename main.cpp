#include <iostream>
#include <vector>
#include <unistd.h>
#include <iomanip>
#include "Decrypter.h"

using namespace std;

void testAndCompare(vector <string> pswToCrack, int runs, vector <int> nThreads);

int main(){

    //read text file of random dictionary words and store in a vector

    string line;
    ifstream file;
    file.open("100rand_words.txt");
    vector<string> pswToCrack;
    while(getline(file, line)) {
        pswToCrack.push_back(line);
    }
    file.close();

    //vector<string> pswToCrack = { "sauske00"};
    vector<int> nThreads = {2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20, 32};
    int runs = 1;
    /*
    string saltpsw = "st";
    Decrypter Decrypter(pswToCrack, saltpsw);
    Decrypter.sequentialDecryption(runs);
    Decrypter.parallelDecryption(runs, 3);
    */

    testAndCompare(pswToCrack,runs,nThreads);


}


void testAndCompare(vector <string> pswToCrack, int runs, vector <int> nThreads){

    vector<long> sequentialTimes, parallelTimes;
    vector<float> speedupsVector;
    vector<float> singleWordSpeedUp;
    string saltpsw = "st";
    Decrypter Decrypter(pswToCrack, saltpsw);

    //sequential time calculation

    sequentialTimes = Decrypter.sequentialDecryption(runs);

    //cycle to test different # threads and speedup calculation

    for(int threads : nThreads) {

        parallelTimes = Decrypter.parallelDecryption(runs, threads);

        speedupsVector = Decrypter.calcSpeedup(sequentialTimes, parallelTimes);
        cout << "*************************** Results with # thread : " << threads << " ***************************"<< endl;

        for(int i=0; i < pswToCrack.size(); i++ ){

            cout << "Password to decrypt : " << pswToCrack[i];
            cout << "  sequential time : " << sequentialTimes[i] << " ms "<< "  parallel time : " << parallelTimes[i] << " ms "<< endl;
            cout << "Number of runs : " << runs << endl ;
            cout << "SPEEDUP : " << fixed << setprecision(5) << speedupsVector[i] << endl;
            cout << "-------------------------------------------------------------------------------"<< endl;
        }
        usleep(5000);

        //singleWordSpeedUp.push_back(Decrypter.getMedian(speedupsVector));
        singleWordSpeedUp.push_back(Decrypter.getMean(speedupsVector));
    }


    //cycle to output a summary of speedup values

    for(int i=0; i < singleWordSpeedUp.size(); i++){

        cout << singleWordSpeedUp[i] << endl;
    }

}
