#include<iostream>
#include<fstream>
#include<sstream>
#include <algorithm>
#include<string>


using namespace std;

int memoryLocation = 1000;
bool globalNextFlag = false;
string globalPreviousType;
string symbolEntry = "Name\tType\tLocation\n";

string getData(string filename){
    ifstream f(filename); 
    string str;
    if(f) {
        ostringstream ss;
        ss << f.rdbuf(); 
        str = ss.str();
    }
    else
    {
        cout<<"\nFile not found or file is empty\n";
        str = "";
    }
    
    return str;
}

void putData(string symbol, string type, int space){
    string line = "";
    line += symbol;
    line += "\t";
    line += type;
    line += "\t";
    line += to_string(memoryLocation);
    line += "\n";
    memoryLocation += space;
    symbolEntry += line;

}

int countIdentifier(string identifier){
    string number = "";
    for (int i = identifier.find('['); i < identifier.find(']'); i++)
    {
        if (identifier[i] >= '0' && identifier[i] <= '9')
        {
            number += identifier[i];
        }
        
    }
    if (number == "")
    {
        number = '1';
    }
    

    return stoi(number);   
}

int calculateSize(int arraySize){
    int size;
    if (globalPreviousType == "int")
    {
        size = sizeof(int) * arraySize;
    }
    else if (globalPreviousType == "float")
    {
        size = sizeof(float) * arraySize;
    }
    else if (globalPreviousType == "double")
    {
        size = sizeof(double) * arraySize;
    }
    else if (globalPreviousType == "char")
    {
        size = sizeof(char) * arraySize;
    }
    return size;
}

void getValues(string identifier){
    int arraysize = countIdentifier(identifier);
    if (arraysize > 1)
    {
        identifier.erase(identifier.find('['));
    }
    

    int space = calculateSize(arraysize);
    // cout<<identifier<<"\t"<<globalPreviousType<<"\t"<<arraysize<<"\t"<<space<<endl;
    if (identifier.find(":") != string::npos)
    {
        identifier.erase(identifier.find(':'));
        putData(identifier, "", 0);
    }
    else
    {
        putData(identifier, globalPreviousType, space);
    }
    
    
}

void getIdentifier(string word){
    size_t found = word.find("(");
    if (found == string::npos){
        word.erase(std::remove(word.begin(), word.end(), ';'), word.end());
        // cout<<word<<" "<<globalPreviousType<<endl;
        stringstream ss(word);
        while (ss.good()) {
            string identifier;
            getline(ss, identifier, ',');
            // cout<<identifier<<" "<<globalPreviousType<<endl;
            getValues(identifier);
        }
    }
}

void checkDataType(string word){
    if (word == "int" || word == "float" || word == "double" || word == "char")
    {
        globalNextFlag = true;
        globalPreviousType = word;
    }
    else if (word.find(":") != string::npos)
    {
        getIdentifier(word);
    }
    

    
}

void startProcedure(string data){
    string word;
    stringstream iss(data);
    while (iss >> word){
        // cout << word << endl;
        if (globalNextFlag)
        {
            // cout<<word<<endl;
            getIdentifier(word);
            globalNextFlag = false;
        }
        checkDataType(word);
        
    }

}


int main(){

    string data = getData("program.txt");
    // cout<<data;
    startProcedure(data);
    cout<<symbolEntry;
    // ofstream out("symbolTable.txt");
    // out<<symbolEntry;
    // out.close();
    return 0;
}