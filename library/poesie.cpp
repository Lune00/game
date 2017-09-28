#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<algorithm>

#define mylibrary "mylibrary.txt"

using namespace std;



//Les doublons sont gérés (normalement) par la librairie elle-meme.
//Inutile donc en pratique de devoir traiter le cas des doublons ici.

//TODO
//Travailler sur infover (comment il fonctionne, comment on le splitte)
//Brancher le travail sur la conjugaison pour les verbes

//Creer des systmes de templates de phrase, vers
//Unite phrase, reliees les unes aux autres mais pas completement random
//Le registre doit etre attachee a des formes de vers, ou enchainements
//Reflechir a un systeme de probabilité conditionelle (je tire cette phrase, quelle chance de tirer l'autre)
//Commencer simple avec des paragraphes (4 phrases, vers max) fixés
//Ne pas oublier de partir du titre (un nom + un adjectif + un registre + un theme)
//Chercher synonymes, lies, et random



// Global functions
vector<std::string> parsestring(string, string);

vector<std::string> parsestring(string stringtoparse, string delimiter)
{
  vector<std::string> tokens;
  size_t pos = 0 ;
  string token;
  while( (pos = stringtoparse.find(delimiter)) != std::string::npos)
  {
    token = stringtoparse.substr(0, pos);
    stringtoparse.erase(0, pos + delimiter.length());
    tokens.push_back(token);
  }
  //On rajoute la derniere entree (ce qu'il reste)
  tokens.push_back(stringtoparse);

  //On retire les caracteres vides (dus au formatage en bash)
  //On place tous les caracteres vides a la fin de string et on se place au premier caractere vide
  //puis on supprime jusqu'a la fin (algorithm)
  for(vector<std::string>::iterator it = tokens.begin() ; it != tokens.end() ; it++ ){
    it->erase(std::remove_if(it->begin(), it->end(), ::isspace), it->end());
  }

  return tokens;
}


// Class / Templates

class Mot{

  private:

    string mot_;
    string lemme_;
    string phon_;
    string infover_;
    string genre_;
    string accord_;
    int nsyll_;

    std::vector<string> grammar_;
    std::vector<string> registre_;
    std::vector<string> syn_;
    std::vector<string> ass_;
    std::vector<string> themes_;

  public:
    Mot();
    Mot(string);
    ~Mot();
    string getmot() {return mot_;}
    string getgenre() {return genre_;}
    string getaccord() {return accord_;}
    vector<string> getgrammar() { return grammar_;}
    string getlastphoneme() {char last = phon_.back(); string last_s(1,last); return last_s;}
};

Mot::Mot(){};

//Constructeur d'un mot avec une entree de la bib
//L'ordre des champs est donne par le formatage de la librairie (libconfig.sh)
//Il doit y avoir 12 champs!
Mot::Mot(string entree)
{
  vector<string> tokens = parsestring(entree,"\t");

  if( tokens.size() < 12 ) {
    cerr<<"Entrée ne contient pas 12 champs!"<<endl;
    return;
  }

  mot_  = tokens[0];
  lemme_= tokens[1];
  phon_ = tokens[2];
  infover_ = tokens[4];
  genre_ = tokens[5];
  accord_= tokens[6];

  if(tokens[7].length() == 0 ) 
  {
    nsyll_ = 0 ; //WTF, ca ne devrait pas etre autorise dans librairie!
  }
  else
  {
    nsyll_ = stoi(tokens[7]);
  }
  //Vecteur et split
  string delimiter = ";";
  grammar_ = parsestring(tokens[3],",");
  registre_ = parsestring(tokens[8],delimiter);
  syn_ = parsestring(tokens[9],delimiter);
  ass_ = parsestring(tokens[10],delimiter);
  themes_ = parsestring(tokens[11],delimiter);
};

Mot::~Mot(){};

//Articles definition: ms fs mfp
//On va ranger des attributs et des fonctions generales dans une classe
class bib
{
  public:
    bib();

    //Variables:
    static const string phon_table[];
    static const string grammar_table[];
    static const string definis[];
    static const string indefinis[];
    //Fonctions:
    static bool check_phonetique(string);
    static bool check_grammar(string);
    static vector<Mot> return_grammar_liste(vector<Mot>&,string);
    static vector<Mot> return_last_phon_liste(vector<Mot>&,string);
    static Mot randomMot(vector<Mot>&);
    static string returnArticle(Mot&,string);
};

const string bib::phon_table[]={
  "a","i","y","u","o","O","e","E","°","2","9","5","1","@","§","3","j","8","w","p","b","t","d","k","g","f","v","s","z","Z","m","n","N","I","R","x","G","S","l"};

const string bib::grammar_table[]={"NOM","VER","ADJ"};

//0 masculin singulier/ 1 feminin singulier / 2 pluriel
const string bib::definis[]= {"le","la","les"};
const string bib::indefinis[]= {"un","une","des"};

//Checker la phonetique pour une phoneme donee (la phonetique de la librairie est deja checkee par la librairie)
//en comparant a phon_table (39 phonemes, a verifier)
bool bib::check_phonetique(string phoneme){
  bool isfound = false;
  for(unsigned int i = 0 ; i != 39 ; i++)
  {
    if ( phoneme == bib::phon_table[i] ) { isfound = true; break ;}
  }
  return isfound;
}

//Check classe grammaticale
bool bib::check_grammar(string grammar){
  bool isfound = false;
  for(unsigned int i = 0 ; i != 3 ; i++)
  {
    if ( grammar == bib::grammar_table[i] ) { isfound = true; break ;}
  }
  return isfound;
}

//Fonction qui renvoie un vecteur de mots qui se terminent par la phoneme phoneme
vector<Mot> bib::return_last_phon_liste(vector<Mot>& corpus, string phoneme)
{
  vector<Mot> liste;
  if( !bib::check_phonetique(phoneme) ) return liste;
  for(vector<Mot>::iterator it = corpus.begin(); it != corpus.end() ; it++){
    if( it->getlastphoneme() == phoneme) liste.push_back(*it);
  }
  return liste;
}

//Renvoie les mots d'une classe grammaticale donnee
vector<Mot> bib::return_grammar_liste(vector<Mot>& corpus, string grammar)
{
  vector<Mot> liste;
  if( !bib::check_grammar(grammar) ) return liste;
  for(vector<Mot>::iterator it = corpus.begin(); it != corpus.end() ; it++){
    //Recupere vector grammar
    vector<string> vgrammar = it->getgrammar();
    for(vector<string>::const_iterator it2 = vgrammar.begin() ; it2 != vgrammar.end(); it2++){
      if( *it2 == grammar){
	liste.push_back(*it);
	break;
      }
    }
  }
  return liste;
}

//Affiche sur la sortie standard les mots d'un vecteur de mots
void affiche_mots(vector<Mot>& liste){
  for(vector<Mot>::iterator it = liste.begin(); it != liste.end() ; it++){
    cout<<it->getmot()<<endl;
  }
}

//Renvoie un mot au hasard dans un vecteur de mot
//WARNING pas un bon hasard (provisoire)
Mot bib::randomMot(vector<Mot>& liste){
  int randomIndex = rand () % liste.size() ;
  return liste[randomIndex];
}


//Prend le mot et le type (défini, indéfini, partitif, démonstratif...)
// Defini: le la les
// Indefini: un une des
// Partitif: du de la des
//Si defini, regarde premiere lettre du mot, si voyelle ou h eliser
string bib::returnArticle(Mot& mot, string type){
  if(mot.getgrammar() != "NOM") return string();
  //A completer
  return strin();
}
// MAIN

int main(){

  srand (time(NULL));

  cout<<"Bonjour poésie."<<endl;
  std::vector<Mot> corpus;
  ifstream mylib(mylibrary);


  //Lecture de la librairie et construction du corpus
  while(true){
    string entree;
    getline(mylib,entree);
    if (mylib.eof() ) break;
    //Chaque entree (entree) de la librairie est contenue dans entree
    Mot mot = Mot(entree);
    corpus.push_back(mot);
  };

  //Tests:

  //vector<Mot> liste = return_last_phon_liste(corpus,"@");
  vector<Mot> liste_ver = bib::return_grammar_liste(corpus,"VER");
  vector<Mot> liste_nom = bib::return_grammar_liste(corpus,"NOM");
  vector<Mot> liste_adj = bib::return_grammar_liste(corpus,"ADJ");

  //affiche_mots(liste);
  Mot nom = bib::randomMot(liste_nom);
  Mot adj = bib::randomMot(liste_adj);

  //Faire une classe qui gere les articles (fem/mas/demonstratifs/L' au lieu de l'...)
  //IL prend en entree la Mot (genre, le nombre et premiere lettre) et la nature(défini par l'utilisateur)
  if ( nom.getgenre() == "m") 
  {
    cout<<"Le "<<nom.getmot();
  }
  else
  {
    cout<<"La "<<nom.getmot();
  }
  cout<<"."<<endl;

}
