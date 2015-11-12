
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* union: We can view a union like a cunk of memory that is used to
 * store variables of different types within it. A union is a class of 
 * whose data membbers are mapped to the same address within its object.
 * Unions are typically used in situation where an object can be one
 * of many things but only one at a time.
*/

 /* define union struct */
union WORD_T {
	struct {
		/* 16 bits = short or signed int */
		// 0000 0000 0001 0000 = 16 in binary (aka. bit5)
		unsigned bit1:1;
		unsigned bit2:1;
		unsigned bit3:1;
		unsigned bit4:1;
		unsigned bit5:1;
		unsigned bit6:1;
		unsigned bit7:1;
		unsigned bit8:1;
		unsigned bit9:1;
		unsigned bit10:1;
		unsigned bit11:1;
		unsigned bit12:1;
		unsigned bit13:1;
		unsigned bit14:1;
		unsigned bit15:1;
		unsigned bit16:1;
	} bit;
	/* 2 bytes = 16 bits or 4 words signed int */
	// 00000000 00010000 = 16 in binary (aka. byte1)
	struct {
		unsigned byte1 :8;
		unsigned byte2 :8;
	} byte;
	/* our integer */
	int num;
} uint16;

int main() {

	//test bit
	uint16.bit.bit5 = 1;
	printf("%u%u%u%u %u%u%u%u %u%u%u%u %u%u%u%u\n", uint16.bit.bit16, uint16.bit.bit15, uint16.bit.bit14,uint16.bit.bit13,uint16.bit.bit12,uint16.bit.bit11,uint16.bit.bit10,uint16.bit.bit9,uint16.bit.bit8,uint16.bit.bit7,uint16.bit.bit6,uint16.bit.bit5,uint16.bit.bit4,uint16.bit.bit3,uint16.bit.bit2,uint16.bit.bit1);

	//test byte
	uint16.byte.byte1 = 16;
	printf("%u %u\n", uint16.byte.byte2,uint16.byte.byte1);

	//test int
	uint16.num = 16;
	printf("%d\n", uint16.num);

	return 0;
}