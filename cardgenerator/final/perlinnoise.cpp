#include <math.h>

// Diese Funktion interpoliert linear zwischen a0 und a1. Das Gewicht w sollte im Intervall [0.0, 1.0] liegen.
double interpolate(double a0, double a1, double x){
    if (x < 0.0) return a0;
    if (x > 1.0) return a1;
    return (a1 - a0) * x + a0;
    //return (a1 - a0) * (3.0 - x * 2.0) * x * x + a0; // Alternative: Kubische Interpolation mit dem Polynom 3 * x^2 - 2 * x^3
    //return (a1 - a0) * ((x * (x * 6.0 - 15.0) + 10.0) * x * x * x) + a0; // Alternative:  Interpolation mit dem Polynom 6 * x^5 - 15 * x^4 + 10 * x^3
}

// Datentyp für zweidimensionale Vektoren
typedef struct{
    double x, y;
} vector2;

// Diese Funktion erzeugt einen zufälligen Richtungsvektor
vector2 randomGradient(int ix, int iy){
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
double dotGridGradient(int ix, int iy, double x, double y){
    // Bestimmt den Gradienten der ganzzahligen Koordinaten
    vector2 gradient = randomGradient(ix, iy);
    // Berechnet den Abstandsvektor
    double dx = x - (double) ix;
    double dy = y - (double) iy;
    return dx * gradient.x + dy * gradient.y; // Skalarprodukt
}

// Diese Funktion berechnet das Perlin noise für die Koordinaten x, y
double perlin(double x, double y){
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
