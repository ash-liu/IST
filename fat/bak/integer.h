/*-------------------------------------------*/
/* Integer type definitions for FatFs module */
/*-------------------------------------------*/

#ifndef _INTEGER
#define _INTEGER

/* These types must be 16-bit, 32-bit or larger integer */
typedef int		INT;
typedef unsigned int	UINT;

/* These types must be 8-bit integer */
typedef signed char	CHAR;
typedef unsigned char	UCHAR;
typedef unsigned char	BYTE;

/* These types must be 16-bit integer */
typedef short		SHORT;
typedef unsigned short	USHORT;
typedef unsigned short	WORD;

/* These types must be 32-bit integer */
typedef long		LONG;
typedef unsigned long	ULONG;
typedef unsigned long	DWORD;

/* Boolean type */
//#ifndef __CONFIG_H    
/*这个文件包含定义typedef enum {FALSE = 0, TRUE = !FALSE} bool;将造成命名冲突 */
//typedef enum { FALSE = 0, TRUE } BOOL;
//#else
//typedef bool BOOL;
//#endif

#endif
