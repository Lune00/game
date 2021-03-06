#include<list>
#include<cmath>
#include<algorithm>
#include<string>
#include<vector>
#include<map>

#include"Archiviste.hpp"
#include"Collectionneur.hpp"
#include"Texte.hpp"

using namespace std;


void initialise(Archiviste& a, Collectionneur& c){
  a.plugtoCollectionneur(c);
  c.plugtoArchiviste(a);
  a.buildThemesCollections();
}



int main()
{

  //Initiatlisation:
  Archiviste archiviste;
  Collectionneur collectionneur;
  initialise(archiviste,collectionneur);

  //TMP
  Texte texte;
  texte.print_template();

  //archiviste.afficher();
  //collectionneur.print();

  //Tests downcast (a partir pointeur Mot(classe mere), je peux acceder a la partie classe fille):

  //std::vector<Mot*> lib;
  //lib.push_back(new NomC("chocolat","Sokola",3,'m','s'));
  //lib.push_back(new Verbe("manger","manger",3));
  //for(vector<Mot*>::iterator it = lib.begin() ; it != lib.end(); it++){
  //  if(NomC* p = dynamic_cast<NomC*>(*it)) p->accorder("le");
  //}   

  //Test finds:
  //Mot * m = archiviste.findword("vaisseau","NOM");
  //m->affiche();

  //m = archiviste.findword("géant","ADJ");
  //m->affiche();

  //const Mot * mot = archiviste.randomword();
  //mot->affiche();
  Collection c = collectionneur.return_collection_by_name ( "navigation") ;
  c.print();
  

  return 0;
}
