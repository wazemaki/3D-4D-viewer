#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

class matrix{
public:
    int dim1 = 0, dim2 = 0;
    
    matrix(){
        dim1 = 0; dim2 = 0;
        data = new float*[0];
    }
    
    matrix(int d){
        this->initId(d);
    }
    
    matrix(int sorok, int oszlopok){
        dim1 = sorok; dim2 = oszlopok;
        data = new float*[dim1];
        for (int i = 0; i < dim1; i++){
            data[i] = new float[dim2];
            for (int j = 0; j < dim2; j++){
                data[i][j] = 0.00f;
            }
        }
    }
    
    matrix(const vector<vector<float>> data_param){
        dim1 = data_param.size(); dim2 = data_param[0].size();
        data = new float*[dim1];
        for (int i = 0; i < dim1; i++)
        {
            data[i] = new float[dim2];
            for (int j = 0; j < dim2; j++)
            {
                data[i][j] = (j < data_param[i].size()) ? data_param[i][j] : 0.00f;
            }
        }
    }
    
    matrix(const matrix& m){
        dim1 = m.dim1; dim2 = m.dim2;
        data = new float*[dim1];
        for (int i = 0; i < dim1; i++){
            data[i] = new float[dim2];
            for (int j = 0; j < dim2; j++){
                data[i][j] = m.data[i][j];
            }
        }
    }
    
    ~matrix(){
        for(int i = 0; i< this->dim1; i++){
            delete[] data[i];
        }
        delete[] data;
    }

    void set(int d1, int d2, float e){
        if (d1 >= 0 && d1<dim1 && d2 >= 0 && d2<dim2) data[d1][d2] = e;
    }
    
    float get(int d1, int d2){
        if (d1 >= 0 && d1<dim1 && d2 >= 0 && d2<dim2){
            return data[d1][d2];
        } else {
            return 0;
        }
    }
    
    matrix& operator= (const matrix& m) {
        dim1 = m.dim1;
        dim2 = m.dim2;
        data = new float*[dim1];
        for (int i = 0; i < dim1; i++){
            data[i] = new float[dim2];
            for (int j = 0; j < dim2; j++){
                data[i][j] = m.data[i][j];
            }
        }
        return *this;
    }

    // matrix szorzas
    matrix operator* (matrix& x) { //HA nem stimmelnek a dimenziok, 0-val feltoltott matrixot ad!!!
        matrix szorzat(dim1, x.dim2);
        if (dim2 == x.dim1){
            float sum;
            int i, j, k;
            for (i = 0; i < dim1; i++){
                for (j = 0; j < x.dim2; j++) {
                    sum = 0;
                    for (k = 0; k < dim2; k++)
                        sum = sum + data[i][k] * x.data[k][j];
                    szorzat.data[i][j] = sum;
                }
            }
        }
        return szorzat;
    }

    // egysegmatrixot hoz letre a megadott dimenzioval
    void initId(int d){
        dim1 = d; dim2 = d;
        data = new float*[dim1];
        for (int i = 0; i < dim1; i++)
        {
            data[i] = new float[dim2];
            for (int j = 0; j < dim2; j++)
            {
                data[i][j] = (i == j) ? 1.0f : 0.0f;
            }
        }
    }
    
    // matrix transzponaltja
    matrix T(){
        matrix out(dim2,dim1);
        for (int i = 0; i < dim1; i++)
            for (int j = 0; j < dim2; j++)
                out.data[i][j] = data[j][i];
        return out;
    }

    // matrix determinansat szamolja ki
    float det(){
        if (dim1 == dim2){
            if (dim1 == 1) return data[0][0];

            float det = 0;

            matrix m_minor(dim1-1,dim1-1);

            for (int i = 0; i < dim1; i++){
                m_minor = this->m_minor(0, i);
                det += (i % 2 == 1 ? -1.0 : 1.0) * data[0][i] * m_minor.det();
            }

//            for (int i = 0; i < dim1 - 1; i++){
//                delete[] m_minor.data[i];
//            }

            return det;
        } else {
            return 0;
        }
    }

    // matrix inverzet szamolja
    matrix invert(){
        matrix out(dim1, dim1);
        if (dim1 == dim2){
            float det = 1.0 / this->det();
            matrix m_minor(dim1 - 1, dim1 - 1);

            for (int j = 0; j < dim1; j++){
                for (int i = 0; i < dim1; i++){
                    m_minor = this->m_minor(j, i);
                    out.data[i][j] = det*m_minor.det();
                    if ((i + j) % 2 == 1){
                        out.data[i][j] = -out.data[i][j];
                    }
                }
            }
        }
        return out;
    }
    
    // skalarral valo szorzas
    void skalar(float s){
        for (int i = 0; i < dim1; i++){
            for (int j = 0; j < dim2; j++){
                data[i][j] *= s;
            }
        }
    }

    void print(){
        cout << "dim1: "<<this->dim1<<" dim2: "<<this->dim2<<endl;
        for (int i = 0; i < dim1; i++){
            cout << " |";
            for (int j = 0; j < dim2; j++){
                printf(" %2.2f |", data[i][j]);
            }
            cout << endl;
        }
    }
    float** data;
	
private:

    // minor matrix a determinans szamitashoz
    matrix m_minor(int x, int y){
        int colCount = 0, rowCount = 0;
        matrix out(dim1 - 1, dim2 - 1);
        for (int i = 0; i < dim1; i++){
            if (i != x){
                colCount = 0;
                for (int j = 0; j < dim2; j++){
                    if (j != y){
                        out.data[rowCount][colCount] = data[i][j];
                        colCount++;
                    }
                }
                rowCount++;
            }
        }
        return out;
    }
};

#endif