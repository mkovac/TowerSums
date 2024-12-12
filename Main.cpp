#include "include/TowerSums.h"

#include <filesystem>
#include <iostream>


using namespace std;


int main()
{
     
//   bool debugMode{false};
//   bool filesCreatedFlag{false};
   
   int num_of_sectors{2}; // 3 sectors
   int num_of_boards{7}; // 14 boards

   for (int i_sector = 1; i_sector < num_of_sectors; ++i_sector)
   {
      for (int i_board = 6; i_board < num_of_boards; ++i_board)
      {
   
         // Define everything needed to read mapping and architecture.
         string input_folder_arch     = "./inputs/Arhitecture/v2";
         string input_folder_energies = "./inputs/Energies/Stage1_Unpacker";
           
         string fileName_E_arch = input_folder_arch + "/CE_E_" + to_string(i_board) + "_v2.vh";
         string fileName_H_arch = input_folder_arch + "/CE_H_" + to_string(i_board) + "_v2.vh";
          
         string fileName_E_energies = input_folder_energies + "/CEE/Sector_" + to_string(i_sector) + "_Board_" + to_string(i_board) + ".txt";
         string fileName_H_energies = input_folder_energies + "/CEH/Sector_" + to_string(i_sector) + "_Board_" + to_string(i_board) + ".txt";
           
         string STC_architecture = "oneSize"; // STC4, STC16 or oneSize
           
           
         TowerSums *ts = new TowerSums();
           
         vector<vector<int>> matVariable_CE_E;
         vector<vector<vector<int>>> matVariable_CE_H;
         pair<int, int> result_E, result_H;
   
   
   
// ***********************************
// Read CE_E VH files and create array
// ***********************************
   
         filesystem::path filePath_CE_E_arch(fileName_E_arch);
         if (filesystem::exists(filePath_CE_E_arch) && filesystem::is_regular_file(filePath_CE_E_arch))
         {
            cout << "The file " << filePath_CE_E_arch << " exists." << endl;
            
            // Number of input and output lines is fetched
            result_E = ts->getParametersFromVhFile(fileName_E_arch);
            
            // Input to output mapping is fetched
            matVariable_CE_E = ts->vhArchInputToArrayCE_E( fileName_E_arch, result_E.second, result_E.first );
            
       
            // Begin test
            cout << endl;            
            cout << "In E num: "  << result_E.first  << endl;
            cout << "Out E num: " << result_E.second << endl;
      
            for (const auto& row : matVariable_CE_E)
            {
               for (const auto& element : row)
               {
                  cout << element << " ";
               }
               cout << endl;
            }
            // End test
         
         }
         else // TO BE DONE
         {
//            matVariable_CE_E = [];
//            in_E_num = 0;
//            out_E_num = 0;
            cout << "Sector " + to_string(i_sector) + "Board " + to_string(i_board) + " CE_E architecture not detected! To be done --> Generating an empty array!" << endl;
         }
  
  
   
   
// ***********************************
// Read CE_H VH files and create array
// ***********************************
   
         filesystem::path filePath_CE_H_arch(fileName_H_arch);
         if (filesystem::exists(filePath_CE_H_arch) && filesystem::is_regular_file(filePath_CE_H_arch))
         {
            cout << "The file " << filePath_CE_H_arch << " exists." << endl;
            
            // Number of input and output lines is fetched
            result_H = ts->getParametersFromVhFile(fileName_H_arch);
            
            // Input to output mapping is fetched
            matVariable_CE_H = ts->vhArchInputToArrayCE_H( fileName_H_arch, result_H.second, result_H.first, STC_architecture);
            
       
            // Begin test                    
            cout << "In H num: "  << result_H.first  << endl;
            cout << "Out H num: " << result_H.second << endl;
      
            int sliceIndex{0};
            for (const auto& slice : matVariable_CE_H)
            {
               cout << "Slice " << sliceIndex++ << ":\n";
               for (const auto& row : slice)
               {
                  for (const auto& element : row)
                  {
                     cout << element << " ";
                  }
                  cout << endl;
               }
               cout << "----" << endl; // Separator between slices
            }
            // End test
         
         }
         else // TO BE DONE
         {
//            matVariable_CE_E = []
//            in_E_num = 0
//            out_E_num = 0
            cout << "Sector " + to_string(i_sector) + "Board " + to_string(i_board) + " CE_H architecture not detected! Generating an empty array!" << endl;
         }

   
   
   
   
// ****************************************************************
// Read CE_E input energies - assumption: inputs are in 5E3M format
// Integer unpacking
// ****************************************************************
   
         filesystem::path filePath_E_inputs(fileName_E_energies);
         if (filesystem::exists(fileName_E_energies) && filesystem::is_regular_file(fileName_E_energies))
         {
            cout << "The file " << fileName_E_energies << " exists." << endl;
            
            // Read input energies
            auto inputArray_E = ts->readInputEnergiesE(fileName_E_energies);
            
            // Test: print energies
            cout << "inputArray_E" << endl;
            for (int energy : inputArray_E)
            {
               cout << energy << ", ";
            }
            cout << endl;
            
            
            
            // Integer unpacking
            auto decodedInputArrayE = ts->unpackInteger3m(inputArray_E);
            
            // Test: print decoded energies
            cout << "decodedInputArrayE" << endl;
            for (int energy : decodedInputArrayE)
            {
               cout << energy << ", ";
            }
            cout << endl;
            
            
            
            // Doing summation on upacked data (integers). Parameter 1 or 2 indicates the mode of summation (for study purposes different summation techniques have been implemented). 
            // If version 2 is selected the subsequent false parameter is disregarded (i.e. it is a valid parameter only for version 1 of the sumator).
            auto summedValues_E = ts->summation(decodedInputArrayE, matVariable_CE_E, 2, false);
            
            // Test: print summed values
            cout << "summedValues_E" << endl;
            for (int value : summedValues_E)
            {
               cout << value << ", ";
            }
            cout << endl;
            
            
            
            // Second parameter is maximum number of allowed bits, 34 bits in case of 5E3M
            auto checkedOutputValues_E = ts->overflowChecker(summedValues_E, 34);

            // Test: print checked values
            cout << "checkedOutputValues_E" << endl;
            for (int value : checkedOutputValues_E)
            {
               cout << value << ", ";
            }
            cout << endl;
            
            
            
            // Trimming to 19 bit (4E4M) - taking first 19 MSBs  (requied parameters> targetNumberBits, maxNzmberBits)
            auto trimmedOutputValues_E = ts->trimming(checkedOutputValues_E, 19, 34);
                  
            // Test: print trimmed values
            cout << endl << "trimmedOutputValues_E" << endl;
            for (int value : trimmedOutputValues_E)
            {
               cout << value << ", ";
            }
            cout << endl;
            
            
            
            // Converting the summs from pure integer number format to 4E4M format
            auto outputValues_E = ts->packInteger4e4m(trimmedOutputValues_E);
            
            // Test: print output values
            cout << endl << "outputValues_E" << endl;
            cout << "[";
            for (int value : outputValues_E)
            {
               cout << value << ", ";
            }
            cout << "]" << endl;
         }
         else
         {
            vector<int> outputValues_E(result_E.second, 0);
            cout << "Sector " + to_string(i_sector) + "Board " + to_string(i_board) + " E inputs not detected! Filling output with zeros!" << endl;  
         }




// ****************************************************************
// Read CE_H input energies - assumption: inputs are in 5E3M format
// ****************************************************************
   
         filesystem::path filePath_H_inputs(fileName_H_energies);
         if (filesystem::exists(fileName_H_energies) && filesystem::is_regular_file(fileName_H_energies))
         {
            cout << "The file " << fileName_H_energies << " exists." << endl;
            
            
            
            // Read input energies
            auto inputArray_H = ts->readInputEnergiesH(fileName_H_energies, result_H.first);
      
            // Test: print energies
            cout << endl << "inputArray_H" << endl;
            for (const auto& row : inputArray_H)
            {
               for (const auto& energy : row)
               {
                  cout << energy << ", ";
               }
               cout << "Test" << endl;
            }
         
         
         
            // Integer unpacking
            auto decodedInputArrayH = ts->unpackInteger4m(inputArray_H);
      
            // Test: print decoded energies
            cout << "decodedInputArrayH" << endl;
            for (const auto& row : decodedInputArrayH)
            {
                  for (const auto& energy : row)
                  {
                     cout << energy << ", ";
                  }
                  cout << "Test" << endl;
            }
            cout << endl;


   
            // Summation
            vector<uint64_t> summedValues_H;
            int x_dim = matVariable_CE_H.size();
            int y_dim = matVariable_CE_H[0].size();
            
            for (int k = 0; k < 6; ++k)
            {
               
               int z_slice = k; // Fixed depth index for the z dimension (equivalent to array[:, :, k] in Python)
       
               // Extract a 2D slice at z = z_slice
               vector<vector<int>> slice2D(x_dim, vector<int>(y_dim));
               for (int i = 0; i < x_dim; ++i)
               {
                  for (int j = 0; j < y_dim; ++j)
                  {
                     slice2D[i][j] = matVariable_CE_H[i][j][z_slice];
                  }
               }
               
               
               vector<uint64_t> temp_sum = ts->summation(decodedInputArrayH[k], slice2D, 2, false);

               if (k == 0)
               {
                  summedValues_H = temp_sum; // Initialize summedValues_H with the first summation result
               }
               else
               {
                  summedValues_H = ts->addVectors(summedValues_H, temp_sum); // Add element-wise with summedValues_H
               }
            }

            // Test: print summed values
            cout << "summedValues_H" << endl;
            for (int value : summedValues_H)
            {
               cout << value << ", ";
            }
               cout << endl;
            
               
            
            // Second parameter is maximum number of allowed bits, 34 bits in case of 5E3M
            auto checkedOutputValues_H = ts->overflowChecker(summedValues_H, 35);
 
            // Test: print checked values
            cout << "checkedOutputValues_H" << endl;
            for (int value : checkedOutputValues_H)
            {
               cout << value << ", ";
            }
            cout << endl;
            
            
            
            // Trimming to 19 bit (4E4M) - taking first 19 MSBs  (requied parameters> targetNumberBits, maxNzmberBits)
            auto trimmedOutputValues_H = ts->trimming(checkedOutputValues_H, 19, 35);
            
            // Test: print trimmed values
            cout << endl << "trimmedOutputValues_H" << endl;
            for (int value : trimmedOutputValues_H)
            {
               cout << value << ", ";
            }
            cout << endl;
            
            
            
            // Converting the summs from pure integer number format to 4E4M format
            auto outputValues_H = ts->packInteger4e4m(trimmedOutputValues_H);
      
            // Test: print output values
            cout << endl << "outputValues_H" << endl;
            cout << "[";
            for (int value : outputValues_H)
            {
               cout << value << ", ";
            }
            cout << "]" << endl;
         }
         else
         {
            vector<int> outputValues_H(result_H.second, 0);
            cout << "Sector " + to_string(i_sector) + "Board " + to_string(i_board) + " H inputs not detected! Filling output with zeros!" << endl;  
         }         
         
      } //End i_sector
   } //End i_board
      
   
   return 0;
}