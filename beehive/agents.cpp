#include <iostream>
#include <vector>

struct coord{
    double x;
    double y;
};

struct ressource{
    // function x^2-x^3
    int maxsize;    // how much ressources can be there
    int size;       // how much ressources are there
    int first;      // first day the growth of the ressource 
    int last;       // last day that that ressource regrowth
};

struct bee{
    bool isscout;
    coord position;

};

class beehive{
    private:
        int* bees[40000];
    
    public:
        int test;

    private:
    coordinates position;

    beehive init(){
        return beehive{ NULL, NULL, 123};
    }
}


int main(){
    std::cout << "done!";

    beehive x;
    std::cout << x.test;
}
