#include "call_registry.hpp"

/* Construeix un call_registry buit. */
call_registry::call_registry() throw(error){
    _M = 13;
    _quants = 0;
    _taula = new node_hash *[_M];
    for(int i = 0; i<_M; ++i){
        _taula[i] = NULL;
    }
}

/* Constructor per còpia, operador d'assignació i destructor. */
call_registry::call_registry(const call_registry& R) throw(error){
    _M = R._M;
    _quants = R._quants;
    _taula = new node_hash *[_M];
    for(int i = 0; i<_M; ++i){
        _taula[i] = R._taula[i];
    }
}

call_registry& call_registry::operator=(const call_registry& R) throw(error){
    call_registry aux = call_registry(R);
    *this = aux;
    return *this;
}

call_registry::~call_registry() throw(){

}

/* Registra que s'ha realitzat una trucada al número donat, 
incrementant en 1 el comptador de trucades associat. Si el número no 
estava prèviament en el call_registry afegeix una nova entrada amb 
el número de telèfon donat, l'string buit com a nom i el comptador a 1. */
void call_registry::registra_trucada(nat num) throw(error){
    int pos = h(num) % _M;
    if (_taula[pos]==NULL){
        node_hash *element = new node_hash;
        phone telefon(num,"", 1);
        element->_p = telefon;
        element->_seg=NULL;
        _taula[pos] = element;
        _quants++;
    } else{
        bool trobat  =false;
        node_hash * element = _taula[pos];
        node_hash * ant = NULL;
        while(element!=NULL and not trobat and element->_p.numero()<=num){
            if(element->_p.numero()==num) trobat = true;
            else {
                ant = element;
                element=element->_seg;
            }
        }if(not trobat){
            node_hash * nou = new node_hash;
            phone telefon(num,"", 1);
            nou->_p = telefon;
            nou->_seg=element;
            if(ant == NULL) _taula[pos] = nou;
            else ant->_seg = nou;
            _quants++;
        } else { //Si hi ha el telefon al call registry freq++
            ++(element->_p);
        }
    }
}

/* Assigna el nom indicat al número donat.
Si el número no estava prèviament en el call_registry, s'afegeix
una nova entrada amb el número i nom donats, i el comptador 
de trucades a 0. 
Si el número existia prèviament, se li assigna el nom donat. */
void call_registry::assigna_nom(nat num, const string& name) throw(error){
    int pos = (h(num))%_M;
    bool trobat  =false;
    node_hash * element = _taula[pos];
    node_hash * ant = NULL;
    while(element!=NULL and not trobat and element->_p.numero()<=num){
        if(element->_p.numero()==num) trobat = true;
        else {
            ant = element;
            element=element->_seg;
        }
    } if(trobat) {
        int f = element->_p.frequencia();
        phone nou(num,name,f);
        element->_p = nou;
    }
    else {
        node_hash * nou = new node_hash;
        phone telefon(num,name, 0);
        nou->_p = telefon;
        nou->_seg=element;
        if(ant == NULL) _taula[pos] = nou;
        else ant->_seg = nou;
        _quants++;
    }
}

/* Elimina l'entrada corresponent al telèfon el número de la qual es dóna.
Es produeix un error si el número no estava present. */
void call_registry::elimina(nat num) throw(error){
    int pos = (h(num))%_M;
    bool trobat = false;
    node_hash * element = _taula[pos];
    node_hash * ant = NULL;
    while(element!=NULL and not trobat and element->_p.numero()<=num){
        if(element->_p.numero()==num) trobat = true;
        else {
            ant = element;
            element=element->_seg;
        }
    }if (trobat){
        if(ant==NULL){
            _taula[pos] = element;
        } else {
            ant->_seg = element->_seg;
            delete element;
        }
    }
}

/* Retorna cert si i només si el call_registry conté un 
telèfon amb el número donat. */
bool call_registry::conte(nat num) const throw(){
    int pos = (h(num))%_M;
    bool trobat  =false;
    node_hash * element = _taula[pos];
    while(element!=NULL and not trobat and element->_p.numero()<=num){
        if(element->_p.numero()==num) trobat = true;
        else {
            element=element->_seg;
        }
    }
    return trobat;

}

/* Retorna el nom associat al número de telèfon que s'indica.
Aquest nom pot ser l'string buit si el número de telèfon no
té un nom associat. Es produeix un error si el número no està en
el call_registry. */
string call_registry::nom(nat num) const throw(error){
    int pos = (h(num))%_M;
    bool trobat  =false;
    node_hash * element = _taula[pos];
    while(element!=NULL and not trobat and element->_p.numero()<=num){
        if(element->_p.numero()==num) trobat = true;
        else {
            element=element->_seg;
        }
    }
    if (not trobat) 
        throw error(ErrNumeroInexistent);
    else 
        return element->_p.nom();
}

/* Retorna el comptador de trucades associat al número de telèfon 
indicat. Aquest número pot ser 0 si no s'ha efectuat cap trucada a
aquest número. Es produeix un error si el número no està en el 
call_registry. */
nat call_registry::num_trucades(nat num) const throw(error){
    int pos = (h(num))%_M;
    bool trobat  =false;
    node_hash * element = _taula[pos];
    while(element!=NULL and not trobat and element->_p.numero()<=num){
        if(element->_p.numero()==num) trobat = true;
        else {
            element=element->_seg;
        }
    }
    if (not trobat) 
        throw error(ErrNumeroInexistent);
    else 
        return element->_p.frequencia();
}

/* Retorna cert si i només si el call_registry està buit. */
bool call_registry::es_buit() const throw(){
    return _quants == 0;
}

/* Retorna quants números de telèfon hi ha en el call_registry. */
nat call_registry::num_entrades() const throw(){
    return _quants;
}

/* Fa un bolcat de totes les entrades que tenen associat un
nom no nul sobre un vector de phone.
Comprova que tots els noms dels telèfons siguin diferents; 
es produeix un error en cas contrari. */
void call_registry::dump(vector<phone>& V) const throw(error){
    int j=0;
    for(int i = 0; i<_M; ++i){
        node_hash * element = _taula[i];
        while(element!=NULL){
            V[j] = element->_p;
            element = element->_seg;
            j++;
        }
    }
}

// Mètodes privats
long call_registry::h(int k) {
  long i = ((k * k * MULT) << 20) >> 4;
  if (i < 0)
    i = -i;
  return i;
};
