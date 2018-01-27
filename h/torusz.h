#ifndef TORUS_H
#define TORUS_H

#include <iostream>
#include <cmath>
#include <vector>
#include "body.h"
#define PI 3.141592653589793

using namespace std;

class torusz : public body{
public:

    torus(float r1p, float r2p, int res_p, float c_r, float c_g, float c_b){
        this->numPoints = res_p * res_p;
        this->numFaces = this->numPoints;
        this->pointsArr = new vec[this->numPoints];
        this->transformated = new vec[this->numPoints];
        this->transformatedToCam = new vec[this->numPoints];
        this->facesArr = new faceIndex[this->numPoints];
        this->color_r = c_r;
        this->color_g = c_g;
        this->color_b = c_b;
        
        this->initTorus(r1p, r2p, res_p); // megcsinalja a nyers toruszt, transzformacio nelkul -> pointsArr-ba... ez nem valtozik
    }

private:
    
    void initTorus(float r1, float r2, int res){
        int index = 0;
        float szogf = PI * 2 / res ;
        for (int v = 0; v < res; v++){
            float rcosv = r1 * cos(szogf * v), rsinv = r1 * sin( szogf * v);
            for (int u = 0; u < res; u++){
                index = v * res + u;
                this->pointsArr[index] = vec({ //belevagjuk a pontokat tarolo egydimenzios tombbe...
                    (r2 + rcosv) * cos(szogf * u),
                    rsinv,
                    (r2 + rcosv) * sin(szogf * u),
                    1.0f
                });
                int sor_1 = (v < res - 1) ? (v + 1) : 0,
                    oszlop_1 = (u < res - 1) ? (u + 1) : 0;
                int jobbf = v * res + oszlop_1,
                    balf = index,
                    bala = sor_1 * res + u,
                    jobba = sor_1 * res + oszlop_1;
                this->facesArr[index] = body::initFaceIndex(jobba,bala,balf,jobbf);
            }
        }
    }
};

#endif