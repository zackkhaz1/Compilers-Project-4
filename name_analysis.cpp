#include "ast.hpp"
#include "symbol_table.hpp"
#include "errors.hpp"

namespace crona{

//TODO here is a subset of the nodes needed to do nameAnalysis,
// you should add the rest to allow for a complete treatment
// of any AST

static bool listAnalysis(std::list<StmtNode*>* stmtList, SymbolTable* symTab){
	bool valid = true;
	for(auto i : *stmtList)
	{
		valid = i->nameAnalysis(symTab) && valid;
	}
	return valid;

}

static bool expAnal(std::list<ExpNode*>* expList, SymbolTable* symTab){
	bool valid = true;
	for(auto i : *expList)
	{
		valid = i->nameAnalysis(symTab) && valid;
	}
	return valid;
}

bool ASTNode::nameAnalysis(SymbolTable * symTab){
	throw new ToDoError("This function should have"
		"been overriden in the subclass!");
}

bool ProgramNode::nameAnalysis(SymbolTable * symTab){
	bool res = true;
	symTab->buildScope();
	for (auto global : *myGlobals){
		res = global->nameAnalysis(symTab) && res;
	}
	return res;
}


bool VarDeclNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	std::string id = this->ID()->getName();
	std::string type = this->getTypeNode()->getType();

	if (type == "void") {
		Report::fatal(this->line(), this->col(), "Invalid type in declaration");
		nameAnalysisOk = false;
	}

	if (symTab->findID(id) != NULL) {
		Report::fatal(this->line(), this->col(), "Multiple declared identifier");
		nameAnalysisOk = false;
	}

	VarSym *newSym = new VarSym(type, id);
	symTab->addSymbols(id, newSym);
	return nameAnalysisOk;
}

bool FnDeclNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	throw new ToDoError("[DELETE ME] I'm an fnDecl."
		" you should add and make current a new"
		" scope table for my body"
	);
	return nameAnalysisOk;
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
	bool v = true;
	v = myCond->nameAnalysis(symTab) && v;
	symTab->buildScope();
	v = listAnalysis(myBody, symTab) && v;
	symTab->popScope();
	return v;
}

bool IfElseStmtNode::nameAnalysis(SymbolTable* symTab){
	bool v = true;
	//True body
	v = myCond->nameAnalysis(symTab) && v;
	symTab->buildScope();
	v = listAnalysis(myBodyTrue, symTab) && v;
	symTab->popScope();
	//false body
	symTab->buildScope();
	v = listAnalysis(myBodyFalse, symTab) && v;
	symTab->popScope();

	return v;

}

bool WhileStmtNode::nameAnalysis(SymbolTable * symTab){
	bool v = true;
	//True body
	v = myCond->nameAnalysis(symTab) && v;
	symTab->buildScope();
	v = listAnalysis(myBody, symTab) && v;
	symTab->popScope();
	return v;
}

bool ReturnStmtNode::nameAnalysis(SymbolTable* symTab){
	return myExp->nameAnalysis(symTab);
}

bool CallExpNode::nameAnalysis(SymbolTable* symTab){
	return myID->nameAnalysis(symTab) && expAnal(myArgs,symTab);

}

bool CallStmtNode::nameAnalysis(SymbolTable* symTab){
	return myCallExp->nameAnalysis(symTab);
}

bool AssignExpNode::nameAnalysis(SymbolTable* symTab){
	return myDst->nameAnalysis(symTab) && mySrc->nameAnalysis(symTab);
}

bool IDNode::nameAnalysis(SymbolTable* symTab){
		mySymbol = symTab->findID(name);
		if (mySymbol == NULL) {
			Report::fatal(this->line(), this->col(), "Undeclared identifier");
			return false;
		}
		return true;
}

}
