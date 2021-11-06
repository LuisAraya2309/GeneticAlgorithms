#include <stdlib.h>
#include "auxiliarFunctions.h"
#include <vector>
#include <unordered_map>
#include <map>
#include <random>
#include <opencv2/opencv.hpp>

using namespace std;

void generateInitialPixels(vector<Pixel> &initialPixels,vector<vector<Pixel>> &pImageInfo, Mat &imageFirstPopulation){
    int max = 90;
    int min = 5;
    map<int,int> xPositionsUsed;
    map<int,int> yPositionsUsed;

    for(int idx = 0;idx<initialPixels.size();idx++){
        int x , y;
        while (true){
            x = (rand() % max) + min;
            y = (rand() % max) + min;
            if((xPositionsUsed.count(x)==0) && (yPositionsUsed.count(y)==0)){
                break;
            }
        }

        //Save the first population into a vector 
        int redChannel = pImageInfo[x][y].getRed();
        int greenChannel = pImageInfo[x][y].getGreen();
        int blueChannel = pImageInfo[x][y].getBlue();
        initialPixels[idx] = Pixel(redChannel,greenChannel,blueChannel,x,y,0.0);

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

    if((redValue == 255) && (greenValue == 255) && (blueValue == 255)){     //Check if its white
        return "White";
    }
    else{
        if((redValue == 34) && (greenValue == 177) && (blueValue == 76)){ //Check if its green
            return "Green";
        }
        else{
            if((redValue == 63) && (greenValue == 72) && (blueValue == 204)){  //Check if its blue
                return "Blue";
            }
            else{    //Then its black
                return "Black";
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
        if(("White" == importantColourFound(pCleanImage[x-search][y])) || ("White" == importantColourFound(pCleanImage[x+search][y])) || ("White" == importantColourFound(pCleanImage[x][y+search])) || ("White" == importantColourFound(pCleanImage[x][y-search])) ){
            pixelFitness +=  1.0;
        }

        if(("Green" == importantColourFound(pCleanImage[x-search][y])) || ("Green" == importantColourFound(pCleanImage[x+search][y])) || ("Green" == importantColourFound(pCleanImage[x][y+search])) || ("Green" == importantColourFound(pCleanImage[x][y-search])) ){
            pixelFitness += 1.5;
        }

        if(("Blue" == importantColourFound(pCleanImage[x-search][y])) || ("Blue" == importantColourFound(pCleanImage[x+search][y])) || ("Blue" == importantColourFound(pCleanImage[x][y+search])) || ("Blue" == importantColourFound(pCleanImage[x][y-search])) ){
            pixelFitness +=  1.5;
        }
    }
    return pixelFitness;
}

void calculateFitness(vector<Pixel> &initialPixels,vector<vector<Pixel>> &pCleanImage){

    for(int idx = 0; idx<initialPixels.size();idx++){
        initialPixels[idx].setFitness(initialPixels[idx].getFitness() + individualFitness(pCleanImage,initialPixels[idx]));
        //cout<<initialPixels[idx].getPositionX()<<" , "<<initialPixels[idx].getPositionY()<<"  Fitness: "<<initialPixels[idx].getFitness()<<endl;
    }
}


vector<double> createNewPositions(vector<string> chainsPosition){
    vector<double> newPositions;
    int randomIndex, middle;
    string firstNewChain ,  secondNewChain;
    
    randomIndex = (rand() % chainsPosition[0].size());
    firstNewChain = chainsPosition[0].substr(0,randomIndex) + chainsPosition[1].substr(randomIndex) ;
    secondNewChain = chainsPosition[1].substr(0,randomIndex) + chainsPosition[0].substr(randomIndex) ;
    middle = firstNewChain.size()/2;

    cout<<chainsPosition[0].substr(0,randomIndex)<< " + " << chainsPosition[1].substr(randomIndex)<<endl;
    cout<<chainsPosition[1].substr(0,randomIndex)<< " + " << chainsPosition[0].substr(randomIndex)<<endl;

    //Add new positions to vector
    newPositions.push_back(toDecimal(std::stod(firstNewChain.substr(0,middle))));
    newPositions.push_back(toDecimal(std::stod(firstNewChain.substr(middle))));
    newPositions.push_back(toDecimal(std::stod(secondNewChain.substr(0,middle))));
    newPositions.push_back(toDecimal(std::stod(secondNewChain.substr(middle))));

    for (int i = 0; i<newPositions.size(); i++){
        cout<<"Numero decimal: "<<newPositions[i]<<endl;
    }
    return newPositions;
}

void crossPixels(vector<Pixel> &bestPixels){
    vector<string> chainsPosition(2);
    for(int idx = 0; idx<bestPixels.size();idx+=2){
        chainsPosition = putTogetherChains(bestPixels[idx], bestPixels[idx + 1]);
        createNewPositions(chainsPosition);
    }
}

float maxFitness(vector<Pixel> &initialPixels){
    float r = 0;
    r = initialPixels[0].getFitness();
    for (int i = 0; i < 5; i++) {
        if (r < initialPixels[i].getFitness())   {

            r = initialPixels[i].getFitness();
        }

    }
    return r;
}




vector<Pixel> chooseBestPixels(vector<Pixel> &initialPixels){

    vector<Pixel> bestPixels; //Initiliaze the vector of the best pixels to choose
    int pixelsChosen = 0;
    int idx = 0;
    float topFitness = maxFitness(initialPixels);
    while (pixelsChosen!=10){
        float pixelFitness = initialPixels[idx].getFitness();
        float chosenProbability = (pixelFitness*100)/topFitness;
        float randomChoice = rand() % 100;
        if(randomChoice<=chosenProbability && randomChoice!=0.0){
            bestPixels.push_back(initialPixels[idx]);
            pixelsChosen++;
        }

        if(idx+1 == initialPixels.size()){
            idx = 0;
        }
        else{
            idx++;
        }
            
        
    }

    return bestPixels;
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

    generateInitialPixels(initialPixels,imageInfo, imageFirstPopulation);    //Generate the initial pixels for the array

    calculateFitness(initialPixels,cleanImage);                             //Calculates the fitness of the initial poblation

    vector<Pixel> bestPixels = chooseBestPixels(initialPixels);           //Choose the best pixels.
    /*
    for(int a = 0;a<bestPixels.size();a++){
        cout<<"Fitness: " <<bestPixels[a].getFitness()<<endl;
    }
    */

    cout<<"BEST PIXELSSSSSSSS"<<endl;
    



}