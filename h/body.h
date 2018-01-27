#ifndef BODYH
#define BODYH

#include <iostream>
#include <cmath>
#include <vector>
#include "vec.h"
#include "matrix.h"
#include "camera.h"

using namespace std;

class body{
public:
    typedef struct{
        vec* p0;
        vec* p1;
    } edge;
    
    typedef struct{
        vec* v[4];
        body* bPointer = NULL;
        float r_ = 0; float g_ = 0; float b_ = 0;
    } face;
    
    typedef struct{
        int v[4];
        float r_ = 0; float g_ = 0; float b_ = 0;
    } faceIndex;
    
    int numPoints;
    float color_r = 1, color_g = 1, color_b = 1;
    vector<body::edge> edges;       // elek
    vector<body::faceIndex> drawableFaces; // kivalogatott, rajzolando lapok
    
    vec* transformatedToCam;        // kamera rendszerebe transzformalt pontok
    
    // lap letrehozasa 
    static face initFace(vec* v0, vec* v1, vec* v2, vec* v3, float r_p,float r_g,float r_b, body* bp) {
        face f;
        f.bPointer = bp;
        f.v[0] = v0;
        f.v[1] = v1;
        f.v[2] = v2;
        f.v[3] = v3;
        f.r_ = r_p;
        f.g_ = r_g;
        f.b_ = r_b;
        return f;
    }
    
    ~body(){
        delete[] pointsArr;
        delete[] transformated;
        delete[] transformatedToCam;
        delete[] facesArr;
    }

    // transzformacios matrix hozzaadasa
    void addTransformation(matrix t){
        this->transformations = this->transformations * t;
    }
    
    // hozzaadott transzformaciok(forgatas, eltolas...stb) alkalmazasa
    void Transformation(){
        for (int i = 0; i < this->numPoints; i++){
            this->transformated[i] = (this->pointsArr[i] * this->transformations).projectTo3D(3);
        }
    }
    
    // transzformacios matrix hozzaadasa, majd alkalmazasa
    void Transformation(matrix Transformation){
        this->transformations = Transformation;
        this->Transformation();
    }

    // A kamera transzformacios matrixaval valo beszorzas, vagyis a kamera koordinatarendszerebe valo athelyezes
    void camTransformation(camera *cam){
        for (int i = 0; i < numPoints; i++){
            this->transformatedToCam[i] = cam->transform( this->transformated[i] ) ;
        }
    }
    
    // laprajzolas soran a feny beallitasa a lapokra
    void setLight(vec* l){
        vec e1;
        vec e2;
        vec n1;
        vec faceToLight;
        for (body::faceIndex &f : drawableFaces) {
            e1 = this->transformated[f.v[0]] - transformated[f.v[1]];
            e2 = this->transformated[f.v[0]] - transformated[f.v[2]];
            n1 = e2.crossProd( e1 ).normalize(); // a lap 2 élének CrossProd-ja ( meroleges a lapra )
            faceToLight = (*l - this->transformated[f.v[0]]).normalize();
            float light = ( n1.dotProd( faceToLight ) + 1) / 2 ;
            f.r_ = this->color_r * light;
            f.g_ = this->color_g * light;
            f.b_ = this->color_b * light;
        }
    }

    void selectFaces(camera *cam){
        vec edge1, edge2;
        this->drawableFaces.clear();
        for (int i = 0; i < numFaces; i++){
                edge1 = transformated[facesArr[i].v[0]] - transformated[facesArr[i].v[1]];
                edge2 = transformated[facesArr[i].v[0]] - transformated[facesArr[i].v[2]];
                vec normal = edge2.crossProd( edge1 ); // a lap 2 élének CrossProd-ja ( meroleges a lapra )
                vec faceToCam = transformated[facesArr[i].v[0]] - cam->eye;
                if (normal.dotProd( faceToCam ) < 0) {
                    this->drawableFaces.push_back(initFaceIndex(
                        facesArr[i].v[0],
                        facesArr[i].v[1],
                        facesArr[i].v[2],
                        facesArr[i].v[3]
                        ));
                }
        }
    }
    
    
protected:
    vec* pointsArr;
    int dim;
    int numEdges;
    int numFaces;
    
    
    void addEdge(int v0, int v1) {
        edge e;
        e.p0 = &this->transformatedToCam[v0];
        e.p1 = &this->transformatedToCam[v1];
        this->edges.push_back(e);
    }
    
    static faceIndex initFaceIndex(int v0, int v1, int v2, int v3) {
        faceIndex f;
        f.v[0] = v0;
        f.v[1] = v1;
        f.v[2] = v2;
        f.v[3] = v3;
        return f;
    }
    
    faceIndex* facesArr;            // lapok
    vec* transformated;             // a valos rendszerben transzformalt pontok
    matrix transformations;         // egysegmatrix legyen az elejen

};

#endif