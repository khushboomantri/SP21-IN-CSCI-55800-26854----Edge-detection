// Khushboo Mantri 
// multimedia
// this code I have done on c++ because I found it to be little easy in c++


// include header files of c++
// input file
// apply sobel function
// choose hight intensdity
//threshhold
// output 

#include<iostream>
#include<cstdio>
#include<fstream>
#include<vector>
// height and width of the image
int width = 512;
int height = 512;
using namespace std;

// I have used vecorts because cpp supports vector and Vectors are sequence containers representing arrays that can change in size.
//The Sobel Operator is a discrete differentiation operator. It computes an approximation of the gradient of an image intensity function.
// The Sobel Operator combines Gaussian smoothing and differentiation.
//. Used unsigned char, because of 0 to 255 range 

std::vector <unsigned char> Sobel_Filter_Application(vector<vector<unsigned char>> image, vector<vector<char>> Sobel_Filter, int maximum_value)
{

    int offset = Sobel_Filter.size()/2;

    int width_height = image[0].size();
    vector<unsigned char> output_image;
    // I am going to use a linear array, since that is easier to write to file
    output_image.resize(width_height*width_height);

    for(int i = 0; i < width_height; ++i){

        for(int j = 0; j < width_height; ++j){
            // for each pixel, get surrounding piels and find the wieghted sum
            int result = 0; 
            for (int k = 0; k < Sobel_Filter.size(); ++k){
                for (int l = 0; l < Sobel_Filter[k].size(); ++l){

                    // the center pixel gets multiplied by Sobel_Filter[1][1], so check if the bounds are fine
                    if ((i + k  - offset> 0 )&& (i + k - offset < width_height)){
                        if ((l + j  - offset> 0 )&&( i + k - offset < width_height)){

                            result += image[i - offset + k][j - offset + l]*Sobel_Filter[k][l];

                        }

                    }
                }
            }
            int positive_result = (result*255)/maximum_value;
            if (positive_result < 0)
                positive_result = -positive_result;
            output_image[i*width_height + j] = positive_result; 
        }
    }
    return output_image;
}
std::vector<unsigned char> HighIntensity(vector<unsigned char> first, vector<unsigned char> second){
    int size = first.size();
    if (second.size() < size)
        size = second.size();
    // define result
    vector<unsigned char> result;
    result.resize(size);

    for (int i = 0; i < size; ++i){
        if (first[i] > second[i])
            result[i] = first[i];
        else
            result[i] = second[i];

    }
    return result;
}
// takes any number above a threshhold (in the array) and sets it to 255
// in this case threshold is 30 so it wil change number above 30 to 255
std :: vector<unsigned char> Raise_Threshold(vector<unsigned char> input, unsigned char thresh){

    vector<unsigned char> output;
    int size = input.size();
     output.resize(size);
    for (int i = 0; i < size; ++i){
        if (input[i] > thresh)
            output[i] = 255;
        else 
            output[i] = input[i];
    }
 return output;
}
void OUTPUT_FILE(vector<unsigned char> image, const char * filename )
{   // 3 because of rgb
    int color = 3*image.size();
    std::vector<unsigned char>color_Output(color, 0);
    for(int i = 0; i <  color; ++i){
       color_Output[i] = image[i/3];
    }

    //typedef std::basic_ofstream<char> std::ofstream
    //static const std::ios_base::openmode std::ios_base::binary = (std::ios_base::openmode)
    //Perform input and output in binary mode (as opposed to text mode).
    std::ofstream output(filename, ios::binary);
    if (output.is_open()){
        output.write((char*)color_Output.data(),color_Output.size());
        output.close();
    }
    else
       std:: cerr << "\n output file not created\n";


}

// Main Function
//task 1 : Read the image
//task2 : Sobel Operator initialize 

int main(){
//Reading the image
const char * Input_File = "test.raw";
    ifstream input_image (Input_File, ios::binary);
    // Size of the image
    int width_height = 512;
    //bool std::ifstream::is_open()
    if (input_image.is_open())
    {   
        //seekg() function is used to move/sets the get pointer at the desired/particular position to getting/reading data from a file
        input_image.seekg(0, input_image.end);
        //tellg() functionis used to  get the current position of pointer from the file.
        int image_size= input_image.tellg();
        input_image.seekg(0, input_image.beg);
        vector<vector<unsigned char>> image_matrix;
        // It will resize matrix and image dimension accordingly
        image_matrix.resize(width_height);

       for(int i = 0; i < width_height; ++i)
        {   //void std::vector<unsigned char>::resize(std::size_t __new_size)
            image_matrix[i].resize(width_height);
            //std::istream &std::istream::read(char *__s, std::streamsize __n)
            input_image.read((char *)image_matrix[i].data(),width_height);
        }
        input_image.close();
        // tried different values 
        // according to me 30 works best
        unsigned char Threshold_Value = 30;

        // initialize Sobel filter
       std:: vector<vector<char>> Sobel_Filter;
        Sobel_Filter.resize(3);
        //3*3 horizontal
        Sobel_Filter[0].assign ( { 1, 0, -1  });
        Sobel_Filter[1].assign({ 2, 0, -2  });
        Sobel_Filter[2].assign({ 1, 0, -1  });
        // Sobel_Filter[0].assign({ 1, 2, 1  });
        // Sobel_Filter[1].assign({ 0, 0, 0  });
        // Sobel_Filter[2].assign({ -1, -2, -1  });
        int max = (4)*255;
        // call to function
        vector<unsigned char> horizontal = Sobel_Filter_Application(image_matrix, Sobel_Filter, max);
        // g3*3 vertical 
        Sobel_Filter[0].assign({ 1, 2, 1  });
        Sobel_Filter[1].assign({ 0, 0, 0  });
        Sobel_Filter[2].assign({ -1, -2, -1  });

        vector<unsigned char>vertical = Sobel_Filter_Application(image_matrix, Sobel_Filter, max);
        vector<unsigned char> edge = HighIntensity(horizontal,vertical);
       OUTPUT_FILE(edge, "33gradient.raw");
        edge = Raise_Threshold(edge, Threshold_Value);
       OUTPUT_FILE(edge, "33edge.raw");


        // generate the5x5 vertical operator
        Sobel_Filter.resize(5);
        Sobel_Filter[0].assign({ 1, 2, 4, 2,  1  });
        Sobel_Filter[1].assign({ 2, 4, 8, 4, 2  });
        Sobel_Filter[2].assign({0, 0, 0, 0 ,0 });
        Sobel_Filter[3].assign({ -2, -4, -8, -4, -2  });
        Sobel_Filter[4].assign({ -1, -2, -4, -2,  -1  });
            // Sobel_Filter.resize(5);
            // Sobel_Filter[0].assign({ 1, 2, 0, -2,  -1  });
            // Sobel_Filter[1].assign({ 2, 4, 0, -4, -2  });
            // Sobel_Filter[2].assign({4, 8, 0, -8 ,-4 });
            // Sobel_Filter[3].assign({ 2, 4, 0, -4, -2  });
            // Sobel_Filter[4].assign({ 1, 2, 0, -2,  -1  });


        max = 0;
        for (int i = 0; i < Sobel_Filter.size(); ++i){
            max += (Sobel_Filter[0][i] + Sobel_Filter[1][i])*255; 
        }
       vertical = Sobel_Filter_Application(image_matrix, Sobel_Filter, max);
            
        Sobel_Filter.resize(5);
            Sobel_Filter[0].assign({ 1, 2, 0, -2,  -1  });
            Sobel_Filter[1].assign({ 2, 4, 0, -4, -2  });
            Sobel_Filter[2].assign({4, 8, 0, -8 ,-4 });
            Sobel_Filter[3].assign({ 2, 4, 0, -4, -2  });
            Sobel_Filter[4].assign({ 1, 2, 0, -2,  -1  });
            horizontal= Sobel_Filter_Application(image_matrix, Sobel_Filter, max);

            edge = HighIntensity(horizontal,vertical);

           OUTPUT_FILE(edge, "55gradient.raw");
            edge = Raise_Threshold(edge, Threshold_Value);
           OUTPUT_FILE(edge, "55edge.raw");
    }
    else {
        std::cerr << "\n  Not able to opean input file \n ";
    }
    
}


