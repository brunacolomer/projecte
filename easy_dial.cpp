#include "easy_dial.hpp"


/* Construeix un easy_dial a partir de la 
informació continguda en el call_registry donat. El
prefix en curs queda indefinit. */
easy_dial::easy_dial(const call_registry& R) throw(error){
   vector<phone> v = R.dump();

   //ordena per freq
    ordena(v);
   //recorrem el vector
   // depenent de si
   for(int i=0; i<v.size(); i++){
    string nom = v[i].nom();
    //dins d'insereix que busca i=0; nom[i]; ++i
    nat j=0;
    node *pare = nullptr;
    insereix(_arrel, j, nom, v[i], pare);
   }
}


easy_dial::insereix(node& *n, nat i, const string &k, phone tel, node *pare){
    if(n==nullptr){
        node * nou = new node;
        nou->telf = tel;
        nou->_c = k[i];
        nou->_esq = nullptr;
        nou->_dre = nullptr;
        nou->_cen = nullptr;
        nou->_pare = pare;
        //Funciona 100%
    }
    else if (n->_c == k[i]){
        insereix(n->_cen, i+1, k, tel, n);
    }
    else if (k[i]<n->_c){
        insereix(n->_esq, i, k, tel, n);
    }
    else {
        insereix(n->_dre, i, k, tel, n);
    }
    //no hem trobat cap lletra coincident
    /*
    else {
        // si és menor a n->_c a l'esquerra
        node *nou = new node;
        nou->telf = tel;
        nou->_c = k[i];
        if(k[i]<n->_c){
            n->_esq = nou;
        } else {
            n->dret = nou;
        }
        // si és major a n->_c a la dreta
    }*/
}

/* Tres grans. Constructor per còpia, operador d'assignació i destructor. */
easy_dial::easy_dial(const easy_dial& D) throw(error){}
easy_dial& easy_dial::operator=(const easy_dial& D) throw(error){}
easy_dial::~easy_dial() throw(){}

/* Inicialitza el prefix en curs a buit. Retorna el nom de F(S, '');
si F (S, '') no existeix llavors retorna l'string buit. */
string easy_dial::inici() throw(){
    _p='';
    _actual = _arrel;
    return _arrel->telf.nom();
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
    _p += c;
    cerca(_actual, c);
    return _actual->telf.nom();
}

void easy_dial::cerca(node *n, char c){
     
    if (n->_c == k[i]){
        _actual = n;
    }
    else if (k[i]<n->_c){
        cerca(n->_esq, c);
    }
    else {
        cerca(n->_dre, c);
    }
}

/* Elimina l'últim caràcter del prefix en curs p = p' · a
(a és el caràcter eliminat). Retorna el nom F(S, p') 
i fa que el nou prefix en curs sigui p'. 
Es produeix un error si p fos buida i si es fa que el prefix en curs
quedi indefinit. Òbviament, també es produeix un error 
si p fos indefinit. */
string easy_dial::anterior() throw(error){
    _actual = _actual->pare;
    _p.pop_back();
    return _actual->telf.nom();
}

/* Retorna el número de telèfon de F(S, p), sent p
el prefix en curs. Es produeix un error si p és indefinit o si
no existeix F(S, p). */
nat easy_dial::num_telf() const throw(error){
    return _actual->telf.numero();
}

/* Retorna en el vector result tots els noms dels contactes de 
telèfon que comencen amb el prefix pref, en ordre lexicogràfic creixent. */
void easy_dial::comencen(const string& pref, vector<string>& result) const throw(error){}

/* Retorna el número mitjà de pulsacions necessàries para obtenir un
telèfon. Formalment, si X és el conjunt de noms emmagatzemats en
el easy_dial i t(s) és el número de pulsacions mínimes
necessàries (= número de crides a l'operació seguent) per
obtenir el telèfon el nom del qual és s. La funció retorna la suma
    Pr(s) · t(s)
per tots els telèfons s del conjunt X, sent Pr(s) la probabilitat de
telefonar a s. La probabilitat s'obté dividint la freqüència de s per
la suma de totes les freqüències. */
double easy_dial::longitud_mitjana() const throw(){}

/* Gestió d'errors. */
static const int  ErrPrefixIndef        = 31;
static const int  ErrNoExisteixTelefon  = 32;
static const int  ErrNoHiHaAnterior     = 33;


void easy_dial::ordena(vector<nat>& V) const{
    if(V.size()<2) return;
    vector<nat> a = V;
    vector<nat> b;
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