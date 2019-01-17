#ifndef GAUSS_H
#define GAUSS_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

using namespace std;

const double PI = 3.1415926535897932384626;

double gauss(int x, int y, double n = 9.0, double delta = 3.0)
{
    return 0.5 / PI / delta / delta * exp(-(double)(x * x + y * y) / 2.0 / delta / delta);
}

#endif  //GAUSS_H