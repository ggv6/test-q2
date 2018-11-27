#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <memory>

using namespace std;

// a type to hold product information
struct Product 
{
    Product(const string& n, int v) : name{n}, value{v} {}
    string name;
    int value;
};

// a pair type to hold 2 products
class ProductPair: public pair< shared_ptr<Product>, shared_ptr<Product> >
{
public:
    using pair::pair;
    int total() 
    {
        if (this->first && this->second)
            return (this->first->value + this->second->value);
        return 0;
    }
};

// use a map to order products by 
// ascending value. If more than 1 product has
// the same value will only keep the first one
using ProductMap = map<int, shared_ptr<Product>>;

// define a variable to hold all the products
// we read from the file
ProductMap _products;

// define a variable to hold the closest match to 
// the balance we have found so far
ProductPair _selectedProducts;

// used for counting how many times we compare pairs
// of products
int _numOfComparisons = 0;
// used to count how many lines we parsed
int _numOflinesParsed = 0;


// look for a pair of products in a list starting from iterator then descending on values
// with sum closest or equal to balance
ProductPair findPairInList(shared_ptr<Product> product, int balance, ProductMap::iterator iterator)
{
    ProductPair foundPair;
    foundPair.first = nullptr;
    foundPair.second = nullptr;

    // as our iterator uses lower bounds we need to start decrementing
    // so we test against all products that have a value < balance - product.value
    for (auto current = make_reverse_iterator(iterator); current != _products.rend(); current++)
    {
        _numOfComparisons++;

        int sum = current->second->value + product->value;
        if (sum == balance) 
        {
            // we found one with exact match
            foundPair.first = current->second;
            foundPair.second = product;                      
            return foundPair;
        } 
        else if (sum < balance)
        {
            // if we found a better pair than the one we hold
            // then keep it.
            if (sum > foundPair.total()) {
                foundPair.first = current->second;
                foundPair.second = product; 
            }
        }
    }

    return foundPair;
}

// parses a line from a file with with products defined
// as "product,price"
bool parseLine(const string& line, string& name, int& value)
{
    // parse the line first if possible
    size_t pos = line.find_first_of(",");
    if (pos == string::npos) return false;
    name = line.substr(0, pos);

    pos = line.find_first_not_of(" ", pos+1);
    if (pos == string::npos) return false;
    size_t endpos = line.find_last_not_of(" \t\f\v\n\r");
    value = stoi(line.substr(pos, endpos-pos+1));

    _numOflinesParsed++;

    return true;
}

// returns true if an exact sum of prod1+prod2 pair is found
// with value == balance
bool findMatchingPair(const string& line, int balance) 
{
    // parse the file line first
    string name;
    int value = 0;
    if (!parseLine(line, name, value)) return false;

    // now start comparing the new porduct with existing ones
    // we are only interested in products below the balance
    if (value < balance) {
        auto product = make_shared<Product>(name, value);
        // we only want to check products that their
        // value < balance - current product value.
        // add 1 to ensure we get the products == balance - value
        auto iterator = _products.lower_bound((balance - value + 1));

        // now look for the best pair in the list using the product
        // which we just parsed
        auto pairFound = findPairInList(product, balance, iterator);

        // now we are done with comparisons ass product to map
        _products.insert(ProductMap::value_type(value, product));

        if (pairFound.first && pairFound.second) // if we got a valid pair back
        {
            // if we found a pair that its value == balance
            if (pairFound.total() == balance)
            {
                _selectedProducts = pairFound;
                return true;
            }
            // check in case our new pair is any better than the one we hold
            // and if it is then keep it
            else if (pairFound.total() >_selectedProducts.total())
            {
                _selectedProducts = pairFound;
            }
        }
    }

    return false;
}

// parses a file with product prices and prints 
// out the closest pair <= balance.
void findProducts(const string& file, int balance)
{
    // open the file
    std::ifstream input(file);

    if (input.is_open())
    {
        // read file line by line
        for( std::string line; getline( input, line);)
        {
            // if we found an exact match then return
            if (findMatchingPair(line, balance))
                return;
        }
    } 
    else 
    {
        cout << "Could not open file " << file << endl;
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
    findProducts(argv[1], balance);

    cout << "n=" << _numOfComparisons << " l=" << _numOflinesParsed << endl;

    // if we found a match then print details
    if (_selectedProducts.first && _selectedProducts.second)
    {
        cout << _selectedProducts.first->name << " " 
                            << _selectedProducts.first->value << ", " 
                            << _selectedProducts.second->name << " " 
                            << _selectedProducts.second->value << endl;
    } 
    else
    {
        cout << "Not possible for balance: " << balance << endl;
    }

    return 0;
}

