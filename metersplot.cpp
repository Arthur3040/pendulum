#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdio> // delete the finaldata.txt to "reset data"
#include <vector>
#include <math.h>

using namespace std;

int main (){

    remove("data/pendulumfinaldata.txt");
    float m = 0.169, l = 0.67, degree = 11, amp;
    amp = l * sin(degree);


    ifstream datatxt;
    //start from zero so colum "1" is 0. "2" is 1...
    int column, qtd=0, editcolum = 1; 
    double token, max, min;
    datatxt.open("data/pendulumdata.txt");
    string line;
    //since i know this code is just for two collons, i will iniciate the vars manually;
    while(qtd < 50){
        datatxt >> max;
        qtd++;
    }
    qtd=0;
    min = max;
    
    //transformar numa função;
    while (std::getline(datatxt, line)){
        istringstream strline (line);
        column = 0;

        while (strline >> token){ //by default its search " "
            if (column == editcolum){
                if(min > token)
                    min = token;
                else if(max < token)
                    max = token;
                cout << "coluna " << editcolum+1 <<" "<< qtd <<" " << token << endl;
                break;
            }
            column++;
            qtd++;
        }
    
    }
    datatxt.close();
    //start again
    fstream fout;
    datatxt.open("data/pendulumdata.txt");
    fout.open("data/pendulumfinaldata.txt", ios::out | ios::app);
    qtd = 0;
    while (std::getline(datatxt, line)){
        istringstream strline (line);
        column = 0;

        while (strline >> token){ //by default its search " "
            if (column == editcolum){
                //tansform to meters
                token -= (max + min)/2;
                token *= 2/(max - min);
                //amplitude = sin(maxdegree)*L (Line size)
                token *= amp;
                cout << "coluna " << editcolum+1 <<" "<< qtd <<" " << token << endl;
            }
            if(qtd%2 != 0)
                fout << token << endl;
            else
                fout << token << " ";
            column++;
            qtd++;
        }
    
    }
    datatxt.close();
    fout.close();

    system("gnuplot <gnuplot.ps");
    ifstream leitorBW;
    leitorBW.open("data/curvefitting.txt");

    double b, w;
    leitorBW >> b;
    leitorBW >> w;
    leitorBW.close();

    ofstream quality;
    quality.open("data/qualityfactor.txt");
    double Q = sqrt((w*w +b*b)/2*b);
    quality << Q << endl;
    quality.close();
    return 0;
}


