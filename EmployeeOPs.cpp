#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <string>
#include <dirent.h>

/**
 * Get the file name(employee / department)
 * from the given command. The text files are stored at
 * location EmpData/employee, EmpData/department
 *
 */
string FindTheDataFile(string tablename)
{
    DIR *dir;
    struct dirent *ent;
    dir = opendir("EmpData");
    ifstream file;
    string filename;
    while ((ent = readdir(dir)) != NULL) 
    {
        filename = "EmpData/";
        filename.append(ent->d_name);
        file.open(filename.c_str());
        string header;
        file >> header;
        header = header.substr(0, header.find(';'));
        header = header.substr(header.find(':')+1);
        if(header == tablename)
	{
	    file.close();
	    break;
	}
	file.close();
    }
    closedir (dir);
    return filename;
}



/**
 * Delete the data to store the other table data.
 * For example, if we read "employee" first and
 * and wants to read department data next.
 * Here delete the data from string pointer
 */
void CleanupData(string **data, int noofrows)
{
    for (int i=0; i<noofrows; i++)
        delete [] data[i];
    delete [] data;
}

/**
 * Get the number of rows from file
 */
int GetNumOfRows(char *filename)
{
    ifstream file;
    file.open(filename);
    string temp;
    file >> temp;
    string r;

    temp = temp.substr(temp.find(';')+1);
    r = temp.substr(0, temp.find(';'));

    int noofrows;
    noofrows = r[0]-'0';
    for(int i=1;i<r.length();i++)
    {
         noofrows *= 10;
	 noofrows += r[i]-'0';
    }
    file.close();
    return noofrows;
}


/**
 * Utility function to get the number of columns
 * from file
 */
int GetNumOfColumns(char *filename)
{ 
    ifstream file;
    file.open(filename);
    string temp;
    file >> temp;
    string c;
    temp = temp.substr(temp.find(';')+1);
    temp = temp.substr(temp.find(';')+1);
    c = temp.substr(0, temp.find(';'));
    int noofcolumns;
    noofcolumns = c[0]-'0';
    for(int i=1;i<c.length();i++)
    {
         noofcolumns *= 10;
         noofcolumns += c[i]-'0';
    }
    file.close();
    return noofcolumns;
}									   }



/**
 * Driver Program to test the functionality
*/
int main()
{
    
    return 0;
}

