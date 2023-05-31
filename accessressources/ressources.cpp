#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

struct ressource{
    double x;
    double y;
};

/*
void sortressource{
}
*/

bool sortby_x(const ressource a, const ressource b){
    return a.x < b.x;
}

bool sortby_y(const ressource a, const ressource b){
    return a.y < b.y;
}

int findelement(int xtofind, int ytofind, int left, int right, ressource* res){
    if(right - left == 1){
        if(xtofind == res[left].x && ytofind == res[left].y){
            return left;
        }
        return -1;
    }

    int middle = (right + left)/2;

    if( xtofind < res[middle].x ){
        return findelement(xtofind, ytofind, left, middle, res);
    }

    if( xtofind > res[middle].x ){
        return findelement(xtofind, ytofind, middle, right, res);
    }

    if( ytofind < res[middle].y ){
        return findelement(xtofind, ytofind, left, middle, res);
    }

    if( ytofind > res[middle].y ){
        return findelement(xtofind, ytofind, middle, right, res);
    }

    return middle;
}
    


int main(){
    std::srand(time(NULL));
    int size = 1000000;
    ressource pos[size];

    
    for(int l = 0; l < 1; l++){       
        for(int k = 0; k < size; k++){
            pos[k].x = std::rand()%1000000;
            pos[k].y = std::rand()%1000000;
        }

        int n = sizeof(pos) / sizeof(pos[0]);
        
        std::stable_sort(pos, pos + n, sortby_y);
        std::stable_sort(pos, pos + n, sortby_x);

        //std::cout << findelement(10,10,0,size,pos) << "\n";
        //findelement(10,10,0,size,pos);
    }

    for( int x = 0; x < 1000000; x++){
        findelement(std::rand()%1000000,std::rand()%1000000,0,size,pos);
    }

    /*
    for (int i = 0; i < n; ++i)
        std::cout << pos[i].x << " - " << pos[i].y << "\n";
    */
}