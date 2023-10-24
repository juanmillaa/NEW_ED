/**
 * @file image.cpp
 * @brief Fichero con definiciones para los métodos primitivos de la clase Image
 *
 */

#include <cstring>
#include <cassert>
#include <iostream>
#include<cmath>

#include <image.h>
#include <imageIO.h>

using namespace std;

const int MAX_BYTE_VALUE = 255;
/********************************
      FUNCIONES PRIVADAS
********************************/
void Image::Allocate(int nrows, int ncols, byte * buffer){
    rows = nrows;
    cols = ncols;

    img = new byte * [rows];

    if (buffer != 0)
        img[0] = buffer;
    else
        img[0] = new byte [rows * cols];

    for (int i=1; i < rows; i++)
        img[i] = img[i-1] + cols;
}

// Función auxiliar para inicializar imágenes con valores por defecto o a partir de un buffer de datos
void Image::Initialize (int nrows, int ncols, byte * buffer){
    if ((nrows == 0) || (ncols == 0)){
        rows = cols = 0;
        img = 0;
    }
    else Allocate(nrows, ncols, buffer);
}

// Función auxiliar para copiar objetos Imagen

void Image::Copy(const Image & orig){
    Initialize(orig.rows,orig.cols);
    for (int k=0; k<rows*cols;k++)
        set_pixel(k,orig.get_pixel(k));
}

// Función auxiliar para destruir objetos Imagen
bool Image::Empty() const{
    return (rows == 0) || (cols == 0);
}

void Image::Destroy(){
    if (!Empty()){
        delete [] img[0];
        delete [] img;
    }
}

LoadResult Image::LoadFromPGM(const char * file_path){
    if (ReadImageKind(file_path) != IMG_PGM)
        return LoadResult::NOT_PGM;

    byte * buffer = ReadPGMImage(file_path, rows, cols);
    if (!buffer)
        return LoadResult::READING_ERROR;

    Initialize(rows, cols, buffer);
    return LoadResult::SUCCESS;
}

/********************************
       FUNCIONES PÚBLICAS
********************************/

// Constructor por defecto

Image::Image(){
    Initialize();
}

// Constructores con parámetros
Image::Image (int nrows, int ncols, byte value){
    Initialize(nrows, ncols);
    for (int k=0; k<rows*cols; k++) set_pixel(k,value);
}

bool Image::Load (const char * file_path) {
    Destroy();
    return LoadFromPGM(file_path) == LoadResult::SUCCESS;
}

// Constructor de copias

Image::Image (const Image & orig){
    assert (this != &orig);
    Copy(orig);
}

// Destructor

Image::~Image(){
    Destroy();
}

// Operador de Asignación

Image & Image::operator= (const Image & orig){
    if (this != &orig){
        Destroy();
        Copy(orig);
    }
    return *this;
}

// Métodos de acceso a los campos de la clase

int Image::get_rows() const {
    return rows;
}

int Image::get_cols() const {
    return cols;
}

int Image::size() const{
    return get_rows()*get_cols();
}

// Métodos básicos de edición de imágenes
void Image::set_pixel (int i, int j, byte value) {
    img[i][j] = value;
}
byte Image::get_pixel (int i, int j) const {
    return img[i][j];
}

// This doesn't work if representation changes
void Image::set_pixel (int k, byte value) {
    // TODO this makes assumptions about the internal representation
    // TODO Can you reuse set_pixel(i,j,value)?
    img[0][k] = value;
}

// This doesn't work if representation changes
byte Image::get_pixel (int k) const {
    // TODO this makes assumptions about the internal representation
    // TODO Can you reuse get_pixel(i,j)?
    return img[0][k];
}

// Métodos para almacenar y cargar imagenes en disco
bool Image::Save (const char * file_path) const {
    // TODO this makes assumptions about the internal representation
    byte * p = img[0];
    return WritePGMImage(file_path, p, rows, cols);
}

// Metodo para invertir una imagen
void Image::Invert(){
    
    for (int i=0; i < this->get_rows(); i++)
        for (int j=0; j < this->get_cols(); j++)
            this->set_pixel( i, j, MAX_BYTE_VALUE - this->get_pixel(i, j));
}

Image Image::Crop(int nrow, int ncol, int height, int width) const{
    
    Image subimage(height,width);
    
    for(int i = 0; i < height; ++i){
        for(int j = 0; j < width; ++j){
            subimage.set_pixel( i, j ,this->get_pixel(i+nrow,j+ncol));
        }
      
    }
    return subimage;

}

double Image::Mean(int i, int j, int height, int width) const{
    double mean = 0;
    for(int row = i; row < i+height; ++row){
        for(int col = j; col < j+width; ++col){
            mean += this->get_pixel(row,col);
        }
    }
    
    mean = mean /(height*width);
    
    return mean;
}

Image Image::Zoom2X() const{
    
    Image zoom(2*this->get_rows() -1, 2*this->get_cols() -1);

    for(int i = 0; i < zoom.get_rows(); i+=2){
        for(int j = 0; j < zoom.get_cols();++j){
            if(j%2==0){
               zoom.set_pixel( i, j, this->get_pixel(i/2,j/2));
            }else{
                zoom.set_pixel( i, j, round((this->get_pixel(i/2,j/2)+this->get_pixel(i/2,j/2+1))/2.0));
            }       
        }
    }

    for(int i = 1; i < zoom.get_rows(); i+=2){
        for(int j = 0; j < zoom.get_cols();++j){
            if(j%2==0){
                zoom.set_pixel(i, j, round((zoom.get_pixel(i-1,j)+zoom.get_pixel(i+1,j))/2.0));
            }
           else{
                zoom.set_pixel(i, j, round((zoom.get_pixel(i-1,j-1)+zoom.get_pixel(i+1,j-1)+zoom.get_pixel(i-1,j+1)+zoom.get_pixel(i+1,j+1))/4.0));
            }
        }
    }

    return zoom;
}

Image Image::Subsample(int factor) const{
    
    int new_rows = this->get_rows()/factor;
    int new_cols = this->get_cols()/factor;
    
    Image subsample(new_rows, new_cols);
    
    for(int i = 0; i < new_rows; ++i){
        for(int j = 0; j < new_cols; ++j){
            subsample.set_pixel(i, j, round(this->Mean(i*factor, j*factor, factor, factor)));
        }
    }
    
    return subsample;
}

//void Image::AdjustContrast (byte in1, byte in2, byte out1, byte out2){
//
//    int n_rows = this->get_rows();
//    int n_cols = this->get_cols();
//    int new_byte;
//    byte parte_fija;
//    byte min, max, a, b;
//    for(int i = 0; i <  n_rows*n_cols; ++i){
//        if(this->get_pixel(i)<=in1){
//            min = 0;
//            max = out1;
//            a = 0;
//            b = in1;
//            
//        } 
//        if(in1< this->get_pixel(i) <=in2){
//            min = out1;
//            max = out2;
//            a = in1;
//            b = in2;
//        }
//        if(this->get_pixel(i)>in2){
//            min = out2;
//            max = MAX_BYTE_VALUE;
//            a = in2;
//            b = MAX_BYTE_VALUE;
//        }
//        parte_fija = round((double)(max-min)/(b-a));
//        new_byte = round(min + (parte_fija*(this->get_pixel(i)-a)));
//        this->set_pixel(i,new_byte);
//    }
//    
//}

void Image::AdjustContrast (byte in1, byte in2, byte out1, byte out2){
    
    byte new_byte;
    
    byte division_1;
    byte division_2 = round((out2-out1)/(in2-in1));
    byte division_3 ;
    if(in1 != 0){
        division_1 = round(out1/in1);
    }
    if(MAX_BYTE_VALUE != in2){
        division_3 = round((MAX_BYTE_VALUE-out2)/(MAX_BYTE_VALUE-in2));
    }
    for(int i = 0; i <  this->size(); ++i){
        new_byte = this->get_pixel(i);
        if((0 < this->get_pixel(i)) && (this->get_pixel(i)<=in1)){
            new_byte = division_1*this->get_pixel(i);

            
        } 
         if((in1< this->get_pixel(i)) && (this->get_pixel(i)  <=in2)){

              new_byte = out1 +(division_2*(this->get_pixel(i)-in1));

            
        }
          if( (this->get_pixel(i) < MAX_BYTE_VALUE) && (this->get_pixel(i)>in2)){
            new_byte = out2 +(division_3*(this->get_pixel(i)-in2));

        }
         this->set_pixel(i,new_byte);

    }
    
}