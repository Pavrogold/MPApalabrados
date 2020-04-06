/**
 * @file main.cpp
 * @author DECSAI
 * @note To be implemented by students either completely or by giving them
 * key functions prototipes to guide the implementation
 */

//Alumno 1: Pablo Millan Cubero
//Alumno 2: Yesenia Gonzalez Davila

#include <string>
#include <iostream>
#include <cassert>
#include <fstream>
#include <stdio.h>

#include "language.h"
#include "wordlist.h"
#include "bag.h"
#include "player.h"
#include "move.h"

using namespace std;


#define ERROR_ARGUMENTS 1
#define ERROR_OPEN 2
#define ERROR_DATA 3

/**
 * @brief Reports an important error and exits the program
 * @param errorcode An integer representing the error detected, which is represented
 * by several symbolic constants: ERROR_ARGUMENTS, ERROR_OPEN y ERROR_DATA
 * @param errorinfo Additional information regarding the error: "" (empty string) for 
 * errors parsing the arguments to main() and, for the case of errors opening or 
 * reading/writing data, the name of the file thas has failed.
 */
void errorBreak(int errorcode, const string & errorinfo);

/**
 * @brief Shows final data
 * @param l Language
 * @param random Random seed
 * @param b Final bag
 * @param p Final player
 * @param nwords Number of words found
 * @param score Number of points scored
 * @param result Aggregation of all valid words found
 */
void HallOfFame(const Language &l, int random, const Bag &b, const Player &p, 
        int nwords, int score, const string &result);


/**
 * @brief Main function. 
 * @return 
 */
int main(int nargs, char * args[]) {
    Bag bag;
    Player player;
    Language language;
    Move move;
    
    cout << endl << endl ;
    
    string word, lang="", goodmoves="", badmoves="", ifilename="", ofilename="", secuencia="", result;
    int random=-1, nwords=0, score=0, word_score;
    const string END="@" ; 
    
    ifstream ifile; 
    ofstream ofile;
    istream *input; 
    ostream *output;
    
    input=&cin ;
    output=&cout ;
    //ofile.open("example.txt");
    //output=&ofile;
    move.print(*output);
    
    if (nargs<1 || nargs>9 || nargs%2==0)
        errorBreak (ERROR_ARGUMENTS, "") ;
    
    string s;
    for (int i=1 ; i<nargs;) {
        s=args[i++];
        
        if (s=="-l"){ 
            lang=args[i++] ;
            language.setLanguage(lang);
        }
        else if (s=="-r") {
            if (!isdigit(*args[i])) 
                errorBreak (ERROR_ARGUMENTS, "") ; 
            random = atoi(args[i++]);
            bag.setRandom(random) ;
        }
        else if (s=="-i") {
            ifilename=args[i++] ;
            ifile.open(ifilename.c_str()) ;
            if (!ifile) 
                errorBreak (ERROR_OPEN, ifilename) ;
            input=&ifile;
        }
        else if (s=="-b") 
            secuencia=args[i++] ;
        else
            errorBreak (ERROR_ARGUMENTS, "") ; 
    }
    
    if (lang == "")
        errorBreak(ERROR_ARGUMENTS, "");
    language.setLanguage(lang) ;
    
    if (secuencia == "")
        bag.define(language) ;
    else
        bag.set(toISO(secuencia));
    
    player.add (bag.extract(7)) ;
    
    
    *output << endl << "ID:" << random ; 
    *output << "\nALLOWED LETTERS: " << toUTF(language.getLetterSet()) << endl;
    *output << "BAG ("<<bag.size()<<"): " << toUTF(bag.to_string()) << endl;
    *output << "PLAYER: " << toUTF(player.to_string()) << endl;
    
    
    *output << "READ: ";
    move.read(*input) ;
    word = move.getLetters() ;
    
    
    while (player.size()>1 && word != END) {
        
        if (word.length()>1 && player.isValid (word)) {
            
            player.extract(word) ;
            player.add(bag.extract(7-player.size())) ;
            
            word_score = move.findScore(language);
            move.setScore(word_score);
            score += word_score ;
            
            if (language.query(word)) {
                *output << " FOUND!\n\n" ;
                goodmoves += word + " - ";
                nwords ++ ;
            }
            else {
                *output << " NOT REGISTERED!\n\n" ;
                badmoves += word + " - " ;
            } 
        } 
        
        else {
            *output << "INVALID! LESS THAN 2 LETTERS OR LETTERS NOT FOUND IN PLAYER.\n\n" ; 
            badmoves += word + " - " ;
        }

        *output << "\nBAG ("<<bag.size()<<"): " << toUTF(bag.to_string()) << endl;
        *output << "PLAYER: " << toUTF(player.to_string()) << endl ;
        *output << "READ: ";
        move.read(*input) ;
        if (ifile.eof())
            errorBreak(ERROR_DATA, ifilename);
        word = move.getLetters() ;
        *output << word << endl;
        //move.print(*output);
    }
    
    if (input != &cin)
        ifile.close() ;
    
    if (output != &cout)
        ofile.close() ;

    HallOfFame(lang, random, bag, player, nwords, score, goodmoves);
    *output << endl ;
    return 0;
}

void HallOfFame(const Language &l, int random, const Bag &b, const Player &p, 
        int nwords, int score, const string &result) {
    cout << endl << "%%%OUTPUT" << endl << "LANGUAGE: "<<l.getLanguage()<< " ID: " << random << endl;
    cout << "BAG ("<<b.size()<<"): " << toUTF(b.to_string()) << endl;
    cout << "PLAYER (" <<p.size() << "): " << toUTF(p.to_string())<<endl;
    cout << nwords << " words and " << score << " points "<<endl << 
            toUTF(result) << endl;    
}

void errorBreak(int errorcode, const string &errordata) {
    cerr << endl << "%%%OUTPUT" << endl;
    switch(errorcode) {
        case ERROR_ARGUMENTS:
            cerr<<"Error in call. Please use:\n -l <language>  [-r <randomnumber> -i <inputfile>]"<<endl;
            break;
        case ERROR_OPEN:
            cerr<<"Error opening file "<<errordata << endl;
            break;
        case ERROR_DATA:
            cerr<<"Data error in file "<<errordata << endl;
            break;
    }
    std::exit(1);
}

