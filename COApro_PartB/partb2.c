/**********************
COA PROJECT: ISA DESIGN
PART B - 2
GROUP:RICHA, AYUSHI, SHRUTI, VASUNDHARA
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max 100
int reg[13]={0};	//register array
int flagreg[3];		//flag registers having bits for Sign, Zero and Negative. 
int cmpval=0;		//comparison result
int counter=-4;		//Global counter to get PC

void getfilename(char *);		//A utility function  to get file name from user.
int conversion(char []);		//A utility function to convert binary number into decimal.
void status();				//A utility function to print the status of all the resgisters.
int regindex(char [],char [][33]);	//A utility function to get the corresponding mnemonic name index from the extracted binary string.
int substr(char [],char []);		//A utility function to check if a string is substring of other string or not (extraction of opcode).
void move1(int ,int );			// Function for MOVE1(16 bit) having parameters- immediate constant and register.
void move2(int ,int );			// Function for MOVE2(24 bit) having parameters as registers.
void cmp(int ,int );			// Function to compare two values. 
void add(int ,int );			// Function to add two values.
void sub(int ,int );			// Function to perform subtraction on two values.
void mul(int ,int );			// Function to perform multipliacation on two values.
void divide(int ,int );			// Function to perform subtraction on two values.
int jeq();				// Function to compare if value is zero.
void halt();				// Function to end the program.

int main()
{
	char ch1[50],ch2[50],ch[50];
	FILE *file1,*fpsym;
	char binfile[max],symfile[max];
	getfilename(binfile);		//Enter the Binary equivalent file name - 1-binoutfile1(for code1) 2-binoutfile2(for code2)
	char mnemonic[23][6]={"RG00","RG01","RG02","RG03","RG04","RG05","RG06","RG07","RG08","RG09","RG10",
			   "RG11","RG12","MOVE1","CMP","ADD","SUB","MUL","DIV","MOVE2","JUMP","JEQ","HLT",
		           };
	char bin_equi[23][33]={"0000","0001","0010","0011","0100","0101","0110","0111","1000","1001","1010","1011","1100",
			   "0000000000000000","000000000000000100000000","000000000000000100000001",
			   "000000000000000100000010","000000000000000100000011","000000000000000100000100",
			   "000000000000000100000101","000000000000000100000110","000000000000000100000111",
			   "00000000000000010000100000000000"
		           };
	int i,j,k,address;
	int index=0,jeqval=0,val=0;
	int fsret;
	int index1[2];
	char arr1[5],arr2[5],label[9],chl[10];
	file1 = fopen(binfile, "r");				//opening the binary output file generated through part1.
	printf("Enter the name of file having Symbol table");
	getfilename(symfile);		//Enter the symbol table file name : 1-symbol1 2-symbol2
	if(file1 == NULL) {            //checks if Binary equivalent code file exists or not.
      		printf("\nFile %s could not be found\n", binfile);             
   	} 
	else 				//if file exists.
	{          
		fscanf(file1,"%s",ch1);
		while(!feof(file1))
		{
			if(substr(ch1,bin_equi[13]))	
			{
				char imm[13];
				k=0;
				for(j=16;j<28;j++)		//Extracting the immediate values.
					imm[k++]=ch1[j];
				imm[k]='\0';
				k=0;
				for(j=28;j<32;j++)
					arr1[k++]=ch1[j];	//Extracting 4-bit register.
				arr1[k]='\0';
				int imm_val=conversion(imm);	
				index=regindex(arr1,bin_equi);
				move1(imm_val,index);
				printf("IR: MOVE1 %d %s\n",imm_val,mnemonic[index]);
				counter=counter+4;
				printf("Status of Program Counter(PC) %d\n",counter);
				status();
			}	
			else if(substr(ch1,bin_equi[14]))
			{
				k=0;
				for(j=24;j<28;j++)		//Extracting 4-bit register.
					arr1[k++]=ch1[j];
				arr1[k]='\0';
				k=0;
				for(j=28;j<32;j++)		//Extracting 4-bit register.
					arr2[k++]=ch1[j];
				arr2[k]='\0';
				index1[0]=regindex(arr1,bin_equi);
				index1[1]=regindex(arr2,bin_equi);
				cmp(index1[0],index1[1]);
				printf("IR: CMP %s %s\n",mnemonic[index1[0]],mnemonic[index1[1]]);
				counter+=4;
				printf("Status of Program Counter(PC) %d\n",counter);
				status();
			}	
			else if(substr(ch1,bin_equi[15]))
			{
				k=0;
				for(j=24;j<28;j++)		//Extracting 4-bit register.
					arr1[k++]=ch1[j];
				arr1[k]='\0';
				k=0;
				for(j=28;j<32;j++)
					arr2[k++]=ch1[j];	//Extracting 4-bit register.
				arr2[k]='\0';
				index1[0]=regindex(arr1,bin_equi);
				index1[1]=regindex(arr2,bin_equi);
				add(index1[0],index1[1]);
				printf("IR: ADD %s %s\n",mnemonic[index1[0]],mnemonic[index1[1]]);
				counter+=4;
				printf("ALU Control Signal for ADD: 00\n");
				printf("Status of Program Counter(PC) %d\n",counter);
				status();
			}
			else if(substr(ch1,bin_equi[16]))
			{
				k=0;
				for(j=24;j<28;j++)
					arr1[k++]=ch1[j];		//Extracting 4-bit register.
				arr1[k]='\0';
				k=0;
				for(j=28;j<32;j++)			//Extracting 4-bit register.
					arr2[k++]=ch1[j];
				arr2[k]='\0';
				index1[0]=regindex(arr1,bin_equi);
				index1[1]=regindex(arr2,bin_equi);
				sub(index1[0],index1[1]);
				printf("IR: SUB %s %s\n",mnemonic[index1[0]],mnemonic[index1[1]]);
				counter+=4;
				printf("ALU Control Signal for SUB: 01\n");
				printf("Status of Program Counter(PC) %d\n",counter);
				status();
			}
			else if(substr(ch1,bin_equi[17]))
			{
				k=0;
				for(j=24;j<28;j++)			//Extracting 4-bit register.
					arr1[k++]=ch1[j];
				arr1[k]='\0';
				k=0;
				for(j=28;j<32;j++)			//Extracting 4-bit register.
					arr2[k++]=ch1[j];
				arr2[k]='\0';
				index1[0]=regindex(arr1,bin_equi);
				index1[1]=regindex(arr2,bin_equi);
				mul(index1[0],index1[1]);
				printf("IR: MUL %s %s\n",mnemonic[index1[0]],mnemonic[index1[1]]);
				counter+=4;
				printf("ALU Control Signal for MUL: 10\n");
				printf("Status of Program Counter(PC) %d\n",counter);
				status();
			}
			else if(substr(ch1,bin_equi[18]))
			{
				k=0;
				for(j=24;j<28;j++)			//Extracting 4-bit register.
					arr1[k++]=ch1[j];
				arr1[k]='\0';
				k=0;
				for(j=28;j<32;j++)			//Extracting 4-bit register.
					arr2[k++]=ch1[j];
				arr2[k]='\0';
				index1[0]=regindex(arr1,bin_equi);
				index1[1]=regindex(arr2,bin_equi);
				divide(index1[0],index1[1]);
				printf("IR: DIV %s %s\n",mnemonic[index1[0]],mnemonic[index1[1]]);
				counter+=4;
				printf("ALU Control Signal for DIV: 11\n");
				printf("Status of Program Counter(PC) %d\n",counter);
				status();
			}
			else if(substr(ch1,bin_equi[19]))
			{
				k=0;
				for(j=24;j<28;j++)			//Extracting 4-bit register.
					arr1[k++]=ch1[j];
				arr1[k]='\0';
				k=0;
				for(j=28;j<32;j++)			//Extracting 4-bit register.
					arr2[k++]=ch1[j];
				arr2[k]='\0';
				index1[0]=regindex(arr1,bin_equi);
				index1[1]=regindex(arr2,bin_equi);
				move2(index1[0],index1[1]);
				printf("IR: MOVE2 %s %s\n",mnemonic[index1[0]],mnemonic[index1[1]]);
				counter+=4;
				printf("Status of Program Counter(PC) %d\n",counter);
				status();
			}
			else if(substr(ch1,bin_equi[20]))
			{
				k=0;
				fpsym = fopen(symfile,"r");	//Opening the symbol table file
				fscanf(fpsym,"%s",ch);
				strcpy(chl,ch);
				while(!feof(fpsym))
				{
					for(j=24;j<32;j++)	
						label[k++]=ch1[j];
					label[k]='\0';
					address=conversion(label);
					fscanf(fpsym,"%s",ch);
					val=atoi(ch);		//Checking the label address by opening the symbol table
					if(val==address)
					{
						counter=address;
						printf("Status of Program Counter(PC) %d\n",counter);
						rewind(file1);
						fsret=fseek(file1,address*8,SEEK_SET);//Sets file pointer on the label address for recursion.
						break;
					}
					else{
						fscanf(fpsym,"%s",ch);
						strcpy(chl,ch);
						fscanf(fpsym,"%s",ch);			
					}
				}
				fclose(fpsym);				//Closing the symbol table file.
				printf("IR: JUMP %s\n",chl);
				status();							
			}
			else if(substr(ch1,bin_equi[21]))
			{
				jeqval=jeq();
				k=0;
				for(j=24;j<32;j++)
					label[k++]=ch1[j];
				label[k]='\0';
				fpsym=fopen(symfile,"r");	
				while(!feof(fpsym))
				{
					fscanf(fpsym,"%s",ch);	
					if(strcmp(label,ch)==0){
						fseek(fpsym, -24,SEEK_CUR); //Move the file pointer to the corres. label name for that address
						fscanf(fpsym,"%s",ch);
						strcpy(chl,ch);		//Copying the label name
						break;
					}
				}
				fclose(fpsym);				//closing the symbol table file.
				if(jeqval==1)
				{
					address=conversion(label);
					fpsym = fopen(symfile,"r");
					fscanf(fpsym,"%s",ch);
					strcpy(chl,ch);
					while(!feof(fpsym))
					{
						counter=address;
						printf("Status of Program Counter(PC) %d\n",counter);
						fscanf(fpsym,"%s",ch);
						val=atoi(ch);
						if(val==address)		//Checking the label address
						{
							rewind(file1);		// Sets the file pointer at the begining
							fsret=fseek(file1,(address+3)*8,SEEK_SET);//Set the file pointer to the corres. label
							break;
						}
						else{
							counter+=4;
							printf("Status of Program Counter(PC) %d\n",counter);
							fscanf(fpsym,"%s",ch);
							fscanf(fpsym,"%s",ch);
							fscanf(fpsym,"%s",ch);
							strcpy(chl,ch);
						}
					}
					fclose(fpsym);		//Closing the symbol table file
				}
				printf("IR: JEQ %s\n",chl);
				status();	
			}
			else if(substr(ch1,bin_equi[22]))
			{
				printf("IR: HLT\n");
				status();
				halt();		//Exit from the program
			}
			fscanf(file1,"%s",ch1);
		}
		fclose(file1);		//Closing the binary equivalent file of the code
	}
	return 0;	
}

int conversion(char a[])	//For the code - Binary to decimal conversion- the highest binary value can be taken is 1011
{
	int rem,base=1;
	long long int val=atoi(a);
	int dec=0;
	while(val>0)
	{
		rem=val % 10;
		dec=dec+rem*base;
		val=val/10;
		base=base*2;
	}
	return dec;
}	

int regindex(char arr1[],char bin_equi[][33])
{
	int j,index;
	for(j=0;j<13;j++)
	{
		if(substr(arr1,bin_equi[j]))
		{	
			index=j;
			break;
		}
		else 
			continue;
	}
	return index;
}

void getfilename(char *assem)		//A utility function to get the name of file from user.
{
	printf("Enter File name:\n");
	scanf("%s",assem);
}

int substr(char str[],char search[])	//A utility function to check if a string is substring of other string or not (extraction of opcode).
{	
	int count1=0,count2=0,i,j,flag;
	while(str[count1]!='\0')
		count1++;
	while(search[count2]!='\0')
		count2++;			//Count = 24,Since opcode length for all control transfer statements(namely JUMP,JEQ) is 24.  
	for(i=0;i<=count1-count2;i++)
	{
		for(j=i;j<i+count2;j++)
		{
			flag=1;
			if (str[j]!=search[j-i])
			{
				flag=0;
			   break;
			}
		}
		if (flag==1)
			break;
	}
	if (flag==1)
		return 1;
	else
		return 0;
}

void status()				//A utility function to show status of all the registers.
{
	printf("The Status of Registers:\n");
	int i;
	for(i=0;i<13;i++)
		printf("RG%d\t%d\n",i,reg[i]);
	printf("Status of flag register:\n");
	printf("__S_Z_N__\n");
	printf("__%d_%d_%d__\n",flagreg[0],flagreg[1],flagreg[2]);
}

void move1(int val,int index)
{
	reg[index]=val;  		//Copying the value to the corresponding index of register array.
}

void cmp(int index1,int index2)
{
	int i;
	for(i=0;i<3;i++)
		flagreg[i]=0;
	int diff=reg[index2]-reg[index1];
	if(diff==0){			//checking the difference is zero or not.
		cmpval=1;
		flagreg[1]=1;
	}
	else if(diff>0)			//checking the difference is positive or not.
		cmpval= 2;
	else{				//checking the difference is negative or not.
		cmpval=3;
		flagreg[2]=1;
	}
}

void add(int index1,int index2)		//A utility function to divide two values.
{
	reg[index2]=reg[index1]+reg[index2];
}

void sub(int index1,int index2)		//A utility function to subtract two values.
{
	reg[index2]=reg[index2]-reg[index1];
}

void mul(int index1,int index2)		//A utility function to multiply two values.
{
	reg[index2]=reg[index1]*reg[index2];
}

void divide(int index1,int index2)	//A utility function to divide two values.
{
	reg[index2]=reg[index2]/reg[index1];
}

void move2(int index1,int index2)	//A utility function to move a value from one register to other
{
	reg[index2]=reg[index1];
}

int jeq()
{
	if(cmpval==1)		//Checking the comparsion condition is true or not.
		return 1;
	else
		return 0;
}

void halt()
{
	exit(0);
}

/*****************************************************************END OF CODE*****************************************************************/
