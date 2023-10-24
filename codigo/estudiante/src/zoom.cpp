#include <iostream>
#include <cstring>
#include <cstdlib>

#include <image.h>

using namespace std;

int main (int argc, char *argv[]){

 char *origen, *destino; // nombres de los ficheros
 Image image;

 // Comprobar validez de la llamada
 if (argc != 6){
   cerr << "Error: Numero incorrecto de parametros.\n";
   cerr << "Uso: negativo <FichImagenOriginal> <FichImagenDestino> <fila> <col> <lado> \n";
   exit (1);
 }
 int fila, col, lado;
 // Obtener argumentos
 origen  = argv[1];
 destino = argv[2];
 fila = atoi(argv[3]);
 col = atoi(argv[4]);
 lado = atoi(argv[5]);

 

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
 
 if((fila+ lado) >image.get_rows() || (col + lado) > image.get_cols() ){
     cerr << "Error: "<<endl;
     cerr << "Debe ser: " << endl;
     cerr << "(fila + lado) <= image.get_rows() && (col + lado) <= image.get_cols()";
     cerr << endl;
     return 1;
 }
 
 // Mostrar los parametros de la Imagen
 cout << endl;
 cout << "Dimensiones de " << origen << ":" << endl;
 cout << "   Imagen   = " << image.get_rows()  << " filas x " << image.get_cols() << " columnas " << endl;
 
 Image zoom;
 // generamos la subimagen
 zoom = image.Crop(fila,col,lado, lado);
 
 // le hacemos el zoom
 zoom = zoom.Zoom2X();
 
 // Guardar la imagen resultado en el fichero
 if (zoom.Save(destino))
   cout  << "La imagen se guardo en " << destino << endl;
 else{
   cerr << "Error: No pudo guardarse la imagen." << endl;
   cerr << "Terminando la ejecucion del programa." << endl;
   return 1;
 }
  

 return 0;
}