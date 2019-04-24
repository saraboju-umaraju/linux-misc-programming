#include <stdio.h>

struct stack_frame {
		struct stack_frame* next;
		void* ret;
};
void dump(void *fp, int num)
{
	int i=0;
	for(;i<num; i++)
		printf ("%02x ", ((char*)fp)[i]&0xff);
}
int get_call_stack() {
		/* x86/gcc-specific: this tells gcc that the fp
		   variable should be an alias to the %ebp register
		   which keeps the frame pointer */
		register struct stack_frame* fp asm("ebp");
		dump(fp,200);
		/* the rest just walks through the linked list */
		struct stack_frame* frame = fp;
		int i = 0;
		while(frame) {

				printf("%x\n",frame->ret);
	#if 0
				if(i < max_size) {
						retaddrs[i++] = frame->ret;
				}
#endif
				frame = frame->next;
		}
		return i;
}
int one ()
{
	two();
}
int two()
{
	get_call_stack();
}
int main (int argc, char *argv[])
{
		one();
		return 0;
}

