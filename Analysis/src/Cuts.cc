#include "InvisibleHiggs/Analysis/interface/Cuts.h"


Cuts::Cuts() {


}

Cuts::~Cuts() {


}


void Cuts::addCut(std::string name, std::string cut) {

  names_.push_back(name);
  TCut c(cut.c_str());
  cuts_.push_back(c);

}


TCut Cuts::cut(int i) {
  return cuts_.at(i);
}


TCut Cuts::allCuts() {

  TCut tmp;
  for (std::vector<TCut>::const_iterator c=cuts_.begin(); c!=cuts_.end(); ++c) {
    tmp = tmp && (*c);
  }
  return tmp;

}

// Cuts::dump(std::ostream o) {

  


// }
