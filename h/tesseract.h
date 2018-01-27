#ifndef TESSERACT_H
#define TESSERACT_H

#include <iostream>
#include <cmath>
#include <vector>
#include "body.h"

// hiperkocka, 4D kocka, "tesseract"
using namespace std;

class tesseract : public body{
public:
    tesseract(float size, float c_r, float c_g, float c_b){
        this->numPoints = 16;
        this->numFaces = 0;
        this->dim = 4;
        this->transformations = matrix(this->dim + 1); // egysegmatrix
        this->pointsArr = new vec[numPoints];
        this->transformated = new vec[numPoints];
        this->transformatedToCam = new vec[numPoints];
        
        this->facesArr = new faceIndex[0];

        this->color_r = c_r;
        this->color_g = c_g;
        this->color_b = c_b;
        
        initCube(size); // megcsinalja a nyers kockat, transzformacio nelkul -> pointsArr-ba... ez nem valtozik
        
    }

    void initCube(float size){ // a nyers kocka, transzformaciok nelkul...
        float s = size / 2;
        
        this->pointsArr[0] = vec({-s,s,-s,-s,1.0f});
        this->pointsArr[1] = vec({s,s,-s,-s,1.0f});
        this->pointsArr[2] = vec({s,s,s,-s,1.0f});
        this->pointsArr[3] = vec({-s,s,s,-s,1.0f});
        this->pointsArr[4] = vec({-s,-s,-s,-s,1.0f});
        this->pointsArr[5] = vec({s,-s,-s,-s,1.0f});
        this->pointsArr[6] = vec({s,-s,s,-s,1.0f});
        this->pointsArr[7] = vec({-s,-s,s,-s,1.0f});
        
        // negyedik dim
        this->pointsArr[8] = vec({-s,s,-s,s,1.0f});
        this->pointsArr[9] = vec({s,s,-s,s,1.0f});
        this->pointsArr[10] = vec({s,s,s,s,1.0f});
        this->pointsArr[11] = vec({-s,s,s,s,1.0f});
        this->pointsArr[12] = vec({-s,-s,-s,s,1.0f});
        this->pointsArr[13] = vec({s,-s,-s,s,1.0f});
        this->pointsArr[14] = vec({s,-s,s,s,1.0f});
        this->pointsArr[15] = vec({-s,-s,s,s,1.0f});
        
        
        this->addEdge(0,8);
        this->addEdge(1,9);
        this->addEdge(2,10);
        this->addEdge(3,11);
        this->addEdge(4,12);
        this->addEdge(5,13);
        this->addEdge(6,14);
        this->addEdge(7,15);
        
        this->addEdge(0,1);
        this->addEdge(1,2);
        this->addEdge(2,3);
        this->addEdge(3,0);
        
        this->addEdge(4,5);
        this->addEdge(5,6);
        this->addEdge(6,7);
        this->addEdge(7,4);
        
        this->addEdge(8,9);
        this->addEdge(9,10);
        this->addEdge(10,11);
        this->addEdge(11,8);
        
        this->addEdge(12,13);
        this->addEdge(13,14);
        this->addEdge(14,15);
        this->addEdge(15,12);
        
        
        this->addEdge(0,4);
        this->addEdge(1,5);
        this->addEdge(2,6);
        this->addEdge(3,7);
        
        this->addEdge(8,12);
        this->addEdge(9,13);
        this->addEdge(10,14);
        this->addEdge(11,15);
        
        
    }

private:
};

#endif