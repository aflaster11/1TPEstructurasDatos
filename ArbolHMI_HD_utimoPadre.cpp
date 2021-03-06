#include "ArbolHMI_HD_utimoPadre.h"
#include <queue>
using namespace std;

//Métodos del NodoArbol
Arbol::NodoArbol::NodoArbol(int etqta){
	this->etqta = etqta;
	this->hijoMasI = 0;
	this->hermanoD = 0;
	this->nHijos = 0;
	this->senalaP = 0;
}

Arbol::NodoArbol::NodoArbol(int etqta,NodoArbol* hermanoD){
	this->etqta = etqta;
	this->hijoMasI = 0;
	this->hermanoD = hermanoD;
	this->nHijos = 0;
	this->senalaP = 0;
}

Arbol::NodoArbol::~NodoArbol(){
	if(hermanoD && !senalaP){
		delete hermanoD;
	}
	
	if(hijoMasI){
		delete hijoMasI;
	}
}

ostream& Arbol::NodoArbol::imprimir(ostream& salida){
	salida<<etqta;
	if(hijoMasI){
		salida<<" { ";
		hijoMasI->imprimir(salida);
		salida<<" }";
	}
	
	if(hermanoD && !senalaP){
		salida<<" , ";
		hermanoD->imprimir(salida);
	}
	return salida;
}


//Métodos del ArbolHMI_HD
Arbol::Arbol(){
	this->nRaiz = 0;
	this->nNodos = 0;
	nodoNulo = 0;
}

Arbol::Arbol(int etqta){
	ponerRaiz(etqta);
	nodoNulo = 0;
}

Arbol::~Arbol(){
	delete nRaiz;
}

void Arbol::vaciar(){
	delete nRaiz;
	nRaiz = 0;
}

int Arbol::vacia(){
	return !nRaiz;
}

Arbol::Nodo Arbol::raiz(){
	return nRaiz;
}

Arbol::NodoArbol* Arbol::hijoMasIzq(Arbol::NodoArbol* nodo){
	return nodo->hijoMasI;
}

Arbol::NodoArbol* Arbol::hermanoDer(Arbol::NodoArbol* nodo){
	Arbol::Nodo herD = 0;
	if(!nodo->senalaP){
		herD = nodo->hermanoD;
	}
	return herD;
}

Arbol::NodoArbol* Arbol::padre(Arbol::NodoArbol* nodo){
	NodoArbol* actual = nodo;
	
	while(actual->senalaP == 0 && actual->hermanoD){
		actual = actual->hermanoD;
	}
	return actual->hermanoD;
}

int Arbol::esHoja(Arbol::NodoArbol* nodo){
	return !hijoMasIzq(nodo);
}

int Arbol::etiqueta(Arbol::NodoArbol* nodo){
	return nodo->etqta;
}

int Arbol::numNodos(){
	return nNodos;
}

int Arbol::numHijos(Arbol::NodoArbol* nodo){
	return nodo->nHijos;
}

void Arbol::modificarEtiq(Arbol::NodoArbol* nodo, int etqta){
	nodo->etqta = etqta;
}

Arbol::NodoArbol* Arbol::agregarHijoIesimo(Arbol::NodoArbol* nodo,int etqta, int posicion){

	NodoArbol* nuevoHijo = 0;
	int seInserto = 0;
	
	if(posicion == 1){
		if(nodo->hijoMasI){
			nuevoHijo = new NodoArbol(etqta,nodo->hijoMasI);
			nodo->hijoMasI = nuevoHijo;
		}else{
			nuevoHijo = new NodoArbol(etqta,nodo);
			nuevoHijo->senalaP = 1;
			nodo->hijoMasI = nuevoHijo;
		}
		seInserto = 1;
	}else{
		NodoArbol* actual = nodo->hijoMasI;
		

		//Empieza en la dos por el hecho de que la pos 1 es el hijoMasIzq
		for(int i = 2;i < posicion && actual && actual->senalaP == 0; ++i){
			actual = actual->hermanoD;
		}
		
		if(actual){
			nuevoHijo = new NodoArbol(etqta,actual->hermanoD);
			actual->hermanoD = nuevoHijo;
			seInserto = 1;
			
			if(actual->senalaP){
				actual->senalaP = 0;
				nuevoHijo->senalaP = 1;
			}
		}
	}
	
	if(seInserto){
		++nodo->nHijos;
		++nNodos;
	}
	
	return nuevoHijo;
}

void Arbol::borrarHoja(Arbol::NodoArbol* nodo){
	NodoArbol* nPadre = padre(nodo);
	int sePuedeB = 0;
	
	if(nPadre){
		if(nPadre->hijoMasI == nodo){
			if(nodo->senalaP){
				nPadre->hijoMasI = 0;
			}else{
				nPadre->hijoMasI = nodo->hermanoD;
			}
			sePuedeB = 1;
		}else{
			NodoArbol* actual = nPadre->hijoMasI;
			
			while(actual->hermanoD && actual->hermanoD != nodo){
				actual = actual->hermanoD;
			}
			
			if(nodo->senalaP){
				actual->senalaP = 1;
			}
			
			actual->hermanoD = nodo->hermanoD;
			sePuedeB = 1;
		}
	}
	
	if(sePuedeB){
		--nPadre->nHijos;
		--nNodos;
	}
	
	nodo->hermanoD = 0;
	delete nodo;
}

void Arbol::ponerRaiz(int etqta){
	if(!nRaiz){
		nRaiz = new NodoArbol(etqta);
		++nNodos;
	}
}

ostream& Arbol::imprimir(ostream& salida){
	return nRaiz->imprimir(salida)<<endl;
}

Arbol::NodoArbol* Arbol::buscarNodo(int etqta){
	NodoArbol* actual = nRaiz;
	while(actual && actual->etqta != etqta){
		if(actual->hijoMasI){
			actual = actual->hijoMasI;
		}else{
			if(actual->senalaP){
				while(actual->senalaP){
					actual = actual->hermanoD;
				}
				
				if(actual->hermanoD){
					actual = actual->hermanoD;
				}else{
					actual = 0;
				}				
			}else{
				if(actual->hermanoD){
					actual = actual->hermanoD;
				}else{
					actual = 0;
				}
			}
		}
	}
	return actual;
}