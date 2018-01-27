#ifndef CAMERA_H
#define CAMERA_H
#include <iostream>
#include <cmath>
#include <vector>
#include "matrix.h"
#include "vec.h"

#define DELTA_ALPHA 0.005
#define DELTA_ELEVATION 0.05

using namespace std;

class camera{
private:
    matrix M;       // EMMMM matrix // ebben taroljuk az osszes matrix szorzatat, amig nem frissul a kamera
    
    matrix Vc;      // centralis vetites matrixa
    matrix Wc;      // Wtv matrix
    matrix view;    // view matrix
    
    vec up;         // a felfele irany
    vec centerVec;  // ahova nezunk
    
public:
    vec eye;        // a kamera helye
    
    float R;        // a kamera tavolsaga centerVec-tol
    float alpha;    // kamera helyzetet meghatarozo szog (a hengeren)
    float elev;     // kamera magassaga
    float s;        // ESSSSS (kamera es lekepezesi sik tavolsaga)

    camera(vec centerv_p, vec up_p , float r_p = 25.0, float alpha_p = 45.0, float elev_p = 2, float s_p = 10){
        centerVec = centerv_p;  // megadja, hogy merre nez a kamera
        up = up_p;              // megdja, hogy merre van a felfele irany 
        this->R = r_p;
        this->alpha = alpha_p;
        this->elev = elev_p;
        this->s = s_p;
        initPersProjMatrix();   // vetitesi matrixok
    }
    
    vec transform(vec v){
        return (v * this->M);
    }
    vec transformToWc(vec v){ // csak a koordinata rendszernek
        
        matrix Wc({
            {100,     0,       0,   100},
            {0,     100,       0,   100},
            {0,       0,       1,   0},
            {0,       0,       0,   1}
        });
        return (v * (Wc * view));
    }
    
    void rotate(float alpha_p){
        this->alpha += alpha_p * DELTA_ALPHA;
    }
    void elevate(float elev_p){
        this->elev += elev_p * DELTA_ELEVATION;
    }
    void zoom(float d_p){
        this->R += d_p;
    }

    void refresh(){
        initViewMatrix();
        M = Wc * Vc * view; // WindowToViewport x CentralProjection x view
    }
    
    vec projectionInverse(vec v){
        return v * view.invert();
    }

    void initWtvMatrix(vec windowCenter, vec windowSize, vec viewCenter, vec viewSize){ // feltolti a Wc matrixot
        float scale_x = viewSize.get(vec_coord_x) / windowSize.get(vec_coord_x);
        float scale_y = viewSize.get(vec_coord_y) / windowSize.get(vec_coord_y);
        
        this->Wc = matrix({
            {scale_x, 0,       0,   windowCenter.get(vec_coord_x) * scale_x + viewCenter.get(vec_coord_x)},
            {0,       scale_y, 0,   windowCenter.get(vec_coord_y) * scale_y + viewCenter.get(vec_coord_y)},
            {0,       0,       1,   0},
            {0,       0,       0,   1}
        });
        this->Wc.print();
    }
    
private:
    
    void initPersProjMatrix(){ //feltolti a Vc matrixot a (0,0,s) kozeppontu centralis vetites matrixszaval
        Vc.initId(4);
        Vc.set(3, 3, 0.0f);
        Vc.set(3, 2, -1.0f / s);
    }
    
    void initViewMatrix() { //feltolti a view matrixot... "K"-matrix
        eye = vec({
            this->R * sin( this->alpha ),
            this->R * cos( this->alpha ),
            this->elev
        }); // a kamera helye a hengerpalaston

        // a kamerabol az iranyt meghatarozo pontba mutato vektor
        // center az a pont,amely fele a kamerat tartjuk, eye a kamera helyet adja
        vec centerMinusEye = eye - centerVec;

        // a fenti vektor -1 szeresenek egyseg hosszra normaltja, ebbol lesz a kamera rendszerenek z-tengelye
        vec z_ = centerMinusEye.normalize();

        // az up vektor es a leendo z tengelyirany vektorialis szorzata adja a kamera x-tengelyiranyat
        vec x_ = up.crossProd(z_).normalize();

        // a kamera y tengelyiranya az x es z vektorialis szorzata
        vec y_ = z_.crossProd(x_).normalize();
        
        this->view = matrix({
            {x_.get(vec_coord_x), x_.get(vec_coord_y), x_.get(vec_coord_z), -x_.dotProd(eye)},
            {y_.get(vec_coord_x), y_.get(vec_coord_y), y_.get(vec_coord_z), -y_.dotProd(eye)},
            {z_.get(vec_coord_x), z_.get(vec_coord_y), z_.get(vec_coord_z), -z_.dotProd(eye)},
            {0,                   0,                   0,                   1               }
        });
    }
};
#endif
