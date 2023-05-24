#include <math.h>
#include <iostream>
#include <fstream>
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;


// Diese Funktion interpoliert linear zwischen a0 und a1. Das Gewicht w sollte im Intervall [0.0, 1.0] liegen.
double interpolate(double a0, double a1, double x)
{
    if (x < 0.0) return a0;
    if (x > 1.0) return a1;
    return (a1 - a0) * x + a0;
    //return (a1 - a0) * (3.0 - x * 2.0) * x * x + a0; // Alternative: Kubische Interpolation mit dem Polynom 3 * x^2 - 2 * x^3
    //return (a1 - a0) * ((x * (x * 6.0 - 15.0) + 10.0) * x * x * x) + a0; // Alternative:  Interpolation mit dem Polynom 6 * x^5 - 15 * x^4 + 10 * x^3
}

// Datentyp für zweidimensionale Vektoren
typedef struct
{
    double x, y;
} vector2;

// Diese Funktion erzeugt einen zufälligen Richtungsvektor
vector2 randomGradient(int ix, int iy)
{
    const unsigned w = 8 * sizeof(unsigned);
    const unsigned s = w / 2;
    unsigned a = ix, b = iy;
    a *= 3284157443;
    b ^= a << s | a >> w - s;
    b *= 1911520717;
    a ^= b << s | b >> w - s;
    a *= 2048419325;
    double random = a * (3.14159265 / ~(~0u >> 1)); // Erzeugt eine Zufallszahl im Intervall [0, 2 * Pi]
    vector2 v;
    v.x = sin(random);
    v.y = cos(random);
    return v;
}

// Diese Funktion berechnet das Skalarprodukt des Abstandsvektors und den Gradientenvektoren
double dotGridGradient(int ix, int iy, double x, double y)
{
    // Bestimmt den Gradienten der ganzzahligen Koordinaten
    vector2 gradient = randomGradient(ix, iy);
    // Berechnet den Abstandsvektor
    double dx = x - (double) ix;
    double dy = y - (double) iy;
    return dx * gradient.x + dy * gradient.y; // Skalarprodukt
}

// Diese Funktion berechnet das Perlin noise für die Koordinaten x, y
double perlin(double x, double y)
{
    // Bestimmt die Koordinaten der vier Ecken der Gitterzelle
    int x0 = (int) floor(x);
    int x1 = x0 + 1;
    int y0 = (int) floor(y);
    int y1 = y0 + 1;
    // Bestimmt die Gewichte für die Interpolation
    double sx = x - (double)x0;
    double sy = y - (double)y0;
    // Interpoliert zwischen den Gradienten der vier Ecken
    double n0, n1, ix0, ix1, value;
    n0 = dotGridGradient(x0, y0, x, y);
    n1 = dotGridGradient(x1, y0, x, y);
    ix0 = interpolate(n0, n1, sx);
    n0 = dotGridGradient(x0, y1, x, y);
    n1 = dotGridGradient(x1, y1, x, y);
    ix1 = interpolate(n0, n1, sx);
    return interpolate(ix0, ix1, sy); // Gibt den Wert der Funktion für das Perlin noise zurück
}

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
	        double t = (map[k*n+i]+0.7)/1.4;
	        if(0.2 > t){
                binfile << (uint8_t) 255 << (uint8_t) 0  << (uint8_t) 0;            // blau
            }
            else if(0.5 > t){
                binfile << (uint8_t) 150 << (uint8_t) 100  << (uint8_t) 0;            // blau
            }
            else if(0.8 > t){
                binfile << (uint8_t) 50 << (uint8_t) 200  << (uint8_t) 0;        // gelb
           }
	        else{
                binfile << (uint8_t) 0 << (uint8_t) 255  << (uint8_t) 0;            // gelb
	        }
	}
    }
}



int main(){
    int len = 1000;
    double factor = 10/(double)len;
    double* picture =  (double*) malloc(sizeof(double)*len*len);
    int counter[10] = { 0 };

    std::srand(time(NULL));
    double x = std::rand()/1000;
    double y = std::rand()/1000;

    for(int k = 0; k < len; k++){
        for(int i = 0; i < len; i++){
            picture[k*len+i] = perlin(k*factor+x,i*factor+y);
            for( int j = 0; j < 10; j++){
                if(picture[k*len+i] > (double)j/10 && picture[k*len+i] < (double)(j+1)/10 ){ counter[j] += 1; }
            }
        }
        if(k%100 == 0){
            std::cout << (double)k/len*100 << "%\n";
        }
    }

    std::cout << "\n";
    for( int j = 0; j < 10; j++){
        std::cout << counter[j] << "\n";
    }


    maptoimage(picture,len);
    std::cout << "\n" << "done" << "\n";
}
