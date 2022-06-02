#ifndef __MEM_SP645_H
#define __MEM_SP645_H

#define __RAM_BASE       	0x00000000
#define __RAM_SIZE       	(1024*1024*512)		//512MB

// openamp sharemem
#define __SHMEM_BASE        0x1E800000
#define __SHMEM_SIZE        (1024*1024*2)

#define __FBIO_BASE		 	0x70000000
#define __FBIO_SIZE		 	(1024*1024*128)		//128MB

#define __SPI_NOR_BASE		0xF0000000			
#define __SPI_NOR_SIZE		(1024*1024*64)		//64MB

#define __DEVICE_BASE		0xF8000000
#define __DEVICE_SIZE		(1024*1024*4)

#define __ROM_BASE			0xFFF00000
#define __ROM_SIZE       	(1024*1024*1)		//1MB 


#endif
