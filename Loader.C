/**
 * Names: Nick Smith, Kyle Nichols
 * Team: 12
*/
#include <iostream>
#include <fstream>
#include <string.h>
#include <ctype.h>

#include "Loader.h"
#include "Memory.h"

//first column in file is assumed to be 0
#define ADDRBEGIN 2   //starting column of 3 digit hex address 
#define ADDREND 4     //ending column of 3 digit hext address
#define DATABEGIN 7   //starting column of data bytes
#define COMMENT 28    //location of the '|' character


Memory * mem = Memory::getInstance(); 
int32_t currMem = 0;

/**
 * Loader constructor
 * Opens the .yo file named in the command line arguments, reads the contents of the file
 * line by line and loads the program into memory.  If no file is given or the file doesn't
 * exist or the file doesn't end with a .yo extension or the .yo file contains errors then
 * loaded is set to false.  Otherwise loaded is set to true.
 *
 * @param argc is the number of command line arguments passed to the main; should
 *        be 2
 * @param argv[0] is the name of the executable
 *        argv[1] is the name of the .yo file
 */
Loader::Loader(int argc, char * argv[])
{
   loaded = false; 

   //Start by writing a method that opens the file (checks whether it ends 
   //with a .yo and whether the file successfully opens; if not, return without 
   //loading)
   if(!Loader::isValid(argv[1]))
       return;

   //The file handle is declared in Loader.h.  You should use that and
   //not declare another one in this file.
   
   //Next write a simple loop that reads the file line by line and prints it out
   std::string line;
   int lineNumber = 1;
   while (std::getline(inf, line))
   {
       if (hasErrors(line))
       {
            std::cout << "Error on line " << std::dec << lineNumber
            << ": " << line << std::endl;
            return;
       }                                      
       //std::cout << line << "\n";
       if (line[DATABEGIN] != ' ' && line[0] == '0')
       {
           Loader::loadMem(line);
       }
       lineNumber++;
   }
 

   //Next, add a method that will write the data in the line to memory 
   //(call that from within your loop)

   //Finally, add code to check for errors in the input line.
   //When your code finds an error, you need to print an error message and return.
   //Since your output has to be identical to your instructor's, use this cout to print the
   //error message.  Change the variable names if you use different ones.
   //  std::cout << "Error on line " << std::dec << lineNumber
   //       << ": " << line << std::endl;
 
   //If control reaches here then no error was found and the program
   //was loaded into memory.
   loaded = true;  
  
}

/**
 * isLoaded
 * returns the value of the loaded data member; loaded is set by the constructor
 *
 * @return value of loaded (true or false)
 */
bool Loader::isLoaded()
{
   return loaded;
}


//You'll need to add more helper methods to this file.  Don't put all of your code in the
//Loader constructor.  When you add a method here, add the prototype to Loader.h in the private
//section.

/**
 * isValid
 * returns true if file can be opened and ends with a .yo extension
 *
 * @return true for valid file, otherwise false
 */
bool Loader::isValid(char * file)
{
   inf.open(file);
   std::string fname(file);

   if(!inf.is_open())
   {
       return false;
   }
   else if(fname.find(".yo") != fname.length()-3)
   {
       return false;
   }
   return true;
}

/**
 * loadMem
 *
 * @param line
 */
void Loader::loadMem(std::string line)
{
    int32_t addr = convert(line, ADDRBEGIN, ADDREND + 1);
    int32_t byte = 0;
    bool error = false;
    while (line[DATABEGIN + byte] != ' ')
    {
        int32_t val = convert(line, DATABEGIN + byte,(DATABEGIN + byte + 2));
        //std::cout << addr  << " ";
        mem -> putByte(val, addr, error);
        byte += 2;
        addr++;
    }
    currMem = addr;
}

bool Loader::validBytes(std::string line)
{
    int32_t addr = convert(line, ADDRBEGIN, ADDREND + 1);
    int32_t bit = 0;
    while (line[DATABEGIN + bit] != ' ')
    {
        if ((line[DATABEGIN+ bit] < '0' || line[DATABEGIN+ bit] > '9')
            && (line[DATABEGIN+ bit] < 'a' || line[DATABEGIN+ bit] > 'f')
            && (line[DATABEGIN+ bit] < 'A' || line[DATABEGIN+ bit] > 'F'))
            return false;
        bit++;
    }
    if (!checkSpaces(line, DATABEGIN + bit, COMMENT - 1))
        return false;
    addr += (bit / 2) - 1;
    if (addr > 0xfff)
        return false;
    if (bit % 2)
        return false;
    return true;
}

/**
 * convert
 *
 */
int32_t Loader::convert(std::string line, int start, int end)
{
    std::string str2 = line.substr(start, end - start);
    return stoul(str2, NULL, 16);
}

/** 
 *
 *
 */
bool Loader::hasErrors(std::string line)
{
   if (blankLine(line))
       return false;
   if (commentLine(line))
   {
          if (line[COMMENT] != '|' || line[COMMENT - 1] != ' '
              || line[COMMENT + 1] != ' ')
              return true;
          return false;

   }
   if (line[COMMENT] != '|' || line[COMMENT - 1] != ' '
       || line[COMMENT + 1] != ' ')
       return true;
   if (!validAddr(line))
       return true;
   if (!validBytes(line))
       return true;
   return false;
}

/**
 * blankLine
 *
 */
bool Loader::blankLine(std::string line)
{
    if (checkSpaces(line, 0, line.length() - 1))
        return true;
    return false;
}


bool Loader::commentLine(std::string line)
{
    if (checkSpaces(line, 0, COMMENT - 2))
        return true;
    return false;
}

bool Loader::checkSpaces(std::string line, int start, int end)
{
    for (int i = start; i <= end; i++)
    {
        if (line[i] != ' ')
            return false;
    }
    return true;
}

bool Loader::validAddr(std::string line)
{
    if (line[0] != '0' || line[1] != 'x')
        return false;
    if (line[ADDREND + 1] != ':' || line[ADDREND + 2] != ' ')
        return false;
    int32_t addr = convert(line, ADDRBEGIN, ADDREND + 1);
    if(addr < currMem)
        return false;

    return true;
}

