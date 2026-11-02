//
// Created by almar on 05/10/2026.
//

#ifndef PRACTICA2_EEDD_CONTROLPARQUES_H
#define PRACTICA2_EEDD_CONTROLPARQUESH
#include <list>
#include <map>
#include <vector>

#include "Parque.h"
using namespace std;
class ControlParques {
private:
    list<Parque> parques;
    vector<Especie> variantes;
    map<string,Arbol> arboles;
    std::multimap<string, Arbol*> treeName;

public:
    ControlParques(const string &nomFichEspecies, const string &nomFichParques, const string &nomFiceroArbol);    //Hay que cargar los ficheros aqui
    ~ControlParques();


    Parque *buscarParque(const string &nombre);
    vector<Especie>& listadoEspecieSubNombre(const string &cadena);
    vector<Especie*>& getVariantes();
    list<Parque>& getparques();
    map<string, Arbol>& getArboles();


    Arbol *buscarArbol(const string &asnum);
    void eliminarParque(const string &nombre);
    vector<Arbol*>* listaArbolesParque(const string &nombre_parque);

    vector<Arbol*>& buscarArbolesRango(const string &asNumMin, const string &asNumMax);

    Arbol *getArbolMasAlto();
    bool buscar_tipo_planta(const TipoPlanta &tipo_planta,  vector<TipoPlanta> tipos);
    unsigned int contarEspecies(const string &nombreparque);

    unsigned int contarTipos();

};


#endif //PRACTICA2_EEDD_CONTROLPARQUES_H