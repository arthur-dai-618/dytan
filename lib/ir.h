#ifndef __ITRACE_IR_H
#define __ITRACE_IR_H

#include "stdint.h"
#include <sys/user.h>

typedef struct _IRReg {
	uint32_t reg;
	struct {
		int start;
		int length;
	} range;
} IRReg;

extern IRReg *IRReg_Reg(int);

typedef enum {
	Expr_Get,	// Read a value from a register
	Expr_Load,	// Read a value from memory
} IRExprType;

typedef struct _IRExpr IRExpr;

struct _IRExpr {
	IRExprType type;
	union {
		struct { 
			IRReg *reg;
		} Get;
		struct {
			uint64_t addr;
		} Load;
	} ex;
};

extern IRExpr* IRExpr_Get(IRReg *);
extern IRExpr* IRExpr_Load(uint64_t);

typedef enum {
	Stmt_IMark,
	Stmt_Put,
	Stmt_Store
} IRStmtType;

typedef struct _IRStmt {
	IRStmtType type;
	union {
		struct {
			uint64_t addr;
			size_t len;
		} IMark;
		struct { 
			IRReg *reg;
			IRExpr *data;
		} Put;
		struct { 
			uint64_t addr;
			IRExpr *data;
		} Store;
	} st;
} IRStmt;

extern IRStmt* IRSstmt_IMark(uint64_t, size_t);
extern IRStmt* IRStmt_Put(IRReg *, IRExpr *);
extern IRStmt* IRStmt_Store(uint64_t, IRExpr *);

typedef struct _IRBlock {
	IRStmt **stmts;
} IRBlock;

extern IRBlock *IRBlock_New();
extern void IRBlock_Add(IRStmt *);

#endif //__ITRACE_IR_H
