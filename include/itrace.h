#ifndef __ITRACE_H__
#define __ITRACE_H__

#include <stdint.h>

typedef void (*peek_mem_func_t)(uint8_t* buf, size_t cnt, off_t addr);

typedef struct itrace_t *itrace_handle;

itrace_handle itrace_init();
void itrace_free(itrace_handle trace);

void itrace_set_peek_mem_func(itrace_handle trace, peek_mem_func_t f);
void itrace_trace(itrace_handle trace, uint32_t eip);

#endif
