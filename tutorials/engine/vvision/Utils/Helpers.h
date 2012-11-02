/* Helpers.h
 *
 * Virtual Vision Engine . Copyright (C) 2012 Abdallah DIB.
 * All rights reserved. Email: Abdallah.dib@virtual-vison.net
 * Web: <http://www.virutal-vision.net/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

namespace vvision
{
    
    class StringManipulator
    {
        public :
        static void SplitFileName(const string& str, string &folder, string &file)
        {
            size_t found;
            found=str.find_last_of("/\\");
#ifdef DEBUG
            cerr << "Splitting: " << str << endl;
            cerr << " folder: " << str.substr(0,found) << endl;
            cerr << " file: " << str.substr(found+1) << endl;
#endif
            folder=str.substr(0,found);
            file=str.substr(found+1) ;
        }
        static void GetExtensitonType(const string& str, string &extension)
        {
            size_t found=str.find_last_of(".");
            extension = str.substr(found+1);
            
        }
        static void GetBaseName(const string& str, string &base)
        {
            size_t found=str.find_last_of(".");
            base = str.substr(0, found);
            
        }
        static void ToLowerCase(string& str)
        {
            std::transform(str.begin(),str.end(),str.begin(),::tolower);
        }
        static void ToUpperCase(string& str)
        {
            std::transform(str.begin(),str.end(),str.begin(),::toupper);
        }
        static int32 IsEqual(const string &str1, const string &str2)
        {
            return str1.compare(str2);
        }
        static void AddCharArrayToString(string &str,const char* array)
        {
            int32 i=0;
            while(array[i]!='\0')
            {
                str+=array[i];
                i++;
            }
        }
    };
    
    // Macro which returns a random value between -1 and 1
#define RAND_MINUS_1_TO_1() ((random() / (float32)0x3fffffff )-1.0f)
    
    // MAcro which returns a random number between 0 and 1
#define RAND_0_TO_1() ((random() / (float32)0x7fffffff ))
}
#endif
