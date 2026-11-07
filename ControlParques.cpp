//
// Created by almar on 05/10/2026.
//

#include "ControlParques.h"
#include "LectorCSV.h"
#include "ListaDE.h"

/**
 * @brief Constructor parametrizado de ControlParques. Carga los datos desde ficheros CSV.
 * @param nomFichEspecies Ruta al fichero CSV de especies.
 * @param nomFichParques Ruta al fichero CSV de parques.
 * @param nomFicheroArboles Ruta al fichero CSV de árboles.
 * @throw std::invalid_argument Si alguno de los nombres de fichero obligatorios está vacío.
 * @throw std::runtime_error Si se produce un error durante la apertura o lectura de cualquiera de los ficheros.
 */
ControlParques::ControlParques(const string &nomFichEspecies, const string &nomFichParques, const string &nomFicheroArboles) {
	if(nomFichEspecies.empty() || nomFichParques.empty()) {
		throw invalid_argument("[ControlParques]: parametros no validos ");
	}
	if(!LectorCSV::cargarEspecies(this->variantes, nomFichEspecies)) {
		throw std::runtime_error("[ControlParques:cargaEspecies]: error al abrir fichero ");
	}
	this->variantes.ordenar();  //ordenamos por id de menor

	LectorCSV a;

	if(!LectorCSV::cargarParques(this->parques, nomFichParques)) {
		throw std::runtime_error("[ControlParques:cargaParques]: error al abrir fichero ");
	}
	if(!a.cargosArbol(this->arboles, nomFicheroArboles, this->parques, this->variantes)) { // lectura de arboles y ligue entre arbol y parque
		throw std::runtime_error("[ControlParques:cargaArboles]: error al abrir fichero ");
	}

}

/**
 * @brief Destructor de la clase ControlParques.
 * Libera la memoria consumida por los objetos Especie guardados en el vector de variantes.
 */
ControlParques::~ControlParques() {
	for(int i =0; i < this->variantes.size(); i++) { //eliminamos las especies
		delete this->variantes[i];
	}
	//la eliminación de parque se gestiona dentro de la clase listaDE
}

/**
 * @brief Busca un parque por su nombre exacto.
 * @param nombre Nombre del parque a buscar.
 * @return Parque& Referencia al objeto Parque encontrado.
 */
Parque *ControlParques::buscarParque(const string &nombre) {

	Iterador<Parque> i= this->parques.iterador();
	Parque *parque = nullptr;
	bool parar= false;
	while(!i.fin() && !parar) {
		if(i.dato().getNombreParque() == nombre) {
			parque = &i.dato();
			parar =true;
		}
		i.siguiente();
	}

	return parque;
}

/**
 * @brief Devuelve aquellas especies que contengan en su nombre científico una subcadena dada como parámetro.
 * @param cadena Subcadena a buscar dentro del nombre científico.
 * @return VDinamico<Especie>& Referencia a un nuevo vector dinámico con las especies coincidentes.
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
 * @brief Obtiene el vector dinámico con los punteros a todas las variantes de especies.
 * @return VDinamico<Especie*>& Referencia al vector dinámico de variantes.
 */
vector<Especie*> & ControlParques::getVariantes() {
	return this->variantes;
}

/**
 * @brief Obtiene la lista doblemente enlazada de parques gestionados.
 * @return ListaDE<Parque>& Referencia a la lista de parques.
 */
list<Parque> & ControlParques::getparques() {
	return this->parques;
}

/**
 * @brief Obtiene el árbol AVL que contiene todos los árboles del sistema.
 * @return AVL<Arbol>& Referencia al árbol AVL de punteros a Arbol.
 */
AVL<Arbol> & ControlParques::getArboles() {
	return this->arboles;
}

/**
 * @brief Busca un árbol en la estructura AVL mediante su identificador/código numérico.
 * @param asnum Identificador único del árbol como cadena (asnum).
 * @return Arbol& Referencia al objeto Arbol encontrado.
 */
Arbol *ControlParques::buscarArbol(const string &asnum) {

	Arbol *a = new Arbol();
	a->setAsNum(asnum);
	return this->arboles.buscaRecursiva(*a);
}

/**
 * @brief Elimina un parque del sistema dado su nombre.
 * @param nombre Nombre del parque a eliminar.
 */
void ControlParques::eliminarParque(const string &nombre) {

	Iterador<Parque> i = this->parques.iterador();

	while(!i.fin()) {
		if(i.dato().getNombreParque() == nombre) {
			this->parques.erase(i);
		}
		i.siguiente();
	}
}

/**
 * @brief Devuelve el listado completo de árboles de un parque dado su nombre.
 * @param nombre_parque Nombre del parque del cual se quiere obtener la lista de árboles.
 * @return VDinamico<Arbol>* Puntero al vector dinámico con los árboles del parque, o nullptr si el parque no existe.
 */
vector<Arbol*> * ControlParques::listaArbolesParque(const string &nombre_parque) {

	Iterador<Parque> i = this->parques.iterador();

	while(!i.fin()) {
		if(i.dato().getNombreParque() == nombre_parque) {
			i.dato().ordenaArboles();

			return &i.dato().get_contenedor();
		}
		i.siguiente(); 
	}
	return nullptr; 


}

/**
 * @brief Busca los árboles cuyos números de asset se encuentran dentro de un rango determinado.
 *
 * @param asNumMin Cota inferior del rango (en formato cadena).
 * @param asNumMax Cota superior del rango (en formato cadena).
 * @return Referencia a un nuevo vector dinámico con los punteros a los árboles encontrados.
 *
 * @throw std::invalid_argument Si la cota inferior es mayor que la cota superior.
 *
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
		if(aux) nuevo->insertar(aux);
	}

	return *nuevo;
}

/**
 * @brief Devuelve el árbol de mayor altura de entre todos los parques. Si hay empate, devuelve el primero encontrado.
 * @return Arbol* Puntero al árbol de mayor altura de todo el sistema.
 */
Arbol *ControlParques::getArbolMasAlto() {
	Iterador<Parque> i = this->parques.iterador();
	Arbol *encontrado = nullptr;
	Arbol *aux; 

	while(!i.fin()) {

			aux = i.dato().arbolMasAlto();
			if((aux ) && (!encontrado || aux->get_altura() > encontrado->get_altura())) {
				encontrado = aux; 
			}

		i.siguiente(); 
	}
	return encontrado; 
}

/**
 * @brief Busca si un tipo de planta existe en un vector de tipos.
 *
 * @param tipo_planta Tipo de planta a buscar.
 * @param tipos Vector dinámico donde realizar la búsqueda.
 * @return true si lo encuentra, false en caso contrario.
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
 * @brief Devuelve el número de especies distintas de un parque dado su nombre.
 * @param nombreparque Nombre del parque a consultar.
 * @return int Cantidad de especies distintas presentes en el parque.
 */
unsigned int ControlParques::contarEspecies(const string &nombreparque) {
	Parque *p = buscarParque(nombreparque);
	if(p) return p->contarEspecies();
	return 0;
}

/**
 * @brief Devuelve el número de tipos de planta distintos presentes en el conjunto de todos los parques.
 * @return int Suma de los tipos de planta distintos en todos los parques.
 */
unsigned int ControlParques::contarTipos() {

	Iterador<Parque> i = this->parques.iterador();
	vector<TipoPlanta> encontradas;
	TipoPlanta aux;
	while (!i.fin()) {
		for(int j=0; j < i.dato().get_contenedor().size(); j++ ) {
			aux = Especie::stringtotp(  i.dato().get_contenedor()[j]->getTipoEspecie());
			if( !buscar_tipo_planta(aux, encontradas) && aux != TipoPlanta::NONE) {
				encontradas.insertar(aux);
			}
		}
		i.siguiente();
	}
	return encontradas.size();
}
