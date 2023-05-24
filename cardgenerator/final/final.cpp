#include "perlinnoise.cpp"
#include <iostream>
#include <fstream>
#include <cmath>
#include <omp.h>


typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

void maptoimage(double* map, int mapsize){

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
	        double t = map[k*n+i];
            if(t == 0){
	            binfile << (uint8_t) 255 << (uint8_t) 0  << (uint8_t) 0;                // blau
            }
            else if(t == 1){
	            binfile << (uint8_t) 0 << (uint8_t) 255  << (uint8_t) 0;                // blau
            }
            else{
                binfile << (uint8_t) 255 << (uint8_t) 255  << (uint8_t) 255;            // blau
            }
        }
    }
}

// biomcount -> the amount of bioms
// percentages -> the percentage of a biom (length must be the same as biomcount)
// layerbits -> will slow down computation significantly and will fragmentize the map more, but will create better percentages
void createmap(int size, int layerbits, int biomcount, double* percentages){
    int len = size;
    double factor = 2/(double)len;
    double* picture =  (double*) malloc(sizeof(double)*len*len);
    int counter[10] = { 0 };

    std::srand(time(NULL));


    // create the array of "offsets" and fill it
    double offsets[layerbits][2];
    for(int k = 0; k < layerbits; k++){
        offsets[k][0] = std::rand()/31415;                          // just fill it with random numbers
        offsets[k][1] = std::rand()/31415;                          // just fill it with random numbers
    }

    // create the array  of the randomizedbioms and fill it
    int vsb = pow(2,layerbits);                                     // vsb -> Value Space for the Bits
    int randomizedbioms[vsb];
    for(int k = 0; k < vsb; k++){
        randomizedbioms[k] = std::rand()%biomcount;                 // fill it with random values accordingly to the percentages (not implemented yet)
    }

    #pragma omp parallel for schedule(dynamic)
    for(int k = 0; k < len; k++){
        for(int i = 0; i < len; i++){
            int t = 0;
            // calc the binary number
            for(int j = 0; j < layerbits; j++){
                // eine der schrittweiten ändern
                if( perlin(k*factor+offsets[j][0],i*factor+offsets[j][1]) > 0) {
                    t += pow(2,j);
                }
            }
            picture[k*len+i] = randomizedbioms[t];

        }
    }

    maptoimage(picture,len);
}

int main(){
    double percentages[3];
    createmap(30000,5,3,percentages);
}