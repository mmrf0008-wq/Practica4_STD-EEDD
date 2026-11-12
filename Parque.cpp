//
// Created by almar on 05/10/2026.
//

#include "Parque.h"

#include <algorithm>
#include <map>
#include <vector>
/**
 * @brief Constructor por defecto de la clase Parque.
 * Inicializa el código de parque a 0 y el nombre a un espacio en blanco.
 */
Parque::Parque():codigo_parque(0),nombre_parque(" ") {}

/**
 * @brief Constructor parametrizado con un contenedor inicial de especies.
 * @param codigo Código identificador del parque.
 * @param nombre Nombre del parque.
 * @param especies Vector dinámico con los punteros a los árboles a incluir en el parque.
 */
Parque::Parque(int codigo, const string &nombre, const vector<Arbol*> &especies):
            codigo_parque(codigo),nombre_parque(nombre),contenedor(especies) {
    ordenaArboles();
}

/**
 * @brief Constructor parametrizado básico.
 * @param codigo Código identificador del parque.
 * @param nombre Nombre del parque.
 */
Parque::Parque(const int &codigo, const string &nombre) {
    this->codigo_parque = codigo;
    this->nombre_parque = nombre;
}

/**
 * @brief Destructor de la clase Parque.
 * Libera la memoria del contenedor de árboles.
 */
Parque::~Parque() {
    contenedor.clear();
}

/**
 * @brief Constructor de copia de la clase Parque. Copia el código, el nombre y los punteros del contenedor (los árboles no se duplican).
 * @param orig Parque de origen a copiar.
 */
Parque::Parque(const Parque &orig) {

    this->nombre_parque = orig.nombre_parque;
    this->codigo_parque = orig.codigo_parque;

    for(int i =0; i < orig.contenedor.size(); i++ ) {
        this->contenedor.push_back(orig.contenedor[i]);
    }

}

/**
 * @brief Obtiene el código del parque.
 * @return int Código identificador del parque.
 */
int Parque::getCodigoParque() const {
    return codigo_parque;
}

/**
 * @brief Establece el código del parque.
 * @param codigo Nuevo código identificador.
 */
void Parque::setCodigoParque(int codigo) {
    this->codigo_parque = codigo;
}

/**
 * @brief Obtiene el nombre del parque.
 * @return string Nombre del parque.
 */
string Parque::getNombreParque() const {
    return nombre_parque;
}

/**
 * @brief Establece el nombre del parque.
 * @param nombre Nuevo nombre para el parque.
 */
void Parque::setNombreParque(const string &nombre) {
    this->nombre_parque = nombre;
}

/**
 * @brief Inserta un nuevo árbol en el contenedor del parque.
 * @param arbol Puntero al árbol que se va a insertar.
 */
void Parque::insertarArbol(Arbol *arbol) {
    if(arbol) this->contenedor.push_back(arbol);
}

/**
 * @brief Comprueba si existe en el parque un árbol con un determinado nombre común.
 * @param nombre Nombre común de la especie a buscar.
 * @return true Si existe al menos un árbol con ese nombre común, false en caso contrario.
 */
bool Parque::existeNComun(const string &nombre) {
    for(int i =0; i < this->contenedor.size(); i++) {
        if(this->contenedor[i]->getNombreComun() == nombre) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Busca y devuelve el árbol de mayor altura presente en el parque.
 * @return Arbol* Puntero al árbol más alto del parque.
 */
Arbol* Parque::arbolMasAlto() {

    Arbol *a = nullptr;

    for(int i =0; i < this->contenedor.size(); i++) {
        if(!a || this->contenedor[i]->get_altura() > a->get_altura()) {
            a= this->contenedor[i];
        }
    }
    return a;
}

/**
 * @brief Devuelve el número de especies distintas presentes en el parque.
 * @return unsigned int Cantidad de especies únicas almacenadas.
 */
unsigned int Parque::contarEspecies() {

    bool repetida= false;
    int cont=0;

    for(int i =0; i < this->contenedor.size(); i++) {
        repetida = false;
        for(int j = i+1; j < this->contenedor.size() && !repetida; j++) {
            if(this->contenedor[i]->getCodigoEspecie() == this->contenedor[j]->getCodigoEspecie()) {
                repetida = true;
            }
        }
        if(!repetida) cont++;
    }
    return cont;
}

/**
 * @brief Devuelve el número de tipos de planta distintos presentes en el parque.
 * @return unsigned int Cantidad de tipos de planta únicos almacenados.
 */
unsigned int Parque::contarTipos() {
   bool repetida= false;
    int cont=0;

    for(int i =0; i < this->contenedor.size(); i++) {
       repetida = false;
        for(int j = i+1; j < this->contenedor.size() && !repetida; j++) {
            if(this->contenedor[i]->getTipoEspecie() == this->contenedor[j]->getTipoEspecie()) {
               repetida = true;
            }
        }
        if(!repetida) cont++;
    }
    return cont;
}


/**
 * @brief Obtiene la referencia al contenedor dinámico con los árboles del parque.
 * @return VDinamico<Arbol>& Referencia al vector dinámico de punteros a objetos Arbol.
 */
vector<Arbol*>& Parque::get_contenedor() {return this->contenedor; }

/**
 * @brief Ordena los árboles del parque de menor a mayor según su número de asset.
 */
void Parque::ordenaArboles() {
    std::sort(contenedor.begin(), contenedor.end(),
        [](const Arbol* a, const Arbol* b) {
            return a->getAssetNum() < b->getAssetNum();
        });
}

/**
 * @brief Obtiene la especie más común (con más ejemplares) del parque.
 *
 * Cuenta los árboles agrupándolos por código de especie.
 * @return Puntero a la especie más común. Ante un empate se devuelve la que alcanza antes el máximo.
 * @throw std::out_of_range Si el parque no contiene ningún árbol.
 */
Especie* Parque::especieMasComun() const {
    if (contenedor.empty()) {
        throw out_of_range("[Parque::especieMasComun]: El vector esta vacio.");
    }
    map<string,int> contadores_especies;        //Almacena el codigo de especie de cada especie del contenedor
                                                // y un contador con el numero de repeticiones de cada una
    int maximo=0;                               //Contador de la especie mas repetida
    Especie* especie_mas_comun = nullptr;       //Especie mas repetida

    for (int i = 0; i < contenedor.size(); i++) {
        string codigo_de_especie = contenedor[i]->getCodigoEspecie();
        contadores_especies[codigo_de_especie]++;                   //Almacena el codigo de esta especie y incrementa su contador

        if (contadores_especies[codigo_de_especie] > maximo) {
            maximo = contadores_especies[codigo_de_especie];
            especie_mas_comun = contenedor[i]->getEspecie();
        }
    }
    return especie_mas_comun;
}

