#include "mydatastore.h"
#include "util.h"
#include <sstream>

using namespace std;

MyDataStore::MyDataStore() {

}
MyDataStore::~MyDataStore() {
  for(unsigned int i = 0; i < products_.size(); i++) {
    delete products_[i];
  }
  
  for(map<string, User*>::iterator it = users_.begin(); it != users_.end(); ++it) {
    delete it->second;
  }
}

void MyDataStore::addProduct(Product* p) {
  // store the product
  products_.push_back(p);

  // update keywordMap
  set<string> keys = p->keywords();
  for(set<string>::iterator it = keys.begin(); it != keys.end(); ++it) {
    keywordMap_[convToLower(*it)].insert(p);
  }
}

void MyDataStore::addUser(User* u) {
  string username = convToLower(u->getName());
  users_[username] = u;
  carts_[username] = vector<Product*>();
}

vector<Product*> MyDataStore::search(std::vector<std::string>& terms, 
  int type)
{
  // type 1 == OR, type 0 == AND
  vector<Product*> res;

  if(terms.size() == 0) {
    return res;
  }

  set<Product*> matches;
  
  // AND
  if(type == 0) {
    // track if first term
    bool first = true;

    for (unsigned int i=0; i<terms.size(); i++) {
      string term = convToLower(terms[i]);

      map<string, set<Product*>>::iterator it = keywordMap_.find(term);

      if(it == keywordMap_.end()) {
        matches.clear();
        break;
      }

      if(first) {
        matches = it->second;
        first = false;
      } else {
        matches = setIntersection(matches, it->second);
      }
    }
  }
  // OR
  else if(type == 1) {
    for (unsigned int i=0; i<terms.size(); i++) {
      string term = convToLower(terms[i]);

      map<string, set<Product*>>::iterator it = keywordMap_.find(term);

      if(it != keywordMap_.end()) {
        // if found
        matches = setUnion(matches, it->second);
      }
    }
  }

  for(set<Product*>::iterator it = matches.begin(); it != matches.end(); ++it) {
    res.push_back(*it);
  }

  return res;
}

void MyDataStore::dump(std::ostream& ofile) {
  ofile << "<products>" << endl;
  for(unsigned int i=0; i<products_.size(); i++) {
    products_[i]->dump(ofile);
  }
  ofile << "</products>" << endl;

  ofile << "<users>" << endl;
  for(map<string, User*>::iterator it = users_.begin(); it != users_.end(); ++it) {
    // depending on the type of it->second, displays diff output
    it->second->dump(ofile);
  }
  ofile << "</users>" << endl;
}

bool MyDataStore::addToCart(std::string username, Product* p) {
  username = convToLower(username);

  if(users_.find(username) == users_.end()) {
    return false;
  }

  carts_[username].push_back(p);

  return true;
}

void MyDataStore::viewCart(std::string username) {
  username = convToLower(username);

  // invalid username
  if(users_.find(username) == users_.end()) {
    cout << "Invalid username" << endl;
    return;
  }

  vector<Product*>& cart = carts_[username];
  for (unsigned int i=0; i < cart.size(); i++) {
    cout << "Item " << i + 1 << endl;
    cout << cart[i]->displayString() << endl;
    cout << endl;
  }
}

void MyDataStore::buyCart(std::string username) {
  username = convToLower(username);

  // invalid username
  map<string, User*>::iterator it = users_.find(username);

  if(it == users_.end()) {
    cout << "Invalid username" << endl;
    return;
  }

  User* user = it->second;
  vector<Product*>& cart = carts_[username];
  vector<Product*>::iterator c = cart.begin();
  
  while(c != cart.end()) {
    Product* product = *c;

    // check if item is in cart && if user has enough money
    if (product->getQty() > 0 && user->getBalance() >= product->getPrice()) {
      // stock qty -1
      product->subtractQty(1);
      // user's balance -= product price
      user->deductAmount(product->getPrice());
      // remove product from cart
      c = cart.erase(c);
    }
    else {
      // move iterator over to the next product
      c++;
    }
  }
}