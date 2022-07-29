#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <sstream>
#include <math.h>

void ReadFile(std::string file_name, std::vector<std::vector<double>> * image_data){
    // Opens the file for reading
    std::ifstream file(file_name);

    // Creates a string to hold each line in temporarily
    std::string str;

    // Iterates over the file, storing one line at a time into `str`
    while (std::getline(file, str)) {
        // Create a temporary 1D Vector of doubles
       std::vector<double> new_row;

       // Create a stringstream object with our line of integers from the file
       std::istringstream ss(str);

       // Create a double that will hold our extracted value from the string
       double token;

        // While there are still numbers in this string, extract them as doubles
        while(ss >> token){
            // Push these doubles into our temp vector
            new_row.push_back(token);
        }

        // The line is empty, push our completed row into our 2D vector
        image_data->push_back(new_row);
    }
}
void WriteFile(std::string file_name, std::vector<std::vector<double>> * const image_data){
    // Open our file with write permissions
    std::ofstream output_file(file_name);

    // Use an iterator to traverse the outer dimension
    for(std::vector<std::vector<double>>::const_iterator i = (*image_data).begin(); i != (*image_data).end(); ++i){
        // Use an interator to traverse the inner dimention
        for(std::vector<double>::const_iterator j = i->begin(); j != i->end(); ++j){
            // Output each element to the file with a space at the end
            output_file << *j << " ";
        }
        // Output a newline symbol to the file
        output_file << "\n";
    }
}
int main(int argc, char** argv){
    std::string type = std::string(argv[1]);
    std::string in_fname = std::string(argv[2]);
    std::string out_fname = std::string(argv[3]);
     // extracts the 6

    std::vector<std::vector<double>> im_data;
    std::vector<std::vector<double>> out_data;
    std::vector<double> im_dataL;

    //int size;
    //std::string type, in_fname, out_fname;
    //std::cin >> type >> in_fname >> out_fname;
    //if (type == "local"){
        //std::cin >> size;
    //}
    ReadFile(in_fname, &im_data);
    ReadFile(in_fname, &out_data);

    //from 2D to 1D
    for (unsigned int i = 0; i < im_data.size(); i++)
        for (unsigned int j = 0; j < im_data[0].size(); j++)
            im_dataL.push_back(im_data[i][j]);

    sort(im_dataL.begin(), im_dataL.end());

    if (type == "global"){
        double median, T;
        // pair number of element median
        if (im_dataL.size() % 2 == 0)
            median = ((im_dataL[((im_dataL.size() - 1) / 2)] + im_dataL[((im_dataL.size() + 1) / 2)]) / 2);
        // unpair number of element median
        else {
            median = im_dataL[im_dataL.size() / 2];
        }
        T = median;
        //pich black or white based on global median treshold
        for (unsigned int i = 0; i < im_data.size(); i++){
            for (unsigned int j = 0; j < im_data[0].size(); j++){
                if (im_data[i][j] < T)
                    im_data[i][j] = 0;
                else {im_data[i][j] = 255;}
            }
        }
        WriteFile(out_fname, &im_data);
    }


    else if (type == "local"){
        int size = std::stoi(argv[4]);
        std::vector<double> neigh;
        
        for (int i = 0; i < im_data.size(); i++)
            for (int j = 0; j < im_data[0].size(); j++){
                //get T
                double sum = 0, count = 0;
                int shift = ((size - 1)/2);
                //std::cout << "pixel: " << i << " " << j << std::endl;
                for (int k = i - shift; k <= i + shift; k++)
                    for (int m = j - shift; m <=  j + shift; m++){
                        if ((k >= 0 && k < im_data.size()) && (m >= 0 && m < im_data[0].size()) ){
                            neigh.push_back(im_data[k][m]);
                            //std::cout << im_data[k][m] << std::endl;
                            sum += im_data[k][m];
                        }
                    }
                    //std::cout << std::endl;
                //sort(neigh.begin(), neigh.end());

                double v = 0;
                double mean = sum / neigh.size();
                for (int s = 0; s < neigh.size(); s++)
                    v += pow((neigh[s] - mean), 2);
                v = v / neigh.size();
                double stdD = sqrt(v);
                double r = (0.5 * (im_dataL[im_dataL.size() - 1] - im_dataL[0]));

                double T = mean * (1 + (0.2 * ((stdD/r) - 1)));
                //std::cout << "thresold: " << T << std::endl;
                //for (unsigned int i = 0; i < im_data.size(); i++){
                  //  for (unsigned int j = 0; j < im_data[0].size(); j++){
                if (im_data[i][j] < T)
                    out_data[i][j] = 0;
                else {out_data[i][j] = 255;}
                    //}
                //}
                neigh.clear();
            }
        WriteFile(out_fname, &out_data);
    }
}
