# imageclasifier

Red neuronal entranada para clasificar imágenes según el animal que aparezca en ella.
Para este proyecto las variables de entrada son imágenes, para crear el dataset se usará una matriz
que tendrá tantas filas como variables de entrada, es decir, imágenes ; y tantas columnas como
píxeles más tres columnas de clasificación que contienen valores de 0 o 1 en función de su
pertenencia a dicha clase.
Las últimas tres columnas contienen un máximo de tres valores distintos, uno por cada clase
correspondiente: “1 0 0” para la clase “Bird”, “0 1 0” para la clase “Dog” y “0 0 1” para la clase
“Horse”.
Cada imagen se almacena en una sola fila de dicha matriz, para ello es necesario convertir cada
imagen en un vector de una sola fila.
Se van a obtener dos dataset distintos, uno del canal V del espacio de color HSV de la imagen, la
cual es la imagen en blanco y negro, y otro del canal H.


# Archivos de codigo
La programación de la red neuronal se puede encontrar en Qt>neuralNetworks>mainwindow.cpp

Se trata de un proyecto realizado en Qt Creator
