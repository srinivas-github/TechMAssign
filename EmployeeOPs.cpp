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

