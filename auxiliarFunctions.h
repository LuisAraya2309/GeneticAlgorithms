#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <unordered_map>
#include <map>
#include "Pixel.h"

using namespace std;
using namespace cv;

bool existsImage(Mat pColorImage){
    /*
    Purpose: 
        -Verify if an image exists or not.
    Paramateres: 
        -Recieves a Mat type variable, with the selected image.
    Returns: 
        -True or false, it depends if the image was found or not.
    */
    try{
        int rowsValue = pColorImage.rows;
        if(rowsValue > 0){
            cout<<"The image was found successfully"<<endl;
            return true;
        } 
        else{
            throw(rowsValue);
        }
            
    }
    catch(int pRowsValue){
        cout<<"The image does not exist, the size is:  "<<pRowsValue<<"x"<<pRowsValue<<endl;
        return false;
    }
}

void uploadImageInfo(vector<vector<Pixel>> &pImageInfo){
    /*
    Purpose: 
        -Load the first image information and converts it to a matrix of colors for futures functionalities.
    Paramateres: 
        -Recieves an empty matrix who is going to be filled with RGB colors.
    Returns: 
        -Nothing. Void
    */
    
    string imagePath = "C:/Users/luist/OneDrive/Escritorio/GeneticAlgorithms/Laberinto.jpg";
    Mat colorImage = imread(imagePath);
    int blueChannel; int greenChannel; int redChannel;
    if(existsImage(colorImage)){
        for (int imageRows = 0; imageRows < colorImage.rows; imageRows++){
            for (int imageColumns = 0; imageColumns < colorImage.cols; imageColumns++) {
                redChannel = colorImage.at<Vec3b>(imageRows, imageColumns)[2];
                greenChannel = colorImage.at<Vec3b>(imageRows, imageColumns)[1];
                blueChannel = colorImage.at<Vec3b>(imageRows, imageColumns)[0];

                Pixel addingPixel = Pixel(redChannel,greenChannel,blueChannel,imageRows,imageColumns);

                pImageInfo[imageRows][imageColumns] = addingPixel;
                
            }
        }
    }
    else{
        exit(1);
    }
    
    
}

int getElementPosition(vector<Pixel> &pixels, Pixel &searchElement){

    /*
    Purpose: 
        - Return the position of an specific Pixel
    Parameters: 
        - A vector of pixels.
        - The pixel to find.
    Returns:
        - An interger with the index of this Pixel in the vector
    */

    size_t pixelsLength = pixels.size();
    for(int iteratePixels = 0; iteratePixels<pixelsLength;iteratePixels++){
        Pixel actualPixel = Pixel();
        actualPixel= pixels[iteratePixels];
        bool isRedEqual = actualPixel.getRed() == searchElement.getRed(); 
        bool isBlueEqual =  actualPixel.getBlue() == searchElement.getBlue();
        bool isGreenEqual =  actualPixel.getGreen() == searchElement.getGreen();
        if((isRedEqual)&&(isBlueEqual)&&(isGreenEqual)){
            return iteratePixels;
        }
    }
    return -1;    

}