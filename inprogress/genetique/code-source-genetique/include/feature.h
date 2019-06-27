// Feature (phenotype)
#ifndef FEATURE_H
#define FEATURE_H

#include<string>
#include<vector>
#include<iostream>

class Feature {

  public :

    // D : Discrete (ex : color of the eyes) C : Continuous value of the trait (ex : height)
    enum Nature { D = 0 , C = 1, Undefined = 2 } ;
    Nature stringToEnum( std::string ) ; 
    std::string enumToString( Nature ) ; 

    void print_debug() ;

    void setName( std::string name ) { name_ = name ; }
    void setNature( std::string nature ) { nature_ = stringToEnum( nature ) ; }
    bool setNumGenes( int nGenes ) ;
  private :

    std::string name_ ;
    Nature nature_  ;
    //Raw phenotype value lies in the interval [-1:1]. Have to be rescaled my min/max to represents a quantity with a meaning and unit.
    double value_ ;
    //The value of the phenotype must lie in the interval [valueMin_:valueMax_]
    double valueMin_ ;
    double valueMax_ ;
    //Number of genes to code for the trait (by default 1 if nature_ = 0 )
    unsigned int nGenes_ ;
    unsigned int nAlleles_ ;
    double codominanceTable_[ 20 ][ 20 ] ;
} ;


#endif // FEATURE_H
