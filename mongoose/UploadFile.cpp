#include <fstream>
#include <sstream>
#include <iostream>
#include "UploadFile.h"

using namespace std;

namespace Mongoose
{
    UploadFile::UploadFile(string filename_, string data_)
        : filename(filename_),
        data(data_)
    {
    }

    string UploadFile::getName()
    {
        return filename;
    }

    string UploadFile::getData()
    {
        return data;
    }
    
    void UploadFile::saveTo(string directory)
    {
        ostringstream oss;
        oss << directory << "/" << filename;
        fstream file;
        file.open(oss.str().c_str(), fstream::out);
        file << data;
        file.close();
    }
}
