#include <sstream>
#include "util.h"
#include "book.h"

using namespace std;

Book::Book(const std::string name, double price, int qty, std::string isbn, std::string author) 
  : Product("book", name, price, qty),
  isbn_(isbn),
  author_(author) 
{}

Book::~Book() {}

// returns the appropriate keywords to index the product
std::set<std::string> Book::keywords() const {
  // the words in the author’s name should be searchable keywords 
  // as well as the book’s ISBN number
  set<string> keys = parseStringToWords(name_);
  set<string> authors = parseStringToWords(author_);
  keys.insert(authors.begin(), authors.end());
  keys.insert(convToLower(isbn_));
  return keys;
}


// create a string that contains the product info
std::string Book::displayString() const {
  stringstream ss;

  ss << name_ << endl;
  ss << "Author: " << author_;
  ss << " ISBN: " << isbn_ << endl;
  ss << price_ << " " << qty_ << " left.";

  return ss.str();
}

// outputs the database format of the product info
void Book::dump(std::ostream& os) const {
  Product::dump(os);
  os << isbn_ << endl;
  os << author_ << endl;
}