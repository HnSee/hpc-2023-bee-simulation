#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <omp.h>

typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

struct bt{
    int type;
    int mostinflunce;
    double influence;
};

int* getbiomcount(bt* map, int mapsize, int* biomcount, int biomnum){

    for(int k = 0; k < mapsize; k++){
        for(int i = 0; i < mapsize; i++){
            int t = map[i+mapsize*k].type;

            if( t != -1){
                biomcount[t] += 1;
            }
        }
    }

    return biomcount;
}

void updatemap(bt* map, int mapsize, double* percentage, int biomnum){
    double cpercentage[biomnum] = { 0 };

    int biomcount[biomnum] = { 0 };
    int* bc = &biomcount[0];

    bc = getbiomcount(map, mapsize, biomcount, biomnum);

    for(int k = 0; k < biomnum; k++){
        cpercentage[k] = (mapsize*mapsize*percentage[k] / bc[k]) * (mapsize*mapsize*percentage[k] / bc[k]);
    }

    // term der wenn die anzahl der pixel klein wird gegen unendlich konvergiert -> (mapsize*mapsize*percentage / bc)^2
    // term der die anzahl nach dem prozenten ausbalanciert pixel ausbalanciert

    std::cout << (double)bc[0]/(mapsize*mapsize) << "\n";
    //std::cout << (double)bc[1]/(mapsize*mapsize) << "\n";
    //std::cout << (double)bc[2]/(mapsize*mapsize) << "\n";

    // openmp hinzufügen
    // schleife nach innen verschieben
    for(int i = 0; i < mapsize; i++){
        for(int j = 0; j < mapsize; j++){
            for(int k = 0; k < biomnum; k++){        
                double t = 0;
                // calc the value from all the neighbor points
                for(int x = -1; x < 2; x++){
                    for(int y = -1; y < 2; y++){
                        if( !(i+x < 0 || i+x == mapsize || j+y < 0 || j+y == mapsize ) && map[j+y+mapsize*(i+x)].type == k){
                            t += cpercentage[k];
                        }
                    }
                }
                
                if( map[j+mapsize*i].influence < t ){
                    map[j+mapsize*i].mostinflunce = k;
                    map[j+mapsize*i].influence = t;                    
                }
            }
        }            
    }

    for(int k = 0; k < mapsize; k++){
        for(int i = 0; i < mapsize; i++){
            map[i+mapsize*k].type = map[i+mapsize*k].mostinflunce;
            map[i+mapsize*k].mostinflunce = -1;
            map[i+mapsize*k].influence = 0;                                
        }
    }
    return;
}

void maptoimage(bt* map, int mapsize){

    std::ofstream binfile("testfile.bmp", std::ios::out | std::ios::binary);

    int n = mapsize;
    int padding = (n*3)%4;
    uint32_t tmp;

    binfile << "BM";
    binfile.write( (char*) &( tmp = (uint32_t) 54+(padding*n) + (n*n*3) ), sizeof(uint32_t) );                  // !    Size of file
    binfile.write( (char*) &( tmp = (uint32_t) 0),  sizeof(uint32_t) );                                         //      Unused
    binfile.write( (char*) &( tmp = (uint32_t) 54), sizeof(uint32_t) );                                         //      Start of Pixel data
    binfile.write( (char*) &( tmp = (uint32_t) 40), sizeof(uint32_t) );                                         //      Number of bytes in the header
    binfile.write( (char*) &( tmp = (uint32_t) n),  sizeof(uint32_t) );                                         // !    width
    binfile.write( (char*) &( tmp = (uint32_t) n),  sizeof(uint32_t) );                                         // !    height
    binfile.write( (char*) &( tmp = (uint32_t) 0x00180001), sizeof(uint32_t) );                                 //      Colorplanes and bits per pixel
    binfile.write( (char*) &( tmp = (uint32_t) 0),  sizeof(uint32_t) );                                         //      Compression
    binfile.write( (char*) &( tmp = (uint32_t) (padding*n) + (n*n*3) ), sizeof(uint32_t) );                     // !    Size of Bitmap data
    binfile.write( (char*) &( tmp = (uint32_t) 0x00000b13), sizeof(uint32_t) );                                 //      resolution
    binfile.write( (char*) &( tmp = (uint32_t) 0x00000b13), sizeof(uint32_t) );                                 //      resolution
    binfile.write( (char*) &( tmp = (uint32_t) 0),  sizeof(uint32_t) );                                         //      colors 
    binfile.write( (char*) &( tmp = (uint32_t) 0),  sizeof(uint32_t) );                                         //      important colors

    for( int k = 0; k < n; k++ ){
        for( int i = 0; i < n; i++ ){
            if( map[k*n+i].type == 0){
                binfile << (uint8_t) 0 << (uint8_t) 0  << (uint8_t) 255;
            }
            if( map[k*n+i].type == 1){
                binfile << (uint8_t) 0 << (uint8_t) 255  << (uint8_t) 0;
            }
            if( map[k*n+i].type == 2){
                binfile << (uint8_t) 255 << (uint8_t) 0  << (uint8_t) 0;
            }
            if( map[k*n+i].type == -1){
                binfile << (uint8_t) 0 << (uint8_t) 0  << (uint8_t) 0;
            }
            
        }
    }
}

// map creator
// Wenn als SVG, dann Polygon mit x (> 1000) seiten und vektor, welcher vom zentrum von x wegzeigt und dieser wird nach der funktion unten verschoben
int main(){
    std::srand(time(NULL));
    
    int mapsize = 1000;
    int biomnum = 3;
    double percentage[biomnum] = {0.7,0.2,0.1};


    bt* map =  (bt*) malloc(sizeof(bt)*mapsize*mapsize);
    std::cout << sizeof(bt)*mapsize*mapsize << "\n";
    
    for(int k = 0; k < mapsize; k++){
        for(int i = 0; i < mapsize; i++){
            map[k*mapsize+i].type = -1;
            map[k*mapsize+i].mostinflunce = -1;
            map[k*mapsize+i].influence = 0;

        }
    }    
    
    for(int k = 0; k < biomnum; k++){
        for(int i = 0; i < mapsize/biomnum/7; i++){         // 7 ist ein expierimenteller wert
            int x = std::rand()%mapsize;
            int y = std::rand()%mapsize;
            
            map[x*mapsize+y].type = k;
        }
    }

    for(int a = 0; a < 200; a++){
        for(int k = 0; k < mapsize; k++){
            for(int i = 0; i < mapsize; i++){
                //std::cout << map[i][k].type << "    ";
            }
            //std::cout << "\n";
        }
        updatemap(map,mapsize,percentage,biomnum);
        std::cout << a << "\n";
    }

    maptoimage(map, mapsize);

    free(map);
        
}
