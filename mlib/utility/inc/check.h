#pragma once 
#ifndef _CHECK_H_
#define _CHECK_H_
void assert_fail(uint8_t* file, uint32_t line);
#define ASSERT(expr) ((expr) ? (void)0U : assert_fail((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */

#endif