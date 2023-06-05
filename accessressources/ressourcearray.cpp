#include <stdlib.h>
#include <iostream>

struct ressource{
    double x;
    double y;
    int size;
    int transmit;
};

int main(){
    std::srand(time(NULL));
    int numres = 100;

    // ressource Array
    ressource globress[numres];

    // change in ressources Array
    int cglobres[numres];

    for(int k = 0; k < 100; k++){
        globress[k] = { (double)std::rand()/1000, (double)std::rand()/1000, std::rand() };
        cglobres[k] = 0;
    }
}