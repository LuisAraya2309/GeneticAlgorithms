#include <iostream>
#include "Pixel.h"
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;

class Generation{
    //Atributes
    int generationNumber;
    vector<Pixel> pixelList;
    float averageFitness; 

    //Constructor 

    public:

        Generation(int pGenerationNumber, vector<Pixel> pPixelList, float pAverageFitness){

            this->generationNumber = pGenerationNumber;
            this->pixelList = pPixelList;
            this->averageFitness = pAverageFitness;
        }

        //Setter

        void setGenerationN( int pGenerationNumber) {
            generationNumber= pGenerationNumber;
        }

        void setPixelList( vector<Pixel> pPixelList) {
            pixelList = pPixelList;
        }

        void setAverageFitness(float pAverageFitness){
            averageFitness = pAverageFitness;
        }

        // Getter

       int getGenerationN() {
            return generationNumber;
        }

        vector<Pixel> getPixelList() {
            return pixelList ;
        }

        float getAverageFitness(){
            return averageFitness;
        }

};