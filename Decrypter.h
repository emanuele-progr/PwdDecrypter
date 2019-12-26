//
// Created by Emanuele on 21/12/2019.
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
    long getMean(vector<long> values);

private:
    vector<string> encryptedPasswords;
    vector<string> dictionaryPSW;
    string saltpsw;
};
#endif //PSWDECRYPTER_DECRYPTER_H
