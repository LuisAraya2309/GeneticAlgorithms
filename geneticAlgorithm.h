#include <stdlib.h>
#include "auxiliarFunctions.h"
#include <vector>
#include <unordered_map>
#include <map>
#include <random>
#include <opencv2/opencv.hpp>
#include <iostream>

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
    }
    imshow("windowName",imageFirstPopulation);
    waitKey(0);
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

    cout<<"PosicionX a buscar: "<<x<<endl;
    cout<<"PosicionY a buscar: "<<y<<endl;

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
        cout<<"Pixel actual: "<<idx<<endl;
        initialPixels[idx].setFitness(individualFitness(pCleanImage,initialPixels[idx]));
        //cout<<initialPixels[idx].getPositionX()<<" , "<<initialPixels[idx].getPositionY()<<"  Fitness: "<<initialPixels[idx].getFitness()<<endl;
    }
}


string mutation(string positionChain){
    vector<int> randomNumbers = generateRandoms(positionChain.size());
    string newPositionChain;
    for(int index=0; index<randomNumbers.size();index++){

        if((positionChain[randomNumbers[index]]) == '0'){
            if(randomNumbers[index]==0){
                newPositionChain = '1'+ positionChain.substr(1);
            }else{
                newPositionChain = positionChain.substr(0,randomNumbers[index]) + '1' + positionChain.substr(randomNumbers[index]+1);
            }
        
        }else{
            if(randomNumbers[index] == 0){
                newPositionChain = '0'+ positionChain.substr(1);
            }else{
                newPositionChain = positionChain.substr(0,randomNumbers[index]) + '0' + positionChain.substr(randomNumbers[index]+1);
            }
        }
        positionChain = newPositionChain;
    }
    return positionChain;
    

}

vector<double> createNewPositions(vector<string> chainsPosition){
    vector<double> newPositions;
    int randomIndex, middle;
    string firstNewChain ,  secondNewChain;
    
    randomIndex = (rand() % chainsPosition[0].size());
    firstNewChain = chainsPosition[0].substr(0,randomIndex) + chainsPosition[1].substr(randomIndex) ;
    secondNewChain = chainsPosition[1].substr(0,randomIndex) + chainsPosition[0].substr(randomIndex) ;
    middle = firstNewChain.size()/2;

    int probabilityNumber = (rand() % 100 + 1);
    if(probabilityNumber<=5){
        //Add new positions to vector
        newPositions.push_back(toDecimal(std::stod(mutation(firstNewChain.substr(0,middle)))));
        newPositions.push_back(toDecimal(std::stod(mutation(firstNewChain.substr(middle)))));
        newPositions.push_back(toDecimal(std::stod(mutation(secondNewChain.substr(0,middle)))));
        newPositions.push_back(toDecimal(std::stod(mutation(secondNewChain.substr(middle)))));

    }else{
        //Add new positions to vector
        newPositions.push_back(toDecimal(std::stod(firstNewChain.substr(0,middle))));
        newPositions.push_back(toDecimal(std::stod(firstNewChain.substr(middle))));
        newPositions.push_back(toDecimal(std::stod(secondNewChain.substr(0,middle))));
        newPositions.push_back(toDecimal(std::stod(secondNewChain.substr(middle))));
    }


    return newPositions;
}

vector<Pixel> crossPixels(vector<Pixel> &bestPixels, Mat &imageFirstPopulation ){
    vector<string> chainsPosition(2);
    vector<double> newPositions(4);
    vector<Pixel> newPopulation;
    for(int idx = 0; idx<bestPixels.size();idx+=2){
        chainsPosition = putTogetherChains(bestPixels[idx], bestPixels[idx + 1]);
        newPositions = createNewPositions(chainsPosition);

        //Create an array of new pixels
        newPopulation.push_back(Pixel(180,0,20,newPositions[0],newPositions[1],0));
        newPopulation.push_back(Pixel(180,0,20,newPositions[2],newPositions[3],0));

        imageFirstPopulation.at<Vec3b>(newPositions[0],newPositions[1]) = Vec3b(20,0,180);
        imageFirstPopulation.at<Vec3b>(newPositions[2],newPositions[3]) = Vec3b(20,0,180);
        imshow("windowName",imageFirstPopulation);
    }
    return newPopulation;
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

void replaceFirstPopulation(vector<Pixel> &initialPixels, vector<Pixel> newPixelPopulation ){

    sort(initialPixels.begin(), initialPixels.end(), Pixel :: compareByFitness);
    for(int index=0; index <10; index++){
        initialPixels.erase(initialPixels.begin() + index);
        initialPixels.push_back(newPixelPopulation[index]);
    }
}


void mainGenetic(){
    vector<Pixel> initialPixels(50); //Initiliaze the vector of the initial pixels to choose

    int dimensionX = 100; //Dimensions of the images
    int dimensionY = 100;

    string imagePath = "C:/Users/Sebastian/Desktop/TEC/IVSemestre/Analisis de algoritmos/GeneticAlgorithms/Laberinto.png"; //Path of the image

    Mat imageFirstPopulation = imread(imagePath);

    vector<vector<Pixel>> imageInfo( dimensionX , vector<Pixel> (dimensionY));        //Generate initial poblation
    uploadImageInfo(imageInfo);

    vector<vector<Pixel>> cleanImage( dimensionX , vector<Pixel> (dimensionY));        //Stores the original copy of the image
    uploadImageInfo(cleanImage);

    generateInitialPixels(initialPixels,imageInfo, imageFirstPopulation);    //Generate the initial pixels for the array

    for (int index = 0; index<10; index++){

        calculateFitness(initialPixels,cleanImage);
        cout<<"Buscando error fitness"<<endl;                             //Calculates the fitness of the initial poblation

        vector<Pixel> bestPixels = chooseBestPixels(initialPixels);           //Choose the best pixels.

        vector<Pixel> newPixelPopulation = crossPixels(bestPixels, imageFirstPopulation);

        replaceFirstPopulation(initialPixels, newPixelPopulation);
        cout<<"Buscando error"<<endl;
    }
    
}