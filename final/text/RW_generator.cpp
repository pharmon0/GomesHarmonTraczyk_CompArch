#include <bits/stdc++.h>
#include <fstream>
#include <bitset>
using namespace std;
 
// Function to find a random
// number between 0 or 1
int findRandom()
{
    // Generate the random number
    int num = ((int)rand() % 2);
 
    // Return the generated number
    return num;
}
 
// Function to generate a random
// binary string of length N
string generateBinaryString(int N, int seed)
{
    srand(seed);
 
    // Stores the empty string
    string S = "";
 
    // Iterate over the range [0, N - 1]
    for (int i = 0; i < N; i++) {
 
        // Store the random number
        int x = findRandom();
 
        // Append it to the string
        S += to_string(x);
    }
 
    // Print the resulting string
    return S;
}
 
// Driver Code
int main()
{
    int N = 20;
    ofstream os1("D1addresses.txt");
    if (os1.is_open()) {
        for (int i = 0; i < 100; i++) {
            string addy;
            addy = generateBinaryString(N, i);
            os1 << addy << "\n";
        }
        os1.close();
    }

    ofstream os2("D2addresses.txt");
    if (os2.is_open()) {
        for (int i = 0; i < 100; i++) {
            string addy;
            addy = generateBinaryString(N, i+100);
            os2 << addy << "\n";
        }
        os2.close();
    }

    ofstream os3("D3addresses.txt");
    if (os3.is_open()) {
        for (int i = 0; i < 100; i++) {
            string addy;
            addy = generateBinaryString(N, i+200);
            os3 << addy << "\n";
        }
        os3.close();
    }
    
    ofstream os4("D4addresses.txt");
    if (os4.is_open()) {
        for (int i = 0; i < 100; i++) {
            string addy;
            addy = generateBinaryString(N, i+300);
            os4 << addy << "\n";
        }
        os4.close();
    }
    return 0;
}