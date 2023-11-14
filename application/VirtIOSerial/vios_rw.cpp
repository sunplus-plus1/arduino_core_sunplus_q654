#include "vios_common.h"
#include "VirtIOSerial.h"

#ifdef __cplusplus
extern "C" {
#endif

void vDoVIOreadwriteTask( void *pvParameters )
{
	/* Remove compiler warning about unused parameter. */
	( void ) pvParameters;    
	for( ;; )
	{		
		if( xVioAction_Semaphore != NULL )
		{
            if (SerialVirtIO.available()) {
                String s = SerialVirtIO.readString();
                SerialVirtIO.print(s); // write back
                printf("VIO:%s \n",s.c_str());
            }
            if (uart1.available()) {
            	String s = uart1.readString();
            	uart1.print(s); // write back
                printf("uart1:%s \n",s.c_str());
            }
            if (uart2.available()) {
            	String s = uart2.readString();
            	uart2.print(s); // write back
                printf("uart2:%s \n",s.c_str());
            }
        }
    }
}

#ifdef __cplusplus
}
#endif