import cv2
import numpy as np
import matplotlib.pyplot as plt

'''
img = cv2.imread("Fondo.jpg")
print(img)
cv2.imshow('Prueba',img)
cv2.waitKey(0)
'''

def procesarImagen(direccionImagen):
    imagen = cv2.imread(direccionImagen)
    filasImagen = 0
    for filas in range(0, imagen.rows):
        for columnas in range(0, imagen.cols):
            colorAzul = imagen.at<Vec3b>(filas, columnas)[0];
            print(colorAzul)
    return 

procesarImagen("Fondo.jpg")

'''

void uploadImageOneInfo(vector<vector<Pixel>> &pImageOneInfo){
    /*
    Purpose: 
        -Load the first image information and converts it to a matrix of colors for futures functionalities.
    Paramateres: 
        -Recieves an empty matrix who is going to be filled with RGB colors.
    Returns: 
        -Nothing. Void
    */
    
    string imagePath = "C:/Users/luist/OneDrive/Escritorio/Proyecto1/Prueba1.png";
    Mat colorImage = imread(imagePath);
    if(existsImage(colorImage)){
        int blueChannel; int greenChannel; int redChannel;
        int rowsImageOneInfo =0;
        for (int imageRows = 0; imageRows < colorImage.rows; imageRows+=54){
            for (int imageColumns = 0; imageColumns < colorImage.cols; imageColumns++) {
                redChannel = colorImage.at<Vec3b>(imageRows, imageColumns)[2];
                greenChannel = colorImage.at<Vec3b>(imageRows, imageColumns)[1];
                blueChannel = colorImage.at<Vec3b>(imageRows, imageColumns)[0];

                Pixel addingPixel = Pixel(redChannel,greenChannel,blueChannel);

                pImageOneInfo[rowsImageOneInfo][imageColumns] = addingPixel;
                
            }
            rowsImageOneInfo++;
        }
    }
    else{
        exit(1);
    }
    
}

'''
