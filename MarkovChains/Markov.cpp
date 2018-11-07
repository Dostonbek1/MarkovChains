// Markov.cpp from Markov.py
//
// by David M.Reed and John Zelle
// from Data Structures and Algorithms Using Python and C++
// downloaded from publisher's website:
// https://www.fbeedle.com/content/data-structures-and-algorithms-using-python-and-c
// on July 23, 2014
// Python version modified by Jan Pearce, Sept. 7, 2014
//  - local variable "model" in the makeWordModel function was renamed to "tmpmodel"
//    to avoid confusion with "model" instance variable in class Markov
//  - input() function is replaced with raw_input() to conform with Python 2.7
// Python version modified by Mario Nakazawa, Sept. 7, 2014
//  - added docstrings to both functions

#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <random>
#include <vector>
#include <array>
using namespace std;

class Markov {
	/*
	   A simple trigram Markov model.  The current state is a sequence
	   of the two words seen most recently. Initially, the state is
	   { None, None }, since no words have been seen. Scanning the
	   sentence "The man ate the pasta" would cause the
	   model to go through the sequence of states: { {NULL,NULL},
	   {NULL, 'The'}, {'The', 'man'}, {'man','ate'}, {'ate','the'},
	   {'the','pasta'} }
	*/

	//--------------------------------------------------------------------

	private:
		/*
		post: creates an empty Markov model with initial state ["", ""].
		*/
		string state[2] = { NULL, NULL };				   // last two words processed
		unordered_map<array<string,2>, vector<string>> model;  // maps states to words


	//--------------------------------------------------------------------

	public:
		Markov();
		void add(string word);
		string _randomNext();
		void _transition(string next);
		void _reset();
};

Markov makeWordModel(string filename) {

	/*
	creates a Morkov model from the words in the file with filename.
    pre: The file with name filename exists.
    post: A Markov chain from the text in the file is returned.
	*/

	ifstream inFile;
	inFile.open(filename);
	Markov tmpmodel;
	string line;
	while (getline(inFile, line)) {
		string words;


	}
	inFile.close();
	// Add a sentinel at the end of the text
	tmpmodel.add(NULL);
	tmpmodel._reset();
	return tmpmodel;
}

string generateWordChain(Markov markov, int n) {
	
	/*
	generates up to n words on output from the model "markov"
    pre: markov is a valid Markov chain where the length is longer than "n"
    post: a string of n words will be returned using the Markov chain.
	*/

	vector<string> words;
	for (int i = 0; i < n; i++) {
		string next = markov._randomNext();
		if (next.empty()) {
			break;
		}
		words.push_back(next);
	}
	// join the individual words in words list and store it to joinedWords
	string joinedWords;
	for (auto const& s : words) { 
		joinedWords += s;
	}
	return joinedWords;
}

int main()
{
	string fname;
	cout << "Enter filename: " << endl;
	cin >> fname;
	Markov m = makeWordModel(fname);
	cout << generateWordChain(m, 50);

	return 0;
}

//---------------------------------
// Markov Class Method Definitions
//---------------------------------

Markov::Markov() {
	/* post: creates an empty Markov model with initial state ["", ""]. */
	state[2] = { "","" };				   // last two words processed
	//not sure?  model = ;  // maps states to words
}

void Markov::add(string word) {

	/*
	post: Adds word as a possible following word for current
          state of the Markov model and sets state to
          incorporate word as most recently seen.

    ex: If state was {"the", "man"} and word is "ate" then
        "ate" is added as a word that can follow "... the man" and
        the state is now {"man", "ate"}
	*/

	if (model.find(state) != model.end()) {
		// we have an existing list of words for this state
		// just add this new one(word).
		vector<string> temp;
		temp = model[state];
		temp.push_back(word);
		model[state] = temp;
		cout << "help!" << endl;
	}
	else {
		// first occurrence of this state, create a new list
		vector<string> w;
		w.push_back(word);
		model[state] = w;
	}
	// transition to the next state given next word
	_transition(word);
}

string Markov::_randomNext() {

	/*
	post: Returns a random choice from among the possible choices
          of next words, given the current state, and updates the 
          state to reflect the word produced.

    ex: If the current state is {"the", "man"}, and the known
        next words are {"ate", "ran", "hit", "ran"}, one of
        these is selected at random. Suppose "ran" is selected,
        then the new state will be: {"man", "ran"}. Note the
        list of next words can contain duplicates so the
        relative frequency of a word in the list represents its
        probablility of being the next word.
	*/

	// get list of next words for this state
	vector<string> lst = model[state];
	// choose one at random
	int randIndex = rand() % lst.size();
	string choice = lst[randIndex];
	// transition to next state, given the word choice
	_transition(choice);
	return choice;
}

void Markov::_transition(string next) {

	/*
	post: sets the state based on the 'next' word
	*/

	// help function to construct next state
	state[0] = state[1];
	state[1] = next;
}

void Markov::_reset() {

	/*
	post: The model state is reset to its initial 
		  {"", ""} state.

	note: This does not change the transition information that
		  has been learned so far (via add()), it
		  just resets the state so we can start adding
		  transitions or making predictions for a "fresh"
		  sequence.
	*/

	vector<string> state{"", ""};
}
