#ifndef TRANSF_H
#define TRANSF_H

#include <cmath>
#include "matrix.h"

using namespace std;

matrix forgat(float szog_x, float szog_y, float szog_z){
    float cx = cos(szog_x);
    float cy = cos(szog_y);
    float cz = cos(szog_z);
    float sx = sin(szog_x);
    float sy = sin(szog_y);
    float sz = sin(szog_z);
    
    matrix x({
            {   1,   0,   0,   0},
            {   0,  cx, -sx,   0},
            {   0,  sx,  cx,   0},
            {   0,   0,   0,   1}
        }),
        y({
            {  cy,   0,  sy,   0},
            {   0,   1,   0,   0},
            { -sy,   0,  cy,   0},
            {   0,   0,   0,   1}
        }),
        z({
            {  cz, -sz,   0,   0},
            {  sz,  cz,   0,   0},
            {   0,   0,   1,   0},
            {   0,   0,   0,   1}
        });
    
    return x * y * z;
}

matrix skala(float s_x, float s_y, float s_z){
    return matrix({
        { s_x, 0, 0, 0},
        { 0, s_y, 0, 0},
        { 0, 0, s_z, 0},
        { 0, 0, 0, 1}
    });
}
matrix eltol(float x, float y, float z){
    return matrix({
        { 1, 0, 0, x},
        { 0, 1, 0, y},
        { 0, 0, 1, z},
        { 0, 0, 0, 1}
    });
}

#endif