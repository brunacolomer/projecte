#include "dialog.hpp"
void dialog::dialog(easy_dial& easy, const string& input, vector<string>& answers, nat& numtelf) throw(){
    string nom;
    nom = easy.inici();
    answers.push_back(nom);
    bool err  = false;
    nat i = 0;
    while(i<input.size()){
        char c = input[i];
        try{
            if(c==phone::DELETECHAR) 
                nom = easy.anterior();
            else if(c==phone::ENDCHAR)
                nom = easy.seguent('\0');
            else
                nom = easy.seguent(c);
            answers.push_back(nom);
        }
        catch (error e) {
            answers.push_back(e.mensaje());
            err = true;
        }
        i++;
    }
    try{
        numtelf = easy.num_telf();
    } catch (error e){
        numtelf = 0;
        if (not err) answers.push_back(e.mensaje());
    }
    
}
