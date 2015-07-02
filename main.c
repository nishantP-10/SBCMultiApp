/********************************************************************************************
The following is a version named 2.2 with operating microkernal
Capability:-
1.Context switch btween 2 processes.
2.Can switch 1st 4k of RAM memory
3.Trying to save the SFRs
4.SP and LR of the process
5.System image is also custom made
6.Try and include a API fuction call
********************************************************************************************/



#include <LPC214X.H>   


#define IAP_LOCATION 0x7ffffff1       //IAP funtionlocation
					//code to obtain the opcodes from a single record
void context_switch();
void push_working_reg();
void setup();
void set_sys_tick();

unsigned int process_flag;
unsigned int *flag_store;

__irq void TIMER0_Isr()
{
  push_working_reg();
  VICIntEnable&=~(1<<4);              //disable timer0 interrupt	
  T0IR|=(1<<0);						//clear interrupt flag
  VICVectAddr=0xFF;					//updating the VIC
  VICIntEnable|=(1<<4);               //reenable timer0 interrupt
  
  context_switch();
}

void main()
{
  //static unsigned char op_arr[2048];	                                               //users op codes
 // static unsigned char rec[2048]={":1000000004F01FE50800000010F021E300D0A0E19B:1000100001AB4DE200009FE510FF2FE12000000042:10002000000000EB250000EB2C008FE2000C90E8B4:1000300000A08AE000B08BE001704AE20B005AE1B8:100040000000001A1D0000EB0F00BAE818E04FE2B4:10005000010013E303F0471013FF2FE12402000017:10006000340200000030B0E30040B0E30050B0E3E1:100070000060B0E3102052E27800A128FCFFFF8A64:10008000822EB0E13000A128003081451EFF2FE113:1000900001C08FE21CFF2FE11FB500F0E3F80598C7:1000A00086460FBC009C02B07047000001C08FE282:1000B0001CFF2FE130B47546AE4630BC70470000DF:1000C0003E0000EB0210A0E1F0FFFFEB1CC09FE53B:1000D0000FC08CE001001CE30DE08F120FE0A001C7:1000E0001CFF2FE101C08FE21CFF2FE100F098F808:1000F000600000007847C04603002DE9EAFFFFEBEF:100100000300BDE84F0000EB0020A0E3060000EA7A:100110000010A0E3000000EA011081E268309FE5D2:10012000030051E1FBFFFF3A012082E2000052E1AF:10013000F6FFFF3A1EFF2FE104E02DE50000A0E3EB:1001400048109FE5140081E5010941E2180090E59F:100150000F0880E3011941E2180081E5090000EA77:100160000F08A0E328109FE5140081E5C800A0E374:10017000E4FFFFEB0F08A0E314109FE51C0081E5EE:10018000C800A0E3DFFFFFEBF4FFFFEA1027000049:1001900000C002E0008002E00C009FE50C109FE52B:1001A00004209FE508309FE51EFF2FE1601000400E:1001B00060140040601000407047704770470000B6:1001C0000E50A0E1180000EB05E0A0E10050B0E106:1001D0000D10A0E10A30A0E10700C0E300D0A0E1CB:1001E00060D08DE220402DE9EAFFFFEB2040BDE822:1001F0000060A0E30070A0E30080A0E300B0A0E3F3:100200000710C1E305C0A0E1C009ACE8C009ACE833:10021000C009ACE8C009ACE801D0A0E11EFF2FE1A5:100220000400C046C0462000FFF764FF00009FE5C1:100230001EFF2FE10010004004009FE51EFF2FE18C:100240001EFF2FE1041000400C109FE51800A0E3F2:10025000563412EF1EFF2FE10800000026000200B6:100260001EFF2FE17847C04610402DE9F1FFFFEB5C:100270000010A0E3001080E51040BDE81EFF2FE154:1002800090020000001000406004000064000000C4:0400000500000020D7:00000001FF"};  //hex records
  //static unsigned char rec[2048]={":1000000004F01FE50800000010F021E300D0A0E19B:1000100001AB4DE200009FE510FF2FE12000000042:10002000000000EB250000EB2C008FE2000C90E8B4:1000300000A08AE000B08BE001704AE20B005AE1B8:100040000000001A1D0000EB0F00BAE818E04FE2B4:10005000010013E303F0471013FF2FE12402000017:10006000340200000030B0E30040B0E30050B0E3E1:100070000060B0E3102052E27800A128FCFFFF8A64:10008000822EB0E13000A128003081451EFF2FE113:1000900001C08FE21CFF2FE11FB500F0E3F80598C7:1000A00086460FBC009C02B07047000001C08FE282:1000B0001CFF2FE130B47546AE4630BC70470000DF:1000C0003E0000EB0210A0E1F0FFFFEB1CC09FE53B:1000D0000FC08CE001001CE30DE08F120FE0A001C7:1000E0001CFF2FE101C08FE21CFF2FE100F098F808:1000F000600000007847C04603002DE9EAFFFFEBEF:100100000300BDE84F0000EB0020A0E3060000EA7A:100110000010A0E3000000EA011081E268309FE5D2:10012000030051E1FBFFFF3A012082E2000052E1AF:10013000F6FFFF3A1EFF2FE104E02DE50000A0E3EB:1001400048109FE5140081E5010941E2180090E59F:100150000F0880E3011941E2180081E5090000EA77:100160000F08A0E328109FE5140081E5C800A0E374:10017000E4FFFFEB0F08A0E314109FE51C0081E5EE:10018000C800A0E3DFFFFFEBF4FFFFEA1027000049:1001900000C002E0008002E00C009FE50C109FE52B:1001A00004209FE508309FE51EFF2FE1601000400E:1001B00060140040601000407047704770470000B6:1001C0000E50A0E1180000EB05E0A0E10050B0E106:1001D0000D10A0E10A30A0E10700C0E300D0A0E1CB:1001E00060D08DE220402DE9EAFFFFEB2040BDE822:1001F0000060A0E30070A0E30080A0E300B0A0E3F3:100200000710C1E305C0A0E1C009ACE8C009ACE833:10021000C009ACE8C009ACE801D0A0E11EFF2FE1A5:100220000400C046C0462000FFF764FF00009FE5C1:100230001EFF2FE10010004004009FE51EFF2FE18C:100240001EFF2FE1041000400C109FE51800A0E3F2:10025000563412EF1EFF2FE10800000026000200B6:100260001EFF2FE17847C04610402DE9F1FFFFEB5C:100270000010A0E3001080E51040BDE81EFF2FE154:1002800090020000001000406004000064000000C4:0400000500000020D7:00000001FF"};
  //static unsigned char rec[2048]={":100000000F30A0E1044043E20100A0E31010A0E3A0:080010000020A1E004F0A0E1D2:0400000500000000F7:00000001FF"};
  /*blinky set*/

 //unsigned char rec[1024]={":10000000070000EA0000A0E10000A0E10000A0E17C:100010000000A0E10000A0E10000A0E10000A0E1DC:100020000000A0E10000A0E354109FE5000081E57E:100030000000A0E34C109FE5000081E50108A0E36B:1000400044109FE5002091E5002082E1002081E539:100050000108A0E334109FE5000081E5FF04E0E320:10006000010050E2FDFFFF5A0108A0E320109FE5C8:10007000000081E5FF04E0E3010050E2FDFFFF5ACC:10008000F2FFFFEAA0C11FE014C002E0188002E006:08009000148002E01C8002E074:0400000500000000F7:00000001FF"};
 //unsigned char rec2[1024]={":1000000020009FE520109FE5000081E50F08A0E398:1000100018109FE5000081E50F08A0E310109FE590:10002000000081E5F8FFFFEAFFFFFFFF088002E024:08003000048002E00C8002E0F4:0400000500000000F7:00000001FF"};
 //unsigned char rec3[1024]={":1000000058009FE5005090E50F10A0E3015085E1F6:10001000005080E50170A0E30F10A0E340009FE5D1:10002000001080E5030000EA38009FE50F10A0E310:10003000001080E50070A0E30F20A0E30030A0E3F3:10004000012052E2020053E10000000AFBFFFFEA38:100050000180A0E3070058E1F2FFFF0AECFFFFEA8E:0C006000088002E00C8002E0048002E056:0400000500000000F7:00000001FF"};
 //unsigned char rec4[1024]={":100000000000A0E30310A0E30220A0E3010082E0CF:04001000FBFFFFEA09:0400000500000000F7:00000001FF"};
  /*add set*/
  //unsigned char rec[1024]={":0C00000000A0A0E301A08AE2FDFFFFEADF:0400000500000000F7:00000001FF"};
  //unsigned char rec2[1024]={":0C00000000B0A0E301B08BE2FDFFFFEABE:0400000500000000F7:00000001FF"};
  /*led set*/
  unsigned char rec[1024]={":10000000070000EA0000A0E10000A0E10000A0E17C:100010000000A0E10000A0E10000A0E10000A0E1DC:100020000000A0E10000A0E354109FE5000081E57E:100030000000A0E34C109FE5000081E50108A0E36B:1000400044109FE5002091E5002082E1002081E539:100050000108A0E334109FE5000081E5FF04E0E320:10006000010050E2FDFFFF5A0108A0E320109FE5C8:10007000000081E5FF04E0E3010050E2FDFFFF5ACC:10008000F2FFFFEAA0C11FE014C002E0188002E006:08009000148002E01C8002E074:0400000500000000F7:00000001FF"};
  unsigned char rec2[1024]={":10000000070000EA0000A0E10000A0E10000A0E17C:100010000000A0E10000A0E10000A0E10000A0E1DC:100020000000A0E10000A0E354109FE5000081E57E:100030000000A0E34C109FE5000081E50208A0E36A:1000400044109FE5002091E5002082E1002081E539:100050000208A0E334109FE5000081E5FF04E0E31F:10006000010050E2FDFFFF5A0208A0E320109FE5C7:10007000000081E5FF04E0E3010050E2FDFFFF5ACC:10008000F2FFFFEAA0C11FE014C002E0188002E006:08009000148002E01C8002E074:0400000500000000F7:00000001FF"};
  unsigned char rec3[1024]={":10000000070000EA0000A0E10000A0E10000A0E17C:100010000000A0E10000A0E10000A0E10000A0E1DC:100020000000A0E10000A0E354109FE5000081E57E:100030000000A0E34C109FE5000081E50107A0E36C:1000400044109FE5002091E5002082E1002081E539:100050000107A0E334109FE5000081E5FF04E0E321:10006000010050E2FDFFFF5A0107A0E320109FE5C9:10007000000081E5FF04E0E3010050E2FDFFFF5ACC:10008000F2FFFFEAA0C11FE014C002E0188002E006:08009000148002E01C8002E074:0400000500000000F7:00000001FF"};
  unsigned char rec4[1024]={":10000000070000EA0000A0E10000A0E10000A0E17C:100010000000A0E10000A0E10000A0E10000A0E1DC:100020000000A0E10000A0E354109FE5000081E57E:100030000000A0E34C109FE5000081E50207A0E36B:1000400044109FE5002091E5002082E1002081E539:100050000207A0E334109FE5000081E5FF04E0E320:10006000010050E2FDFFFF5A0207A0E320109FE5C8:10007000000081E5FF04E0E3010050E2FDFFFF5ACC:10008000F2FFFFEAA0C11FE014C002E0188002E006:08009000148002E01C8002E074:0400000500000000F7:00000001FF"};

  unsigned char *ch,*op;
  //unsigned int *c,*o;	                                                   //pointer to records

  
  unsigned char i,pass,msb,lsb,temp,t1,t2,flag;
  unsigned char count,type,f=0; 
  //typedef void (*t_funcPtr)(void);
  //t_funcPtr fnptr = (t_funcPtr)0x40001000;
  unsigned long command[5];
  unsigned long result[3];
  typedef void (*IAP)(unsigned int [],unsigned int[]);
  IAP iap_entry;
  iap_entry=(IAP) IAP_LOCATION;
  ch=&rec[0];
  op=(unsigned char*)0x40001000;
  

  while((f==0))
  {  
  ch++;	                                                        
  pass=0;
  count=0x00;                                                             //no of opcodes init
  for(i=0;i<2;i++)														   //loop to obtain no of opcodes in a single record
  {
    if(*ch>0x2f && *ch<0x3a)
	{
	  switch(*ch)
	  {
	    case 0x30: temp=0x00;
		           break;
		case 0x31: temp=0x01;
		           break;
		case 0x32: temp=0x02;
		           break;
		case 0x33: temp=0x03;
		           break;
		case 0x34: temp=0x04;
		           break;
		case 0x35: temp=0x05;
		           break;
		case 0x36: temp=0x06;
		           break;
		case 0x37: temp=0x07;
		           break;
		case 0x38: temp=0x08;
		           break;
		case 0x39: temp=0x09;
		           break;
		default:   temp=0;  
	  }
	}  
	if(*ch>0x40 && *ch<0x47)
	{
	  switch(*ch)
	  {
	    case 0x41:temp=0x0a;
		          break; 
		case 0x42:temp=0x0b;
		          break; 
		case 0x43:temp=0x0c;
		          break; 
		case 0x44:temp=0x0d;
		          break; 
		case 0x45:temp=0x0e;
		          break;
		case 0x46:temp=0x0f;
		          break; 				   
	  }
	}
	if(pass==0)
	{
	  msb=temp<<4;
	}
	else
	{
	  lsb=temp;
	}
	pass++;
	ch++;
	if(pass==2)
      count=(msb|lsb);                                          //if last pass get count
  }
  ch=ch+4;		                                              //point to type of record
  pass=0;
   for(i=0;i<2;i++)
  {
    if(*ch>0x2f && *ch<0x3a)
	{
	  switch(*ch)
	  {
	    case 0x30: temp=0x00;
		           break;
		case 0x31: temp=0x01;
		           break;
		case 0x32: temp=0x02;
		           break;
		case 0x33: temp=0x03;
		           break;
		case 0x34: temp=0x04;
		           break;
		case 0x35: temp=0x05;
		           break;
		case 0x36: temp=0x06;
		           break;
		case 0x37: temp=0x07;
		           break;
		case 0x38: temp=0x08;
		           break;
		case 0x39: temp=0x09;
		           break;
		default:   temp=0;  
	  }
	}  
	if(*ch>0x40 && *ch<0x47)
	{
	  switch(*ch)
	  {
	    case 0x41:temp=0x0a;
		          break; 
		case 0x42:temp=0x0b;
		          break; 
		case 0x43:temp=0x0c;
		          break; 
		case 0x44:temp=0x0d;
		          break; 
		case 0x45:temp=0x0e;
		          break;
		case 0x46:temp=0x0f;
		          break; 				   
	  }
	}
	if(pass==0)
	{
	  t1=temp<<4;
	}
	else
	{
	  t2=temp;
	}
	pass++;
	ch++;
	if(pass==2)
      type=(t1|t2);                                       //store type of record
  }
  if(type==0x01 | type==0x05)
  {
   f=1;
   break;
  }
  if(type==0x00)										//if data record store op codes in op[]
  {
    pass=0;
	for(i=0;i<count*0x02;i++)
	{
	  if(*ch>0x2f && *ch<0x3a)
	{
	  switch(*ch)
	  {
	    case 0x30: temp=0x00;
		           break;
		case 0x31: temp=0x01;
		           break;
		case 0x32: temp=0x02;
		           break;
		case 0x33: temp=0x03;
		           break;
		case 0x34: temp=0x04;
		           break;
		case 0x35: temp=0x05;
		           break;
		case 0x36: temp=0x06;
		           break;
		case 0x37: temp=0x07;
		           break;
		case 0x38: temp=0x08;
		           break;
		case 0x39: temp=0x09;
		           break;
		default:   temp=0;  
	  }
	} 
	if(*ch>0x40 && *ch<0x47)
	{
	  switch(*ch)
	  {
	    case 0x41:temp=0x0a;
		          break; 
		case 0x42:temp=0x0b;
		          break; 
		case 0x43:temp=0x0c;
		          break; 
		case 0x44:temp=0x0d;
		          break; 
		case 0x45:temp=0x0e;
		          break;
		case 0x46:temp=0x0f;
		          break; 				   
	  }
	}
	if(pass==0)
	{
	  t1=temp<<4;
	}
	else
	{
	  t2=temp;
	}
	pass++;
	ch++;
	if(pass==2)
	{
      temp=(t1|t2);
	  *op=temp;
	  op++; 
	  //flag++;
	  /*if(flag==4)
	   {
	     flag=0;
		 op=op+8;
	   }*/
	  pass=0;  
	}  
   }
   ch+=2;
  }
  }
  command[0]=50;
  command[1]=8;					 //sector no.
  command[2]=8;

  iap_entry (command, result);

  command[0]=52;
  command[1]=8;
  command[2]=8;
  command[3]=60000;

  iap_entry (command, result);

  command[0]=50;
  command[1]=8;
  command[2]=8;

  iap_entry (command, result);

  command[0]=51;
  command[1]=0x00008000;
  command[2]=0x40001000;
  command[3]=512;
  command[4]=60000;

  iap_entry (command, result);

  op=(unsigned char*)0x40001000;

  while(op<=(unsigned char*)0x40001400)
  {
    *op=0;
	op++;
  }

  ch=&rec2[0];
  op=(unsigned char*)0x40001000;
  f=0;
  while((f==0))
  {  
  ch++;	                                                        
  pass=0;
  count=0x00;                                                             //no of opcodes init
  for(i=0;i<2;i++)														   //loop to obtain no of opcodes in a single record
  {
    if(*ch>0x2f && *ch<0x3a)
	{
	  switch(*ch)
	  {
	    case 0x30: temp=0x00;
		           break;
		case 0x31: temp=0x01;
		           break;
		case 0x32: temp=0x02;
		           break;
		case 0x33: temp=0x03;
		           break;
		case 0x34: temp=0x04;
		           break;
		case 0x35: temp=0x05;
		           break;
		case 0x36: temp=0x06;
		           break;
		case 0x37: temp=0x07;
		           break;
		case 0x38: temp=0x08;
		           break;
		case 0x39: temp=0x09;
		           break;
		default:   temp=0;  
	  }
	}  
	if(*ch>0x40 && *ch<0x47)
	{
	  switch(*ch)
	  {
	    case 0x41:temp=0x0a;
		          break; 
		case 0x42:temp=0x0b;
		          break; 
		case 0x43:temp=0x0c;
		          break; 
		case 0x44:temp=0x0d;
		          break; 
		case 0x45:temp=0x0e;
		          break;
		case 0x46:temp=0x0f;
		          break; 				   
	  }
	}
	if(pass==0)
	{
	  msb=temp<<4;
	}
	else
	{
	  lsb=temp;
	}
	pass++;
	ch++;
	if(pass==2)
      count=(msb|lsb);                                          //if last pass get count
  }
  ch=ch+4;		                                              //point to type of record
  pass=0;
   for(i=0;i<2;i++)
  {
    if(*ch>0x2f && *ch<0x3a)
	{
	  switch(*ch)
	  {
	    case 0x30: temp=0x00;
		           break;
		case 0x31: temp=0x01;
		           break;
		case 0x32: temp=0x02;
		           break;
		case 0x33: temp=0x03;
		           break;
		case 0x34: temp=0x04;
		           break;
		case 0x35: temp=0x05;
		           break;
		case 0x36: temp=0x06;
		           break;
		case 0x37: temp=0x07;
		           break;
		case 0x38: temp=0x08;
		           break;
		case 0x39: temp=0x09;
		           break;
		default:   temp=0;  
	  }
	}  
	if(*ch>0x40 && *ch<0x47)
	{
	  switch(*ch)
	  {
	    case 0x41:temp=0x0a;
		          break; 
		case 0x42:temp=0x0b;
		          break; 
		case 0x43:temp=0x0c;
		          break; 
		case 0x44:temp=0x0d;
		          break; 
		case 0x45:temp=0x0e;
		          break;
		case 0x46:temp=0x0f;
		          break; 				   
	  }
	}
	if(pass==0)
	{
	  t1=temp<<4;
	}
	else
	{
	  t2=temp;
	}
	pass++;
	ch++;
	if(pass==2)
      type=(t1|t2);                                       //store type of record
  }
  if(type==0x01 | type==0x05)
  {
   f=1;
   break;
  }
  if(type==0x00)										//if data record store op codes in op[]
  {
    pass=0;
	for(i=0;i<count*0x02;i++)
	{
	  if(*ch>0x2f && *ch<0x3a)
	{
	  switch(*ch)
	  {
	    case 0x30: temp=0x00;
		           break;
		case 0x31: temp=0x01;
		           break;
		case 0x32: temp=0x02;
		           break;
		case 0x33: temp=0x03;
		           break;
		case 0x34: temp=0x04;
		           break;
		case 0x35: temp=0x05;
		           break;
		case 0x36: temp=0x06;
		           break;
		case 0x37: temp=0x07;
		           break;
		case 0x38: temp=0x08;
		           break;
		case 0x39: temp=0x09;
		           break;
		default:   temp=0;  
	  }
	} 
	if(*ch>0x40 && *ch<0x47)
	{
	  switch(*ch)
	  {
	    case 0x41:temp=0x0a;
		          break; 
		case 0x42:temp=0x0b;
		          break; 
		case 0x43:temp=0x0c;
		          break; 
		case 0x44:temp=0x0d;
		          break; 
		case 0x45:temp=0x0e;
		          break;
		case 0x46:temp=0x0f;
		          break; 				   
	  }
	}
	if(pass==0)
	{
	  t1=temp<<4;
	}
	else
	{
	  t2=temp;
	}
	pass++;
	ch++;
	if(pass==2)
	{
      temp=(t1|t2);
	  *op=temp;
	  op++; 
	  //flag++;
	  /*if(flag==4)
	   {
	     flag=0;
		 op=op+8;
	   }*/
	  pass=0;  
	}  
   }
   ch+=2;
  }
  }
  command[0]=50;
  command[1]=9;
  command[2]=9;

  iap_entry (command, result);

  command[0]=52;
  command[1]=9;
  command[2]=9;
  command[3]=60000;

  iap_entry (command, result);

  command[0]=50;
  command[1]=9;
  command[2]=9;

  iap_entry (command, result);

  command[0]=51;
  command[1]=0x00010000;
  command[2]=0x40001000;
  command[3]=512;
  command[4]=60000;
  iap_entry (command, result);
  op=(unsigned char*)0x40001000;

  while(op<=(unsigned char*)0x40001400)
  {
    *op=0;
	op++;
  }
  
//3rd
ch=&rec3[0];
op=(unsigned char*)0x40001000;
f=0;  
  while((f==0))
  {  
  ch++;	                                                        
  pass=0;
  count=0x00;                                                             //no of opcodes init
  for(i=0;i<2;i++)														   //loop to obtain no of opcodes in a single record
  {
    if(*ch>0x2f && *ch<0x3a)
	{
	  switch(*ch)
	  {
	    case 0x30: temp=0x00;
		           break;
		case 0x31: temp=0x01;
		           break;
		case 0x32: temp=0x02;
		           break;
		case 0x33: temp=0x03;
		           break;
		case 0x34: temp=0x04;
		           break;
		case 0x35: temp=0x05;
		           break;
		case 0x36: temp=0x06;
		           break;
		case 0x37: temp=0x07;
		           break;
		case 0x38: temp=0x08;
		           break;
		case 0x39: temp=0x09;
		           break;
		default:   temp=0;  
	  }
	}  
	if(*ch>0x40 && *ch<0x47)
	{
	  switch(*ch)
	  {
	    case 0x41:temp=0x0a;
		          break; 
		case 0x42:temp=0x0b;
		          break; 
		case 0x43:temp=0x0c;
		          break; 
		case 0x44:temp=0x0d;
		          break; 
		case 0x45:temp=0x0e;
		          break;
		case 0x46:temp=0x0f;
		          break; 				   
	  }
	}
	if(pass==0)
	{
	  msb=temp<<4;
	}
	else
	{
	  lsb=temp;
	}
	pass++;
	ch++;
	if(pass==2)
      count=(msb|lsb);                                          //if last pass get count
  }
  ch=ch+4;		                                              //point to type of record
  pass=0;
   for(i=0;i<2;i++)
  {
    if(*ch>0x2f && *ch<0x3a)
	{
	  switch(*ch)
	  {
	    case 0x30: temp=0x00;
		           break;
		case 0x31: temp=0x01;
		           break;
		case 0x32: temp=0x02;
		           break;
		case 0x33: temp=0x03;
		           break;
		case 0x34: temp=0x04;
		           break;
		case 0x35: temp=0x05;
		           break;
		case 0x36: temp=0x06;
		           break;
		case 0x37: temp=0x07;
		           break;
		case 0x38: temp=0x08;
		           break;
		case 0x39: temp=0x09;
		           break;
		default:   temp=0;  
	  }
	}  
	if(*ch>0x40 && *ch<0x47)
	{
	  switch(*ch)
	  {
	    case 0x41:temp=0x0a;
		          break; 
		case 0x42:temp=0x0b;
		          break; 
		case 0x43:temp=0x0c;
		          break; 
		case 0x44:temp=0x0d;
		          break; 
		case 0x45:temp=0x0e;
		          break;
		case 0x46:temp=0x0f;
		          break; 				   
	  }
	}
	if(pass==0)
	{
	  t1=temp<<4;
	}
	else
	{
	  t2=temp;
	}
	pass++;
	ch++;
	if(pass==2)
      type=(t1|t2);                                       //store type of record
  }
  if(type==0x01 | type==0x05)
  {
   f=1;
   break;
  }
  if(type==0x00)										//if data record store op codes in op[]
  {
    pass=0;
	for(i=0;i<count*0x02;i++)
	{
	  if(*ch>0x2f && *ch<0x3a)
	{
	  switch(*ch)
	  {
	    case 0x30: temp=0x00;
		           break;
		case 0x31: temp=0x01;
		           break;
		case 0x32: temp=0x02;
		           break;
		case 0x33: temp=0x03;
		           break;
		case 0x34: temp=0x04;
		           break;
		case 0x35: temp=0x05;
		           break;
		case 0x36: temp=0x06;
		           break;
		case 0x37: temp=0x07;
		           break;
		case 0x38: temp=0x08;
		           break;
		case 0x39: temp=0x09;
		           break;
		default:   temp=0;  
	  }
	} 
	if(*ch>0x40 && *ch<0x47)
	{
	  switch(*ch)
	  {
	    case 0x41:temp=0x0a;
		          break; 
		case 0x42:temp=0x0b;
		          break; 
		case 0x43:temp=0x0c;
		          break; 
		case 0x44:temp=0x0d;
		          break; 
		case 0x45:temp=0x0e;
		          break;
		case 0x46:temp=0x0f;
		          break; 				   
	  }
	}
	if(pass==0)
	{
	  t1=temp<<4;
	}
	else
	{
	  t2=temp;
	}
	pass++;
	ch++;
	if(pass==2)
	{
      temp=(t1|t2);
	  *op=temp;
	  op++; 
	  //flag++;
	  /*if(flag==4)
	   {
	     flag=0;
		 op=op+8;
	   }*/
	  pass=0;  
	}  
   }
   ch+=2;
  }
  }
  command[0]=50;
  command[1]=0x0A;					 //sector no.
  command[2]=0x0A;

  iap_entry (command, result);

  command[0]=52;
  command[1]=0x0a;
  command[2]=0x0a;
  command[3]=60000;

  iap_entry (command, result);

  command[0]=50;
  command[1]=0x0a;
  command[2]=0x0a;

  iap_entry (command, result);

  command[0]=51;
  command[1]=0x00018000;
  command[2]=0x40001000;
  command[3]=512;
  command[4]=60000;

  iap_entry (command, result);

  op=(unsigned char*)0x40001000;

  while(op<=(unsigned char*)0x40001400)
  {
    *op=0;
	op++;
  }
 
 ch=&rec4[0];
  op=(unsigned char*)0x40001000;
   f=0;
   while((f==0))
  {  
  ch++;	                                                        
  pass=0;
  count=0x00;                                                             //no of opcodes init
  for(i=0;i<2;i++)														   //loop to obtain no of opcodes in a single record
  {
    if(*ch>0x2f && *ch<0x3a)
	{
	  switch(*ch)
	  {
	    case 0x30: temp=0x00;
		           break;
		case 0x31: temp=0x01;
		           break;
		case 0x32: temp=0x02;
		           break;
		case 0x33: temp=0x03;
		           break;
		case 0x34: temp=0x04;
		           break;
		case 0x35: temp=0x05;
		           break;
		case 0x36: temp=0x06;
		           break;
		case 0x37: temp=0x07;
		           break;
		case 0x38: temp=0x08;
		           break;
		case 0x39: temp=0x09;
		           break;
		default:   temp=0;  
	  }
	}  
	if(*ch>0x40 && *ch<0x47)
	{
	  switch(*ch)
	  {
	    case 0x41:temp=0x0a;
		          break; 
		case 0x42:temp=0x0b;
		          break; 
		case 0x43:temp=0x0c;
		          break; 
		case 0x44:temp=0x0d;
		          break; 
		case 0x45:temp=0x0e;
		          break;
		case 0x46:temp=0x0f;
		          break; 				   
	  }
	}
	if(pass==0)
	{
	  msb=temp<<4;
	}
	else
	{
	  lsb=temp;
	}
	pass++;
	ch++;
	if(pass==2)
      count=(msb|lsb);                                          //if last pass get count
  }
  ch=ch+4;		                                              //point to type of record
  pass=0;
   for(i=0;i<2;i++)
  {
    if(*ch>0x2f && *ch<0x3a)
	{
	  switch(*ch)
	  {
	    case 0x30: temp=0x00;
		           break;
		case 0x31: temp=0x01;
		           break;
		case 0x32: temp=0x02;
		           break;
		case 0x33: temp=0x03;
		           break;
		case 0x34: temp=0x04;
		           break;
		case 0x35: temp=0x05;
		           break;
		case 0x36: temp=0x06;
		           break;
		case 0x37: temp=0x07;
		           break;
		case 0x38: temp=0x08;
		           break;
		case 0x39: temp=0x09;
		           break;
		default:   temp=0;  
	  }
	}  
	if(*ch>0x40 && *ch<0x47)
	{
	  switch(*ch)
	  {
	    case 0x41:temp=0x0a;
		          break; 
		case 0x42:temp=0x0b;
		          break; 
		case 0x43:temp=0x0c;
		          break; 
		case 0x44:temp=0x0d;
		          break; 
		case 0x45:temp=0x0e;
		          break;
		case 0x46:temp=0x0f;
		          break; 				   
	  }
	}
	if(pass==0)
	{
	  t1=temp<<4;
	}
	else
	{
	  t2=temp;
	}
	pass++;
	ch++;
	if(pass==2)
      type=(t1|t2);                                       //store type of record
  }
  if(type==0x01 | type==0x05)
  {
   f=1;
   break;
  }
  if(type==0x00)										//if data record store op codes in op[]
  {
    pass=0;
	for(i=0;i<count*0x02;i++)
	{
	  if(*ch>0x2f && *ch<0x3a)
	{
	  switch(*ch)
	  {
	    case 0x30: temp=0x00;
		           break;
		case 0x31: temp=0x01;
		           break;
		case 0x32: temp=0x02;
		           break;
		case 0x33: temp=0x03;
		           break;
		case 0x34: temp=0x04;
		           break;
		case 0x35: temp=0x05;
		           break;
		case 0x36: temp=0x06;
		           break;
		case 0x37: temp=0x07;
		           break;
		case 0x38: temp=0x08;
		           break;
		case 0x39: temp=0x09;
		           break;
		default:   temp=0;  
	  }
	} 
	if(*ch>0x40 && *ch<0x47)
	{
	  switch(*ch)
	  {
	    case 0x41:temp=0x0a;
		          break; 
		case 0x42:temp=0x0b;
		          break; 
		case 0x43:temp=0x0c;
		          break; 
		case 0x44:temp=0x0d;
		          break; 
		case 0x45:temp=0x0e;
		          break;
		case 0x46:temp=0x0f;
		          break; 				   
	  }
	}
	if(pass==0)
	{
	  t1=temp<<4;
	}
	else
	{
	  t2=temp;
	}
	pass++;
	ch++;
	if(pass==2)
	{
      temp=(t1|t2);
	  *op=temp;
	  op++; 
	  //flag++;
	  /*if(flag==4)
	   {
	     flag=0;
		 op=op+8;
	   }*/
	  pass=0;  
	}  
   }
   ch+=2;
  }
  }
  command[0]=50;
  command[1]=0x0B;					 //sector no.
  command[2]=0x0B;

  iap_entry (command, result);

  command[0]=52;
  command[1]=0x0B;
  command[2]=0x0B;
  command[3]=60000;

  iap_entry (command, result);

  command[0]=50;
  command[1]=0x0B;
  command[2]=0x0B;

  iap_entry (command, result);

  command[0]=51;
  command[1]=0x00020000;
  command[2]=0x40001000;
  command[3]=512;
  command[4]=60000;

  iap_entry (command, result);


  op=(unsigned char*)0x40001000;

  while(op<=(unsigned char*)0x40001400)
  {
    *op=0;
	op++;
  }
  setup();

 /* set_sys_tick();
  {
  typedef void (*t_funcPtr)(void);
  t_funcPtr fnptr = (t_funcPtr)0x00008000;
  (*fnptr)();
  }
  */	
  while(1);
}

void set_sys_tick()
{
  T0TCR|=(1<<1);                      //reset timer and hold count to 0

	T0CTCR&=~((1<<0)|(1<<1));           //set timer operation on timer 1
	
	T0PR=0x00000000;                    //no perscalar
	
	T0MCR|=(1<<0);                      //generate interrupt on match
	T0MCR|=(1<<1);                      //reset timer on match
	
	//T0MR0=0x00E4E79C;                   //value to match before interrupt
	T0MR0=0x000008FF;
	
	VICIntEnable|=(1<<4);               //enable timer0 interrupt
	//VICIntSelect&=!(1<<4);              //make as timer0 interrupt IRQ
	VICIntSelect|=(1<<4);              //make as timer0 interrupt FIQ
	VICVectCntl0 = 0x20 | 4;            //assign interrup no 4 that is timer0 to slot 0
	VICVectCntl0|=(1<<5);               //enable vectored interrupt

	VICVectAddr0=(unsigned long)TIMER0_Isr;            //assign isr address

	//VICSoftInt|=(1<<4);					//test ISR

	T0TCR&=~(1<<1);						//enable timer
	T0TCR|=(1<<0);                      //enable timer 0

}