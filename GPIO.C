#include <cyg/hal/953x.h>
#include <commands.h>
#include <cyg/kernel/kapi.h>

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include<time.h>

#include <cyg/hal/qca.h>

#define ATH_GPIO_IN                     ATH_GPIO_BASE+0x4                        
#define ATH_GPIO_OUT                    ATH_GPIO_BASE+0x8  

#define WPS_LED_GPIO 5

int GLOBAL = 0 ;

int init = 0;

void ath_gpio_config_output(int gpio)                                            
{                                                                                
    ath_reg_rmw_clear(ATH_GPIO_OE, (1 << gpio));                                 
}                                                                                
                                                                                 
void ath_gpio_config_input(int gpio)                                             
{                                                                                
    ath_reg_rmw_set(ATH_GPIO_OE, (1 << gpio));                                   
}                                                                                
                                                                                 
void ath_gpio_out_val(int gpio, int val)                                         
{                                                                                
    if (val & 0x1) {                                                             
        ath_reg_rmw_set(ATH_GPIO_OUT, (1 << gpio));                              
    } else {                                                                     
        ath_reg_rmw_clear(ATH_GPIO_OUT, (1 << gpio));                            
    }                                                                            
}                                                                                
                                                                                 
int ath_gpio_in_val(int gpio)                                                    
{                                                                                
    return ((1 << gpio) & (ath_reg_rd(ATH_GPIO_IN)));                            
} 

print(char *str, unsigned int addr)
{
	printf ("%s : 0x%08x\n",str,ath_reg_rd(addr));
}

void config_gpio(int argc, char *argv[])
{
	
	if ( argc == 1 ) {
	
		unsigned int mask = simple_strtoul((const char *)argv[0], NULL, 10);

		ath_reg_wr(GPIO_OE_ADDRESS, ath_reg_rd(GPIO_OE_ADDRESS) | ( (1 << mask)) );	

		return ;

	}
	
	if (argc < 2 ) {
		printf ("co gpio value (o/i)(0/1)\n");
		return -1;
	}

	int io = -1;	

	int gpio = simple_strtoul((const char *)argv[0], NULL, 10) ;

	if ( argc > 2 ) {
	
			io   = simple_strtoul((const char *)argv[2], NULL, 10) ;
	}
	
	unsigned int val  = simple_strtoul((const char *)argv[1], NULL, 16) ;
	
	if ( gpio > 17 || io > 1 || val > 255 ) {
			printf ("Send proper values\n");
			return -1;
		}

	int remainder = gpio % 4 ;
	
	int which_function = gpio / 4 ;

	unsigned int fbase = GPIO_OUT_FUNCTION0_ADDRESS ;

	fbase += (4*which_function) ;	

	unsigned int mask = 0xff ;

	val = ((unsigned)val << (8*remainder) );
	
	printf ("writing %08x to %08x\n",val,fbase);

	if ( io == -1  || io == 0 )
			ath_reg_wr(GPIO_OE_ADDRESS, ath_reg_rd(GPIO_OE_ADDRESS) & ( ~(1 << gpio)) );
	else 
			ath_reg_wr(GPIO_OE_ADDRESS, ath_reg_rd(GPIO_OE_ADDRESS) | ( (1 << gpio)) );
	

	ath_reg_wr(fbase, (ath_reg_rd(fbase) & ~(mask << 8*remainder))| val);
}

int prev = -1 ;

void changeIt(int num)
{
	ath_reg_wr(GPIO_OE_ADDRESS, ath_reg_rd(GPIO_OE_ADDRESS) | (1<<num));
	if (prev != -1 )
			ath_reg_wr(GPIO_OE_ADDRESS, ath_reg_rd(GPIO_OE_ADDRESS) & ~((1<<prev)));
	prev = num;
}

void helpme(int num)
{
    if (!GLOBAL) {
            ath_reg_wr(GPIO_OE_ADDRESS, ath_reg_rd(GPIO_OE_ADDRESS) | (1<<num));
    }else {
            ath_reg_wr(GPIO_OE_ADDRESS, ath_reg_rd(GPIO_OE_ADDRESS) & (~(1<<num)));
    }
	
}

blinkEm()
{
	int l = 0;
	int flag = 0 ;
	int arr[] = {4,15,14,11,12,13};
	while(1) {
			for(l=0; l<6; l++) {
					//helpme(arr[l]);	
					if (!flag)
						changeIt(arr[l]);
					else
						changeIt(arr[5-l]);
					cyg_thread_delay(20);
			}
			flag ^= 1;
			GLOBAL ^= 1;
	}
}

#if 0
qca_gpio_out(SYSTEM_LED_PIN);
qca _gpio_write(SYSTEM_LED_PIN,LED_ON);
qca _gpio_read(WIFI_SWITCH_BTN_PIN);
qca _gpio_in(WIFI_SWITCH_BTN_PIN
#endif


void qca_gpio_out(int gpio)
{
        ath_reg_rmw_clear(ATH_GPIO_OUT, (1 << gpio));
}

void qca_gpio_in(int gpio)
{
        ath_reg_rmw_set(ATH_GPIO_OUT, (1 << gpio));
}

int qca_gpio_read(int gpio)
{
	return !(ath_reg_rd(ATH_GPIO_OUT) & (1 << gpio) );
}

void qca_gpio_write(int gpio, int val)
{
    if (val & 0x1) {
        ath_reg_rmw_clear(ATH_GPIO_OUT, (1 << gpio));
    } else {
        ath_reg_rmw_set(ATH_GPIO_OUT, (1 << gpio));
    }

}

void gpios()
{

	print("FUNCTION0",GPIO_OUT_FUNCTION0_ADDRESS);
	print("FUNCTION1",GPIO_OUT_FUNCTION1_ADDRESS);
	print("FUNCTION2",GPIO_OUT_FUNCTION2_ADDRESS);
	print("FUNCTION3",GPIO_OUT_FUNCTION3_ADDRESS);
	print("FUNCTION4",GPIO_OUT_FUNCTION4_ADDRESS);
	print("GPIO_OE_ADDRESS",GPIO_OE_ADDRESS);	
	print("GPIO_OE_OUT",GPIO_OUT_ADDRESS);	
	
	blinkEm();
	
	return 0;
#if 0
	if (!GLOBAL) {
			int val = ath_reg_rd(GPIO_OE_ADDRESS) | (0x10u);
			ath_reg_wr(GPIO_OE_ADDRESS, val);
	}else {
			int val = ath_reg_rd(GPIO_OE_ADDRESS) & (~0x10u);
			ath_reg_wr(GPIO_OE_ADDRESS, val);
	}
	if (!GLOBAL) {
			int val = ath_reg_rd(GPIO_OE_ADDRESS) | (0x8000u);
			ath_reg_wr(GPIO_OE_ADDRESS, val);
	}else {
			int val = ath_reg_rd(GPIO_OE_ADDRESS) & (~0x8000u);
			ath_reg_wr(GPIO_OE_ADDRESS, val);
	}

	cyg_thread_delay(200);
	
	GLOBAL ^= 1;

	return 0;

	if (!init) {
		ath_gpio_config_output(WPS_LED_GPIO); 
		ath_gpio_out_val(WPS_LED_GPIO, 0);
		init = 1 ;
	}

	ath_gpio_out_val(WPS_LED_GPIO, GLOBAL); 
	
	printf ("Value is %x %x\n", ath_gpio_in_val(WPS_LED_GPIO),GLOBAL);

#endif	
	
}
