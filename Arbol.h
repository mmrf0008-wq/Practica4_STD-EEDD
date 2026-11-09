
#ifndef PRACTICA2_EEDD_ARBOL_H
#define PRACTICA2_EEDD_ARBOL_H
#include <iostream>
#include "Especie.h"
class Parque;           //El include no funciona. (Forward Declaration)
using namespace std;

class Arbol {
private:
    string assetnum;
    int num_parque;
    int num_distrito;
    string nombre_distrito;
    int num_barrio;
    string nombre_barrio;
    double x;
    double y;
    float perimetro;
    float altura;

    Especie *especie_arbol;    //Cada arbol tiene 1 sola especie
    Parque *isInto;             //Cada arbol pertenece a 1 parque
public:
    Arbol();
    Arbol(const string &assetnum, int num_parque, int num_distrito,
        const string &nombre_distrito, int num_barrio, const string &nombre_barrio,
        double x, double y, float perimetro, float altura, Especie *especie);

    Arbol(const string &assetnum, int num_parque, int num_distrito,
     const string &nombre_distrito, int num_barrio, const string &nombre_barrio,
     double x, double y, float perimetro, float altura);
    Arbol(const Arbol &orig);
    ~Arbol();

    Arbol &operator=(const Arbol &arbol);
    bool operator>(const Arbol &arbol);
    bool operator<(const Arbol &arbol);
    bool operator==(const Arbol &arbol);

    void setEspecie(Especie *e);
    Especie* getEspecie() const;

    string getCodigoEspecie() const;
    string getNombreComun() const;
    void setAsNum(const string &asNum);

    float get_altura() const;
    string getAssetNum() const;

    int get_num_parque() const;

    string getNombreCientifico() const;
    string getTipoEspecie() const;

    void setParque(Parque *parque);
    Parque* getParque()const;

    [[nodiscard]] int get_num_barrio() const;

    void set_num_barrio(int num_barrio);
};


#endif //PRACTICA2_EEDD_ARBOL_H
