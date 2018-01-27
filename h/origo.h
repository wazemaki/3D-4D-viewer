#ifndef ORIGO_H
#define ORIGO_H

#include <iostream>
#include <cmath>
#include <vector>
#include "body.h"

using namespace std;

class origo : public body{
public:
    void camTransformation(camera *cam){
        for (int i = 0; i < numPoints; i++){
            this->transformatedToCam[i] = cam->transformToWc( this->transformated[i] ) ;
        }
    }

    void init(){
        this->transformated[0] = vec({0,0,0,1.0f});
        
        this->transformated[1] = vec({0,0,1.0f,1.0f});
        this->transformated[2] = vec({0,1.0f,0,1.0f});
        this->transformated[3] = vec({1.0f,0,0,1.0f});
    }

    origo(){
        this->numPoints = 4;
        this->numFaces = 0;
        this->dim = 3;
        this->transformated = new vec[numPoints];
        this->transformatedToCam = new vec[numPoints];

        init();
    }

private:
};

#endif