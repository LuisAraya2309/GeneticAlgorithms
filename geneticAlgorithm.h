#include <stdlib.h>
#include "auxiliarFunctions.h"
#include <vector>
#include <unordered_map>
#include <map>
#include <random>
#include <opencv2/opencv.hpp>

using namespace std;

void generateInitialPixels(vector<Pixel> &initialPixels,vector<vector<Pixel>> &pImageInfo){
    int maxX = 63;
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
        int redChannel = pImageInfo[x][y].getRed();
        int greenChannel = pImageInfo[x][y].getGreen();
        int blueChannel = pImageInfo[x][y].getBlue();
        initialPixels[idx] = Pixel(redChannel,greenChannel,blueChannel,x,y);
    }

}

void mainGenetic(){
    vector<Pixel> initialPixels(50);
    int dimensionX = 63;
    int dimensionY = 100;
    
    vector<vector<Pixel>> imageInfo( dimensionX , vector<Pixel> (dimensionY));
    uploadImageInfo(imageInfo);

    generateInitialPixels(initialPixels,imageInfo);
    for(int a = 0;a<initialPixels.size();a++){
        initialPixels[a].printPixel();
    }

}