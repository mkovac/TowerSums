#ifndef Emulator_h
#define Emulator_h

// C++
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <bitset>
#include <random>
#include <cassert>

#include "Utilities.h"


using namespace std;


class Emulator
{

public:
	
	Emulator();
	~Emulator();
   
   pair<int, int> getParametersFromVhFile( const string& );
   
   vector<vector<int>> vhArchInputToArrayCE_E( const string& , int, int );
   vector<vector<vector<int>>> vhArchInputToArrayCE_H( const string& , int, int, const string& );
   
   vector<uint64_t>         readInputEnergiesE( const string& );
   vector<vector<uint64_t>> readInputEnergiesH( const string&, int );
   
   vector<uint64_t>         unpackInteger3m( const vector<uint64_t>& );
   vector<vector<uint64_t>> unpackInteger4m( const vector<vector<uint64_t>>& );

   vector<uint64_t> packInteger4e4m( const vector<uint64_t>& );
   
   vector<uint64_t> summation( const vector<uint64_t>&, const vector<vector<int>>&, int, bool );
   vector<uint64_t> overflowChecker( const vector<uint64_t>&, int );
   vector<uint64_t> trimming(const vector<uint64_t>&, int, int );

   vector<string> generateInputShifts( int, int );
   
   vector<uint64_t> addVectors(const vector<uint64_t>&, const vector<uint64_t>& );


private:

   Utilities *utilities;


};
#endif
