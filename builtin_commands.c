/*
 * Copyright (c) 2005, 2006
 *
 * James Hook (james@wmpp.com) 
 * Chris Zimman (chris@wmpp.com)
 *
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the University of California, Berkeley nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <cyg/infra/cyg_type.h>
#include <cyg/infra/diag.h>
#include <cyg/io/devtab.h>
#include <cyg/hal/hal_io.h>
#include <cyg/hal/hal_intr.h>
#include <cyg/hal/var_intr.h>
#include <cyg/hal/plf_io.h>
#include <cyg/kernel/kapi.h>
#include <cyg/io/io.h>
#include <cyg/hal/hal_arch.h>

#include <ctype.h>
#include <stdlib.h>
#include <shell.h>
#include <shell_err.h>
#include <shell_thread.h>
#include <commands.h>


#include <stdio.h>
#if 1
#include <sys/socket.h>
#include <netinet/in.h>
#include <cyg/hal/drv_api.h>
//#include <net/dhcpelios/dhcpelios.h>
#include <network.h>
#include <pkgconf/system.h>
#include <pkgconf/net.h>
#include <net/if_dl.h>
#include <net/if_var.h>
#include <net/if_types.h>
#endif


#define TWO_THREADS   

void get_thread_info();
extern void ag7100_poll_link(); 

shell_cmd("force_link", 
"force_link", 
"", 
force_link); 

shell_cmd("set",
	 "Write to flash",
	 "",
	 set);

shell_cmd("get",
	 "Read from flash",
	 "",
	 get);

shell_cmd("ps",
	 "Shows a list of threads",
	 "",
	 ps);

shell_cmd("dump",
	 "Shows a memory dump",
	 "",
	 hexdump);

shell_cmd("help",
	 "Displays a list of commands",
	 "",
	 help_func);

shell_cmd("?",
	 "Displays a list of commands",
	 "",
	 help_func2);

shell_cmd("kill",
	 "Kills a running thread",
	 "[thread ID]",
	 thread_kill);

shell_cmd("sp",
	 "Sets a threads priority",
	 "[thread ID]",
	 set_priority);

shell_cmd("timeron",
	 "Enables the timer interrupt",
	 "",
	 timer_on);

shell_cmd("timeroff",
	 "Enables the timer interrupt",
	 "",
	 timer_off);

shell_cmd("version",
	 "Shows build version",
	 "",
	 print_build_tag);

/* commands either appear or not appear in your system via conditional compile */
shell_cmd("ethreg",
	"ethreg spi read/write tool",
	"<p> <phyaddr> <reg> <value>", 
	ethreg);

shell_cmd("ifconfig",
          "config tool for bsd tcp/ip",
          "",
          ifconfig_cmd);

#ifdef TWO_THREADS
shell_cmd("nthread",
	 "run twothread apps",
	 "[num]",
	 n_threads);
#endif

shell_cmd("regdump",
	"dumping reg values",
	"",
	regdump);

shell_cmd("meminfo",
	"dumping reg values",
	"",
	mem_info);

shell_cmd("hello",
         "sample cmd to print hello world",
         "[hello_id]",
         hello_world_fn);

CMD_DECL(ifconfig_cmd){
    int ret = 0;

    ret = ifconfig_main(argc, argv);
    return SHELL_OK;
}


CMD_DECL(hello_world_fn)
{
	hello_world();
}

void hello_world() 
{       
	printf("Hello eCos World\n");
}

CMD_DECL(ethreg)
{   
	do_ethreg(0, argc, argv);
}

CMD_DECL(n_threads)
{
	int num;
        char *erptr = NULL;

        if(argc != 1) {
                SHELL_DEBUG_PRINT("Usage: nthread [num]\n");
                return SHELL_OK;
        }

        num = strtol((const char *)argv[0], &erptr, 0);

        if(*erptr) {
                SHELL_PRINT("Invalid thread handle\n");
                return SHELL_OK;
        }
	n_thread_create(num);
}



CMD_DECL(regdump)
{
    
    cyg_handle_t system_clock;

    cyg_tick_count_t ticks;
                           
    system_clock = cyg_real_time_clock();

    ticks = cyg_current_time(); 

    unsigned int cp0_reg;

    diag_printf("tick=%d\n", ticks);

    cp0_reg = __read_32bit_c0_register($12, 0);

    diag_printf("status register:%x\n", cp0_reg);

            cp0_reg = __read_32bit_c0_register($13, 0);

    diag_printf("cause register:%x\n", cp0_reg);

            cp0_reg = __read_32bit_c0_register($9, 0);

    diag_printf("count register:%x\n", cp0_reg);

            cp0_reg = __read_32bit_c0_register($11, 0);

    diag_printf("compare register:%x\n", cp0_reg);

            cp0_reg = __read_32bit_c0_register($15, 1);

    diag_printf("base register:%x\n", cp0_reg);

}
CMD_DECL(get)
{
	volatile char *buf = malloc (120);

	if (NULL == buf) {
		diag_printf ("MEM\n"); 
		return -1;
	}

    if ( cyg_flash_getconf(buf, 119) ) {
		diag_printf ("GETCONF\n"); 
		free (buf);
		return -1;
	}

	printf ("DATA: %s\n" ,buf);

	buf[0] = 0;

	memset(buf, 0, 120);

	free (buf);

	return SHELL_OK ;
	

}
CMD_DECL(set)
{
	if ( strlen( argv[0] ) != cyg_flash_setconf(argv[0], strlen( argv[0] )) ) {
		diag_printf ("writing failed \n");
		return -1;
	}
}

CMD_DECL(ps)
{
    get_thread_info();
    
    return SHELL_OK;
}

CMD_DECL(set_priority)
{
    cyg_handle_t thandle = 0;
    cyg_priority_t cur_pri, set_pri;
    unsigned int tid, pri;
    char *erptr = NULL;

    if(argc == 2) {
	thandle = strtol((const char *)argv[0], &erptr, 0);
	
	if(erptr && *erptr) {
	    SHELL_PRINT("Value '%s' is not a valid thread ID\n", argv[1]);
	    return SHELL_OK;
	}   	    

	set_pri = strtol((const char *)argv[1], &erptr, 0);
	
	if(erptr && *erptr) {
	    SHELL_PRINT("Value '%s' is not a valid thread priority\n", argv[2]);
	    return SHELL_OK;
	}

	cur_pri = cyg_thread_get_current_priority(thandle);
	
	SHELL_PRINT("Changing thread %d priority from %d to %d\n", thandle, cur_pri, set_pri);

	cyg_thread_set_priority(thandle, set_pri);

	cur_pri = cyg_thread_get_current_priority(thandle);
	SHELL_PRINT("Thread %d priority now @ %d\n", thandle, cur_pri);
    }
    else SHELL_PRINT("Usage: sp [tid] [priority]\n");

    return SHELL_OK;
}
	

CMD_DECL(help_func)
{
    ncommand_t *shell_cmd = __shell_CMD_TAB__;

    const char cmds[] = "Commands", dsr[] = "Descriptions";
    const char usage[] = "Usage", location[] = "File Location";
    unsigned char helpar[sizeof(cmds) + sizeof(dsr) + sizeof(usage) + sizeof(location) + 10 ];
    unsigned char i;

    snprintf(helpar, sizeof(helpar) - 1, "%%-11s %%-60s %%-20s %%-20s\n");
    
    SHELL_PRINT(helpar, cmds, dsr, usage, location);

    for(i = 0; i < sizeof(cmds) - 1; i++) putchar('-');
    SHELL_PRINT("    ");
    for(i = 0; i < sizeof(dsr) - 1; i++) putchar('-');
    SHELL_PRINT("                                                 ");
    for(i = 0; i < sizeof(usage) - 1; i++) putchar('-');
    SHELL_PRINT("                ");
    for(i = 0; i < sizeof(location) - 1; i++) putchar('-');
    putchar('\n');

    while(shell_cmd != &__shell_CMD_TAB_END__) {
	SHELL_PRINT("%-11s %-60s %-20s %-20s\n",
		   shell_cmd->name,
		   shell_cmd->descr,
		   shell_cmd->usage,
		   shell_cmd->file);
	shell_cmd++;
    }

    return SHELL_OK;
}

CMD_DECL(help_func2)
{
    return(help_func(argc, argv));
}

CMD_DECL(hexdump)
{
    unsigned int i = 0, j = 0;
    unsigned int len = 100, width = 16;
    unsigned char *buf;
    char *cp = NULL;

    switch(argc) {

    case 1:
	buf = (unsigned char *)strtol((const char *)argv[0], &cp, 0);

	if(cp && *cp) {
	    SHELL_PRINT("Value '%s' is not a valid address\n", argv[0]);
	    return SHELL_OK;
	}

	break;

    case 2:
	buf = (unsigned char *)strtol((const char *)argv[0], &cp, 0);

	if(cp && *cp) {
	    SHELL_PRINT("Value '%s' is not a valid address\n", argv[0]);
	    return SHELL_OK;
	}   

	len = strtol((const char *)argv[1], &cp, 0);

	if(cp && *cp) {
	    SHELL_PRINT("Value '%s' is not a valid length\n", argv[1]);
	    return SHELL_OK;
	}   	    

	break;

    case 3:
	buf = (unsigned char *)strtol((const char *)argv[0], &cp, 0);

	if(cp && *cp) {
	    SHELL_PRINT("Value '%s' is not a valid address\n", argv[0]);
	    return SHELL_OK;
	}   

	len = strtol((const char *)argv[1], &cp, 0);

	if(cp && *cp) {
	    SHELL_PRINT("Value '%s' is not a valid length\n", argv[1]);
	    return SHELL_OK;
	}   	    

	width = strtol((const char *)argv[2], &cp, 0);

	if(cp && *cp) {
	    SHELL_PRINT("Value '%s' is not a valid width\n", argv[2]);
	    return SHELL_OK;
	}   	    	
	
	break;

    default:
	SHELL_PRINT("Usage: hexdump [address] [length] [width]\n");
	return SHELL_OK;
    }

    SHELL_PRINT("%08X: ", (unsigned int)buf);
    
    for(i = 0; i < len; i++) {
	if(i && !(i % width)) {
	    j = i - width;
	    SHELL_PRINT("\t\t");
	    for(; j < i; j++) SHELL_PRINT("%c", isprint(buf[j]) ? buf[j] : '.');
	    SHELL_PRINT("\n%08X: ", (unsigned int)buf + i);
	    j = 0;
	}
	SHELL_PRINT("%02X ", buf[i]);
	j++;
    }
    
    if(j) {
	for(i = width - j; i > 0; i--) SHELL_PRINT("   ");
	SHELL_PRINT("\t\t"); 
	for(i = len - j; i < len; i++) SHELL_PRINT("%c", isprint(buf[i]) ? buf[i] : '.');
    }
    SHELL_PRINT("\n");

    return SHELL_OK;
}

CMD_DECL(thread_kill)
{
    cyg_handle_t th;
    char *erptr = NULL;

    if(argc != 1) {
	SHELL_DEBUG_PRINT("Usage: kill [tid]\n");
	return SHELL_OK;
    }

    th = strtol((const char *)argv[0], &erptr, 0);

    if(*erptr) {
	SHELL_PRINT("Invalid thread handle\n");
	return SHELL_OK;
    }

    cyg_thread_kill(th);
    cyg_thread_delete(th);

    return SHELL_OK;
}

CMD_DECL(timer_on)
{
    SHELL_PRINT("Turning timer interrupt on\n");
    cyg_interrupt_unmask(CYGNUM_HAL_INTERRUPT_RTC);

    return 0;
}

CMD_DECL(timer_off)
{
    SHELL_PRINT("Turning timer interrupt off\n");

    cyg_interrupt_mask(CYGNUM_HAL_INTERRUPT_RTC);

    return 0;
}

CMD_DECL(force_link) 
{ 
ag7100_poll_link(); 
} 

shell_cmd("wsetip",
         "ping cmd to ping to the ip of the target eth0 192.168.1.5",
         "ping",
         wsetip);

CMD_DECL(wsetip)
{
	struct ifaddr ifa;
//	struct sockaddr_in addr;

//	init_all_network_interfaces();
        struct ifreq ifr;
        const char * name = "eth0";
        int fd;


	diag_printf ("%d %s FORCE_LINK \n",__LINE__,__func__); 

        fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

        if (fd == -1){
                perror("sock");
                return -1;
        }
        

        memcpy(ifr.ifr_name, name, IFNAMSIZ);
        ifr.ifr_addr.sa_family = AF_INET;
        ifr.ifr_flags |= (IFF_UP | IFF_RUNNING);
        fprintf(stderr, "Configuring the IF_UP flags\n");

        if(ioctl(fd, SIOCSIFFLAGS, &ifr)) {
                perror("SIOCSIFFLAGS");
                fprintf(stderr, "Error !! in configuring the IF_UP flags\n");
                close(fd);
                return -1;
        }
        
	cyg_thread_delay(300);
        fprintf(stderr, "Configuring the IP Address for ETH0  ..\n");

        memcpy(ifr.ifr_name, name, IFNAMSIZ);
        ifr.ifr_addr.sa_family = AF_INET;
        inet_pton(AF_INET, "192.168.1.5", ifr.ifr_addr.sa_data + 2);
        if(ioctl(fd, SIOCSIFADDR, &ifr)) {
                perror("SIOCSIFADDR");
                fprintf(stderr, "Error !! in configuring the IP Address \n");
                close(fd);
                return -1;
        }
        fprintf(stderr, "Configuring the IP Address for ETH0  done ..\n");

        inet_pton(AF_INET, "255.255.255.0", ifr.ifr_addr.sa_data + 2);
        if(ioctl(fd, SIOCSIFNETMASK, &ifr)){
                perror("SIOCSIFNETMASK");
                close(fd);
                return -1;
        }
#if 0
        if(ioctl(fd1, SIOCGIFFLAGS, &ifr1)) {
                perror("SIOCGIFFLAGS");
                close(fd1);
                return -1;
        }
#endif
        close (fd);
	show_network_tables(printf);
	return 0;

}

shell_cmd("lsetip",
         "ping cmd to ping to the ip of the target eth1 172.16.9.6",
         "ping",
         lsetip);

CMD_DECL(lsetip)
{
	struct ifaddr ifa1;
//	struct sockaddr_in addr;

//	init_all_network_interfaces();
        struct ifreq ifr1;
        const char * name1 = "eth1";
        int fd1;


	diag_printf ("%d %s FORCE_LINK \n",__LINE__,__func__); 

        fd1 = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

        if (fd1 == -1){
                perror("sock");
                return -1;
        }
        

        memcpy(ifr1.ifr_name, name1, IFNAMSIZ);
        ifr1.ifr_addr.sa_family = AF_INET;
        ifr1.ifr_flags |= (IFF_UP | IFF_RUNNING);
        fprintf(stderr, "Configuring the IF_UP flags\n");

        if(ioctl(fd1, SIOCSIFFLAGS, &ifr1)) {
                perror("SIOCSIFFLAGS");
                fprintf(stderr, "Error !! in configuring the IF_UP flags\n");
                close(fd1);
                return -1;
        }
        
	cyg_thread_delay(300);
        fprintf(stderr, "Configuring the IP Address for ETH1  ..\n");

//        memcpy(ifr.ifr_name, name1, IFNAMSIZ);
        ifr1.ifr_addr.sa_family = AF_INET;
        inet_pton(AF_INET, "172.16.9.5", ifr1.ifr_addr.sa_data + 2);
        if(ioctl(fd1, SIOCSIFADDR, &ifr1)) {
                perror("SIOCSIFADDR");
                fprintf(stderr, "Error !! in configuring the IP Address \n");
                close(fd1);
                return -1;
        }
        fprintf(stderr, "Configuring the IP Address for ETH1  done ..\n");

        inet_pton(AF_INET, "255.255.255.0", ifr1.ifr_addr.sa_data + 2);
        if(ioctl(fd1, SIOCSIFNETMASK, &ifr1)){
                perror("SIOCSIFNETMASK");
                close(fd1);
                return -1;
        }
#if 0
        if(ioctl(fd1, SIOCGIFFLAGS, &ifr1)) {
                perror("SIOCGIFFLAGS");
                close(fd1);
                return -1;
        }
#endif
        close (fd1);
	show_network_tables(printf);
	return 0;

}

shell_cmd("ping-eth-all",
         "ping cmd to ping to the ip of the target eth0  and eth1 for 172.16.9.5 ,172.16.9.6 respectively",
         "ping",
         ping_eth_all);

CMD_DECL(ping_eth_all)
{
	struct ifaddr ifa;
//	struct sockaddr_in addr;

//	init_all_network_interfaces();
        struct ifreq ifr, ifr1;
        const char * name = "eth0";
        int fd, fd1;
        const char * name1 = "eth1";



	diag_printf ("%d %s FORCE_LINK \n",__LINE__,__func__); 

        fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

        if (fd == -1){
                perror("sock");
                return -1;
        }
        

        memcpy(ifr.ifr_name, name, IFNAMSIZ);
        ifr.ifr_addr.sa_family = AF_INET;
        ifr.ifr_flags |= (IFF_UP | IFF_RUNNING);
        fprintf(stderr, "Configuring the IF_UP flags\n");

        if(ioctl(fd, SIOCSIFFLAGS, &ifr)) {
                perror("SIOCSIFFLAGS");
                fprintf(stderr, "Error !! in configuring the IF_UP flags\n");
                close(fd);
                return -1;
        }
        
	ag7100_poll_link();
	cyg_thread_delay(300);
        fprintf(stderr, "Configuring the IP Address  for ETH0..\n");

        memcpy(ifr.ifr_name, name, IFNAMSIZ);
        ifr.ifr_addr.sa_family = AF_INET;
        inet_pton(AF_INET, "172.16.9.5", ifr.ifr_addr.sa_data + 2);
        if(ioctl(fd, SIOCSIFADDR, &ifr)) {
                perror("SIOCSIFADDR");
                fprintf(stderr, "Error !! in configuring the IP Address \n");
                close(fd);
                return -1;
        }

        fd1 = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

        if (fd1 == -1){
                perror("sock");
                return -1;
        }
        

        memcpy(ifr1.ifr_name, name1, IFNAMSIZ);
        ifr1.ifr_addr.sa_family = AF_INET;
        ifr1.ifr_flags |= (IFF_UP | IFF_RUNNING);
        fprintf(stderr, "Configuring the IF_UP flags\n");

        if(ioctl(fd1, SIOCSIFFLAGS, &ifr1)) {
                perror("SIOCSIFFLAGS");
                fprintf(stderr, "Error !! in configuring the IF_UP flags\n");
                close(fd1);
                return -1;
        }
        
	cyg_thread_delay(300);
        fprintf(stderr, "Configuring the IP Address for ETH1  ..\n");

        memcpy(ifr1.ifr_name, name1, IFNAMSIZ);
        ifr1.ifr_addr.sa_family = AF_INET;
        inet_pton(AF_INET, "172.16.9.6", ifr1.ifr_addr.sa_data + 2);
        fprintf(stderr, "DEBUG MESSAGE 1\n");
        if(ioctl(fd1, SIOCSIFADDR, &ifr1)) {
                perror("SIOCSIFADDR");
                fprintf(stderr, "Error !! in configuring the IP Address \n");
                close(fd1);
                return -1;
        }
        fprintf(stderr, "Configuring the IP Address for ETH1 done ..\n");
        inet_pton(AF_INET, "255.255.255.0", ifr.ifr_addr.sa_data + 2);
        if(ioctl(fd, SIOCSIFNETMASK, &ifr)){
                perror("SIOCSIFNETMASK");
                close(fd);
                return -1;
        }

	diag_printf ("%d %s FORCE_LINK \n",__LINE__,__func__); 


        inet_pton(AF_INET, "255.255.255.0", ifr1.ifr_addr.sa_data + 2);
        if(ioctl(fd1, SIOCSIFNETMASK, &ifr1)){
                perror("SIOCSIFNETMASK");
                close(fd1);
                return -1;
        }

/*        if(ioctl(fd1, SIOCGIFFLAGS, &ifr1)) {
                perror("SIOCGIFFLAGS");
                close(fd1);
                return -1;
        }
        if(ioctl(fd, SIOCGIFFLAGS, &ifr)) {
                perror("SIOCGIFFLAGS");
                close(fd);
                return -1;
        }
*/
#if 0
        memcpy(ifr.ifr_name, name, IFNAMSIZ);
        ifr.ifr_flags |= (IFF_UP | IFF_RUNNING);

        if(ioctl(fd, SIOCSIFFLAGS, &ifr)) {
                perror("SIOCSIFFLAGS");
                close(fd);
                return -1;
        }

#endif
        close (fd);
        close (fd1);
	show_network_tables(printf);
	while(1){
	cyg_thread_delay(300);
	diag_printf ("hello\n");
}	
//	dhcpd_startup();
/*	while (1) {
		show_network_tables(printf);
		cyg_thread_delay (500);
	}
*/

	return 0;
}
