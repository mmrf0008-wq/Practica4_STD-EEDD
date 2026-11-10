//
// Created by almar on 05/10/2026.
//

#include "ControlParques.h"
#include <algorithm>
#include "LectorCSV.h"


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
	if(!a.cargosArbol(this->arboles, nomFicheroArboles, this->parques, this->variantes)) { // lectura de arboles y ligue entre arbol y parque
		throw std::runtime_error("[ControlParques:cargaArboles]: error al abrir fichero ");
	}
	cout << "Cargados los arboles" << endl;

}
ControlParques::~ControlParques() {
	for(vector<Especie>::iterator i = this->variantes.begin(); i != this->variantes.end(); i++) { //eliminamos las especies
		this->variantes.erase(i);
	}
}

Parque * ControlParques::buscarParque(const string &nombre) {
	for(list<Parque>::iterator i = this->parques.begin(); i != this->parques.end(); i++) {
		if(i->getNombreParque() == nombre) {
			return &i.operator*();
		}
	}
	return nullptr;
}


vector<Especie> & ControlParques::listadoEspecieSubNombre(const string &cadena) {

	vector<Especie> *listad = new vector<Especie>();
	for(int i =0; i < this->variantes.size(); i++) {
		if(this->variantes[i].get_nombre_cientifico().find(cadena)) {
			listad->push_back(this->variantes[i]);
		}
	}
	return *listad;
}


vector<Especie> & ControlParques::getVariantes() {
	return this->variantes;
}


list<Parque> & ControlParques::getparques() {
	return this->parques;
}

map<string,Arbol> & ControlParques::getArboles() {
	return this->arboles;
}

/**
 * @def localiza un arbol a partie de su identificador
 * @param asnum
 * @return
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


bool ControlParques::buscar_tipo_planta(const TipoPlanta &tipo_planta,  vector<TipoPlanta> tipos) {
	for(int i =0; i < tipos.size(); i++) {
		if(tipos[i] == tipo_planta) {
			return true;
		}
	}
	return false;
}


unsigned int ControlParques::contarEspecies(const string &nombreparque) {
	Parque *p = buscarParque(nombreparque);
	if(p) return p->contarEspecies();
	return 0;
}


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
 * @return
 */
list<Parque>* ControlParques::listarParqueArbol(string nombre_comun) {

	list<Parque> * encontrados = new list<Parque>();

	//recorremos los parques
	for(list<Parque>::iterator i = this->parques.begin(); i!= this->parques.end(); i++) {
		//recorremos los arboles dentro de los parques
		for(vector<Arbol*>::iterator j = i->get_contenedor().begin(); j != i->get_contenedor().end(); j++ ) {
			if((j.operator*()->getNombreComun() == nombre_comun)) {
				encontrados->push_back(*i);
			}
		}
	}
	if(encontrados->size() >0) return  encontrados;

	return nullptr;
}

/**
 * @def localiza en tiempo optimo todos los ejemplares de arboles con ese nombre comun exacto 
 * @param nombre_comun 
 * @return 
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
 * @return 
 */
vector<Arbol*> *ControlParques::buscarArbolNombreComunComienzo(string ncomun) {

	vector<Arbol*> *encontrados = new vector<Arbol *>();

	for(map<string,Arbol>::iterator i = this->arboles.begin(); i != this->arboles.end(); i++) {
		/*if(i->second.getNombreComun() == nombre_comun) {
			encontrados->push_back(new Arbol(i->second));
		}*/
	}
	if(encontrados->size() >0) return encontrados;

	return nullptr;
}

/**
 * @def devuelve el numero de arbolees que tienen un barrio dado su codigo
 * @param num_barrio 
 * @return 
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

void ControlParques::ordenarEspecies() {

	std::sort(variantes.begin(), variantes.end(),
		[](const Especie& a, const Especie& b) {
			return a.get_codigo_especie() < b.get_codigo_especie();
		});
}

