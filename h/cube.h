#ifndef CUBE_H
#define CUBE_H

#include <iostream>
#include <cmath>
#include <vector>
#include "body.h"

using namespace std;

class cube : public body{
public:

    void initCube(float size){ // a nyers kocka, transzformaciok nelkul...
        float s = size / 2;
        
        this->pointsArr[0] = vec({-s,s,-s,1.0f});
        this->pointsArr[1] = vec({s,s,-s,1.0f});
        this->pointsArr[2] = vec({s,s,s,1.0f});
        this->pointsArr[3] = vec({-s,s,s,1.0f});
        this->pointsArr[4] = vec({-s,-s,-s,1.0f});
        this->pointsArr[5] = vec({s,-s,-s,1.0f});
        this->pointsArr[6] = vec({s,-s,s,1.0f});
        this->pointsArr[7] = vec({-s,-s,s,1.0f});
        
        this->facesArr[0] = body::initFaceIndex(2,3,0,1); // teteje
        this->facesArr[1] = body::initFaceIndex(5,6,2,1); // jobb
        this->facesArr[2] = body::initFaceIndex(6,7,3,2); // szembe
        this->facesArr[3] = body::initFaceIndex(7,4,0,3); // bal
        this->facesArr[4] = body::initFaceIndex(4,5,1,0); // hatso
        this->facesArr[5] = body::initFaceIndex(5,4,7,6); // also
    }

    cube(float size, float c_r, float c_g, float c_b){
        this->numPoints = 8;
        this->numFaces = 6;
        this->dim = 3;
        this->transformations = matrix(this->dim + 1); // egysegmatrix
        this->pointsArr = new vec[numPoints];
        this->transformated = new vec[numPoints];
        this->transformatedToCam = new vec[numPoints];
        this->facesArr = new faceIndex[this->numFaces];

        this->color_r = c_r;
        this->color_g = c_g;
        this->color_b = c_b;
        
        initCube(size); // megcsinalja a nyers toruszt, transzformacio nelkul -> pointsArr-ba... ez nem valtozik
    }

private:
};

#endif