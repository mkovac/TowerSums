// Include classes
#include "Emulator.h"


//=================
Emulator::Emulator()
{
   utilities = new Utilities();
}
//=================


//===========
Emulator::~Emulator()
{
}
//===========



//============================================================================
pair<int, int> Emulator::getParametersFromVhFile( const string& inputFileName )
{
    ifstream f(inputFileName);
    string line;

    getline(f, line); // Read first line
    getline(f, line); // Read second line
    int in_num = 0;
    int out_num = 0;

    bool flag_current_digit = false;
    bool flag_current_alpha = false;
    bool flag_last_digit = false;
    bool flag_last_alpha = false;

    bool flag_start_condition = false;
    bool flag_stop_condition = false;

    bool firstTime = false;

    for (char element : line) {
        if (isdigit(element)) {
            flag_current_digit = true;
            flag_current_alpha = false;
            if (!firstTime) {
                firstTime = true;
            }
        } else {
            flag_current_digit = false;
            flag_current_alpha = true;
        }

        if (flag_current_digit && flag_last_alpha && firstTime) {
            flag_start_condition = true;
        }
        if (flag_current_alpha && flag_last_digit && firstTime) {
            firstTime = false;
            flag_stop_condition = true;
        }

        if (flag_start_condition && !flag_stop_condition) {
            in_num = in_num * 10 + (element - '0');
        }

        flag_last_alpha = flag_current_alpha;
        flag_last_digit = flag_current_digit;
    }


    getline(f, line); // Read third line

    flag_current_digit = false;
    flag_current_alpha = false;
    flag_last_digit = false;
    flag_last_alpha = false;

    flag_start_condition = false;
    flag_stop_condition = false;

    firstTime = false;

    for (char element : line) {
        if (isdigit(element)) {
            flag_current_digit = true;
            flag_current_alpha = false;
            if (!firstTime) {
                firstTime = true;
            }
        } else {
            flag_current_digit = false;
            flag_current_alpha = true;
        }

        if (flag_current_digit && flag_last_alpha && firstTime) {
            flag_start_condition = true;
        }
        if (flag_current_alpha && flag_last_digit && firstTime) {
            firstTime = false;
            flag_stop_condition = true;
        }

        if (flag_start_condition && !flag_stop_condition) {
            out_num = out_num * 10 + (element - '0');
        }

        flag_last_alpha = flag_current_alpha;
        flag_last_digit = flag_current_digit;
    }

    return {in_num, out_num};
}
//============================================================================



//==================================================================================================================
vector<vector<int>> Emulator::vhArchInputToArrayCE_E( const string& inputFileName, int output_size, int input_size )
{
   vector<vector<int>> output(output_size, vector<int>(input_size, 0));
   ifstream f(inputFileName);
   string line;
   int counter{-1};
   int counter_preamble{5}; // Since architecture starts at sixth row from the top (zero indexed)
   

   // Check 2D vector
//   for (const auto& row : output)
//   {
//      for( int element : row )
//      {
//         cout << element << " ";
//      }
//      cout << endl;
//   }



   if (f.is_open())
   {
      while (getline(f, line))
      {         
         counter++;
         cout << line << endl;

         if (counter > counter_preamble)
         {
            if (line.find("*/") == string::npos) continue;
              
            auto start_index = line.find("*/") + 3;
//            cout << "start_index = " << start_index << endl;
            auto tempString = line.substr(start_index);
//            cout << "tempString = " << tempString << endl;
            
            stringstream iss(tempString);
            string token;
            
            vector<string> tempString_variable;

            while (getline(iss, token, ','))
            {
               token.erase(remove(token.begin(), token.end(), ' '), token.end());
               tempString_variable.push_back(token);  
            }
                                   
            tempString_variable.pop_back();
            

//            std::erase_if(tempString_variable, [](auto&& str){return str.find(',', 0) != std::string::npos;});
                          

            // Test vector
//            cout << "tempString_variable.size() = " << tempString_variable.size() << endl;
//            for (const auto& string : tempString_variable)
//            {
//               cout << "string = " << string << endl;
//
//            }


            if (tempString_variable.size() > 1)
            {
               int counter1 = 1;
               
//               cout << "stoi(tempString_variable[0]) = " << stoi(tempString_variable[0]) << endl;
             
               for (int i = 0; i < stoi(tempString_variable[0]); i++)
               {
                  output[counter - counter_preamble - 1][stoi(tempString_variable[counter1])] = stoi(tempString_variable[counter1 + 1]);
                  counter1 += 2;
               }
            }// end if
         
         }// end if
      }// end while
      f.close();
   }// end if
    
   return output;

}
//==================================================================================================================



// TO BE DONE
//=====================================================================================================================================
vector<vector<vector<int>>> Emulator::vhArchInputToArrayCE_H( const string& inputFileName, int output_size, int input_size, const string& STC )
{
   
   int number_of_subchannels;
   
   if (STC == "STC4")
   {
      number_of_subchannels = 12;
   }
   else if (STC == "STC16")
   {
      number_of_subchannels = 3;
   }
   else if (STC == "oneSize")
   {
      number_of_subchannels = 6;
   }
   else
   {
      cout << "Unknown STC architecture!! Please use STC4 or STC16 parameters." << endl;
      exit(0);
      
   }
   
   
   vector<vector<vector<int>>> output(output_size, vector<vector<int>>(input_size, vector<int>(number_of_subchannels, 0)));
   
   
   ifstream f(inputFileName);
   string line;
   int counter{-1};
   int counter_preamble{5}; // Since architecture starts at sixth row from the top (zero indexed)

   if (f.is_open())
   {
      while (getline(f, line))
      {         
         counter++;
         cout << line << endl;

         if (counter > counter_preamble)
         {
            if (line.find("*/") == string::npos) continue;
              
            auto start_index = line.find("*/") + 3;
            cout << "start_index = " << start_index << endl;
            auto tempString = line.substr(start_index);
            cout << "tempString = " << tempString << endl;
            
            stringstream iss(tempString);
            string token;
            
            vector<string> tempString_variable;

            while (getline(iss, token, ','))
            {
               token.erase(remove(token.begin(), token.end(), ' '), token.end());
               tempString_variable.push_back(token);  
            }
                                   
            tempString_variable.pop_back();
            

            if (tempString_variable.size() > 1)
            {
               int counter1 = 1;
               
               cout << "stoi(tempString_variable[0]) = " << stoi(tempString_variable[0]) << endl;
             
               for (int i = 0; i < stoi(tempString_variable[0]); i++)
               {
                  output[counter - counter_preamble - 1][stoi(tempString_variable[counter1])][stoi(tempString_variable[counter1 + 1])] = stoi(tempString_variable[counter1 + 2]);
                  counter1 += 3;
               }
            }// end if
         
         }// end if
      }// end while
      f.close();
   }// end if
    
   return output;

}
//=====================================================================================================================================



//=======================================================================
vector<uint64_t> Emulator::readInputEnergiesE( const string& inputFileName )
{
   
   vector<uint64_t> output;
   ifstream inputFile(inputFileName);
    
   if (!inputFile.is_open())
   {
      cerr << "Error: Could not open file " << inputFileName << endl;
      return output;  // Return an empty vector if the file cannot be opened
   }

   string line;
   while (getline(inputFile, line))
   {
      // Convert the binary string to an integer
      int value = stoi(line, nullptr, 2);
      output.push_back(value);
   }
   
   inputFile.close();
   return output;


/*
//********************************************
// Old generateInputEnergies part of the code
//********************************************
   
   random_device rd;
   mt19937 gen(rd());
   uniform_int_distribution<> dis(0, 255);
   
   vector<string> output;
   string stringForCommand = "0" + to_string(lengthOfItems) + "b"; // "08b"

   for( int i = 0; i < numberOfItems; ++i )
   {
      bitset<8> bits(dis(gen));
      output.push_back(bits.to_string());
      
      string binarySubstring = output.back().substr(output.back().length() - lengthOfItems, lengthOfItems - manLength);
      
      cout << "binarySubstring = " << binarySubstring << endl;


      int validationExp = 0;
      for( string::size_type i = 0; i < binarySubstring.length(); ++i )
      {
        validationExp += (binarySubstring[i] - '0') * pow(2, binarySubstring.length() - 1 - i);
      }
      
      
      if( validationExp > outputLengthNextStep - manLength )
      {
         output.back() = "10110" + output.back().substr(output.back().size() - manLength);
      }
   }
 */

}
//=======================================================================



//==================================================================================================
vector<vector<uint64_t>> Emulator::readInputEnergiesH( const string& inputFileName, int input_size )
{
   
   vector<vector<uint64_t>> output(6, vector<uint64_t>(input_size, 0));
   ifstream inputFile(inputFileName);
    
   if (!inputFile.is_open())
   {
      cerr << "Error: Could not open file " << inputFileName << endl;
      return output;  // Return an empty vector if the file cannot be opened
   }
   
   
   string line;
   int counter = -1;
   
   while (getline(inputFile, line) && counter < input_size - 1)
   {
      counter++;
        
      // Strip the line and split by whitespace
      stringstream iss(line);
      string binaryString;
      int col = 0;

      // Process each binary number in the line
      while (iss >> binaryString && col < 6)
      {
         // Convert binary string to integer
         output[col][counter] = stoi(binaryString, nullptr, 2);
         col++;
      }
   }

   inputFile.close();
   return output;

}
//==================================================================================================



//===========================================================================
vector<uint64_t> Emulator::unpackInteger3m(const vector<uint64_t>& inputData)
{
   
   return utilities->unpack5e3mToInt(inputData);
   
}
//===========================================================================



//===========================================================================================
vector<vector<uint64_t>> Emulator::unpackInteger4m(const vector<vector<uint64_t>>& inputData)
{
   
   return utilities->unpack5e4mToInt(inputData);
   
}
//===========================================================================================



//=========================================================================================
vector<uint64_t> Emulator::addVectors(const vector<uint64_t>& a, const vector<uint64_t>& b)
{
   
   return utilities->addVectors(a, b);
   
}
//===========================================================================================



//===============================================================================================================================================
vector<uint64_t> Emulator::summation( const vector<uint64_t>& inputData, const vector<vector<int>>& matrixArc, int summatorVersion, bool summationTypeFlag)
{
   vector<uint64_t> output;
   
   int outputControlNo = 467; // Debugging purposes

   for (size_t i = 0; i < matrixArc.size(); i++)
   {
      if (summatorVersion == 1)
      {
         int sumTemp = 0;
         const vector<int>& currentArc = matrixArc[i]; // getting one line representing architecture for one output
         
         for (size_t j = 0; j < currentArc.size(); j++)
         {
            if (currentArc[j] == 0)
            {
               // Do nothing; the i-th output is not connected to i-th input
            }
            else if (currentArc[j] == 1)
            {
               int temp1 = inputData[j] >> 4;
               
               if (((inputData[j] >> 3) & 1) && summationTypeFlag)
               {
                    temp1 += 1;
               }
               
               sumTemp += temp1;
            }
            else if (currentArc[j] == 2)
            {
               int temp2 = inputData[j] >> 3;
               
               if (((inputData[j] >> 2) & 1) && summationTypeFlag)
               {
                    temp2 += 1;
               }
               
               sumTemp += temp2;
            }
            else if (currentArc[j] == 3)
            {
               int temp3_1 = inputData[j] >> 3;
               int temp3_2 = inputData[j] >> 4;
               
               if (((inputData[j] >> 2) & 1) && ((inputData[j] >> 3) & 1) && summationTypeFlag)
               {
                  temp3_1 += 1;
                  temp3_2 += 1;
               }
               else if (((inputData[j] >> 2) & 1) && summationTypeFlag)
               {
                  temp3_1 += 1;
               }
               else if (((inputData[j] >> 3) & 1) && summationTypeFlag)
               {
                  temp3_2 += 1;
               }
               
               sumTemp += temp3_1 + temp3_2;
            }
            else if (currentArc[j] == 4)
            {
               int temp4 = inputData[j] >> 2;
               
               if (((inputData[j] >> 1) & 1) && summationTypeFlag)
               {
                    temp4 += 1;
               }
               
               sumTemp += temp4;
            }
            else if (currentArc[j] == 5)
            {
               int temp5_1 = inputData[j] >> 2;
               int temp5_2 = inputData[j] >> 4;
               
               if (((inputData[j] >> 1) & 1) && ((inputData[j] >> 3) & 1) && summationTypeFlag)
               {
                  temp5_1 += 1;
                  temp5_2 += 1;
               }
               else if (((inputData[j] >> 1) & 1) && summationTypeFlag)
               {
                  temp5_1 += 1;
               }
               else if (((inputData[j] >> 3) & 1) && summationTypeFlag)
               {
                  temp5_2 += 1;
               }
               
               sumTemp += temp5_1 + temp5_2;
            }
            else if (currentArc[j] == 6)
            {
               int temp6_1 = inputData[j] >> 2;
               int temp6_2 = inputData[j] >> 3;
               
               if (((inputData[j] >> 1) & 1) && ((inputData[j] >> 2) & 1) && summationTypeFlag)
               {
                  temp6_1 += 1;
                  temp6_2 += 1;
               }
               else if (((inputData[j] >> 1) & 1) && summationTypeFlag)
               {
                  temp6_1 += 1;
               }
               else if (((inputData[j] >> 2) & 1) && summationTypeFlag)
               {
                  temp6_2 += 1;
               }
               
               sumTemp += temp6_1 + temp6_2;
            }
            else if (currentArc[j] == 7)
            {
               int temp7_1 = inputData[j] >> 2;
               int temp7_2 = inputData[j] >> 3;
               int temp7_3 = inputData[j] >> 4;
               
               if (((inputData[j] >> 1) & 1) && ((inputData[j] >> 2) & 1) && ((inputData[j] >> 3) & 1) && summationTypeFlag)
               {
                  temp7_1 += 1;
                  temp7_2 += 1;
                  temp7_3 += 1;
               }
               else if (((inputData[j] >> 1) & 1) && ((inputData[j] >> 2) & 1) && summationTypeFlag)
               {
                  temp7_1 += 1;
                  temp7_2 += 1;
               }
               else if (((inputData[j] >> 1) & 1) && ((inputData[j] >> 3) & 1) && summationTypeFlag)
               {
                  temp7_1 += 1;
                  temp7_3 += 1;
               }
               else if (((inputData[j] >> 2) & 1) && ((inputData[j] >> 3) & 1) && summationTypeFlag)
               {
                  temp7_2 += 1;
                  temp7_3 += 1;
               }
               else if (((inputData[j] >> 1) & 1) && summationTypeFlag)
               {
                  temp7_1 += 1;
               }
               else if (((inputData[j] >> 2) & 1) && summationTypeFlag)
               {
                  temp7_2 += 1;
               }
               else if (((inputData[j] >> 3) & 1) && summationTypeFlag)
               {
                  temp7_3 += 1;
               }
               
               sumTemp += temp7_1 + temp7_2 + temp7_3;
            }
            else if (currentArc[j] == 8)
            {
               int temp8 = inputData[j] >> 1;
              
               if ((inputData[j] & 1) && summationTypeFlag)
               {
                  temp8 += 1;
               }
               
               sumTemp += temp8;
            }
            else if (currentArc[j] == 9)
            {
               int temp9_1 = inputData[j] >> 1;
               int temp9_2 = inputData[j] >> 4;
               
               if ((inputData[j] & 1) && ((inputData[j] >> 3) & 1) && summationTypeFlag)
               {
                  temp9_1 += 1;
                  temp9_2 += 1;
               }
               else if ((inputData[j] & 1) && summationTypeFlag)
               {
                  temp9_1 += 1;
               }
               else if (((inputData[j] >> 3) & 1) && summationTypeFlag)
               {
                  temp9_2 += 1;
               }
               
               sumTemp += temp9_1 + temp9_2;
            }
            else if (currentArc[j] == 10)
            {
               int temp10_1 = inputData[j] >> 1;
               int temp10_2 = inputData[j] >> 3;
               
               if ((inputData[j] & 1) && ((inputData[j] >> 2) & 1) && summationTypeFlag)
               {
                    temp10_1 += 1;
                    temp10_2 += 1;
               }
               else if ((inputData[j] & 1) && summationTypeFlag)
               {
                  temp10_1 += 1;
               }
               else if (((inputData[j] >> 2) & 1) && summationTypeFlag)
               {
                  temp10_2 += 1;
               }
               
               sumTemp += temp10_1 + temp10_2;
            }
            else if (currentArc[j] == 11)
            {
               int temp11_1 = inputData[j] >> 1;
               int temp11_2 = inputData[j] >> 3;
               int temp11_3 = inputData[j] >> 4;
               
               if ((inputData[j] & 1) && ((inputData[j] >> 2) & 1) && ((inputData[j] >> 3) & 1) && summationTypeFlag)
               {
                  temp11_1 += 1;
                  temp11_2 += 1;
                  temp11_3 += 1;
               }
               else if ((inputData[j] & 1) && ((inputData[j] >> 2) & 1) && summationTypeFlag)
               {
                  temp11_1 += 1;
                  temp11_2 += 1;
               }
               else if ((inputData[j] & 1) && ((inputData[j] >> 3) & 1) && summationTypeFlag)
               {
                  temp11_1 += 1;
                  temp11_3 += 1;
               }
               else if (((inputData[j] >> 2) & 1) && ((inputData[j] >> 3) & 1) && summationTypeFlag)
               {
                  temp11_2 += 1;
                  temp11_3 += 1;
               }
               else if ((inputData[j] & 1) && summationTypeFlag)
               {
                  temp11_1 += 1;
               }
               else if (((inputData[j] >> 2) & 1) && summationTypeFlag)
               {
                  temp11_2 += 1;
               }
               else if (((inputData[j] >> 3) & 1) && summationTypeFlag)
               {
                  temp11_3 += 1;
               }
               
               sumTemp += temp11_1 + temp11_2 + temp11_3;      
            }
            else if (currentArc[j] == 12)
            {
               int temp12_1 = (inputData[j]) >> 1;
               int temp12_2 = (inputData[j]) >> 2;

               if ((inputData[j] & 1) && ((inputData[j] >> 1) & 1) && summationTypeFlag)
               {
                  temp12_1 += 1;
                  temp12_2 += 1;
               }
               else if ((inputData[j] & 1) && summationTypeFlag)
               {
                  temp12_1 += 1;
               }
               else if (((inputData[j] >> 1) & 1) && summationTypeFlag)
               {
                  temp12_2 += 1;
               }

               sumTemp += temp12_1 + temp12_2;
            }
            else if (currentArc[j] == 13)
            {
               int temp13_1 = (inputData[j]) >> 1;
               int temp13_2 = (inputData[j]) >> 2;
               int temp13_3 = (inputData[j]) >> 4;

               if ((inputData[j] & 1) && ((inputData[j] >> 1) & 1) && ((inputData[j] >> 3) & 1) && summationTypeFlag)
               {
                  temp13_1 += 1;
                  temp13_2 += 1;
                  temp13_3 += 1;
               }
               else if ((inputData[j] & 1) && ((inputData[j] >> 1) & 1) && summationTypeFlag)
               {
                  temp13_1 += 1;
                  temp13_2 += 1;
               }
               else if ((inputData[j] & 1) && ((inputData[j] >> 3) & 1) && summationTypeFlag)
               {
                  temp13_1 += 1;
                  temp13_3 += 1;
               }
               else if (((inputData[j] >> 1) & 1) && ((inputData[j] >> 3) & 1) && summationTypeFlag)
               {
                  temp13_2 += 1;
                  temp13_3 += 1;
               }
               else if ((inputData[j] & 1) && summationTypeFlag)
               {
                  temp13_1 += 1;
               }
               else if (((inputData[j] >> 1) & 1) && summationTypeFlag)
               {
                  temp13_2 += 1;
               }
               else if (((inputData[j] >> 3) & 1) && summationTypeFlag)
               {
                  temp13_3 += 1;
               }
               
               sumTemp += temp13_1 + temp13_2 + temp13_3;
            }
            else if (currentArc[j] == 14)
            {
               int temp14_1 = (inputData[j]) >> 1;
               int temp14_2 = (inputData[j]) >> 2;
               int temp14_3 = (inputData[j]) >> 3;

               if ((inputData[j] & 1) && ((inputData[j] >> 1) & 1) && ((inputData[j] >> 2) & 1) && summationTypeFlag)
               {
                  temp14_1 += 1;
                  temp14_2 += 1;
                  temp14_3 += 1;
               }
               else if ((inputData[j] & 1) && ((inputData[j] >> 1) & 1) && summationTypeFlag)
               {
                  temp14_1 += 1;
                  temp14_2 += 1;
               }
               else if ((inputData[j] & 1) && ((inputData[j] >> 2) & 1) && summationTypeFlag)
               {
                  temp14_1 += 1;
                  temp14_3 += 1;
               }
               else if (((inputData[j] >> 1) & 1) && ((inputData[j] >> 2) & 1) && summationTypeFlag)
               {
                  temp14_2 += 1;
                  temp14_3 += 1;
               }
               else if ((inputData[j] & 1) && summationTypeFlag)
               {
                  temp14_1 += 1;
               }
               else if (((inputData[j] >> 1) & 1) && summationTypeFlag)
               {
                  temp14_2 += 1;
               }
               else if (((inputData[j] >> 2) & 1) && summationTypeFlag)
               {
                  temp14_3 += 1;
               }
               
               sumTemp += temp14_1 + temp14_2 + temp14_3;
            }
            else if (currentArc[j] == 15)
            {
               int temp15_1 = (inputData[j]) >> 1;
               int temp15_2 = (inputData[j]) >> 2;
               int temp15_3 = (inputData[j]) >> 3;
               int temp15_4 = (inputData[j]) >> 4;

               if ((inputData[j] & 1) && ((inputData[j] >> 1) & 1) && ((inputData[j] >> 2) & 1) && ((inputData[j] >> 3) & 1) && summationTypeFlag)
               {
                  temp15_1 += 1;
                  temp15_2 += 1;
                  temp15_3 += 1;
                  temp15_4 += 1;
               }
               else if ((inputData[j] & 1) && ((inputData[j] >> 1) & 1) && ((inputData[j] >> 2) & 1) && summationTypeFlag)
               {
                  temp15_1 += 1;
                  temp15_2 += 1;
                  temp15_3 += 1;
               }
               else if ((inputData[j] & 1) && ((inputData[j] >> 1) & 1) && ((inputData[j] >> 3) & 1) && summationTypeFlag)
               {
                  temp15_1 += 1;
                  temp15_2 += 1;
                  temp15_4 += 1;
               }
               else if ((inputData[j] & 1) && ((inputData[j] >> 2) & 1) && ((inputData[j] >> 3) & 1) && summationTypeFlag)
               {
                  temp15_1 += 1;
                  temp15_3 += 1;
                  temp15_4 += 1;
               }
               else if (((inputData[j] >> 1) & 1) && ((inputData[j] >> 2) & 1) && ((inputData[j] >> 3) & 1) && summationTypeFlag)
               {
                  temp15_2 += 1;
                  temp15_3 += 1;
                  temp15_4 += 1;
               }
               else if ((inputData[j] & 1) && ((inputData[j] >> 1) & 1) && summationTypeFlag)
               {
                  temp15_1++;
                  temp15_2++;
               }
               else if ((inputData[j] & 1) && ((inputData[j] >> 2) & 1) && summationTypeFlag)
               {
                  temp15_1++;
                  temp15_3++;
               }
               else if ((inputData[j] & 1) && ((inputData[j] >> 3) & 1) && summationTypeFlag)
               {
                  temp15_1++;
                  temp15_4++;
               }
               else if (((inputData[j] >> 1) & 1) && ((inputData[j] >> 2) & 1) && summationTypeFlag)
               {
                  temp15_2++;
                  temp15_3++;
               }
               else if (((inputData[j] >> 1) & 1) && ((inputData[j] >> 3) & 1) && summationTypeFlag)
               {
                  temp15_2++;
                  temp15_4++;
               }
               else if (((inputData[j] >> 2) & 1) && ((inputData[j] >> 3) & 1) && summationTypeFlag)
               {
                  temp15_3++;
                  temp15_4++;
               }
               else if (inputData[j] & 1)
               {
                  temp15_1++;
               }
               else if (((inputData[j] >> 1) & 1) && summationTypeFlag)
               {
                  temp15_2++;
               }
               else if (((inputData[j] >> 2) & 1) && summationTypeFlag)
               {
                  temp15_3++;
               }
               else if (((inputData[j] >> 3) & 1) && summationTypeFlag)
               {
                  temp15_4++;
               }

               sumTemp += temp15_1 + temp15_2 + temp15_3 + temp15_4;
            }
            else if (currentArc[j] == 16)
            {
               sumTemp += inputData[j];
            }
            
            output.push_back(sumTemp);
         
         } // End for loop
      }
      else if (summatorVersion == 2)
      {
         uint64_t sumTemp = 0;
         vector<int> currentArc(matrixArc[i]);  // getting one line representing architecture for one output
         
//         cout << "matrixArc.size() = " << matrixArc.size() << endl << "currentArc.size() = " << currentArc.size() << endl;
         
         for (size_t j = 0; j < currentArc.size(); ++j)
         {
            if (currentArc[j] == 0)
            {
               // the i-th output is not connected to i-th input, thus pass through
            }
            else if (currentArc[j] == 1)
            {
               sumTemp += inputData[j];
            }
            else if (currentArc[j] == 2)
            {
               sumTemp += 2 * inputData[j];
            }
            else if (currentArc[j] == 3)
            {
               sumTemp += 3 * inputData[j];
            }
            else if (currentArc[j] == 4)
            {
               sumTemp += 4 * inputData[j];
            }
            else if (currentArc[j] == 5)
            {
               sumTemp += 5 * inputData[j];
            }
            else if (currentArc[j] == 6)
            {
               sumTemp += 6 * inputData[j];
            }
            else if (currentArc[j] == 7)
            {
               sumTemp += 7 * inputData[j];
            }
            else if (currentArc[j] == 8)
            {
               sumTemp += 8 * inputData[j];
            }
            else if (currentArc[j] == 9)
            {
               sumTemp += 9 * inputData[j];
            }
            else if (currentArc[j] == 10)
            {
               sumTemp += 10 * inputData[j];
            }
            else if (currentArc[j] == 11)
            {
               sumTemp += 11 * inputData[j];
            }
            else if (currentArc[j] == 12)
            {
               sumTemp += 12 * inputData[j];
            }
            else if (currentArc[j] == 13)
            {
               sumTemp += 13 * inputData[j];
            }
            else if (currentArc[j] == 14)
            {
               sumTemp += 14 * inputData[j];
            }
            else if (currentArc[j] == 15)
            {
               sumTemp += 15 * inputData[j];
            }
            else if (currentArc[j] == 16)
            {
               sumTemp += 16 * inputData[j];
            }
         } // End for loop
         
         output.push_back(sumTemp >> 4);
               
      } // End if loop
   } // End for loop
      
   return output;
}
//===============================================================================================================================================



//==================================================================================
vector<uint64_t> Emulator::overflowChecker(const vector<uint64_t>& inputData, int overflowBit)
{
   vector<uint64_t> output;
   

   for (size_t i = 0; i < inputData.size(); ++i)
   {
            
      // Check if overflow occurred
      if (inputData[i] >> overflowBit)
      {
         // Overflow occurred, saturate to max value at overflowBit number of bits
         int maxValue = (1 << overflowBit) - 1;  // Equivalent to '111...111' (overflowBit number of bits)
         output.push_back(maxValue);

         // Debugging messages
         cout << "!!Overflow occurred!! Saturating output to max value! Check results!" << std::endl;
         cout << "Index: " << i << "Value: " << inputData[i] << std::endl;
      }
      else
      {
         output.push_back(inputData[i]);
      }
   }
    
   return output;
}
//==================================================================================



//=========================================================================================
vector<uint64_t> Emulator::trimming(const vector<uint64_t>& inputData, int targetNumberBits, int MSB)
{

   assert(MSB >= targetNumberBits);  // Ensure MSB is greater than or equal to targetNumberBits

   vector<uint64_t> output;

   // Constructing selection mask so to set all bits left of MSB to zero
   unsigned long long maskInt = (1ULL << 34) - 1;
   
   cout << "maskInt = " << maskInt << endl;
   
   int criticalBitNumber = MSB - targetNumberBits;

   for (size_t i = 0; i < inputData.size(); i++)
   {
      output.push_back((inputData[i] & maskInt) >> criticalBitNumber);
//      cout << "inputData[i] = " << inputData[i] << "   " << "output[i] = " << output[i] << endl;

   }

   return output;
}
//=========================================================================================



//=================================================================
vector<uint64_t> Emulator::packInteger4e4m(const vector<uint64_t>& inputData)
{
   return utilities->pack4e4mFromInt(inputData);
}
//=================================================================
   




//=============================================================================
vector<string> Emulator::generateInputShifts( int numberOfItems, int noOfBits )
{
   
   random_device rd;
   mt19937 gen(rd());
   uniform_int_distribution<> dis(0, (1 << noOfBits) - 1);
    
   vector<string> output;


   for (int i = 0; i < numberOfItems; ++i)
   {
      output.push_back(bitset<2>(dis(gen)).to_string());
   }

   return output;
}
//=============================================================================

