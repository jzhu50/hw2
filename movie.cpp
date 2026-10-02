#include <sstream>
#include "util.h"
#include "movie.h"

using namespace std;

Movie::Movie(const std::string name, double price, int qty, std::string genre, std::string rating) 
  : Product("movie", name, price, qty),
  genre_(genre),
  rating_(rating) 
{}

Movie::~Movie() {}

// returns the appropriate keywords to index the product
std::set<std::string> Movie::keywords() const {
  // the movie’s genre should be a searchable keyword
  set<string> keys = parseStringToWords(name_);
  keys.insert(convToLower(genre_));
  return keys;
}


// create a string that contains the product info
std::string Movie::displayString() const {
  stringstream ss;

  ss << name_ << endl;
  ss << "Genre: " << genre_;
  ss << " Rating: " << rating_ << endl;
  ss << price_ << " " <<  qty_ << " left.";

  return ss.str();
}

// outputs the database format of the product info
void Movie::dump(std::ostream& os) const {
  Product::dump(os);
  os << genre_ << endl;
  os << rating_ << endl;
}