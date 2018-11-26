#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

// use a map to order products by 
// ascending value. If more than 1 product has
// the same value will only keep the first one
using ProductMap = map<int, string>;

// define a map to hold all the products
// we read from the file
ProductMap _products;

// parses a line from a file with with products defined
// as "product,price"
void parseLine(const string& line, int balance) 
{
    size_t pos = line.find_first_of(",");
    string name, value;
    if (pos != string::npos)
    {
        name = line.substr(0, pos);
        cout << "Product: " << name;
        pos = line.find_first_not_of(" ", pos+1);

        if (pos != string::npos) 
        {
            size_t endpos = line.find_last_not_of(" \t\f\v\n\r");
            int value = stoi(line.substr(pos, endpos-pos+1));
            cout << " Value: " << value << endl;
            // we are only interested in products below the balance
            if (value < balance)
                _products.insert(ProductMap::value_type(value, name));
        }
    }
}

// parses a file with product prices and prints 
// out the closest pair <= balance.
void parseFile(const string& file, int balance)
{
    // open the file
    std::ifstream input(file);

    if (input.is_open())
    {
        // read file line by line
        for( std::string line; getline( input, line);)
        {
            parseLine(line, balance);
        }
    } 
    else 
    {
        cout << "Could not open file " << file << endl;
    }
}

// look for a pair of products closest or equal to the
// balance
void findPair(int balance)
{
    int prod1 = 0, prod2 = 0, bestSum = 0;
    for (auto current : _products)
    {
        for (auto value = _products.rbegin(); value != _products.rend(); value++)
        {
            // donot compar with self
            if (*value == current) continue;
            
            int sum = current.first + value->first;
            if (sum == balance) 
            {
                // we found one with exact match
                cout << current.second << " " 
                                       << current.first << ", " 
                                       << value->second << " " 
                                       << value->first << endl;
                return;
            } 
            else if (sum < balance)
            {
                if (sum > bestSum) 
                {
                    prod1 = current.first;
                    prod2 = value->first;
                    bestSum = sum;
                }
            }
        }
    }

    // if we have something close to the balance then print it.
    if (bestSum > 0) {
        auto p1 = _products.find(prod1);
        auto p2 = _products.find(prod2);

        cout << p1->second << " " 
                            << p1->first << ", " 
                            << p2->second << " " 
                            << p2->first << endl;
    } 
    else
    {
        cout << "Not possible for balance: " << balance << endl;
    }
}

// reads the parameters passed by the caller and
// calls a function to parse the input file
int main(int argc, char** argv)
{
    if (argc < 3)
    {    
        cout << "Usage: FindPair <filename> <balance>" << std::endl;
    }

    // parse file and keep all the products that have value
    // below the balance
    auto balance = stoi(argv[2]);
    parseFile(argv[1], balance);

    findPair(balance);

    return 0;
}

