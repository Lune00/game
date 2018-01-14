#include"Mot.hpp"

using namespace std;

Mot::Mot(std::string mot,std::string phon, int nsyll): mot_(mot), phon_(phon), nsyll_(nsyll)
{

}


Mot::~Mot(){

}

bool operator==(const Mot& a, const Mot& b){
  return ( a.mot_ == b.mot_);
}

void Mot::linksynonymes(Mot * linked){
  synonymes_.push_back(linked);
}
void Mot::linkassocies(Mot * linked){
  associes_.push_back(linked);
}

void Mot::affiche()const{
  cout<<"Mot : "<<mot_<<endl;
  cout<<"- synonymes :"<< synonymes_.size()<<endl;
  for(std::vector<Mot*>::const_iterator it = synonymes_.begin(); it != synonymes_.end();it++){
    cout<<(*it)->getmot()<<" "<<(*it)->nature()<<endl;
  }
  cout<<"- associes :"<<associes_.size()<<endl;
  for(std::vector<Mot*>::const_iterator it = associes_.begin(); it != associes_.end();it++){
    cout<<(*it)->getmot()<<" "<<(*it)->nature()<<endl;
  }
  cout<<endl;
}


