//
// Created by Emanuele on 21/12/2019.
//
#include <stdexcept>
#include <string.h>
#include <omp.h>
#include <math.h>
#include <chrono>
#include <iostream>

#include "Decrypter.h"

using namespace std;

Decrypter::Decrypter(vector<string> pswToCrack, string salt) {

    encryptedPasswords.reserve(pswToCrack.size());

    string passwordString;

    for (string & password : pswToCrack) {
        passwordString = (password.c_str(), salt.c_str());
        encryptedPasswords.push_back(passwordString);
    }

    this->saltpsw = salt;


}


vector<long> Decrypter::sequentialDecryption(int runs)  {

    bool found = false;
    vector<long> times;
    times.reserve(encryptedPasswords.size());
    vector<long> singleRunTimes;
    singleRunTimes.reserve((unsigned long)runs);

    for (string& pswToCrack : encryptedPasswords) {
        singleRunTimes.clear();

        for (int i = 0; i < runs; i++) {
            auto start = chrono::steady_clock::now();

            for (string& password : dictionaryPSW) {
                string pswEncrypted(crypt(password.c_str(), saltpsw.c_str()));
                //cout << pswEncrypted << endl;

                if (pswToCrack == pswEncrypted) {
                    cout << "Generated : " << pswEncrypted << endl;
                    cout << "Target : " << pswToCrack << endl;
                    cout << "Password FOUND : " << password << endl;
                    found = true;
                    break;
                }
            }

            if(!found){
                throw std::invalid_argument("password NOT found!");
            }

            auto end = chrono::steady_clock::now();
            auto elapsed_time = chrono::duration_cast<chrono::nanoseconds>(end - start);
            singleRunTimes.push_back(elapsed_time.count());
        }

        times.push_back(Decrypter::getMean(singleRunTimes));
    }

    return times;
}



long Decrypter::getMean(vector<long> times) {
    long sum = 0;

    for (long t : times) {
        sum += t;
    }

    return (long)(sum / times.size());
}




vector <long> Decrypter::parallelDecryption(int runs, int nThreads) {

    volatile bool found = false;
    vector<long> times;
    times.reserve(encryptedPasswords.size());
    vector<long> singleRunTimes;
    singleRunTimes.reserve((unsigned long) runs);

    for (string &pswToCrack : encryptedPasswords) {
        singleRunTimes.clear();

        for (int i = 0; i < runs; i++) {

            int workloadThread = static_cast<int>(ceil((double) dictionaryPSW.size() / (double) nThreads));

            auto start = chrono::steady_clock::now();
            #pragma omp parallel num_threads(nThreads) shared(found)
            {
                int threadID = omp_get_thread_num();
                struct crypt_data data;
                data.initialized = 0;

                for (int pswID = threadID * workloadThread; pswID < (threadID + 1) * workloadThread; pswID++)
                    if (pswID < dictionaryPSW.size() && !found) {
                        char *pswEncrypted = crypt_r(dictionaryPSW[pswID].c_str(), saltpsw.c_str(), &data);
                        if (pswToCrack == string(pswEncrypted)) {
                            cout << "Generated : " << pswEncrypted << endl;
                            cout << "Target : " << pswToCrack << endl;
                            cout << "Password FOUND : " << dictionaryPSW[pswID] << endl;
                            found = true;
                            break;

                        }

                    } else {
                        break;
                    }


            }
            if (!found) {
                throw std::invalid_argument("password NOT found!");
            }

            auto end = chrono::steady_clock::now();
            auto elapsed_time = chrono::duration_cast<chrono::nanoseconds>(end - start);
            singleRunTimes.push_back(elapsed_time.count());

        }
        times.push_back(Decrypter::getMean(singleRunTimes));

    }

    return times;

}

vector<float> Decrypter::calcSpeedup(vector<long> sequentialTimes, vector<long> parallelTimes) {
    vector<float> speedups;
    speedups.reserve(parallelTimes.size());

    for (int i = 0; i < sequentialTimes.size(); i++) {
        speedups.push_back((float)sequentialTimes[i] / (float)parallelTimes[i]);
    }

    return speedups;
}