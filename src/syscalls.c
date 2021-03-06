/****************************************************************************
*  Copyright (C) 2009 by Michael Fischer. All rights reserved.
****************************************************************************
*  History:
*
*  28.03.09  mifi   First Version, based on the original syscall.c from
*                   newlib version 1.17.0
****************************************************************************/

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

int _read_r (struct _reent *r, int file, char * ptr, int len)
{
	errno = EINVAL;
	return -1;
}

int _lseek_r (struct _reent *r, int file, int ptr, int dir)
{
	return 0;
}

int _write_r (struct _reent *r, int file, char * ptr, int len)
{
#ifdef USART_DEBUG
	int index;
	/* For example, output string by UART */
	for (index=0; index<len; index++) {
		if (ptr[index] == '\n') {
			uart_putc('\r');
		}
		uart_putc(ptr[index]);
	}
#endif
	return len;
}

int _close_r (struct _reent *r, int file)
{
	return 0;
}

/***************************************************************************/
/* Register name faking - works in collusion with the linker.  */
register char * stack_ptr asm ("sp");
caddr_t _sbrk_r (struct _reent *r, int incr)
{
	extern char   end asm ("end"); /* Defined by the linker.  */
	static char * heap_end;
	char *        prev_heap_end;

	if (heap_end == NULL)
		heap_end = & end;

	prev_heap_end = heap_end;
	if (heap_end + incr > stack_ptr) {
	      /* Some of the libstdc++-v3 tests rely upon detecting
        	out of memory errors, so do not abort here.  */
#if 0
		extern void abort (void);
		_write (1, "_sbrk: Heap and stack collision\n", 32);
		abort ();
#else
		errno = ENOMEM;
		return (caddr_t) -1;
#endif
	}
	heap_end += incr;
	return (caddr_t) prev_heap_end;
}

int _fstat_r (struct _reent *r, int file, struct stat * st)
{
	memset (st, 0, sizeof (* st));
	st->st_mode = S_IFCHR;
	return 0;
}


int _isatty_r(struct _reent *r, int fd)
{
	return 1;
}

#ifdef USART_DEBUG

int uart_putc( const char ch)
{
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
	{}
        USART_SendData(USART2, (uint8_t) ch);
	return 0;
}

#endif
