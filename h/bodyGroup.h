#ifndef BODYGRP_H
#define BODYGRP_H

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "vec.h"
#include "matrix.h"
#include "camera.h"
#include "body.h"
#include "origo.h"
#define DRAWMODE_POINTS 0
#define DRAWMODE_LINES 1
#define DRAWMODE_FACES 2
#define DRAWMODE_LINES_POINTS 10

using namespace std;

class bodyGroup{
public:
    vec* light = NULL;
    
    int drawMode;
    
    body* activeBody = NULL; // kivalasztott test
    
    vector<body*> bodys; // a group-on belul levo testek

    vector<body::face> allDrawableFacesArr; // az osszes rajzolando face
    vector<vec*> allDrawablePointsArr; // az osszes rajzolando pont
    
    bodyGroup(vector<body*> b_param, int drawMode = DRAWMODE_POINTS, bool needOrigo = false){
        this->bodys = b_param;
        this->drawMode = drawMode;
        
        if(needOrigo){
            this->o = new origo();
        }
    }
    
    // a sajat transzformacios matrix alkalmazasa az osszes testre
    void transformation(){
        for (body *b : bodys) {
            if(b != NULL){
                b->Transformation();
            }
        }
    }
    
    // sorba megy a testeken es beszorozza a kamera elore legyartott matrixaval
    void cameraTransformation(camera *cam){
        for (body *b : bodys) {
            if(b != NULL){
                b->camTransformation(cam);
                if(this->drawMode == DRAWMODE_FACES){
                    // ha lapokat kell rajzolni, kivalogatja a rajzolando lapokat
                    b->selectFaces(cam);
                }
                
                if(this->light != NULL){
                    b->setLight(this->light);
                }
            }
        }
        // origot is
        if(this->o){
            this->o->camTransformation(cam);
        }
    }
    
    // testek kivalasztasa
//    void searchBody(int x, int y){
//        body* bp = NULL;
//        for (body::face f : this->allDrawableFacesArr) {
//            bool inside = false;
//            for ( int i = 0, j = 3 ; i < 4 ; j = i++ ){
//                if (( f.v[i]->inhomogen().get(1) > y ) != ( f.v[j]->inhomogen().get(1) > y ) && x < ( f.v[j]->inhomogen().get(0) - f.v[i]->inhomogen().get(0) ) * ( y - f.v[i]->inhomogen().get(1) ) / ( f.v[j]->inhomogen().get(1) - f.v[i]->inhomogen().get(1) ) + f.v[i]->inhomogen().get(0) ){
//                    inside = !inside;
//                }
//            }
//            if(inside) {bp = f.bPointer;}
//        }
//        if(this->activeBody != bp){
//            this->activeBody = bp;
//        } else {
//            this->activeBody = NULL;
//        }
//    }
    
    void getDrawableFaces(){
        this->allDrawableFacesArr.clear();
        for (body *b : bodys) {
                if(b != NULL){
                for (body::faceIndex f : b->drawableFaces) {
                    this->allDrawableFacesArr.push_back(body::initFace(
                        &b->transformatedToCam[f.v[0]],
                        &b->transformatedToCam[f.v[1]],
                        &b->transformatedToCam[f.v[2]],
                        &b->transformatedToCam[f.v[3]],
                            f.r_, f.g_, f.b_,
                        b
                        ));
                }
            }
        }
    }
    
    
    void draw(){
        switch(this->drawMode){
            case DRAWMODE_POINTS:
                this->drawPoints();
                break;
            case DRAWMODE_LINES:
                this->drawLines();
                break;
            case DRAWMODE_LINES_POINTS:
                this->drawLines();
                this->drawPoints();
                break;
            case DRAWMODE_FACES:
                this->drawFaces();
                break;
        }
        this->drawOrigo();
    }

private:
    
    origo* o;
    
    void drawOrigo(){
        if(this->o){
            glLineWidth(4);
            vec* drawable = this->o->transformatedToCam;
            glBegin(GL_LINES);
                glColor3f(0,1,0);
                drawable[0].inhomogen().gldraw();
                drawable[1].inhomogen().gldraw();
                glColor3f(1,0,0);
                drawable[0].inhomogen().gldraw();
                drawable[2].inhomogen().gldraw();
                glColor3f(0,0,1);
                drawable[0].inhomogen().gldraw();
                drawable[3].inhomogen().gldraw();
            glEnd();
        }
    }
    
    // testek pontjainak rajzolasa
    void drawPoints(){
        glPointSize(7);
        glColor3f(.8,.2,.2);
        glBegin(GL_POINTS);
        
        for (body *b : this->bodys) {
            if(b != NULL){
                for (int i=0; i < b->numPoints; i++) {
                    b->transformatedToCam[i].inhomogen().gldraw();
                }
            }
        }
        
        glEnd();
    }
    
    void drawLines(){
        this->getDrawableFaces();
        glLineWidth(1);
        glBegin(GL_LINES);
        for (body *b : bodys) {
            glColor3f(b->color_r, b->color_g, b->color_b);
            for (body::edge e : b->edges) {
                e.p0->inhomogen().gldraw();
                e.p1->inhomogen().gldraw();
            }
        }
        glEnd();
        
    }
    void drawFaces(){
        this->getDrawableFaces();
        qsort(this->allDrawableFacesArr.data(), this->allDrawableFacesArr.size(), sizeof(body::face), bodyGroup::compareFaces);
        
        vec p[4];
        for (body::face f : this->allDrawableFacesArr) {
            if(this->light != NULL){
                glColor3f(f.r_, f.g_, f.b_);
            } else {
                glColor3f(f.bPointer->color_r * .8, f.bPointer->color_g * .8, f.bPointer->color_b * .8);
            }  
            
            glBegin(GL_QUADS);
            for (int j = 0; j < 4; j++){
                p[j] = f.v[j]->inhomogen();
                p[j].gldraw();
            }
            glEnd();
            if(this->activeBody == f.bPointer){
                glLineWidth(4);
            } else {
                glLineWidth(2);
            }
            glColor3f(f.bPointer->color_r, f.bPointer->color_g, f.bPointer->color_b);
            glBegin(GL_LINE_LOOP);
            for (int j = 0; j <  4; j++){
                p[j].gldraw();
            }
            glEnd();
        }
    }
    
    static int compareFaces(const void *a, const void *b) {
	float a_k = (
                (*(body::face*)a).v[0]->get(vec_coord_z)
		+ (*(body::face*)a).v[2]->get(vec_coord_z)) / 2;
        
	float b_k = (
                (*(body::face*)b).v[0]->get(vec_coord_z)
		+ (*(body::face*)b).v[2]->get(vec_coord_z)) / 2;
        
	if (a_k < b_k) return -1;
	if (a_k == b_k) return  0;
	if (a_k > b_k) return  1;
   }
    
};

#endif