#include "symbol_table.hpp"
namespace crona{

ScopeTable::ScopeTable(){
	symbols = new HashMap<std::string, SemSymbol *>();
}


bool ScopeTable::addToTable(string id, SemSymbol* sym){
	SemSymbol* check = findSymbol(id);
	//Add invalid type error for check variable
	//Add multiple declaration error if symbol already exists
	//return false if either multiple declaration or invalid type is true
	//and throw the appropriate error types
	symbols->insert({id, sym});
	return true;
}

SymbolTable::SymbolTable(){

	scopeTableChain = new std::list<ScopeTable *>();
}

bool SymbolTable::addSymbols(string id, SemSymbol* sym){
	if(scopesEmpty())
	{
		return false;
	}
	else
	{
		getTopScope()->addToTable(id, sym);
		return true;
	}
}

SemSymbol* ScopeTable::findSymbol(string s)
{
	auto lookup = symbols->find(s);
	if(lookup != symbols->end())
	{
		return lookup->second;
	}
	else
	{
		return NULL;
	}
}

bool SymbolTable::scopesEmpty(){
		return(scopeTableChain->empty());
}

ScopeTable* SymbolTable::buildScope(){
	ScopeTable* s = new ScopeTable();
	scopeTableChain->push_front(s);
	return s;
}

ScopeTable* SymbolTable::popScope(){
	if(!scopesEmpty())
	{
		ScopeTable* s = scopeTableChain->front();
		scopeTableChain->pop_front();
		return s;
	}
	return NULL;
}

ScopeTable* SymbolTable::getTopScope(){
	if(!scopesEmpty())
	{
		return scopeTableChain->front();
	}
	return NULL;
}

SemSymbol* SymbolTable::findID(string id){
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
