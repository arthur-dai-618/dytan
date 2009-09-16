#include <stdlib.h>
#include <assert.h>
#include <inttypes.h>

#include "itrace.h"
#include "udis86.h"

struct _itrace_t {
	ud_t *ud_obj;	
	peek_mem_func_t peek_mem;
	peek_reg_func_t peek_reg;
};

itrace_t *itrace_init() {
	itrace_t *trace;

	trace = (itrace_t *) malloc(sizeof(*trace));
	assert(trace);

	trace->ud_obj = (ud_t *) malloc(sizeof(*(trace->ud_obj)));

	ud_init(trace->ud_obj);
	ud_set_mode(trace->ud_obj, 32);
	ud_set_syntax(trace->ud_obj, UD_SYN_INTEL);

        return trace;
}

void itrace_free(itrace_t *trace) {
	free(trace->ud_obj);
	free(trace);
}

void itrace_set_peek_reg_func(itrace_t *trace, peek_reg_func_t f) {
	trace->peek_reg = f;
}

void itrace_set_peek_mem_func(itrace_t *trace, peek_mem_func_t f) {
	trace->peek_mem = f;
}

void itrace_trace(itrace_t *trace, uint32_t eip) {
	uint8_t code[12];
	trace->peek_mem(code, sizeof(code), eip);	

	ud_set_pc(trace->ud_obj, eip);
	ud_set_input_buffer(trace->ud_obj, code, sizeof(code));
	ud_disassemble(trace->ud_obj);

	printf("[%#"PRIx32"] %-24s\n", eip, ud_insn_asm(trace->ud_obj));
}
