#ifndef TRANSF4_H
#define TRANSF4_H

#include <cmath>
#include "matrix.h"

using namespace std;

matrix rotate4d(float szog_xy, float szog_xz, float szog_xt, float szog_yz, float szog_yt, float szog_zt){
    float 
        cxy = cos(szog_xy),
        cxz = cos(szog_xz),
        cxt = cos(szog_xt),
        cyz = cos(szog_yz),
        cyt = cos(szog_yt),
        czt = cos(szog_zt),
            
        sxy = sin(szog_xy),
        sxz = sin(szog_xz),
        sxt = sin(szog_xt),
        syz = sin(szog_yz),
        syt = sin(szog_yt),
        szt = sin(szog_zt);
    
    matrix
        xy({
            {   1,   0,   0,   0,   0},
            {   0,   1,   0,   0,   0},
            {   0,   0, cxy,-sxy,   0},
            {   0,   0, sxy, cxy,   0},
            {   0,   0,   0,   0,   1}
        }),
        xz({
            {   1,   0,   0,   0,   0},
            {   0, cxz,   0,-sxz,   0},
            {   0,   0,   1,   0,   0},
            {   0, sxz,   0, cxz,   0},
            {   0,   0,   0,   0,   1}
        }),
        xt({
            {   1,   0,   0,   0,   0},
            {   0, cxt,-sxt,   0,   0},
            {   0, sxt, cxt,   0,   0},
            {   0,   0,   0,   1,   0},
            {   0,   0,   0,   0,   1}
        }),
        yz({
            { cyz,   0,   0,-syz,   0},
            {   0,   1,   0,   0,   0},
            {   0,   0,   1,   0,   0},
            { syz,   0,   0, cyz,   0},
            {   0,   0,   0,   0,   1}
        }),
        yt({
            { cyt,   0,-syt,   0,   0},
            {   0,   1,   0,   0,   0},
            { syt,   0, cyt,   0,   0},
            {   0,   0,   0,   1,   0},
            {   0,   0,   0,   0,   1}
        }),
        zt({
            { czt,-szt,   0,   0,   0},
            { szt, czt,   0,   0,   0},
            {   0,   0,   1,   0,   0},
            {   0,   0,   0,   1,   0},
            {   0,   0,   0,   0,   1}
        });
    
    return xy * xz * xt * yz * yt * zt;
}

matrix scale4d(float s_x, float s_y, float s_z, float s_t){
    return matrix({
        { s_x, 0, 0, 0,   0},
        { 0, s_y, 0, 0,   0},
        { 0, 0, s_z, 0,   0},
        { 0, 0,   0, s_t, 0},
        { 0, 0,   0, 0,   1}
    });
}
matrix push4d(float x, float y, float z, float t){
    return matrix({
        { 1, 0, 0, 0, x},
        { 0, 1, 0, 0, y},
        { 0, 0, 1, 0, z},
        { 0, 0, 0, 1, t},
        { 0, 0, 0, 0, 1}
    });
}

#endif