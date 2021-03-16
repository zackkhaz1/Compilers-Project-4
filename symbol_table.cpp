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
	scopeTableChain = new std::list<ScopeTable *>();
}

auto findSymbol(string s)
{
	auto lookup = symbols->find(s);
	if(lookup != symbols->end())
	{
		return lookup->second();
	}
	else
	{
		return NULL;
	}
}

bool scopesExist(){
		return(scopeTableChain->empty());
}

ScopeTable* buildScope(){
	ScopeTable* s = new ScopeTable();
	scopeTableChain->push_front(s);
	return s;
}

ScopeTable* popScope(){
	if(!scopesExist())
	{
		ScopeTable* s = scopeTableChain->front();
		scopeTableChain->pop_front();
		return s;
	}
	return NULL;
}

ScopeTable* checkScope(){
	if(!scopesExist())
	{
		return scopeTableChain->front();
	}
	return NULL;
}

SemSymbol* findID(string id){
	for(auto scp : *scopeTableChain)
	{
		SemSymbol* s = scp->findSymbol(id);
		if(s != NULL)
		{
			return s;
		}
	}
	return NULL;
}

}
