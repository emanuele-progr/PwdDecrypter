//
// Created by Emanuele on 21/12/2019.
//
#include <stdexcept>
#include <string.h>
#include <omp.h>
#include <math.h>
#include <chrono>
#include <iostream>
#include <crypt.h>
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


vector<long> Decrypter::sequentialBruteforce(int runs)  {

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

        times.push_back(getMean(singleRunTimes));
    }

    return times;
}