#include <sstream>
#include "util.h"
#include "clothing.h"

using namespace std;

Clothing::Clothing(const std::string name, double price, int qty, std::string size, std::string brand) 
  : Product("clothing", name, price, qty),
  size_(size),
  brand_(brand) 
{}

Clothing::~Clothing() {}

// returns the appropriate keywords to index the product
std::set<std::string> Clothing::keywords() const {
  // the words in the brand should be searchable keywords
  set<string> keys = parseStringToWords(name_);
  set<string> brand = parseStringToWords(brand_);
  keys.insert(brand.begin(), brand.end());
  return keys;
}


// create a string that contains the product info
std::string Clothing::displayString() const {
  stringstream ss;

  ss << name_ << endl;
  ss << "Size: " << size_;
  ss << " Brand: " << brand_ << endl;
  ss << price_ << " " << qty_ << " left.";

  return ss.str();
}

// outputs the database format of the product info
void Clothing::dump(std::ostream& os) const {
  Product::dump(os);
  os << size_ << endl;
  os << brand_ << endl;
}