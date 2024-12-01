#include "include/Emulator.h"

#include <filesystem>
#include <iostream>


using namespace std;


int main()
{
     
   bool debugMode{false};
   bool filesCreatedFlag{false};
   
   int i_sector{0}; // 3 sectors
   int i_board{0}; // 14 boards

   for (int i_sector = 1; i_sector < 2; ++i_sector)
   {
      for (int i_board = 6; i_board < 7; ++i_board)
      {

   
   
   
   // Define everything needed to read mapping and architecture.
   string input_folder_arch   = "../../Firmware/inputs/v2/";
   string input_folder_inputs = "../../Emulator/output/Stage1_Unpacker";
   
   string fileName_CE_E_arch = input_folder_arch + "CE_E_" + to_string(i_board) + "_v2.vh"; // matrica arhitekture za CE_E ulaze
   string fileName_CE_H_arch = input_folder_arch + "CE_H_" + to_string(i_board) + "_v2.vh"; // matrica arhitekture za CE_H ulaze
   
   string fileName_E_inputs = input_folder_inputs + "/CEE/Sector_" + to_string(i_sector) + "_Board_" + to_string(i_board) + ".txt";
   string fileName_H_inputs = input_folder_inputs + "/CEH/Sector_" + to_string(i_sector) + "_Board_" + to_string(i_board) + ".txt";
   
   string STC_architecture = "oneSize"; // STC4, STC16 or oneSize
   
   
   Emulator *emulate = new Emulator();
   
   vector<vector<int>> matVariable_CE_E;
   vector<vector<vector<int>>> matVariable_CE_H;
   pair<int, int> result_E, result_H;
   
   
// ***********************************
// Read CE_E VH files and create array
// ***********************************
   
   filesystem::path filePath_CE_E_arch(fileName_CE_E_arch);
   if (filesystem::exists(filePath_CE_E_arch) && filesystem::is_regular_file(filePath_CE_E_arch))
   {
      cout << "The file " << filePath_CE_E_arch << " exists." << endl;
      
      // Number of input and output lines is fetched
      result_E = emulate->getParametersFromVhFile(fileName_CE_E_arch);
      
      // Input to output mapping is fetched
      matVariable_CE_E = emulate->vhArchInputToArrayCE_E( fileName_CE_E_arch, result_E.second, result_E.first );
      
 
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
   else
   {
//      matVariable_CE_E = [];
//      in_E_num = 0;
//      out_E_num = 0;
      cout << "Sector " + to_string(i_sector) + "Board " + to_string(i_board) + " CE_E architecture not detected! To be done --> Generating an empty array!" << endl;
   }
   
  
  
 
 
  
  
   
   
// ***********************************
// Read CE_H VH files and create array
// ***********************************
   
   filesystem::path filePath_CE_H_arch(fileName_CE_H_arch);
   if (filesystem::exists(filePath_CE_H_arch) && filesystem::is_regular_file(filePath_CE_H_arch))
   {
      cout << "The file " << filePath_CE_H_arch << " exists." << endl;
      
      // Number of input and output lines is fetched
      result_H = emulate->getParametersFromVhFile(fileName_CE_H_arch);
      
      // Input to output mapping is fetched
      matVariable_CE_H = emulate->vhArchInputToArrayCE_H( fileName_CE_H_arch, result_H.second, result_H.first, STC_architecture);
      
 
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
   else
   {
//      matVariable_CE_E = []
//      in_E_num = 0
//      out_E_num = 0
      cout << "Sector " + to_string(i_sector) + "Board " + to_string(i_board) + " CE_H architecture not detected! Generating an empty array!" << endl;
   }

   
   
   
   
// ****************************************************************
// Read CE_E input energies - assumption: inputs are in 5E3M format
// Integer unpacking
// ****************************************************************
   
   filesystem::path filePath_E_inputs(fileName_E_inputs);
   if (filesystem::exists(fileName_E_inputs) && filesystem::is_regular_file(fileName_E_inputs))
   {
      cout << "The file " << fileName_E_inputs << " exists." << endl;
      
      // Read input energies
      auto inputArray_E = emulate->readInputEnergiesE(fileName_E_inputs);
      
      // Test: print energies
      cout << "inputArray_E" << endl;
      for (int energy : inputArray_E)
      {
         cout << energy << ", ";
      }
      cout << endl;



      // Integer unpacking
      auto decodedInputArrayE = emulate->unpackInteger3m(inputArray_E);
      
      // Test: print decoded energies
      cout << "decodedInputArrayE" << endl;
      for (int energy : decodedInputArrayE)
      {
         cout << energy << ", ";
      }
      cout << endl;
      
      
      
      // Doing summation on upacked data (integers). Parameter 1 or 2 indicates the mode of summation (for study purposes different summation techniques have been implemented). 
      // If version 2 is selected the subsequent false parameter is disregarded (i.e. it is a valid parameter only for version 1 of the sumator).
      auto summedValues_E = emulate->summation(decodedInputArrayE, matVariable_CE_E, 2, false);
      
      // Test: print summed values
      cout << "summedValues_E" << endl;
      for (int value : summedValues_E)
      {
         cout << value << ", ";
      }
      cout << endl;

      
      
      auto checkedOutputValues_E = emulate->overflowChecker(summedValues_E, 34); // Second parameter is maximum number of allowed bits, 34 bits in case of 5E3M
 
      // Test: print checked values
      cout << "checkedOutputValues_E" << endl;
      for (int value : checkedOutputValues_E)
      {
         cout << value << ", ";
      }
      cout << endl;


      
      auto trimmedOutputValues_E = emulate->trimming(checkedOutputValues_E, 19, 34); //trimming to 19 bit (4E4M) - taking first 19 MSBs  (requied parameters> targetNumberBits, maxNzmberBits)

      // Test: print trimmed values
      cout << endl << "trimmedOutputValues_E" << endl;
      for (int value : trimmedOutputValues_E)
      {
         cout << value << ", ";
      }
      cout << endl;

      
      
      auto outputValues_E = emulate->packInteger4e4m(trimmedOutputValues_E); // converting the summs from pure integer number format to 4E4M format
      
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
   
         filesystem::path filePath_H_inputs(fileName_H_inputs);
         if (filesystem::exists(fileName_H_inputs) && filesystem::is_regular_file(fileName_H_inputs))
         {
            cout << "The file " << fileName_H_inputs << " exists." << endl;
      
            // Read input energies
            auto inputArray_H = emulate->readInputEnergiesH(fileName_H_inputs, result_H.first);
      
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
            auto decodedInputArrayH = emulate->unpackInteger4m(inputArray_H);
      
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
               
               
               vector<uint64_t> temp_sum = emulate->summation(decodedInputArrayH[k], slice2D, 2, false);

               if (k == 0)
               {
                  summedValues_H = temp_sum; // Initialize summedValues_H with the first summation result
               }
               else
               {
                  summedValues_H = emulate->addVectors(summedValues_H, temp_sum); // Add element-wise with summedValues_H
               }
            }

            // Test: print summed values
            cout << "summedValues_H" << endl;
            for (int value : summedValues_H)
            {
               cout << value << ", ";
            }
               cout << endl;
               
               
            auto checkedOutputValues_H = emulate->overflowChecker(summedValues_H, 35); // Second parameter is maximum number of allowed bits, 34 bits in case of 5E3M
 
            // Test: print checked values
            cout << "checkedOutputValues_H" << endl;
            for (int value : checkedOutputValues_H)
            {
               cout << value << ", ";
            }
            cout << endl;


      
            auto trimmedOutputValues_H = emulate->trimming(checkedOutputValues_H, 19, 35); //trimming to 19 bit (4E4M) - taking first 19 MSBs  (requied parameters> targetNumberBits, maxNzmberBits)

            // Test: print trimmed values
            cout << endl << "trimmedOutputValues_H" << endl;
            for (int value : trimmedOutputValues_H)
            {
               cout << value << ", ";
            }
            cout << endl;
            
            
            auto outputValues_H = emulate->packInteger4e4m(trimmedOutputValues_H); // converting the summs from pure integer number format to 4E4M format
      
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
   
   
   

   
   
   
   
   



   
   
/*
   
   
   auto inputArray_CE_H = emulate->generateInputEnergies(result_H.first, 8, 25, 3);


// Test inputArray vector
//==========================================
   for( const auto& str : inputArray_CE_H )
   {
      cout << str << endl;
   }
//==========================================


   auto shiftArray_CE_E = emulate->generateInputShifts(result_E.first, 2);
   
   
// Test shiftArray vector
//==============================
   for( const auto& s : shiftArray_CE_E )
   {
      cout << s << std::endl;
   }
//==============================
  
  
*/
   
   
   return 0;
}