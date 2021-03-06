//
// Created by Emanuele on 11/12/2019.
//

#ifndef PSWDECRYPTER_DECRYPTER_H
#define PSWDECRYPTER_DECRYPTER_H

#include <fstream>
#include<vector>
#include<string>

using namespace std;

class Decrypter {
public:
    Decrypter(vector<string> pswToCrack, string salt);
    vector<long> sequentialDecryption(int runs);
    vector<long> parallelDecryption(int runs, int nThreads);
    vector<float> calcSpeedup(vector<long> sequentialTimes, vector<long> parallelTimes);
    float getMean(vector<float> values);
    float getMedian(vector<float> values);
    vector<string> dictionaryPSW;

private:
    vector<string> encryptedPasswords;
    string saltpsw;
};

#endif //PSWDECRYPTER_DECRYPTER_H
