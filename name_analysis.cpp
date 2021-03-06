#include "ast.hpp"
#include "symbol_table.hpp"
#include "errors.hpp"
#include <iostream>

namespace crona{

//TODO here is a subset of the nodes needed to do nameAnalysis,
// you should add the rest to allow for a complete treatment
// of any AST

static bool listAnalysis(std::list<StmtNode*>* stmtList, SymbolTable* symTab){
	bool isValid = true;
	for(auto i : *stmtList)
	{
		isValid = i->nameAnalysis(symTab) && isValid;
	}
	return isValid;
}

bool ASTNode::nameAnalysis(SymbolTable * symTab){
	throw new ToDoError("This function should have"
		"been overriden in the subclass!");
}

bool ProgramNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	symTab->buildScope();
	for (auto global : *myGlobals){
		nameAnalysisOk = global->nameAnalysis(symTab) && nameAnalysisOk;
	}
	symTab->popScope();
	return nameAnalysisOk;
}

bool VarDeclNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	std::string id = this->ID()->getName();
	std::string type = this->getTypeNode()->getType();

	if (type == "void") {
		Report::fatal(this->line(), this->col(), "Invalid type in declaration");
		nameAnalysisOk = false;
	}

	if (symTab->getTopScope()->findSymbol(id) != NULL) {
		Report::fatal(this->line(), this->col(), "Multiply declared identifier");
		nameAnalysisOk = false;
	}

	VarSym *newSym = new VarSym(type, id);
	bool symbolAdded = symTab->addSymbols(id, newSym);
	this->ID()->setSemSymbol(newSym);
	return nameAnalysisOk && symbolAdded;
}

bool FnDeclNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	bool bodyisOk = true;

	std::string id = this->ID()->getName();
	std::string type = this->getTypeNode()->getType();
	std::list<string> args;

	for (auto formal : *myFormals) {
		nameAnalysisOk = formal->nameAnalysis(symTab) && nameAnalysisOk;
		TypeNode * typeNode = formal->getTypeNode();
		args.push_back(typeNode->getType());
	}

	FnSym *newSym = new FnSym(args, type, id); 
	bool symbolAdded = symTab->addSymbols(id, newSym);
	this->ID()->setSemSymbol(newSym);

	symTab->buildScope();

	// Make sure function body statements are ok
	bodyisOk = listAnalysis(myBody, symTab) && bodyisOk;

	symTab->popScope();

	return nameAnalysisOk && symbolAdded && bodyisOk;
}

bool WriteStmtNode::nameAnalysis(SymbolTable* symTab){
	return mySrc->nameAnalysis(symTab);
}

bool ReadStmtNode::nameAnalysis(SymbolTable* symTab){
	return myDst->nameAnalysis(symTab);
}

bool IndexNode::nameAnalysis(SymbolTable* symTab){
	return myBase->nameAnalysis(symTab) && myOffset->nameAnalysis(symTab);
}

bool AssignStmtNode::nameAnalysis(SymbolTable* symTab){
	return myExp->nameAnalysis(symTab);
}

bool PostDecStmtNode::nameAnalysis(SymbolTable* symTab){
	return myLVal->nameAnalysis(symTab);
}

bool PostIncStmtNode::nameAnalysis(SymbolTable* symTab){
	return myLVal->nameAnalysis(symTab);
}

bool IfStmtNode::nameAnalysis(SymbolTable* symTab){
	bool nameAnalysisOk = true;
	nameAnalysisOk = myCond->nameAnalysis(symTab) && nameAnalysisOk;
	symTab->buildScope();
	nameAnalysisOk = listAnalysis(myBody, symTab) && nameAnalysisOk;
	symTab->popScope();
	return nameAnalysisOk;
}

bool IfElseStmtNode::nameAnalysis(SymbolTable* symTab){
	bool nameAnalysisOk = true;
	//True body
	nameAnalysisOk = myCond->nameAnalysis(symTab) && nameAnalysisOk;
	symTab->buildScope();
	nameAnalysisOk = listAnalysis(myBodyTrue, symTab) && nameAnalysisOk;
	symTab->popScope();
	//false body
	symTab->buildScope();
	nameAnalysisOk = listAnalysis(myBodyFalse, symTab) && nameAnalysisOk;
	symTab->popScope();

	return nameAnalysisOk;
}

bool WhileStmtNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	//True body
	nameAnalysisOk = myCond->nameAnalysis(symTab) && nameAnalysisOk;
	symTab->buildScope();
	nameAnalysisOk = listAnalysis(myBody, symTab) && nameAnalysisOk;
	symTab->popScope();
	return nameAnalysisOk;
}

bool ReturnStmtNode::nameAnalysis(SymbolTable* symTab){
	// check if returnstmtnode doesn't have a value (for void functions)
	if (myExp == NULL) {
		return true;
	}
	return myExp->nameAnalysis(symTab);
}

bool CallExpNode::nameAnalysis(SymbolTable* symTab){
	bool nameAnalysisOk = true;
	nameAnalysisOk = myID->nameAnalysis(symTab) && nameAnalysisOk;
	for (auto arg : *myArgs) {
		nameAnalysisOk = arg->nameAnalysis(symTab) && nameAnalysisOk;
	}
	return nameAnalysisOk;
}

bool CallStmtNode::nameAnalysis(SymbolTable* symTab){
	return myCallExp->nameAnalysis(symTab);
}

bool AssignExpNode::nameAnalysis(SymbolTable* symTab){
	return myDst->nameAnalysis(symTab) && mySrc->nameAnalysis(symTab);
}

bool IDNode::nameAnalysis(SymbolTable* symTab){
	bool nameAnalysisOk = true;
	SemSymbol * tempSym = symTab->findID(this->getName());
	if (tempSym == NULL) {
		Report::fatal(this->line(), this->col(), "Undeclared identifier ");
		nameAnalysisOk = false;
	}
	else {
		this->mySymbol = tempSym;
		nameAnalysisOk = true;
	}
	return nameAnalysisOk;
}

bool BinaryExpNode::nameAnalysis(SymbolTable* symTab){
	return myExp1->nameAnalysis(symTab) && myExp2->nameAnalysis(symTab);
}

bool UnaryExpNode::nameAnalysis(SymbolTable* symTab){
	return myExp->nameAnalysis(symTab);
}

}
