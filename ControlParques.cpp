//
// Created by almar on 05/10/2026.
//

#include "ControlParques.h"
#include <algorithm>
#include <ranges>

#include "LectorCSV.h"

/**
 * @brief Constructor de ControlParques. Carga las especies, los parques y los árboles desde los ficheros CSV y enlaza cada árbol con su parque.
 *
 * Tras cargar las especies se ordenan por código de especie.
 * @param nomFichEspecies Ruta del fichero CSV de especies.
 * @param nomFichParques Ruta del fichero CSV de parques.
 * @param nomFicheroArboles Ruta del fichero CSV de árboles.
 * @throw std::invalid_argument Si la ruta del fichero de especies o la de parques está vacía.
 * @throw std::runtime_error Si no se puede abrir alguno de los ficheros.
 */
ControlParques::ControlParques(const string &nomFichEspecies, const string &nomFichParques, const string &nomFicheroArboles) {
	if(nomFichEspecies.empty() || nomFichParques.empty()) {
		throw invalid_argument("[ControlParques]: parametros no validos ");
	}
	if(!LectorCSV::cargarEspecies(this->variantes, nomFichEspecies)) {
		throw std::runtime_error("[ControlParques:cargaEspecies]: error al abrir fichero ");
	}
	cout << "cargadas las especies " << endl;
	  ordenarEspecies();   //ordenamos por id de menor

	LectorCSV a;

	if(!LectorCSV::cargarParques(this->parques, nomFichParques)) {
		throw std::runtime_error("[ControlParques:cargaParques]: error al abrir fichero ");
	}
	cout << "cargados los parques" << endl;
	if(!a.cargosArbol(this->arboles, nomFicheroArboles, this->parques, this->variantes,treeName)) { // lectura de arboles y ligue entre arbol y parque
		throw std::runtime_error("[ControlParques:cargaArboles]: error al abrir fichero ");
	}
	cout << "Cargados los arboles" << endl;

}

/**
 * @brief Destructor de ControlParques. Vacía el vector de especies.
 */
ControlParques::~ControlParques() {
	this->variantes.clear();
}

/**
 * @brief Busca un parque por su nombre.
 * @param nombre Nombre exacto del parque.
 * @return Puntero al parque, o nullptr si no existe.
 */
Parque * ControlParques::buscarParque(const string &nombre) {
	for(list<Parque>::iterator i = this->parques.begin(); i != this->parques.end(); i++) {
		if(i->getNombreParque() == nombre) {
			return &i.operator*();
		}
	}
	return nullptr;
}

/**
 * @brief Obtiene las especies cuyo nombre científico contiene una subcadena.
 *
 * @note La memoria del vector devuelto debe liberarla el llamador.
 * @param cadena Subcadena a buscar en el nombre científico.
 * @return Referencia a un vector reservado con new con las especies seleccionadas.
 */
vector<Especie> & ControlParques::listadoEspecieSubNombre(const string &cadena) {

	vector<Especie> *listad = new vector<Especie>();
	for(int i =0; i < this->variantes.size(); i++) {
		if(this->variantes[i].get_nombre_cientifico().find(cadena)) {
			listad->push_back(this->variantes[i]);
		}
	}
	return *listad;
}

/**
 * @brief Obtiene el vector de especies cargadas.
 * @return Referencia al vector de especies (ordenado por código de especie).
 */
vector<Especie> & ControlParques::getVariantes() {
	return this->variantes;
}

/**
 * @brief Obtiene la lista de parques cargados.
 * @return Referencia a la lista de parques.
 */
list<Parque> & ControlParques::getparques() {
	return this->parques;
}

/**
 * @brief Obtiene el mapa de árboles cargados.
 * @return Referencia al mapa de árboles, cuya clave es el assetnum.
 */
map<string,Arbol> & ControlParques::getArboles() {
	return this->arboles;
}

/**
 * @def localiza un arbol a partir de su identificador
 * @param asnum
 * @return Devuelve el dato que estamos buscando
 */
Arbol *ControlParques::buscarArbol(const string &asnum) {
	map<string,Arbol>::iterator iterator=this->arboles.find(asnum);
	if(iterator == this->arboles.end()) return nullptr; //no se encontro
	return &iterator->second;
}

/**
 * @brief Elimina un parque del sistema dado su nombre.
 * @param nombre Nombre del parque a eliminar.
 */
void ControlParques::eliminarParque(const string &nombre) {
	bool terminar = false;

	for(list<Parque>::iterator i= this->parques.begin(); i!=this->parques.end() &&!terminar; i++) {
		if(i->getNombreParque() == nombre) {
			this->parques.erase(i);
			terminar=true;
		}
	}
}

/**
 * @brief Devuelve el listado completo de árboles de un parque dado su nombre.
 * @param nombre_parque Nombre del parque del cual se quiere obtener la lista de árboles.
 * @return VDinamico<Arbol>* Puntero al vector dinámico con los árboles del parque, o nullptr si el parque no existe.
 */
vector<Arbol*> * ControlParques::listaArbolesParque(const string &nombre_parque) {

	for(list<Parque>::iterator i = this->parques.begin(); i != this->parques.end(); i++) {
		if(i->getNombreParque() == nombre_parque) {
			i->ordenaArboles();

			return &i->get_contenedor();
		}
	}
	return nullptr;
}

/**
 * @brief Busca los árboles cuyo identificador numérico está en el rango [asNumMin, asNumMax].
 *
 * @note La memoria del vector devuelto debe liberarla el llamador; los árboles apuntados pertenecen al mapa.
 * @param asNumMin Cota inferior del rango (identificador como cadena numérica).
 * @param asNumMax Cota superior del rango (identificador como cadena numérica).
 * @return Referencia a un vector reservado con new con punteros a los árboles encontrados.
 * @throw std::invalid_argument Si asNumMin es mayor que asNumMax (comparados como cadenas).
 */
vector<Arbol *> & ControlParques::buscarArbolesRango(const string &asNumMin, const string &asNumMax) {
	vector<Arbol*> *nuevo = new vector<Arbol *>;

	if(asNumMin > asNumMax) {
		throw invalid_argument("[BUSCAR_ARBOLES_RANGO] se a introducido una cota inferior con valor mayor que la cota superior");
	}

	int min = stoi(asNumMin);
	int max = stoi (asNumMax);
	Arbol *aux;
	for(int i=min; i <= max; i++) {
		aux = this->buscarArbol(std::to_string(i));
		if(aux) nuevo->push_back(aux);
	}

	return *nuevo;
}

/**
 * @brief Obtiene el árbol de mayor altura de entre todos los parques.
 * @return Puntero al árbol más alto, o nullptr si ningún parque tiene árboles.
 */
Arbol *ControlParques::getArbolMasAlto() {

	Arbol *encontrado = nullptr;
	Arbol *aux; 

	for(list<Parque>::iterator i = this->parques.begin(); i != this->parques.end(); i++) {

		aux = i->arbolMasAlto();
		if((aux ) && (!encontrado || aux->get_altura() > encontrado->get_altura())) {
			encontrado = aux;
		}
	}
	return encontrado; 
}

/**
 * @brief Comprueba si un tipo de planta está presente en un vector de tipos.
 * @param tipo_planta Tipo de planta a buscar.
 * @param tipos Vector de tipos donde buscar.
 * @return true si el tipo está en el vector, false en caso contrario.
 */
bool ControlParques::buscar_tipo_planta(const TipoPlanta &tipo_planta,  vector<TipoPlanta> tipos) {
	for(int i =0; i < tipos.size(); i++) {
		if(tipos[i] == tipo_planta) {
			return true;
		}
	}
	return false;
}

/**
 * @brief Cuenta las especies distintas presentes en un parque.
 * @param nombreparque Nombre del parque.
 * @return Número de especies distintas, o 0 si el parque no existe.
 */
unsigned int ControlParques::contarEspecies(const string &nombreparque) {
	Parque *p = buscarParque(nombreparque);
	if(p) return p->contarEspecies();
	return 0;
}

/**
 * @brief Cuenta los tipos de planta distintos presentes entre los árboles de todos los parques (se ignora el tipo NONE).
 * @return Número de tipos de planta distintos.
 */
unsigned int ControlParques::contarTipos() {

	vector<TipoPlanta> encontradas;
	TipoPlanta aux;
	for (list<Parque>::iterator i = this->parques.begin(); i != this->parques.end(); i++ ){
		for(int j=0; j < i->get_contenedor().size(); j++ ) {
			aux = Especie::stringtotp(  i->get_contenedor()[j]->getTipoEspecie());
			if( !buscar_tipo_planta(aux, encontradas) && aux != TipoPlanta::NONE) {
				encontradas.push_back(aux);
			}
		}
	}
	return encontradas.size();
}

/**
 * @def devuelve un conjunto de parques que tienen al menos un ejemplar del arbol
 * dado por parametro mediante su nombre comun 
 * @param nombre_comun
 * @return Una lista de parques que tienen arboles con un nombre comun determinado
 */
list<Parque>* ControlParques::listarParqueArbol(string nombre_comun) {

	list<Parque> * encontrados = new list<Parque>();
	bool hay = false;

	//recorremos los parques
	for(list<Parque>::iterator i = this->parques.begin(); i!= this->parques.end(); i++) {
		hay = false;
		//recorremos los arboles dentro de los parques
		for(vector<Arbol*>::iterator j = i->get_contenedor().begin(); j != i->get_contenedor().end() && !hay; j++ ) {
			if((j.operator*()->getNombreComun() == nombre_comun) ) {
				hay = true;
			}
		}
		if(hay) encontrados->push_back(*i);
	}
	if(encontrados->size() >0) return  encontrados;

	return nullptr;
}

/**
 * @def localiza en tiempo optimo todos los ejemplares de arboles con ese nombre comun exacto 
 * @param nombre_comun 
 * @return Un vector de arboles que hemos encontrado a través de su nombre comun
 */
vector<Arbol *>* ControlParques::buscarArbolNombreComun(string nombre_comun) {
	vector<Arbol*> *encontrados = new vector<Arbol *>();

	for(map<string,Arbol>::iterator i = this->arboles.begin(); i != this->arboles.end(); i++) {
		if(i->second.getNombreComun() == nombre_comun) {
			encontrados->push_back(new Arbol(i->second));
		}
	}
	if(encontrados->size() >0) return encontrados;

	return nullptr;
}

/**
 * @def localiza en tiempo optimo todos los ejemplares de arboles dando una
 * palabra del comienzo 
 * @param nombre_comun 
 * @return Un vector de arboles que hemos encontrado que contengan una determinada cadena
 */
vector<Arbol*> *ControlParques::buscarArbolNombreComunComienzo(string ncomun) {

	vector<Arbol*> *encontrados = new vector<Arbol *>();

	string aux;
	for(map<string,Arbol>::iterator i = this->arboles.begin(); i != this->arboles.end(); i++) {

		size_t pos = i->second.getNombreComun().find(" ");
		aux = i->second.getNombreComun().substr(0, pos);
		if(aux == ncomun) {
			encontrados->push_back(new Arbol(i->second));
		}
	}
	if(encontrados->size() >0) return encontrados;

	return nullptr;
}

/**
 * @def devuelve el numero de arbolees que tienen un barrio dado su codigo
 * @param num_barrio 
 * @return El numero de veces que se repite un arbol en un determinado barrio
 */
int ControlParques::contarArbolesBarrio(int num_barrio) {

	int contador=0;

	//recorremos todos los arboles
	for(map<string,Arbol>::iterator i = this->arboles.begin(); i != this->arboles.end(); i++) {
		if(i->second.get_num_barrio() == num_barrio) {
			contador++;
		}
	}
	return contador;
}

/**
 * @brief Ordena el vector de especies de menor a mayor según su código de especie.
 */
void ControlParques::ordenarEspecies() {

	std::sort(variantes.begin(), variantes.end(),
		[](const Especie& a, const Especie& b) {
			return a.get_codigo_especie() < b.get_codigo_especie();
		});
}

/**
 * @brief Obtiene la especie más común (con más ejemplares) de un parque.
 * @param nombreParque Nombre del parque.
 * @return Puntero a la especie más común, o nullptr si el parque no existe.
 * @throw std::out_of_range Si el parque existe pero no contiene ningún árbol.
 */
Especie * ControlParques::especieMasComunEnParque(const string &nombreParque) {

	for(list<Parque>::iterator i = this->parques.begin(); i != this->parques.end(); i++) {
		if(i->getNombreParque() == nombreParque) {
			return i->especieMasComun();
		}
	}
	return nullptr;
}

/**
 * @brief Busca los árboles de una especie (por nombre común exacto) situados en un distrito.
 *
 * @note El llamador debe liberar la lista, pero no los árboles, que pertenecen al mapa.
 * @param nombreComun Nombre común exacto de la especie.
 * @param numDistrito Número de distrito.
 * @return Puntero a una lista reservada con new con punteros a los árboles del mapa (no copias); la lista está vacía si no hay coincidencias.
 */
list<Arbol *> *ControlParques::buscarArbolesPorEspecieYDistrito(const string &nombreComun, int numDistrito) {
	list<Arbol *> *lista = new list<Arbol *>();

	for (map<string,Arbol>::iterator i= arboles.begin(); i != arboles.end(); i++) {
		if (i->second.getNombreComun() == nombreComun && i->second.getNumDistrito() == numDistrito) {
			lista->push_back(&i->second);
		}
	}
	return lista;
}

