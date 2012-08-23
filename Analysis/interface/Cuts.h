#ifndef Cuts_h
#define Cuts_h

#include "TCut.h"


class Cuts {
 public:

  Cuts();
  ~Cuts();

  void addCut(std::string name, std::string cut);

  TCut cut(int i);

  TCut allCuts();


 private:
  
  std::vector<std::string> names_;
  std::vector<TCut> cuts_;

};

#endif
