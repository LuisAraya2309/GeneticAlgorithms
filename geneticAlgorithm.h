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
            random_device rd;
            default_random_engine eng(rd());
            uniform_int_distribution<int> distr(5, 94);
            x = distr(eng);
            y = distr(eng);

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


    float pixelFitness = 0.0;

    
    for(int search = 1;search<=rangeSearched;search++){
        //Upside search
        if(("White" == importantColourFound(pCleanImage[x-search][y])) || ("White" == importantColourFound(pCleanImage[x+search][y])) || ("White" == importantColourFound(pCleanImage[x][y+search])) || ("White" == importantColourFound(pCleanImage[x][y-search])) ){
            pixelFitness +=  1.5;
        }

        if(("Green" == importantColourFound(pCleanImage[x-search][y])) || ("Green" == importantColourFound(pCleanImage[x+search][y])) || ("Green" == importantColourFound(pCleanImage[x][y+search])) || ("Green" == importantColourFound(pCleanImage[x][y-search])) ){
            pixelFitness +=  4.0;
            break;
        }

        if(("Blue" == importantColourFound(pCleanImage[x-search][y])) || ("Blue" == importantColourFound(pCleanImage[x+search][y])) || ("Blue" == importantColourFound(pCleanImage[x][y+search])) || ("Blue" == importantColourFound(pCleanImage[x][y-search])) ){
            pixelFitness +=  4.0;
            break;
        }

        if(("Black" == importantColourFound(pCleanImage[x-search][y])) || ("Black" == importantColourFound(pCleanImage[x+search][y])) || ("Black" == importantColourFound(pCleanImage[x][y+search])) || ("Black" == importantColourFound(pCleanImage[x][y-search])) ){
            pixelFitness -=  0.2;
            break;
        }

        pixelFitness += 0.3;

    }
    return pixelFitness;
}

void calculateFitness(vector<Pixel> &initialPixels,vector<vector<Pixel>> &pCleanImage){

    for(int idx = 0; idx<initialPixels.size();idx++){
        initialPixels[idx].setFitness(individualFitness(pCleanImage,initialPixels[idx]));
        //cout<<initialPixels[idx].getPositionX()<<" , "<<initialPixels[idx].getPositionY()<<"  Fitness: "<<initialPixels[idx].getFitness()<<endl;
    }
}


string mutation(string positionChain){
    vector<int> randomNumbers = generateRandoms(positionChain.size()-1,3);
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

vector<int> adjustNewPositions(string disAdjustedPositions[]){
    vector<int> adjustedPositions;
    random_device rd;
    default_random_engine eng(rd());
    uniform_int_distribution<int> distr(5, 94);
    for(int idx = 0;idx<4;idx++){
        int actualPosition = toDecimal(std::stoi(disAdjustedPositions[idx]));
        if((actualPosition - 5) <= 5){
            actualPosition= distr(eng);
        }
        else if ((actualPosition + 5) >= 94){
            actualPosition=distr(eng);
        }
        adjustedPositions.push_back(actualPosition);
    }
    return adjustedPositions;
}


vector<int> createNewPositions(vector<string> chainsPosition){
    vector<int> newPositions , adjustedPositions;
    int randomIndex, middle;
    string firstNewChain ,  secondNewChain, FNCbegin , FNCend, SNCbegin,SNCend;
    random_device rd;
    default_random_engine eng(rd());
    uniform_int_distribution<int> distr(2, chainsPosition[0].size()-1);
    randomIndex = (distr(eng));

    
    firstNewChain = chainsPosition[0].substr(0,randomIndex) + chainsPosition[1].substr(randomIndex) ;
    secondNewChain = chainsPosition[1].substr(0,randomIndex) + chainsPosition[0].substr(randomIndex) ;
    middle = firstNewChain.size()/2;


    FNCbegin = firstNewChain.substr(0,middle);
    FNCend = firstNewChain.substr(middle);
    SNCbegin = secondNewChain.substr(0,middle);
    SNCend = secondNewChain.substr(middle);

    int probabilityNumber = (distr(eng) % 100);
    if(probabilityNumber<=10){
        FNCbegin = mutation(FNCbegin);
        FNCend = mutation(FNCend);
        SNCbegin = mutation(SNCbegin);
        SNCend = mutation(SNCend);
    }

    string disAdjustedPositions[4] = {FNCbegin,FNCend,SNCbegin,SNCend};
    adjustedPositions = adjustNewPositions(disAdjustedPositions);

    /*
    cout<<"--------------Pareja--------------"<<endl;
    cout<<"Entra cadena1: "<<chainsPosition[0]<<endl;
    cout<<"Corta en: "<<randomIndex<<endl;
    cout<<"Sale cadena1: "<<FNCbegin+FNCend<<endl;
    cout<<"Salida en decimal: "<<adjustedPositions[0]<<" , "<<adjustedPositions[1]<<endl;
    cout<<endl;
    cout<<"Entra cadena2: "<<chainsPosition[1]<<endl;
    cout<<"Corta en: "<<randomIndex<<endl;
    cout<<"Sale cadena2: "<<SNCbegin+SNCend<<endl;
    cout<<"Salida en decimal: "<<adjustedPositions[2]<<" , "<<adjustedPositions[3]<<endl;
    cout<<endl;
    /*
    cout<<"NEW POSITIONS"<<endl;
    for(int z =0;z<4;z++){
        cout<<adjustedPositions[z]<<endl;
    }*/
    
    return adjustedPositions;
}

vector<int> squareCross(Pixel firstPixel , Pixel secondPixel){

    vector<int> newPositions;

    int x1 = firstPixel.getPositionX();
    int y1 = firstPixel.getPositionY();
    int x2 = secondPixel.getPositionX();
    int y2 = secondPixel.getPositionY();
    int newX1, newX2, newY1, newY2;

    random_device rd;
    default_random_engine eng(rd());
    
    if(x1 < x2){
        uniform_int_distribution<int> distr(x1, x2);
        newX1 = distr(eng); 
        newX2 = distr(eng); 

    }else{
        
        uniform_int_distribution<int> distr(x2, x1);
        newX1 = distr(eng); 
        newX2 = distr(eng); 

    }
    
    random_device rd2;
    default_random_engine eng2(rd2());

    if(y1 < y2){
        uniform_int_distribution<int> distr2(y1, y2);
        newY1 = distr2(eng2);
        newY2 = distr2(eng2);
    }else{
        uniform_int_distribution<int> distr2(y2, y1);
        newY1 = distr2(eng2);
        newY2 = distr2(eng2);
    
    }


    newPositions.push_back(newX1);
    newPositions.push_back(newY1);
    newPositions.push_back(newX2);
    newPositions.push_back(newY2);

    return newPositions;

}


vector<Pixel> crossPixels(vector<Pixel> &bestPixels, Mat &imageFirstPopulation ){
    vector<string> chainsPosition;
    vector<int> newPositions;
    vector<Pixel> newPopulation;

    vector<int> randomIndexes = generateRandoms(9,10);

    random_device rd;
    default_random_engine eng(rd());
    uniform_int_distribution<int> distr(0,1);
    
    for(int idx = 0; idx<bestPixels.size();idx+=2){

       if(distr(eng) == 1){
            chainsPosition = putTogetherChains(bestPixels[randomIndexes[idx]], bestPixels[randomIndexes[idx+1]]);
            newPositions = createNewPositions(chainsPosition);
        }else{
            newPositions = squareCross(bestPixels[randomIndexes[idx]], bestPixels[randomIndexes[idx+1]]);
        }
       
        //Create an array of new pixels
        newPopulation.push_back(Pixel(180,0,20,newPositions[0],newPositions[1],0));
        newPopulation.push_back(Pixel(180,0,20,newPositions[2],newPositions[3],0));
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
        random_device rd;
        default_random_engine eng(rd());
        uniform_int_distribution<int> distr(1, 100);
        float randomChoice = distr(eng);
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

void replaceFirstPopulation(vector<Pixel> &initialPixels, vector<Pixel> &newPixelPopulation ){

    sort(initialPixels.begin(), initialPixels.end(), Pixel :: compareByFitness);

    /*for(int index=0; index <10; index++){
        initialPixels.erase(initialPixels.begin() + index);
    }
    for(int index=0; index <10; index++){
        initialPixels.insert(initialPixels.begin(),newPixelPopulation[index]);
    }*/
    for(int index=0; index <10; index++){
        initialPixels.erase(initialPixels.begin() + index);
        initialPixels.push_back(newPixelPopulation[index]);

    }
    
}

double averageFitness(vector<Pixel> poblation){
    double totalFitness = 0.0;
    for(int idx = 0;idx<poblation.size();idx++){
        totalFitness+=poblation[idx].getFitness();
    }
    return totalFitness/poblation.size();
}


void mainGenetic(){
    vector<Pixel> initialPixels(200); //Initiliaze the vector of the initial pixels to choose

    int dimensionX = 100; //Dimensions of the images
    int dimensionY = 100;

    string imagePath = "C:/Users/Sebastian/Desktop/TEC/IVSemestre/Analisis de algoritmos/GeneticAlgorithms/Laberinto.png"; //Path of the image

    Mat imageFirstPopulation = imread(imagePath);
    Mat draftImage = imageFirstPopulation;

    vector<vector<Pixel>> imageInfo( dimensionX , vector<Pixel> (dimensionY));        //Generate initial poblation
    uploadImageInfo(imageInfo);

    vector<vector<Pixel>> cleanImage( dimensionX , vector<Pixel> (dimensionY));        //Stores the original copy of the image
    uploadImageInfo(cleanImage);

    generateInitialPixels(initialPixels,imageInfo, imageFirstPopulation);    //Generate the initial pixels for the array

    int generation = 0;
    double avgFitness = 0.0;
    while(avgFitness<=8){
    //for(int z =0;z!=50;z++){
        calculateFitness(initialPixels,cleanImage);         //Calculates the fitness of the initial poblation                          

        vector<Pixel> bestPixels = chooseBestPixels(initialPixels);           //Choose the best pixels.
        
        Mat newImage = draftImage;
        vector<Pixel> newPixelPopulation = crossPixels(bestPixels, newImage);
        replaceFirstPopulation(initialPixels, newPixelPopulation);
        avgFitness = averageFitness(initialPixels);
        generation++;
        
    //}
    }
    cout<<"Resultado final"<<endl;
    Mat finalImage = imread(imagePath);
    cout<<"Generacion "<<generation<<endl;
    cout<<"AVG FITNESS: "<<avgFitness<<endl;

    for(int idx = 0;idx<initialPixels.size();idx++){
        Pixel p = initialPixels[idx];
        //cout<<"Posicion en X "<<p.getPositionX()<<" - Posicion en Y "<<p.getPositionY()<<endl;
        finalImage.at<Vec3b>(p.getPositionX(),p.getPositionY()) = Vec3b(20,0,180);
    }
    imshow("Final",finalImage);

}