#include "symbol_table.hpp"
namespace crona{

ScopeTable::ScopeTable(){
	symbols = new HashMap<std::string, SemSymbol *>();
}

SymbolTable::SymbolTable(){
	//TODO: implement the list of hashtables approach
	// to building a symbol table:
	// Upon entry to a scope a new scope table will be
	// entered into the front of the chain and upon exit the
	// latest scope table will be removed from the front of
	// the chain.
	ScopeTable* st;
	scopeTableChain = new std::list<ScopeTable *>();
	scopeTableChain->push_front(st);
}

}
