/*
 Image convolution file using C++.
 Assuimption: This program assumes the following PGM file header order:
 
 Line 1: Type
 Line 2: Comment
 Line 3: rows columns
 Line 4: Highest Pixel
 
 Rest of the file: image data.
 
 */
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
using namespace std;

int main(int argc, char* argv[])
{
    ifstream infile;
    ofstream outfile;
    string line, type, maxpx, comment;
    string imagepath = "path/to/file.pgm";
    string outputimage = "path/to/output.pgm";
    int cols, rows;
    const int KERNEL_SIZE = 3;
    
    infile.open(imagepath, ios::binary | ios::in);
    outfile.open(outputimage, ios::binary | ios::out);
    
    getline(infile, type);  //Read first line and grab the type of file
    getline(infile, comment);  //Read second line and discard it
    
    infile >> rows >> cols; //Read third line and get columns and rows
    
    getline(infile, line);
    getline(infile, maxpx);  //Read highest pixel
    
    vector<vector<uint8_t>> imageArray(rows, vector<uint8_t>(cols, '\0'));
    vector<vector<uint8_t>> outputArray(rows, vector<uint8_t>(cols, '\0'));
    vector<vector<int>> kernel(KERNEL_SIZE, vector<int>(KERNEL_SIZE, 0));
    //vector<vector<int>> kernel1 = { { 0, -1, 0 }, { -1, 5, -1 }, { 0, -1, 0 } };
    //vector<vector<int>> kernel1 = { { 0, -1, 0 }, { -1, 5, -1 }, { 0, -1, 0 } }; //Sharpen
    //vector<vector<int>> kernel1 = { { -1, -1, 0 }, { -1, 0, 1 }, { 0, 1, 1 } };
    
    //Generate random integers to fill up Kernel
    srand((unsigned int)time(NULL));
    for (int i = 0; i < KERNEL_SIZE; i++)
        for (int j = 0; j < KERNEL_SIZE; j++)
            kernel[i][j] = rand() % 2;
    
    /*
     Insert image data into image array.
     make sure not to use infile >> imageArray[i][j] to read data into imageArray,
     for some reasons, ifstream >> is interpreting values close to 255 as a newline or some other special chars.
     If you reduce level of image to range somewhere between 0 and 190, default >> works.
     To avoid this, use read() method from ifstream class.
     
     Phew!!
     */
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            infile.read((char*)&imageArray[i][j],1);
    
    infile.close();
    
    //Insert default header attributes into output pgm file.
    outfile << type << "\n" << comment << "\n" << rows << " " << cols << "\n" << maxpx << "\n";
    
    //Image convolution
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            uint8_t sum = '\0';
            for (int k = 0; k < KERNEL_SIZE ; k++)
            {
                for (int l = 0; l < KERNEL_SIZE; l++)
                {
                    sum += imageArray[i][j] * kernel[k][l];
                }
            }
            outputArray[i][j] = sum;
        }
    }
    
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            outfile << outputArray[i][j];
    
    outfile.close();
    
    return 0;
}
