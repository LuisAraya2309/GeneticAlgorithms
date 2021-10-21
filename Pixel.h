#include <iostream>

using namespace std;

class Pixel{
    //Atributes
    int redValue, greenValue,blueValue,positionX,positionY;

    //Constructor

    public:
        Pixel(){
            this->redValue = 0;
            this->greenValue = 0;
            this->blueValue = 0;
            this->positionX = 0;
            this->positionY = 0;
        }


        Pixel(int pRedValue,int pGreenValue,int pBlueGreen,int pX,int pY){

            this->redValue = pRedValue;
            this->greenValue = pGreenValue;
            this->blueValue = pBlueGreen;
            this->positionX = pX;
            this->positionY = pY;

        }

        void printPixel(){
            cout<<"R: "<<redValue<<" ,G: "<<greenValue<<" ,B: "<<blueValue<<" ,X: "<<positionX<<" ,Y: "<<positionY<<endl;
        }

        void printPixelSegment(){
            cout<<redValue<<","<<greenValue<<","<<blueValue<<"  ";
        }

        //Setter

        void setRed( int pRed) {
        redValue= pRed;
        }

        void setGreen( int pGreen) {
        greenValue = pGreen;
        }

        void setBlue( int pBlue) {
        blueValue = pBlue;
        }

        void setPositionX(int pX){
            positionX = pX;
        }

        void setPositionY(int pY){
            positionY = pY;
        }

        // Getter

        int getRed() {
        return redValue;
        }

        int getGreen(){
            return greenValue;
        }

        int getBlue(){
            return blueValue;
        }

        int getPositionX(){
            return positionX;
        }
        int getPositionY(){
            return positionY;
        }
};