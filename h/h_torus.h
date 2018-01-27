#ifndef HTORUS_H
#define HTORUS_H

#include <iostream>
#include <cmath>
#include <vector>
#include "body.h" 
#define PI 3.141592653589793

using namespace std;

class htorus : public body{
public:

    htorus(float r1p, float r2p, int res_p, float c_r, float c_g, float c_b){
        this->numPoints = res_p * res_p * res_p;
        this->numFaces = 0;
        this->dim = 4;
        this->pointsArr = new vec[this->numPoints];
        this->transformations = matrix(this->dim + 1); // egysegmatrix
        this->transformated = new vec[this->numPoints];
        this->transformatedToCam = new vec[this->numPoints];
        this->facesArr = new faceIndex[0];
        this->color_r = c_r;
        this->color_g = c_g;
        this->color_b = c_b;
        
        this->initTorusz(r1p, r2p, res_p); // megcsinalja a nyers toruszt, transzformacio nelkul -> pointsArr-ba... ez nem valtozik
    }

private:
    
    void initTorusz(float r1, float r2, int res){
        int index = 0;
        float szogf = PI * 2 / res ;
        for (int a = 0; a < res; a++){ // negyedik dim
                float cosa = r1 * cos( szogf * a ),
                      sina = r1 * sin( szogf * a );
            for (int b = 0; b < res; b++){ // kulso kor

                float cosb = cos( szogf * b ),
                      sinb = sin( szogf * b );

                for (int c = 0; c < res; c++){ // belso korok
                    index = a * res * res + b * res + c;
                    float cosc = cos( szogf * c ),
                          sinc = sin( szogf * c );

                    this->pointsArr[index] = vec({ //belevagjuk a pontokat tarolo egydimenzios tombbe...

                        (r2 + (r1 + cosa) * cosb) * cosc, // x
                        (r2 + (r1 + cosa) * cosb) * sinc, // y
                        (r1 + cosa) * sinb, // z
                        sina, // t
                        1.0f // i
                    });

                    int a_1 = (a < res - 1) ? (a + 1) : 0,  // elozo a
                     //   b_1 = (b < res - 1) ? (b + 1) : 0, // elozo b
                        c_1 = (c < res - 1) ? (c + 1) : 0; // elozo c

                    int q0 = index,
                        q_c = a * res * res + b * res + c_1,
                        q_a = a_1 * res * res + b * res + c;
                    //    q_b = a * felbontas * felbontas + b_1 * felbontas + c;

                        this->addEdge(q_c, q0);
                        this->addEdge(q0, q_a);
                      //   this->addEdge(q_c, q_a);
                }
            }
        }
    }
};

#endif