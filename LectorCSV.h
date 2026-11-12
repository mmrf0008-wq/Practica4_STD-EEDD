#ifndef LECTOR_CSV_H
#define LECTOR_CSV_H

#include <cstddef>
#include <list>
#include <map>
#include <string>


#include "Especie.h"
#include "Parque.h"

// Lee el fichero data/arbolado-especies.csv: valores separados por comas,
// donde un valor puede ir entrecomillado y contener comas y comillas
// escapadas ("") dentro de las comillas (formato tipo RFC4180). Asume que
// cada fila lógica ocupa una única línea física del fichero.
//
// No almacena las especies leídas: cada una se construye y se muestra por
// pantalla en el momento en que se lee, y se descarta a continuación. No
// utiliza std::vector ni ninguna otra estructura de datos en ningún punto.
class LectorCSV {
public:
    bool static  cargarEspecies(vector<Especie> &vector, const std::string& rutaFichero);
    bool static cargarParques(list<Parque> &parques, const std::string &rutaFichero);

    bool  cargosArbol(map<string,Arbol> &arboles, const std::string &rutaFichero, list<Parque> &parques, vector<Especie> &especies, multimap<string,Arbol*> &treeName);

    std::size_t numeroEspecies() const;
    std::size_t filasDescartadas() const;

private:
    std::size_t static numeroEspecies_;
    std::size_t static filasDescartadas_;

    static std::size_t contarCampos(const std::string& linea);
    static std::string extraerCampo(const std::string& linea, std::size_t indiceObjetivo);
    static void eliminarBOM(std::string& linea);
    static void mostrarEspecie(const Especie& especie);
     float leerMedida(string campo);
     Arbol leerArbol_csv(const string &linea,  vector<Especie> &especies) ;
     Especie* buscaEspecie(const string &cod, vector<Especie> &especies);


};

#endif // LECTOR_CSV_H
