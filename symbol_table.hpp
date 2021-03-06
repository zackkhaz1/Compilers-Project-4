#ifndef CRONA_SYMBOL_TABLE_HPP
#define CRONA_SYMBOL_TABLE_HPP
#include <string>
#include <unordered_map>
#include <list>

//Use an alias template so that we can use
// "HashMap" and it means "std::unordered_map"
template <typename K, typename V>
using HashMap = std::unordered_map<K, V>;

using namespace std;

namespace crona{

//A semantic symbol, which represents a single
// variable, function, etc. Semantic symbols
// exist for the lifetime of a scope in the
// symbol table.

class SemSymbol {
	//TODO add the fields that
	// each semantic symbol should track
	// (i.e. the kind of the symbol (either a variable or function)
	// and functions to get/set those fields
	public:
	SemSymbol(string dType, string sym){
		type = dType;
		symType = sym;
	}
	virtual string getType() = 0;
	string getSymType() {return symType;}
private:
	string type;
	string symType; //var or fn


};

class VarSym : public SemSymbol{
public:
		VarSym(string t, string n) : SemSymbol(t,n) {type = t; name = n;}
		string type;
		string name;
	string getType() { return type; }
};

class FnSym : public SemSymbol{
public:
	 FnSym(list<string> args, string t, string n) : SemSymbol(t,n) {
		 for(auto i : args)
		 {
			 argTypes.push_back(i);
		 }
		 retType = t;
		 name = n;
	 }
	 list<string> argTypes; //list of arguments to function
	 string retType;
	 string name;
	 string getType() {
		 int count = 0;
		 string concat = "";
		 for (auto i : argTypes){
			 count++;
			 if(count == int(argTypes.size())){
				concat = concat + i;
				break;
			 }
			 concat = concat + i + ",";
		 }
		 concat = concat + "->" + retType;
		 return concat;
	 }
};

//A single scope. The symbol table is broken down into a
// chain of scope tables, and each scope table holds
// semantic symbols for a single scope. For example,
// the globals scope will be represented by a ScopeTable,
// and the contents of each function can be represented by
// a ScopeTable.
class ScopeTable {
	public:
		ScopeTable();
		//TODO: add functions for looking up symbols
		// and/or returning information to indicate
		// that the symbol does not exist within the
		// current scope.
		SemSymbol* findSymbol(string s);
		bool addToTable(string id, SemSymbol* sym);
		HashMap<std::string, SemSymbol*>* getSymbols() {return (symbols);}
	private:
		HashMap<std::string, SemSymbol *> * symbols;
};

class SymbolTable{
	public:
		SymbolTable();

		bool scopesEmpty();

		ScopeTable* buildScope();

		ScopeTable* popScope();

		ScopeTable* getTopScope();

		SemSymbol* findID(string id);

		bool addSymbols(string id, SemSymbol* sym);
		//TODO: add functions to create a new ScopeTable
		// when a new scope is entered, drop a ScopeTable
		// when a scope is exited, etc.
	private:
		std::list<ScopeTable *> * scopeTableChain;
};


}

#endif
