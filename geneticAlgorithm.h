#include <stdlib.h>
#include "auxiliarFunctions.h"
#include <vector>
#include <unordered_map>
#include <map>
#include <random>
#include <opencv2/opencv.hpp>

using namespace std;

void generateInitialPixels(vector<Pixel> &initialPixels,vector<vector<Pixel>> &pImageInfo, Mat &image){
    int maxX = 100;
    int maxY = 100;
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
        image.at<Vec3b>(x,y) = Vec3b(20,0,180);
        imshow("windowName",image);
    }
}

void mainGenetic(){
    vector<Pixel> initialPixels(50);
    int dimensionX = 100;
    int dimensionY = 100;
    string imagePath = "C:/Users/Sebastian/Desktop/TEC/IVSemestre/Analisis de algoritmos/GeneticAlgorithms/Laberinto.png";
    Mat image = imread(imagePath);
    
    vector<vector<Pixel>> imageInfo( dimensionX , vector<Pixel> (dimensionY));
    uploadImageInfo(imageInfo);

    generateInitialPixels(initialPixels,imageInfo, image);
    for(int a = 0;a<initialPixels.size();a++){
        initialPixels[a].printPixel();
    }
}