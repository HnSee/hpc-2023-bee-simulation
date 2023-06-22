#include <iostream>
#include <cmath>

struct bounds{
    double xlower;
    double xupper;
    double ylower;
    double yupper;
};

// finds the largest divisor closest to the squareroot
int get_square_divisor(int processes){
    int t = sqrt(processes)+1;

    for(int k = 0; k >= -processes; k--){
        if( processes % (t+k) == 0 ){
            return (t+k);
        }
    }

    throw std::logic_error( "There was no divisor found !!!" );
    return 0;
}

bounds getbounds(int pixelx, int pixely, int xdim, int ydim, int number){
    bounds b;
    int x, y = 1;

    while( number - xdim > 0 ){
        number -= xdim;
        y = y + 1;
    }
    x = number;

    double xdist = (double)pixelx/xdim;
    double ydist = (double)pixely/ydim;

    b.xlower = xdist*(x-1);
    b.xupper = xdist*x;
    b.ylower = ydist*(y-1);
    b.yupper = ydist*y;
    
    return b;
}

int main(){
    int pixelx = 1000, pixely = 1000;
    int xdim, ydim = 0;
    int processes = 12;
    int number = 1;
    
    // get dimensions
    std::cout << "Sqrt: " << (int)sqrt(processes) << "\n\n";

    xdim = get_square_divisor(processes);
    ydim = processes/xdim;

    bounds b = getbounds(pixelx, pixely, xdim, ydim, number);
    
    std::cout << "X-Dimension-lower: " << b.xlower << "\n";
    std::cout << "X-Dimension-upper: " << b.xupper << "\n";
    std::cout << "Y-Dimension lower: " << b.ylower << "\n";
    std::cout << "Y-Dimension upper: " << b.yupper << "\n";

    std::cout << "\ndone!";
}