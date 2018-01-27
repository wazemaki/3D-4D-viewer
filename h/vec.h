#ifndef VECTORH
#define VECTORH
#include <iostream>
#include <cmath>
#include <vector>
#include "matrix.h"

    enum vec_coords{vec_coord_x,vec_coord_y,vec_coord_z,vec_coord_v};
    enum vec_ops{vec_op_add,vec_op_sub,vec_op_mul,vec_op_div};
    
    
    // geometriai vektor, lehet akarmennyi dimenzios
class vec{
private:
    vec opFloat(const float f, vec_ops op);
    vec opVec(const vec& param, vec_ops op);
public:
    int dim = 0;
    float* data;
    
    vec(){
        this->dim = 0;
        this->data = new float[this->dim];
    }

    vec(int dparam){
        this->dim = dparam;
        this->data = new float[this->dim];
        for(int i = 0; i < this->dim; i++){
            this->data[i] = 0.0f;
        }
    }

    vec(const vector<float> data_param) {
        this->set(data_param);
    }
    
    vec(const vec& v){
        this->dim = v.dim;
        this->data = new float[this->dim];
        for(int i = 0; i < this->dim; i++){
            this->data[i] = v.data[i];
        }
    }
    
    ~vec(){ 
        delete[] data;
    }

    // skalaris szorzat
    float dotProd(const vec& vec_param){
        int d = min(this->dim,vec_param.dim);
        float sum = 0;
            for(int i = 0; i < d; i++){
                sum += this->data[i] * vec_param.data[i];
            }
            return sum;
    }
    
    // 2 vektor crossproductja, meroleges a vektorok altal kifeszitett sikra
    vec crossProd(const vec& vec_param){
       // if(this->dim == vec_param.dim){
            return vec({
                this->data[1] * vec_param.data[2] - this->data[2] * vec_param.data[1],
                this->data[2] * vec_param.data[0] - this->data[0] * vec_param.data[2],
                this->data[0] * vec_param.data[1] - this->data[1] * vec_param.data[0]
            });
      //  } else {
      //      return vec(3);
      //  }
    }
    
    // tavolsag egy masik, ugyanannyi dimenzioju vektortol, parameter koordinatakban!
    float dist(const vector<float> data_param){
        if(data_param.size() == this->dim){
            float sum = 0;
            for(int i = 0; i < this->dim; i++){
                sum += pow(data_param[i] - this->data[i], 2);
            }
            return sqrt(sum);
        } else {
            return false;
        }
    }
    
    void set(const vector<float> param){
      //  delete[] this->data;
        this->dim = param.size();
        this->data = new float[this->dim];
        for(int i = 0; i < this->dim; i++){
            this->data[i] = param[i];
        }
    }
    
    void setDim(int dim, float fill){
        if(this->dim != dim){
            float* tmpPointer = this->data;
            int minDim = min(this->dim, dim);
            this->dim = dim;
            this->data = new float[this->dim];
            for(int i = 0; i < this->dim && this->dim > minDim; i++){
                this->data[i] = fill;
            }
            for(int i = 0; i < minDim; i++){
                this->data[i] = tmpPointer[i];
            }
            delete[] tmpPointer;
        }
    }

    vec& operator= (const vec& v) {
        delete[] this->data;
        this->dim = v.dim;
        this->data = new float[this->dim];
        for(int i = 0; i < this->dim; i++){
            this->data[i] = v.data[i];
        }
        return *this;
    }

    vec operator*(const matrix& m){
        if ( m.dim1 == m.dim2){ 
            int d = m.dim1;
            float out[d];
            float seged[d];
            for (int i = 0; i < this->dim; i++) {
                seged[i] = this->data[i];
            }
            if(this->dim < d){
                for (int i = this->dim; i < d; i++) {
                    seged[i] = 1.0f;
                }
            }
        
            for (int i = 0; i < d; i++) {
                out[i] = 0.0f;
                for (int j = 0; j < d; j++) {
                    out[i] += seged[j] * m.data[i][j];
                }
            }
            return vec(vector<float>(out,out + this->dim));
        } else {
            return vec();
        }
    }

    vec operator*(const vec& v){
        return this->opVec(v,vec_op_mul);
    }
    vec operator+(const vec& v){
        return this->opVec(v,vec_op_add);
    }
    vec operator-(const vec& v){
        return this->opVec(v,vec_op_sub);
    }
    
    vec operator*(const float f){
        return this->opFloat(f,vec_op_mul);
    }
    vec operator/(const float f){
        return this->opFloat(f,vec_op_div);
    }

    vec projectTo(vec& v){
        vec v1 = *this;
        float tmp = this->dotProd(v) / (v.length() * v.length());
        return v1 * tmp;
    }

    // normalvektor (meroleges az eredetire)
    vec normal(){
        vec v = *this;
        v.data[0] = this->data[1];
        v.data[1] = this->data[0] * -1;
        return v;
    }
    
    // egysegnyi hosszu, eredetivel egy iranyu vektor
    vec normalize(){
        vec v = *this / this->length();
        return v;
    }

    // a vektor hosszat adja vissza
    float length(){
        float sum = 0;
        for(int i = 0; i < this->dim; i++){
            sum += pow(this->data[i], 2);
        }
        return sqrt(sum);
    }
    
    // inhomogen koordinataval oszt, 3D -> 2D projekcio
    vec inhomogen(){ // vec3-at adjon vissza
        if(this->dim > 3){
            vec v = *this / this->data[ this->dim - 1 ];
            v.dim = 3;
            return v;
        } else {
            return vec(3);
        }
    }
    
    // 4D -> 3D projekcio
    vec projectTo3D(int s = 1){ // vec4-et adjon vissza
        if(this->dim > 4){
            matrix m(5);
            float proj = this->data[3] + s;
            
            m.set(0,0,proj);
            m.set(1,1,proj);
            m.set(2,2,proj);
            m.set(3,3,0);
            m.set(4,4,0);
            
            m.set(3,4,1);
            vec out = *this * m;
            out.dim = 4;
            return out;
        } else {
            return *this;
        }
    }
        
    float get(int i){
        return this->data[i];
    }
    
    float get(vec_coords c){
        switch(c){
            case vec_coord_x: return this->data[0];
            case vec_coord_y: return this->data[1];
            case vec_coord_z: return this->data[2];
            case vec_coord_v: return this->data[3];
        }
    }

    void print(string str = ""){
        cout << str << " : " << this->dim << " ( ";
        for(int i = 0; i < this->dim; i++){
            if(i > 0) cout << " | ";
            cout << this->data[i];
        }
        cout << " )" << std::endl;
    }
    
    void draw(float r, float g, float b, int s = 10){
        glPointSize(s);
        float color_old[3];
        glGetFloatv(GL_CURRENT_COLOR, color_old);
        glColor3f(r, g, b);
        glBegin(GL_POINTS);
            this->gldraw();
        glEnd();
        glColor3f(color_old[0],color_old[1],color_old[2]);
    }

    void gldraw(){
        glVertex2d(this->data[0], this->data[1]);
    }
    
};

vec vec::opVec(const vec& param, vec_ops op){
    int d = min(this->dim, param.dim);
    vec v(d);
    for(int i = 0; i < d; i++){
        switch(op){
            case vec_op_mul: v.data[i] = this->data[i] * param.data[i];
            break;
            case vec_op_add: v.data[i] = this->data[i] + param.data[i];
            break;
            case vec_op_sub: v.data[i] = this->data[i] - param.data[i];
        }
    }
    return v;
}

vec vec::opFloat(const float f, vec_ops op){
        vec v(this->dim);
        for(int i = 0; i < this->dim; i++){
            switch(op){
                case vec_op_mul: v.data[i] = this->data[i] * f;
                break;
                case vec_op_div: v.data[i] = this->data[i] / f;
            }
        }
        return v;
    }

#endif
