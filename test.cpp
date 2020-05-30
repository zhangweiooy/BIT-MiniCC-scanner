#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
using namespace std;
string add(string a, string b)
{
    return a + b;
}
int main()
{
    // ifstream fin("../../testFile.c"); // filename: xly2016I.txt
    // stringstream buffer;              // stringstream object
    // buffer << fin.rdbuf();            // read file content in stringstream object
    // string str(buffer.str());         // store file content in a string
    // string ss = str;
    // cout << ss; // output file content
    // cout << endl
    //      << ss.length() << ss.size() << endl; //>>
    vector<string> ss = {"Hello", "world", "mmppmmp"};
    string sm[2] = {"Hello", "world"};
    string m1 = "hello";
    string m2 = "world";
    char c = 'a';
    for (int i = 0; i < 2; i++)
    {
        if (ss[i] == m2)
            cout << true << endl;
    }
    cout << m1.length() << m1[5] << "1";
    cout << m1.length() << "1" << endl;
    char cc = m1[5];
    cout << cc << "2" << endl;
    cout << sizeof(cc) << endl;
    string m3(m1, 0, m1.rfind('o'));
    cout << m3;
    // cout << false << ss.size();
    // ofstream out("Test.txt");
    // out << sm[1] << sm[0] << endl;
    // out.close();
    return 0;
}
