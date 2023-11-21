#include "phone.hpp"
/* Construeix un telèfon a partir del seu número (num), el seu nom
(name) i el seu comptador de trucades (compt). 
Es produeix un error si name no és un identificador legal. */


phone::phone(nat num, const string& name, nat compt) throw(error){
    for(nat i=0; i<name.size();i++){
        if(name[i]==DELETECHAR or name[i]==ENDCHAR or name[i]==ENDPREF) throw error(ErrNomIncorrecte);
    }
    name_phone = name;
    freq= compt;
    number = num;
}

/* Tres grans. Constructor per còpia, operador d'assignació i destructor. */
phone::phone(const phone& T) throw(error) {
    name_phone = T.name_phone;
    number = T.number;
    freq = T.freq;
}


phone& phone::operator=(const phone& T) throw(error){
    phone aux(T);
    *this = aux;
    return *this;
}

phone::~phone() throw(){}

/* Retorna el número de telèfon. */
nat phone::numero() const throw(){
    return number;
}

/* Retorna el name associat al telèfon, eventualment l'string buit. */
string phone::nom() const throw(){
    return name_phone;
}

/* Retorna el número de vegades que s'ha trucat al telèfon. */
nat phone::frequencia() const throw(){
    return freq;
}

/* Operador de preincrement. 
Incrementa en 1 el número de vegades que s'ha trucat al telèfon i
retorna una referència a aquest telèfon. */
phone& phone::operator++() throw(){
    freq = freq + 1;
    return *this;
}

/* Operador de postincrement. 
Incrementa en 1 el número de vegades que s'ha trucat al telèfon i
retorna una còpia d'aquest telèfon sense incrementar. */
phone phone::operator++(int) throw(){
    phone aux = *this;
    freq = freq + 1;
    return aux;
}

/* Operadors de comparació.  L'operador > retorna cert, si i nameés si, el
telèfon sobre el que s'aplica el mètode és més freqüent que el
telèfon T, o a igual freqüència, el name associat al telèfon és
major en ordre lexicogràfic que el name associat a T. 
La resta d'operadors es defineixen consistentment respecte a >. */

bool phone::operator==(const phone& T) const throw(){
    bool igual = false;
    if(freq == T.freq) igual = true;
    return igual;
}

bool phone::operator!=(const phone& T) const throw(){
    return not (*this==T);
}

bool phone::operator<(const phone& T) const throw(){
    bool petit = false;
    if (freq<T.freq) petit = true;
    else if(freq == T.freq and name_phone<T.name_phone) petit = true;
    return petit;

}

bool phone::operator>(const phone& T) const throw(){
    return not (*this<T);
}

bool phone::operator<=(const phone& T) const throw(){
    return (*this<T) or  (*this==T);
}
bool phone::operator>=(const phone& T) const throw(){
    return (*this>T) or (*this==T);
}

/* Caràcters especials no permesos en un name de telèfon. */
static const char DELETECHAR = '<';
static const char ENDCHAR = '|';
static const char ENDPREF = '\0';

/* Gestió d'errors. */
static const int  ErrNomIncorrecte = 11;


// g++ phone.cpp -o phone.e -O0 -g -Wall -Wextra -Werror -Wno-sign-compare -std=c++14 -ansi -I /home/bruna/ESIN/projecte/incl -lesin

int main() {
    // Your program logic here
    phone aux1(645345324,"Jaume",0);
    phone aux2(aux1);
    cout << aux1.nom() << endl;
    if(aux2==aux1) cout << "Funciona!!!!!" << endl;
    cout << aux1.numero() << endl;
    return 0;
}