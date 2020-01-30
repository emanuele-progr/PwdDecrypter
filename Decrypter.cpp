//
// Created by Emanuele on 21/12/2019.
//
#include <stdexcept>
#include <string.h>
#include <omp.h>
#include <math.h>
#include <crypt.h>
#include <chrono>
#include <iostream>
#include <unistd.h>
#include <algorithm>
#include "Decrypter.h"

using namespace std;

Decrypter::Decrypter(vector<string> pswToCrack, string salt) {

    encryptedPasswords.reserve(pswToCrack.size());
    string line;
    string passwordString;

    for (string & password : pswToCrack) {
        passwordString = crypt(password.c_str(), salt.c_str());
        encryptedPasswords.push_back(passwordString);
    }

    this->saltpsw = salt;

    ifstream file;
    file.open("psw.txt");


    while(getline(file, line)) {
        dictionaryPSW.push_back(line);
    }


    file.close();

}
float Decrypter::getMedian(vector<float> values)
{
    size_t size = values.size();

    if (size == 0)
    {
        return 0;  // Undefined, really.
    }
    else
    {
        sort(values.begin(), values.end());
        if (size % 2 == 0)
        {
            return (values[size / 2 - 1] + values[size / 2]) / 2;
        }
        else
        {
            return values[size / 2];
        }
    }
}
float Decrypter::getMean(vector<float> values) {
    float sum = 0;

    for (float value : values) {
        sum += value;
    }
    return sum / values.size();
}

vector<long> Decrypter::sequentialDecryption(int runs)  {


    vector<long> times;
    times.reserve(encryptedPasswords.size());
    long singleRunTimes = 0;

    for (string& pswToCrack : encryptedPasswords) {
        singleRunTimes = 0;

        for (int i = 0; i < runs; i++) {
            auto start = chrono::steady_clock::now();

            for (string& password : dictionaryPSW) {
                string pswEncrypted(crypt(password.c_str(), saltpsw.c_str()));
                //cout << pswEncrypted << endl;

                if (pswToCrack == pswEncrypted) {
                    //cout << "Generated : " << pswEncrypted << endl;
                    //cout << "Target : " << pswToCrack << endl;
                    //cout << "Password FOUND : " << password << endl;
                    break;
                }
            }



            auto end = chrono::steady_clock::now();
            auto elapsed_time = chrono::duration_cast<chrono::microseconds>(end - start).count();
            singleRunTimes += elapsed_time;
        }

        times.push_back(singleRunTimes/runs);
    }

    return times;
}


vector <long> Decrypter::parallelDecryption(int runs, int nThreads) {


    vector<long> times;
    times.reserve(encryptedPasswords.size());
    long singleRunTimes = 0;

    for (string &pswToCrack : encryptedPasswords) {
        singleRunTimes = 0;

        for (int i = 0; i < runs; i++) {

            int workloadThread = static_cast<int>(ceil((double) dictionaryPSW.size() / (double) nThreads));
            volatile bool found = false;
            auto start = chrono::steady_clock::now();
            #pragma omp parallel num_threads(nThreads) shared(found)
            {
                int threadID = omp_get_thread_num();
                struct crypt_data data;
                data.initialized = 0;

                for (int pswID = threadID * workloadThread; pswID < (threadID + 1) * workloadThread; pswID++) {
                    if (pswID < dictionaryPSW.size() && !found) {
                        char *pswEncrypted = crypt_r(dictionaryPSW[pswID].c_str(), saltpsw.c_str(), &data);
                        if (pswToCrack == string(pswEncrypted)) {
                            //cout << "Generated : " << pswEncrypted;
                            //cout << "Target : " << pswToCrack << endl;
                            //cout << "Password FOUND : " << dictionaryPSW[pswID] << endl;
                            found = true;
                            break;

                        }

                    }
                    else {
                        break;
                    }
                }

            }


            auto end = chrono::steady_clock::now();
            auto elapsed_time = chrono::duration_cast<chrono::microseconds>(end - start).count();
            singleRunTimes += elapsed_time;

        }
        times.push_back(singleRunTimes/runs);

    }

    return times;

}

vector<float> Decrypter::calcSpeedup(vector<long> sequentialTimes, vector<long> parallelTimes) {
    vector<float> speedups;
    speedups.reserve(sequentialTimes.size());

    for (int i = 0; i < sequentialTimes.size(); i++) {
        speedups.push_back((float)sequentialTimes[i] / (float)parallelTimes[i]);
    }

    return speedups;
}