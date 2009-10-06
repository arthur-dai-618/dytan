#include "ir.h"

#include <stdlib.h>

IRReg *IRReg_Reg(int r) {
		
}


IRExpr* IRExpr_Get(IRReg *reg) {
	IRExpr *e = (IRExpr *) malloc(sizeof(IRExpr));
	e->type = Expr_Get;
	e->ex.Get.reg = reg;
	return e;
}

IRExpr* IRExpr_Load(uint64_t addr) {
	IRExpr *e = (IRExpr *) malloc(sizeof(IRExpr));
	e->type = Expr_Load;
	e->ex.Load.addr = addr;
	return e;
}


IRStmt* IRSstmt_IMark(uint64_t addr, size_t len) {
	IRStmt *s = (IRStmt *) malloc(sizeof(IRStmt));
	s->type = Stmt_IMark;
	s->st.IMark.addr = addr;
	s->st.IMark.len = len;
	return s;
}

IRStmt* IRStmt_Put(IRReg *reg, IRExpr *data) {
	IRStmt *s = (IRStmt *) malloc(sizeof(IRStmt));
	s->type = Stmt_Put;
	s->st.Put.reg = reg;
	s->st.Put.data = data;
	return s;
}

IRStmt* IRStmt_Store(uint64_t addr, IRExpr *data) {
	IRStmt *s = (IRStmt *) malloc(sizeof(IRStmt));
	s->type = Stmt_Store;
	s->st.Store.addr = addr;
	s->st.Store.data = data;
	return s;
}
