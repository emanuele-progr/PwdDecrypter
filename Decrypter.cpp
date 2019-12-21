//
// Created by Emanuele on 21/12/2019.
//

#include <string.h>
#include <omp.h>
#include <math.h>
#include "Decrypter.h"

using namespace std;

Decrypter::Decrypter(vector<string> pswToCrack, string salt) {
    this->salt = salt;
    encryptedPasswords.reserve(pswToCrack.size());

    string passwordString;

    for (string & password : pswToCrack) {
        passwordString = (password.c_str(), salt.c_str());
        encryptedPasswords.push_back(passwordString);
    }


}


