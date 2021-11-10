#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <unordered_map>
#include <map>
#include "Pixel.h"

using namespace std;
using namespace cv;

using std::cout;
using std::endl;
using std::string;

bool existsImage(Mat pColorImage){
    /*
    Purpose: 
        -Verify if an image exists or not.
    Paramateres: 
        -Receives a Mat type variable, with the selected image.
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
    
    imagePath:
        - "C:/Users/Sebastian/Desktop/TEC/IVSemestre/Analisis de algoritmos/GeneticAlgorithms/Laberinto.png"
        - "C:/Users/luist/OneDrive/Escritorio/GeneticAlgorithms/Laberinto.png"
    */
    string imagePath =  "C:/Users/Sebastian/Desktop/TEC/IVSemestre/Analisis de algoritmos/GeneticAlgorithms/Laberinto.png";
    Mat colorImage = imread(imagePath);
    int blueChannel; int greenChannel; int redChannel;
    if(existsImage(colorImage)){
        for (int imageRows = 0; imageRows < colorImage.rows; imageRows++){
            for (int imageColumns = 0; imageColumns < colorImage.cols; imageColumns++) {
                redChannel = colorImage.at<Vec3b>(imageRows, imageColumns)[2];
                greenChannel = colorImage.at<Vec3b>(imageRows, imageColumns)[1];
                blueChannel = colorImage.at<Vec3b>(imageRows, imageColumns)[0];

                Pixel addingPixel = Pixel(redChannel,greenChannel,blueChannel,imageRows,imageColumns,0.0);

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

int toBinary(int numberToConvert){
    int exp, digito;
    int binario;
    exp=0;
    binario=0;
    while(numberToConvert/2!=0){
        digito = numberToConvert % 2;
        binario = binario + digito * pow(10,exp);
        exp++;
        numberToConvert=numberToConvert/2;
    }
    binario = binario + numberToConvert * pow(10,exp);
    return binario;
}

int toDecimal(int numerToConvert){
    int exp, digit;
    int decimal;
    exp = 0; decimal = 0;
    while(((int)(numerToConvert/10))!=0){
        digit = (int)numerToConvert % 10;
        decimal = decimal + digit * pow(2.0,exp);
        exp++;
        numerToConvert = (int)(numerToConvert/10);
    }
    decimal=decimal + numerToConvert * pow(2.0,exp);

    return decimal;
}

vector<string> putTogetherChains(Pixel firstPixel, Pixel secondPixel){
    vector<string> chainsPositions(2);
    int firstXPosition, secondXPosition, firstYPosition, secondYPosition;

    firstXPosition = toBinary(firstPixel.getPositionX());
    secondXPosition = toBinary(secondPixel.getPositionX());
    firstYPosition = toBinary(firstPixel.getPositionY());
    secondYPosition = toBinary(secondPixel.getPositionY());

    chainsPositions[0] = to_string(firstXPosition) + to_string(firstYPosition);
    chainsPositions[1] = to_string(secondXPosition) + to_string(secondYPosition);
    
    int lenFirst = chainsPositions[0].length();
    int lenSecond = chainsPositions[1].length();
    int neededBits;
    int idxChanged;
    string filledChain;
    if(lenFirst<lenSecond){
        neededBits = lenSecond - lenFirst;
        filledChain = chainsPositions[0];
        idxChanged = 0;
    }
    else if (lenFirst == lenSecond){
        return chainsPositions;
    }
    else{
        neededBits =lenFirst - lenSecond;
        filledChain = chainsPositions[1];
        idxChanged = 1;
    }

    for(int filling = 0;filling<neededBits;filling++){
        filledChain = "0" + filledChain;
    }
    chainsPositions[idxChanged] = filledChain;
    return chainsPositions;
    
}


bool checkNumber(vector<int> arrayNumbers, int number){

    if(arrayNumbers.size() != 0){
        for(int index=0; index< arrayNumbers.size(); index++){
            if(arrayNumbers[index] == number){
                return false;
            }
        }
        return true;
    }else{
        return true;
    }
    
}


vector<int> generateRandoms(int size,int randomAmount){
    vector<int> arrayNumbers;
    int randomNumber;
    while (arrayNumbers.size()<randomAmount){
        random_device rd;
        default_random_engine eng(rd());
        uniform_int_distribution<int> distr(0, size);
        randomNumber = (distr(eng));
        if(checkNumber(arrayNumbers, randomNumber)){
            arrayNumbers.push_back(randomNumber);
        }
    }
    return arrayNumbers;
}