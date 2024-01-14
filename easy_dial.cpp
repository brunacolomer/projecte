#include "easy_dial.hpp"
#include <algorithm> 

// Representacio de Theta amb 0

// Pre: Cert
// Post: El vector V està ordenat de menor a major
// Cost: 0(n) cost lineal
template <typename T>
void ordena(vector<T>& V);

// Pre: b es buit.
// Post: a conte la meitat d'elements que tenia inicialment
// i b conte l'altra meitat
// Cost: 0(n) cost lineal. Sent n la mida de a
template <typename T>
void parteix(vector<T>& a, vector<T>& b);

// Pre: a i b estan ordenats en orde ascentend
// Post: retorna un vector ordenat que conte tots 
// els elements de a i b
// Cost: 0(n) cost lineal
template <typename T>
vector<T> fusiona(const vector<T>& a, const vector<T>& b);


/* Construeix un easy_dial a partir de la 
informació continguda en el call_registry donat. El
prefix en curs queda indefinit. */
easy_dial::easy_dial(const call_registry& R) throw(error) : _arrel(nullptr), _actual(nullptr){ 
    // Cost: 0(n) cost lineal
    _ant = nullptr;
    freqtotal=0;
    
    _p = false; // Prefix en curs indefinit

   vector<phone> v;
   R.dump(v);

   // Ordena els telefons de menor a major
    ordena(v);
   
   for(int i=v.size()-1; i>=0; i--){ // Recorrem el vector de major a menor
    string nom = v[i].nom();
    nat j=0;
    node *pare = nullptr;
    _arrel = insereix(_arrel, j, nom+'\0', v[i], pare);
   }
}

easy_dial::node* easy_dial::insereix(node *n, nat i, const string &k, phone tel, node *pare){
    // Cost
    if(i>=k.size()) return n;
    if(n==nullptr){
        n = new node(k[i],tel);
        n->_pare = pare;
        freqtotal+=tel.frequencia();
    }
    else if (n->_c == k[i]){
        n->_cen=insereix(n->_cen, i+1, k, tel, n);
    }
    else if (k[i]<n->_c){
        n->_esq=insereix(n->_esq, i, k, tel, n);
    }
    else {
        n->_dre=insereix(n->_dre, i, k, tel, n);
    } return n;
}

/* Tres grans. Constructor per còpia, operador d'assignació i destructor. */
easy_dial::easy_dial(const easy_dial& D) throw(error){
    freqtotal = D.freqtotal;
    _arrel = copia_nodes(D._arrel);
    _actual = nullptr;
}

easy_dial::node::node (const char &c, const phone &telf, node* esq, node* cen, node* dre ) : _c(c), _telf(telf), _esq(esq),_cen(cen), _dre(dre){};

easy_dial::node* easy_dial::copia_nodes(node* m) {
    node* n;
    if (m == nullptr) n = nullptr;
    else{
        n = new node(m->_c, m->_telf);    
        n->_esq = copia_nodes(m->_esq);
        n->_dre = copia_nodes(m->_dre);
        n->_cen = copia_nodes(m->_cen);        
    }
    return n;
};

easy_dial& easy_dial::operator=(const easy_dial& D) throw(error){
    freqtotal = D.freqtotal;
    _arrel = copia_nodes(D._arrel);
    _actual = nullptr;
    return *this;
}

easy_dial::~easy_dial() throw(){
    esborra_nodes(_arrel);
}

void easy_dial::esborra_nodes(node* m) {
    // Cost: 0(n), sent n el nombre de nodes de l'arbre del P.I.
    if (m != nullptr) {
        esborra_nodes(m->_esq);
        esborra_nodes(m->_dre);
        esborra_nodes(m->_cen);
        delete m;
    }
};

/* Inicialitza el prefix en curs a buit. Retorna el nom de F(S, '');
si F (S, '') no existeix llavors retorna l'string buit. */
string easy_dial::inici() throw(){
    _p=true;
    _pref = "";
    _actual = _arrel;
    _ant = nullptr;
    string nom;
    if(_actual==nullptr) {
        nom = "";
    }
    else
        nom = _arrel->_telf.nom();
    return nom;
}

/* Retorna el nom de F(S, p') on p' és el prefix resultant d'afegir
el caràcter c al final del prefix en curs */
string easy_dial::seguent(char c) throw(error){
    
    if(_actual==nullptr){
        _p=false; // el prefix queda indefinit
        throw error(ErrPrefixIndef);
    } else if (_p==false) {
        throw error(ErrPrefixIndef);
    }
    _ant = _actual;
    
    if(_actual->_c==_pref[_pref.size()-1]) {
        _pref+=c;
        _actual = cercador(_actual->_cen,c);
    }
    else {
        _pref+=c;
        _actual = cercador(_actual,c);
    }

    if(_actual==nullptr) 
        return "";
    else{
        _actual->_pare = _ant;
        return _actual->_telf.nom();
    }
}

/* Elimina l'últim caràcter del prefix en curs p = p' · a
(a és el caràcter eliminat). Retorna el nom F(S, p') */
string easy_dial::anterior() throw(error){
    if(_p==false) throw error(ErrPrefixIndef);
    if(_ant==nullptr) {
        _p = false;
        throw error(ErrNoHiHaAnterior);
    }
    _actual=_ant;
    //if(_actual->_pare==nullptr) throw error(ErrNoHiHaAnterior);
    _ant = _actual->_pare;
   // _actual = _anterior;
    if(_pref.size()>0)_pref.pop_back();
    return _actual->_telf.nom();
}

/* Retorna el número de telèfon de F(S, p), sent p
el prefix en curs. */
nat easy_dial::num_telf() const throw(error){
    if(_p==0)throw error(ErrPrefixIndef);
    if(_actual==nullptr) throw error(ErrNoExisteixTelefon);
    return _actual->_telf.numero();
}

/* Retorna en el vector result tots els noms dels contactes de 
telèfon que comencen amb el prefix pref, en ordre lexicogràfic creixent. */
void easy_dial::comencen(const string& pref, vector<string>& result) const throw(error){
    node* n = _arrel;
    if(pref.size()==0){
        inordre(n, result);
    }
    else {
        nat i =0;
        while(i<pref.size() and n!=nullptr){
            n = cerca(n, pref[i]);
            if (n!=nullptr and comenca(pref,n->_telf.nom())){
                result.push_back(n->_telf.nom());
    
            } if(n!=nullptr)n=n->_cen;
            i ++;
        }
        
        if(n!=nullptr){
            inordre(n, result);
        }
        
    } ordena(result);
}
bool easy_dial::comenca(string pref, const string nom) const{
    nat i =0;
    bool com = true;
    while(i<pref.size() and com){
        if(nom[i]!=pref[i]) com = false;
        else i++;
    }
    return com;
}


void easy_dial::inordre(node* n, vector<string>& result) const{
    if(n!=nullptr){
        inordre(n->_esq,result);
        result.push_back(n->_telf.nom());
        inordre(n->_cen,result);
        inordre(n->_dre,result);
    }
}

easy_dial::node* easy_dial::cercador(node *n, char c) {
 if(n!=nullptr){
        if (comenca(_pref, n->_telf.nom()) and _ant!=n)
            return n;

        else if(comenca(_pref, n->_telf.nom()) and _ant==n)
            n = cercador(n->_cen, c);

        else if (c<n->_c)
            n = cercador(n->_esq, c);

        else if(c>n->_c)
            n = cercador(n->_dre, c);
    }
    return n;
}

easy_dial::node* easy_dial::cerca(node *n, char c) const{
if(n!=nullptr){
        if (n->_c == c){
            return n;
        }
        else if (c<n->_c){
            n = cerca(n->_esq, c);
        }
        else if(c>n->_c){
            n = cerca(n->_dre, c);
        }
    }
    return n;
}

/* Retorna el número mitjà de pulsacions necessàries para obtenir un
telèfon. Formalment, si X és el conjunt de noms emmagatzemats en
el easy_dial i t(s) és el número de pulsacions mínimes
necessàries (= número de crides a l'operació seguent) per
obtenir el telèfon el nom del qual és s. La funció retorna la suma
    Pr(s) · t(s)
per tots els telèfons s del conjunt X, sent Pr(s) la probabilitat de
telefonar a s. La probabilitat s'obté dividint la freqüència de s per
la suma de totes les freqüències. */
double easy_dial::longitud_mitjana() const throw(){
    double result = 0;
    if(freqtotal!=0)
        calcula_mitjana(result, _arrel, 0, true);
    return result;
}

void easy_dial::calcula_mitjana(double& result, node* n, nat i, bool central) const{
    if(n!=nullptr){
        if(central){
            calcula_mitjana(result, n->_esq, i+1, false);
            calcula_mitjana(result, n->_dre, i+1, false);
            calcula_mitjana(result, n->_cen, i+1, true);
        }else {
            calcula_mitjana(result, n->_esq, i, false);
            calcula_mitjana(result, n->_dre, i, false);
            calcula_mitjana(result, n->_cen, i+1, true);
        }
        double f = n->_telf.frequencia();
        result += i * f/freqtotal;
    }
}

/* Gestió d'errors. */
static const int  ErrPrefixIndef        = 31;
static const int  ErrNoExisteixTelefon  = 32;
static const int  ErrNoHiHaAnterior     = 33;


// FUNCIONS AUXILIARS

template <typename T>
void ordena(vector<T>& V) { // Mergesort
    if(V.size()<2) return;
    vector<T> a = V;
    vector<T> b;
    parteix(a,b);
    ordena(a);
    ordena(b);
    V = fusiona(a,b);
};

template <typename T>
void parteix(vector<T>& a, vector<T>& b){
    int  mida = int(a.size()/2);
    int n = int(a.size()) -1;
    for(unsigned int i = n; i >= mida; i--){
        b.push_back(a[i]);
        a.pop_back();
    }
}

template <typename T>
vector<T> fusiona(const vector<T>& a, const vector<T>& b){
    vector<T> res;
    int sa = int(a.size());
    int sb = int(b.size());
    int ia = 0;
    int ib = 0;
    while((ia<sa) and (ib<sb)){
        if(a[ia]<b[ib]) {
            res.push_back(a[ia]);
            ia++;
        } else {
            res.push_back(b[ib]);
            ib++;
        }
    }
    while(ia<sa){
        res.push_back(a[ia]);
        ia++;
    }

    while(ib<sb){
        res.push_back(b[ib]);
        ib++;
    } return res;
}
