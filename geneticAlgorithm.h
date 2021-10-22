#include <stdlib.h>
#include "auxiliarFunctions.h"
#include <vector>
#include <unordered_map>
#include <map>
#include <random>
#include <opencv2/opencv.hpp>

using namespace std;

void generateInitialPixels(vector<Pixel> &initialPixels,vector<vector<Pixel>> &pImageInfo, Mat &imageFirstPopulation){
    int maxX = 94;
    int maxY = 94;
    map<int,int> xPositionsUsed;
    map<int,int> yPositionsUsed;

    for(int idx = 0;idx<initialPixels.size();idx++){
        int x , y;
        while (true){
            x = rand() % maxX;
            y = rand() % maxY;
            if((xPositionsUsed.count(x)==0) && (yPositionsUsed.count(y)==0)){
                break;
            }
        }

        //Save the first population into a vector 
        int redChannel = pImageInfo[x][y].getRed();
        int greenChannel = pImageInfo[x][y].getGreen();
        int blueChannel = pImageInfo[x][y].getBlue();
        initialPixels[idx] = Pixel(redChannel,greenChannel,blueChannel,x,y);

        //Set the first population in the maze matrix 
        pImageInfo[x][y].setRed(180);
        pImageInfo[x][y].setGreen(0);
        pImageInfo[x][y].setBlue(20);

        // set pixel
        imageFirstPopulation.at<Vec3b>(x,y) = Vec3b(20,0,180);
        imshow("windowName",imageFirstPopulation);
    }
}

string importantColourFound(Pixel pixel){
    int redValue = pixel.getRed();
    int greenValue = pixel.getGreen();
    int blueValue = pixel.getBlue();
    
    bool itsGrey = redValue && greenValue && blueValue;

    if(){     //Check if its white

    }
    else{
        if(){ //Check if its green

        }
        else{
            if(){  //Check if its blue

            }
            else{    //Then its black

            }

        }
    }

}

float individualFitness(vector<vector<Pixel>> &pCleanImage,Pixel pixel){
    int rangeSearched = 5;
    int x = pixel.getPositionX();
    int y = pixel.getPositionY();

    float pixelFitness = 0.0;

    
    for(int search = 1;search<=rangeSearched;search++){
        //Upside search
        if((importantColourFound(pCleanImage[x-search][y]))){

        }
    }

}

float calculateFitness(vector<Pixel> &initialPixels,vector<vector<Pixel>> &pCleanImage){

    for(int idx = 0; idx<initialPixels.size();idx++){
        initialPixels[idx].setFitness(individualFitness(pCleanImage,initialPixels[idx]));
    }


}


void mainGenetic(){
    vector<Pixel> initialPixels(50); //Initiliaze the vector of the initial pixels to choose

    int dimensionX = 100; //Dimensions of the images
    int dimensionY = 100;

    string imagePath = "C:/Users/luist/OneDrive/Escritorio/GeneticAlgorithms/Laberinto.png"; //Path of the image

    Mat imageFirstPopulation = imread(imagePath);
    
    vector<vector<Pixel>> imageInfo( dimensionX , vector<Pixel> (dimensionY));        //Generate initial poblation
    uploadImageInfo(imageInfo);

    vector<vector<Pixel>> cleanImage( dimensionX , vector<Pixel> (dimensionY));        //Stores the original copy of the image
    uploadImageInfo(cleanImage);

    generateInitialPixels(initialPixels,imageInfo, imageFirstPopulation);



    for(int a = 0;a<initialPixels.size();a++){
        initialPixels[a].printPixel();
    }
}