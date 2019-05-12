
class Loader
{
   private:
      bool loaded;        //set to true if a file is successfully loaded into memory
      std::ifstream inf;  //input file handle
   public:
      Loader(int argc, char * argv[]);
      bool isLoaded();
      bool isValid(char * file);
      bool hasErrors(std::string line);
      bool blankLine(std::string line);
      bool commentLine(std::string line);
      bool checkSpaces(std::string line, int start, int end);
      bool validBytes(std::string line);
      bool validAddr(std::string line);
      void loadMem(std::string line);
      int32_t convert(std::string line, int start, int end);
};
