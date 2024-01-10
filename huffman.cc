/**************************************************************
 *  C++ code reads in .txt file, generates Huffman Encoding,
 *  encodes, writes, reads and decodes messages.
 * 
 * Instructions:
 * 1. Include "grail_testfile.txt" (Could be any text file) and the makefile in
 *     current directory.
 * 2. Make.
 * 3. ./huffman to view output.
**************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <queue>
#include <math.h>
// #include <bits/stdc++.h>
using namespace std;

// Struct to hold elements of a character
struct Letter{
    int ascii, occurances;
    double probability;
    string huffman;
    Letter(int a = 0, int o = 0, string h = "na", double p = 0){
        ascii = a; occurances = o; huffman = h; probability = p;
    }
};

// Node Class used for min-heap
struct Nodes{
    int ascii;
    double prob;
    Nodes* llink;
    Nodes* rlink;
    Nodes(int a, double p){
        ascii = a; prob = p;
        llink = rlink = NULL;
    }    
};

// Comparison op on char probability
struct Comparator{
    int operator() (Nodes* p1, Nodes* p2){
        return p1->prob > p2->prob;
    }
};

// Make Huffman Tree
Nodes* Huffman_Tree(priority_queue <Nodes*, vector<Nodes*>, Comparator > pq){
    while(pq.size() > 1){
        // Take top 2 (i.e. min prob nodes)
        Nodes* left = pq.top();
        pq.pop();
        Nodes* right = pq.top();
        pq.pop();

        // Create node with combined prob then link
        Nodes* tmp = new Nodes(0, left -> prob + right ->prob);
        tmp -> llink = left;
        tmp -> rlink = right;
        // Push new node to queue
        pq.push(tmp);
    }
    // Return root node of tree
    return pq.top();
}

// Recursively Traverse Huffman Tree to get Codes
void Huffman_Coding(Nodes* root, Letter arr[], string code){
    // Traverse left add "0" to encoding
    if (root -> llink) {
        Huffman_Coding(root -> llink, arr, code + "0");
    }
    // Traverse right add "1" to encoding
    if (root -> rlink) {
        Huffman_Coding(root -> rlink, arr, code + "1");
    }
    // Leaf Node so store code for each letter
    if (!root -> llink && !root -> rlink) {
        // cout << root -> ascii << " ";  // Assign code value
        for(int i = 0; i < 30; i++){
            if(root -> ascii == arr[i].ascii){
                arr[i].huffman = code;
            }
        }
    }
}

// Helper code for centering text to formatt table
// Source: https://stackoverflow.com/questions/14765155/how-can-i-easily-format-my-data-table-in-c
std::string center(const string s) {
    stringstream ss, spaces;
    int padding = 20 - s.size();
    for(int i=0; i<padding/2; ++i)
        spaces << " ";
    ss << spaces.str() << s << spaces.str();
    if(padding>0 && padding%2!=0)
        ss << " ";
    return ss.str();
}

// Print Neat Formatted Table for Q.3
void print_table(Letter arr[]){
    // Headings
    cout << "|" << center("Character") << "|" << center("Probability") << "|" << 
    center("Huffman Code") << "|" << center("Code Length") << "|\n";
    // Data
    for(int i = 0; i<30; i++){ cout << "|" <<
        center(to_string(arr[i].ascii)) << "|" <<
        center(to_string(arr[i].probability)) << "|" <<
        center(arr[i].huffman) << "|" <<
        center(to_string(arr[i].huffman.length()))<<"|\n";
    }
}

// Encode string using Huffman Code
string encoder(string str, Letter arr[]){
    string encoded = "";
    for(int i = 0; i < str.size(); i++){
        for(int j = 0; j < 30; j++){
            if(str[i] == arr[j].ascii){
                encoded.append(arr[j].huffman);
            }
        }
    }
    return encoded;
}

// Spaces string to 8 char sequences
string spaced(string str){
    int l = str.size();
    string spaced = "";
    for(int i = 0; i < l; i = i + 8){
        // int tmp = stoi(str.substr(i, 8), nullptr, 2);
        // spaced += to_string(tmp) + " ";
        spaced += str.substr(i, 8) + " ";
    }
    return spaced;
}

// Spaces string to 8 char sequences, then makes it decimal int
string dec_spaced(string str){
    int l = str.size();
    string spaced = "";
    for(int i = 0; i < l; i = i + 8){
        int tmp = stoi(str.substr(i, 8), nullptr, 2);
        spaced += to_string(tmp) + " ";
    }
    return spaced;
}

// Calculates entropy of the source
double src_entropy(Letter arr[]){
    double entropy = 0;
    for(int i = 0; i < 30; i++){
        entropy -= arr[i].probability *log2(arr[i].probability);
    }
    return entropy;
}

// Calculates L_avg
double L_avg(Letter arr[]){
    double length = 0;
    for(int i = 0; i < 30; i++){
        length += arr[i].probability *arr[i].huffman.length();
    }
    return length;
}

// Function converts int string back to 8 bit
string int_to_bin(string input){
    int length = input.size();
    int temp;
    string tmp = "";
    string binary_input = "";
    for(int i = 0; i < length; i++){
        if(input[i] != ' '){
            tmp.append(1,input[i]);
        }
        else{
            // cout<<tmp<< endl;
            temp = stoi(tmp);
            bitset<8> bset(temp);
            binary_input.append(bset.to_string());
            binary_input.append(" ");
            // cout<<temp<<endl;
            // cout<< binary_input << endl;
            tmp = "";
        }
    }
    return binary_input;
}

// Decode string using Huffman Code
string decoder(string input, Letter arr[]){
    int length = input.size();
    string tmp = "";
    string decrypt = "";
    for(int i = 0; i < length; i++){
        if(input[i] != ' '){
            tmp.append(1,input[i]);
        }
        for(int j = 0; j < 30; j++){
            if(tmp == arr[j].huffman){
                char letter = arr[j].ascii;
                decrypt.push_back(letter);
                tmp = "";
            }
        }
    }
    return decrypt;
}

bool compare_probs(Letter p1, Letter p2){
    return (p1.probability > p2.probability);
}



int main(){
    // Input from File
    ifstream ins;
    ins.open("grail_testfile.txt");
    stringstream stream;
    stream << ins.rdbuf();
    string src_str = stream.str();
    ins.close();

    // cout << src_str << src_str.length() == 61,392 << "\n"; // Input check
    // cout << src_str.length() << endl;

    // Define array of Letters -> add ascii member
    Letter array[30];
    array[0].ascii = 03;            // “end of text (etx)”
    array[1].ascii = 10;            // "line feed (lf)""
    array[2].ascii = 13;            // “carriage return (cr)”
    array[3].ascii = 32;            // “space (sp)”
    for(int i = 4; i < 30; i++){    // lowercase a-z
        array[i].ascii = 93 + i;    
        // cout<<array[i].ascii<<"\n";
    }

    // Calculate Individual Symbol Probabilities
    for(int i = 0; i < 30; i++){
        array[i].occurances = count(src_str.begin(), src_str.end(), array[i].ascii);
        array[i].probability = array[i].occurances/61392.0;
    }

    // Reorder by decreasing probability
    sort(array, array + 30, compare_probs);

    // Use pq with min ordering on char probability
    priority_queue <Nodes*, vector<Nodes*>, Comparator > pq;
    for(int i = 0; i<30; i++){
        Nodes *tmp = new Nodes(array[i].ascii, array[i].probability);
        pq.push(tmp);
    }
    
    // Make Huffman tree and return root node
    Nodes* root = Huffman_Tree(pq);

    // Generate Huffman encoding
    Huffman_Coding(root, array, "");

    // Output Source Symbols with Huffman Encoding
    print_table(array);

    // Encode original text
    string coded_str = encoder(src_str, array);

    // Space output into 8 bit sequence and convert to decimal
    string encoded_final_bin = spaced(coded_str);
    string encoded_final = dec_spaced(coded_str);

    // Output Length of final encoding
    cout << "\nLength Of Compressed Decimal Output File : " << encoded_final.size() << " bits\n";
    cout << "\nLength Of Compressed Binary Output File: " << encoded_final_bin.size() << " bits\n";
    cout << "\nDivide by 8 = Length Compressed Binary File : " << encoded_final_bin.size()/8 << " bytes\n";

    // Output encoded text to file
    ofstream output_file;
    output_file.open("Huffman_Encoded.txt");
    output_file << encoded_final;
    output_file.close();
    output_file.open("Huffman_Encoded_Bin.txt");
    output_file<< encoded_final_bin;
    output_file.close();

    // Output Source Entropy
    cout << "\nEntropy of the source : " << src_entropy(array) << " bits/bit\n";
    
    // Output L_avg
    cout << "\nRatio of output bits per char (i.e. L_avg) : " << L_avg(array) <<  " bits/bit\n";

    // Open encoded text file
    ins.open("Huffman_Encoded.txt");
    stringstream streams;
    streams << ins.rdbuf();
    string input = streams.str();
    ins.close();

    // Convert encoded input decimals to binary
    string encoded_input = int_to_bin(input);

    // Decode encoded text using Huffman Encoding
    string decoded = decoder(encoded_input, array);
    decoded.pop_back(); // Remove final char

    // Check reconstructed message matches original source message
    if(decoded == src_str){ cout << "\nSUCCESS! Both texts match.\n" ;}
    else{ cout << "\nERROR: Text Misconstruction\n" ;}
}
