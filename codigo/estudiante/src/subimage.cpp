#include <iostream>
#include <cstring>
#include <cstdlib>

#include <image.h>

using namespace std;

int main (int argc, char *argv[]){

 char *origen, *destino; // nombres de los ficheros
 Image image;

 // Comprobar validez de la llamada
 if (argc != 7){
   cerr << "Error: Numero incorrecto de parametros.\n";
   cerr << "Uso: negativo <FichImagenOriginal> <FichImagenDestino> <nfil> <ncol> <height> <width> \n";
   exit (1);
 }
 int nfil, ncol, height, width;
 // Obtener argumentos
 origen  = argv[1];
 destino = argv[2];
 nfil = atoi(argv[3]);
 ncol = atoi(argv[4]);
 height = atoi(argv[5]);
 width = atoi(argv[6]);
 

 // Mostramos argumentos
 cout << endl;
 cout << "Fichero origen: " << origen << endl;
 cout << "Fichero resultado: " << destino << endl;

 // Leer la imagen del fichero de entrada
 if (!image.Load(origen)){
   cerr << "Error: No pudo leerse la imagen." << endl;
   cerr << "Terminando la ejecucion del programa." << endl;
   return 1;
 }
 
 if((nfil + height) >image.get_rows() || (ncol + width) > image.get_cols() ){
     cerr << "Error: "<<endl;
     cerr << "Debe ser: " << endl;
     cerr << "(nfil + height) <= image.get_rows() && (ncol + width) <= image.get_cols()";
     cerr << endl;
     return 1;
 }
 
 // Mostrar los parametros de la Imagen
 cout << endl;
 cout << "Dimensiones de " << origen << ":" << endl;
 cout << "   Imagen   = " << image.get_rows()  << " filas x " << image.get_cols() << " columnas " << endl;
 
   Image subimage;
//  // generamos la subimagen
   subimage = image.Crop(nfil,ncol,height, width);

 // Guardar la imagen resultado en el fichero
 if (subimage.Save(destino))
   cout  << "La imagen se guardo en " << destino << endl;
 else{
   cerr << "Error: No pudo guardarse la imagen." << endl;
   cerr << "Terminando la ejecucion del programa." << endl;
   return 1;
 }
  

 return 0;
}