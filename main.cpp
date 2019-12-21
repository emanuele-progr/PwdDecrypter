#include <iostream>
#include <vector>
#include "Decrypter.h"


using namespace std;

int main(){
vector<string> pswToCrack = {"warbec14", "1212MARI", "squigium", "sauske00"};
vector<int> NThreads = {2, 3, 4, 5, 6, 7, 8, 9, 10, 20, 30, 40, 50, 100, 200, 500, 1000, 5000, 10000};
string saltpsw = "username";
Decrypter Decrypter(pswToCrack, saltpsw);

}