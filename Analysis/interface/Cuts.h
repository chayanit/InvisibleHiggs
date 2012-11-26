#ifndef Cuts_h
#define Cuts_h

#include "TCut.h"


class Cuts {
 public:

  Cuts();
  ~Cuts();

  void addCut(std::string name, std::string cut);

  TCut cut(unsigned i);

  TCut allCuts();

  TCut nMinusOneCuts(unsigned i);

 private:
  
  std::vector<std::string> names_;
  std::vector<TCut> cuts_;

};

#endif
