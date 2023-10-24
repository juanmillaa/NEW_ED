/**
 * @file imageop.cpp
 * @brief Fichero con definiciones para el resto de métodos de la clase Image
 */

#include <iostream>
#include <cmath>
#include <image.h>

#include <cassert>
const int MAX_BYTE_VALUE = 255;


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
