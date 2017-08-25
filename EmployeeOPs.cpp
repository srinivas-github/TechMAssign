#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <string>
#include <dirent.h>

using namespace std;
/**
 * Get the file name(employee / department)
 * from the given command. The text files are stored at
 * location EmpData/employee, EmpData/department
 *
 */
string FindTheDataFile(string tablename)
{
   cout<<"FindTheDataFile: tablename: "<<tablename<<"\n";

    DIR *dir;
    struct dirent *ent;
    dir = opendir("./EmpData");
    //dir = opendir("/home/vagrant/TMAssignment/EmpData");
    ifstream file;
    string filename;
    while ((ent = readdir(dir)) != NULL) 
    {
        filename = "./EmpData/";
        //filename = "/home/vagrant/TMAssignment/EmpData/";
        if ( !strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..") || !strcmp(ent->d_name, " " ))
        {   

        }
        else
        {
             filename.append(ent->d_name);
        }
    
       //cout<<"FileName after append: "<<filename<<"\n";
       
       size_t found = filename.find(".txt");
       if ( found != string::npos)
       {
            //cout<<"Opening file... "<<filename<<"\n";
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
    
    }
    closedir (dir);
    //cout<<"Returning file name: "<<filename<<"\n";
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
}


/**
 * This function is to used to load the data from file to memory
 * memory, So that we will read data from this instead of file IO 
 * operation every time(expensive operation)
 */
string** LoadTheDataFile(char *filename, int noofrows, int noofcolumns)
{
    ifstream file;
    file.open(filename);
    string temp;
    file >> temp;
    try
    {
        string **data = new string*[noofrows];
	for(int i=0; i<noofrows; i++)
	{
            data[i] = new string[noofcolumns];
	}

        for(int i=0; i<noofrows; i++)
	{
            file >> temp;
	    for(int j=0; j<noofcolumns; j++)
	    {
                if(temp.find(';') != -1)
		{
                    data[i][j] = temp.substr(0, temp.find(';'));
		    temp = temp.substr(temp.find(';')+1);
		}
		else
		{
		    data[i][j] = temp;
	        }
	    }
	}
																					        file.close();
	return data;
    }
    catch (const std::bad_alloc& e)
    {
         cout << "Allocation Failed: " << e.what() << endl;
    }
}


/**
 * Write a the data to the file
 * Updates the header with the number of rows and appends the data to the file
 */
void WriteData(char *filename, string row, int rows)
{
    ifstream file;
    file.open(filename);
    string header;
    file >> header;
    ostringstream stream;
    string temp = "";
    temp.append(header.substr(0, header.find(';')+1));
    stream << rows;
    temp.append(stream.str());
    header = header.substr(header.find(';')+1);
    header = header.substr(header.find(';'));
    temp.append(header);
    header = temp;
    ofstream newfile;
    newfile.open("./EmpData/temp");
    newfile << header << endl;
    while(!file.eof())
    {
        temp = "";
	file >> temp;
	newfile << temp << endl;
    }
    newfile << row;
    file.close();
    newfile.close();
    remove(filename);
    rename("./EmpData/temp", filename);
}

/**
 * This method is used to get the column number to search data
 */
int FindColumnNumber(char *filename, string column)
{
    ifstream file;
    file.open(filename);
    string temp;
    file >> temp;
    temp = temp.substr(temp.find(';')+1);
    temp = temp.substr(temp.find(';')+1);
    temp = temp.substr(temp.find(';')+1);
    string columnname = "";
    int counter = 0;
    while(column != columnname)
    {
        counter++;
	columnname = temp.substr(0,temp.find(','));
	columnname = columnname.substr(0, columnname.find('-'));
	temp = temp.substr(temp.find(',')+1);
    }
    return counter;
}


/**
 * Search for the record with the given search data
*/
void SearchData(string **data, int column, string value, int rows, int columns)
{
    bool found = false;
    for(int i=0; i<rows; i++)
    {
        string temp = data[i][column];
        if(temp == value)
        {
	    for(int j=0; j<columns; j++)
	    {
	        cout << data[i][j] << " ";
	        found = true;
	    }
	    if(found)
	        cout << endl;
	}
    }

    if(!found)
    {
        cout << "No rows found." << endl;
    }
}


/**
 * Extended Search
 */
void ExtendedSearch(string **data, int column1, string value1, int column2, string value2, int rows, int columns)
{
    bool found = false;
    for(int i=0; i<rows; i++)
    {
       if((data[i][column1] == value1) && (data[i][column2] == value2))
       {
           for(int j=0; j<columns; j++)
           {
	       cout << data[i][j] << " ";
	       found = true;
	   }
	   if(found)
	        cout << endl;
	}
    }
    if(!found)
    {
        cout << "No rows found." << endl;
    }
}



/**
 * Driver Program to test the functionality
*/
int main()
{
    string tablename = "";
    bool cache = false;
    string **data;
    int noofrows=0;
    int noofcolumns=0;
    char *filename;
    int exit = 0;

    while(1)
    {
        cout << "Enter The Command: ";
	string command;
	getline(cin, command);
	string operations = command.substr(0, command.find(' '));
	int operation;
	if(operations == "READ")
	{
            operation = 0;
        }
        else if(operations == "WRITE")
	{
            tablename = "";
            operation = 1;
        }
        else if(operations == "SEARCH")
        {
	    operation = 2;
	}
	else if(operations == "EXTSEARCH")
	{
	    operation = 3;
	}
	else if(operations == "EXIT" || operations == "QUIT" || operations == "exit" || operations == "quit")
	{
	    operation = 4;
	}
	else
	{
	    operation = -1;
	}
	
	/**
	 * Get the table info (.i.e., employee or department
	 */
	command = command.substr(command.find(' ')+1);
	if(tablename == command.substr(0, command.find(' ')))
	{
	   //Make the cache true, so that will get the data from memory
	   cache = true;
	}
	else
	{
	    //clean the data in memory and make cache is false
	    tablename = command.substr(0, command.find(' '));
	    string _filename = FindTheDataFile(tablename);
	    filename = new char[_filename.length()+1];
	    strcpy(filename, _filename.c_str());

            //cout<<"The Name of file is: "<<filename<<"\n";
	
	    if(noofrows != 0)
	        CleanupData(data, noofrows);

	    noofrows = GetNumOfRows(filename);
	    noofcolumns = GetNumOfColumns(filename); 
	    cache = false;
	}
        command = command.substr(command.find(' ')+1);

        switch(operation)
	{
        case 0:  //Read operation
	    if(!cache)
	    {
	        if(noofrows != 0)
	            data = LoadTheDataFile(filename, noofrows, noofcolumns);
	    }
	
	    for(int i=0; i<noofrows; i++)
	    {
	        for(int j=0; j<noofcolumns; j++)
		{
		    cout << data[i][j] << " ";
		}
		cout << endl;
	    }
	    break;
	case 1: //write operation
	    noofrows++;
	    WriteData(filename, command, noofrows);
	    if(noofrows != 0)
	        data = LoadTheDataFile(filename, noofrows, noofcolumns);
	    break;
	case 2:  //search operation
	    if(!cache)
	    {
	        data = LoadTheDataFile(filename, noofrows, noofcolumns);
	    }
	 
	    {
	        int tempNo = FindColumnNumber(filename, command.substr(0, command.find(' ')));
	        command = command.substr(command.find(' ')+1);
	        SearchData(data, tempNo-1, command, noofrows, noofcolumns);
	    }
	    
	    break;
	case 3: // Extended Search
	    if(!cache)
	    {
	         data = LoadTheDataFile(filename, noofrows, noofcolumns);
	    }

	    {
	       int tempNo1 = FindColumnNumber(filename, command.substr(0, command.find(' ')));
	       command = command.substr(command.find(' ')+1);
	       string value1 = command.substr(0, command.find(' '));
	       command = command.substr(command.find(' ')+1);
	       int tempNo2 = FindColumnNumber(filename, command.substr(0, command.find(' ')));
	       command = command.substr(command.find(' ')+1);
	       ExtendedSearch(data, tempNo1-1, value1, tempNo2-1, command, noofrows, noofcolumns);
	    }
	    break;
	case 4:
	    exit = 1;
	    break;
	default:
	    cout << "Invalid command..." << endl;
	    break;
        } //end of switch
  
        if(exit == 1)
        {
             return 0;
        }
    }//end of while
    
    return 0;
}


