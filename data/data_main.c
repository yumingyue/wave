#include "data.h"
#include "stdio.h"

int main()
{
		sec_data a;
		int m = sizeof(a);
		int version = sizeof(a.protocol_version);
		int content = sizeof(a.type);
		int un = sizeof(a.u);
		int data = sizeof(a.u.data);
		int sign_data = sizeof(a.u.signed_data);
		printf("sec_data的大小为%d\n",m);
		printf("sec_data.version的大小为%d\n",version);
		printf("sec_data.type的大小为%d\n",content);
		printf("sec_data.sign_data的大小为%d\n",sign_data);
		printf("sec_data.u的大小为%d\n",un);
		printf("sec_data.data的大小为%d\n",data);	
		return 0;
}
