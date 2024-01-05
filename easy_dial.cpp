#include "easy_dial.hpp"


/* Construeix un easy_dial a partir de la 
informació continguda en el call_registry donat. El
prefix en curs queda indefinit. */
easy_dial::easy_dial(const call_registry& R) throw(error) : _arrel(nullptr){ 
    
   // _arrel=nullptr;
    _actual = nullptr;
 
    freqtotal=0;

    _p = false;

   vector<phone> v;
   R.dump(v);

   //ordena per freq
    ordena(v);

   //recorrem el vector
   // depenent de si
   //_arrel=nullptr;
   ///*
   //freqtotal = 0;
   
   for(int i=0; i<v.size(); i++){
    string nom = v[i].nom();
    //dins d'insereix que busca i=0; nom[i]; ++i
    nat j=0;
    node *pare = nullptr;
    _arrel = insereix(_arrel, j, nom, v[i], pare);
   }
}


easy_dial::node* easy_dial::insereix(node *n, nat i, const string &k, phone tel, node *pare){
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
    //_anterior = D._anterior;
}

easy_dial::node* easy_dial::copia_nodes(node* m) {
    node* n;
    if (m == nullptr) n = nullptr;
    else{
        n = new node(m->_c, m->_telf);    
        n->_esq = copia_nodes(m->_esq);
        n->_dre = copia_nodes(m->_dre);
        n->_cen = copia_nodes(m->_cen);
        //n->_pare = copia_nodes(m->_pare);
        
    }
    return n;
};

easy_dial& easy_dial::operator=(const easy_dial& D) throw(error){
    freqtotal = D.freqtotal;
    _arrel = copia_nodes(D._arrel);
    _actual = nullptr;
    //_anterior = D._anterior;
    return *this;
}

easy_dial::~easy_dial() throw(){
    esborra_nodes(_arrel);
}

void easy_dial::esborra_nodes(node* m) {
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
    _actual = _arrel;
    //_anterior = _arrel;
    string nom;
    if(_actual==nullptr) 
        nom = "";
    else 
        nom = _arrel->_telf.nom();
    return nom;
}

/* Retorna el nom de F(S, p') on p' és el prefix resultant d'afegir
el caràcter c al final del prefix en curs p i
fa que el nou prefix en curs sigui p'.
Si F(S, p) existeix però F(S, p') no existeix llavors retorna 
l'string buit. 
Si no existeix F(S, p) (i per tant tampoc pot existir F(S, p'))
llavors es produeix un error i el prefix en curs queda indefinit. 
Naturalment, es produeix un error si el prefix en curs inicial p 
fos indefinit. */
string easy_dial::seguent(char c) throw(error){
    //_p += c;
    if(_actual==nullptr) {
        _p = false;
        throw error(ErrPrefixIndef);
    }
    else{
    //_anterior = _actual;
    _actual = cercador(_actual, c);
    if(_actual==nullptr) return "";
    return _actual->_telf.nom();
    }
}

void easy_dial::cerca(node *n, char c){
    if (n!=nullptr){
        if (n->_c == c){
            _actual = n->_cen;
        }
        else if (c<n->_c){
            cerca(n->_esq, c);
        }
        else {
            cerca(n->_dre, c);
        }
    }
}

/* Elimina l'últim caràcter del prefix en curs p = p' · a
(a és el caràcter eliminat). Retorna el nom F(S, p') 
i fa que el nou prefix en curs sigui p'. 
Es produeix un error si p fos buida i si es fa que el prefix en curs
quedi indefinit. Òbviament, també es produeix un error 
si p fos indefinit. */
string easy_dial::anterior() throw(error){
    if(_p==false) throw error(ErrPrefixIndef);
    if(_actual==nullptr) {
        _p = false;
        throw error(ErrNoHiHaAnterior);
    }

    if(_actual->_pare==nullptr) throw error(ErrNoHiHaAnterior);
    
   // _actual = _anterior;
    //_p.pop_back();
    return _actual->_telf.nom();
}

/* Retorna el número de telèfon de F(S, p), sent p
el prefix en curs. Es produeix un error si p és indefinit o si
no existeix F(S, p). */
nat easy_dial::num_telf() const throw(error){
    if(_p==0)throw error(ErrPrefixIndef);
    if(_actual==nullptr) throw error(ErrNoExisteixTelefon);
    return _actual->_telf.numero();
}

/* Retorna en el vector result tots els noms dels contactes de 
telèfon que comencen amb el prefix pref, en ordre lexicogràfic creixent. */
void easy_dial::comencen(const string& pref, vector<string>& result) const throw(error){
    node* n = _arrel;
    for(int i=0; i<pref.size(); i++){
        n = cercador(n, pref[i]);
    }
    //bool pare = true;
    //for(int i=0; i<pref.size(); i++) if(pref[i] != n->_pare->telf.nom()[i]) pare = false;
    //if(pare) result.push_back(n->_pare->telf.nom());
    inordre(n, result);  
}

void easy_dial::inordre(node* n, vector<string>& result) const{
    if(n!=nullptr){
        inordre(n->_esq,result);
        result.push_back(n->_telf.nom());
        inordre(n->_cen,result);
        inordre(n->_dre,result);
    }

}

easy_dial::node* easy_dial::cercador(node *n, char c) const{
    if(n!=nullptr){
        if (n->_c == c){
            return n;
        }
        else if (c<n->_c){
            cercador(n->_esq, c);
        }
        else {
            cercador(n->_dre, c);
        }
    }
    return nullptr;
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
    calcula_mitjana(result, _arrel, 0);
    return result;
}

void easy_dial::calcula_mitjana(double& result, node* n, nat i) const{
    if(n!=nullptr){
        calcula_mitjana(result, n->_esq, i+1);
        calcula_mitjana(result, n->_dre, i+1);
        calcula_mitjana(result, n->_cen, i+1);
        if (freqtotal != 0) 
            result += i * (n->_telf.frequencia() / freqtotal);
 
    // Maneig de l'error o comportament adequat en cas de freqtotal igual a zero
}
}

/* Gestió d'errors. */
static const int  ErrPrefixIndef        = 31;
static const int  ErrNoExisteixTelefon  = 32;
static const int  ErrNoHiHaAnterior     = 33;


void easy_dial::ordena(vector<phone>& V) const{
    if(V.size()<2) return;
    vector<phone> a = V;
    vector<phone> b;
    parteix(a,b);
    ordena(a);
    ordena(b);
    V = fusiona(a,b);
};

void easy_dial::parteix(vector<phone>& a, vector<phone>& b) const{
    //cout << "parteix" <<endl;
    int  mida = int(a.size()/2);
    int n = int(a.size()) -1;
    for(unsigned int i = n; i >= mida; i--){
        b.push_back(a[i]);
        a.pop_back();
    }
    //cout << size(b) << endl;
}

vector<phone> easy_dial::fusiona(const vector<phone>& a, const vector<phone>& b) const{
    //cout << "fusiona" <<endl;
    vector<phone> res;
    int sa = int(a.size());
    int sb = int(b.size());
    //cout << sb << endl;
    int ia = 0;
    int ib = 0;
    while((ia<sa) and (ib<sb)){
        //cout << "m" << endl;
        if(a[ia]<b[ib]) {
            //cout << a[ia] << endl;
            res.push_back(a[ia]);
            ia++;
        } else {
            res.push_back(b[ib]);
            ib++;
        }
    }
    while(ia<sa){
        //cout << "hola";
        res.push_back(a[ia]);
        ia++;
    }

    while(ib<sb){
        res.push_back(b[ib]);
        ib++;
    } return res;
}

easy_dial::node::node (const char &c, const phone &telf, node* esq, node* cen, node* dre , node* pare) : _c(c), _telf(telf), _esq(esq),_cen(cen), _dre(dre), _pare(pare){};