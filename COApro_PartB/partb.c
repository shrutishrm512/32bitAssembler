/**********************
COA PROJECT: ISA DESIGN
PART B - 1
GROUP:RICHA, AYUSHI, SHRUTI, VASUNDHARA
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define binaryoutfile "binoutfile3"		// Binary equivalent output file.
// Change the name of Binary output text file name to get the corresponding output file of the respective mnemonic code file

#define symboltable "symbol3"			// Symbol table file
/* format of symbol table :
 label name	logical address(in decimal)	lablel value taken before replacement	logical address(in binary)
*/

#define max 100

void getfilename(char *);
int substr(char [],char []);

int main()
{
	char asmfile[max];
	char ch1[50],ch2[50],ch3[50],ch4[50];
	int len=0;int k=0;
	int counter=0;
	int i,j;
	int quo;
	getfilename(asmfile);
	FILE *file1,*file2,*file3,*file4,*file5;
	file1 = fopen(asmfile, "r");   // open assembly code file for reading (Contains Mnemonic code).
	file3 = fopen("binaryoutfile.txt", "w");    //opens the Intermediate file for writing.
	file4 = fopen(symboltable, "w"); 
   	if(file1 == NULL) {            //checks if mnemonic code file exists or not.
      		printf("\nFile %s could not be found\n", asmfile);             
   	} 
	else 				//if file exists.
	{           
     		printf("\nFile %s is now the Assembly Language code file.\n", asmfile);
		fscanf(file1,"%s",ch1);	
		while(!feof(file1))
		{	
			file2 = fopen("binary.txt","r");	//open the binary text file to get the equivalent binary number.
			int val=atoi(ch1);
			while(!feof(file2))
			{
				fscanf(file2,"%s",ch2);
				if(strcmp(ch1,ch2)==0)
				{
					fscanf(file2,"%s",ch2);
					fprintf(file3,"%s",ch2);
					len=len+strlen(ch2);
					if(len==32){
						counter=counter+4;			
						fprintf(file3,"\n");
						len=0;
					}
					else if(len==8){				//checking for label.
						i=1,j=0;
						quo=counter;
						int binvalue[13]={0};
						while(quo!=0)
						{
							binvalue[i++]=quo%2;
							quo=quo/2;
						}
						fprintf(file4,"%s\t",ch1);
						fprintf(file4,"%d\t",counter);
						fprintf(file4,"%s\t",ch2);
						for(j=8;j>0;j--)
						{
							fprintf(file4,"%d",binvalue[j]);
						}
						fprintf(file4,"\n");
						fprintf(file3,"\n");
						len=0;
					}
					break;
				}		
				else if(val>0 && val<=4095)				//checking for immediate constant.
				{
					i=1,j=0;
					quo=val;
					int binvalue[13]={0};
					while(quo!=0)
					{
						binvalue[i++]=quo%2;
						quo=quo/2;
					}
					for(j=12;j>0;j--)
					{
						fprintf(file3,"%d",binvalue[j]);
					}
					len=len+12;	
					break;
				}	
				else 
					fscanf(file2,"%s",ch2);
			}
			fclose(file2); 				//closing the binary value files.
			fscanf(file1,"%s",ch1);
		}
		fclose(file1);					//closing the file containing mnemonic code.
		fclose(file3);					//closing the intermediate file
		fclose(file4);
		file5 = fopen(binaryoutfile, "w");    		//opens the final output file for writing.
		file3 = fopen("binaryoutfile.txt", "r"); 	//opens the intermediate file for reading.
		fscanf(file3,"%s",ch3);
		char sub[10];
		while(!feof(file3))
		{	
			k=strlen(ch3);
			if(substr(ch3,"000000000000000100000110")) 	//comparing the extracted opcode with equivalent JUMP opcode.
			{
				j=0;
				for(i=24;i<32;i++)
					sub[j++]=ch3[i];
				sub[j]='\0';
				file4 = fopen(symboltable, "r"); 	//opening Symbol table file.
				while(!feof(file4))
				{
					for(i=0;i<3;i++)
						fscanf(file4,"%s",ch4);
					if(strcmp(sub,ch4)==0)	//comparing the extracted label with binary equivalent value in symbol table.
					{
						fscanf(file4,"%s",ch4);
						fprintf(file5,"%s","000000000000000100000110");
						fprintf(file5,"%s\n",ch4);
					}
					else
						fscanf(file4,"%s",ch4);
				}
				k=0;
				fclose(file4);					//closing Symbol table file.
			}
			else if(substr(ch3,"000000000000000100000111"))		//comparing the extracted opcode with equivalent JEQ opcode.
			{
				
				j=0;
				for(i=24;i<32;i++)
					sub[j++]=ch3[i];
				sub[j]='\0';
				file4 = fopen(symboltable, "r"); 		//opening Symbol table file for reading.
				while(!feof(file4))
				{
					for(i=0;i<3;i++)
						fscanf(file4,"%s",ch4);
					if(strcmp(sub,ch4)==0)	//comparing the extracted label with binary equivalent value in symbol table.
					{
						fscanf(file4,"%s",ch4);
						fprintf(file5,"%s","000000000000000100000111");
						fprintf(file5,"%s\n",ch4);
					}
					else
						fscanf(file4,"%s",ch4);
				}
				k=0;
				fclose(file4);				//closing Symbol table file.
			}
			else if(k==8)
			{
				file4 = fopen(symboltable, "r"); 	//opening Symbol table file for reading.
				while(!feof(file4))
				{
					for(i=0;i<3;i++)
						fscanf(file4,"%s",ch4);
					if(strcmp(ch3,ch4)==0)
					{
						fscanf(file4,"%s",ch4);
						fprintf(file5,"%s\n",ch4);
					}
					else
						fscanf(file4,"%s",ch4);
				}
				k=0;
				fclose(file4);	
			}
			else
			{
				fprintf(file5,"%s\n",ch3);
				k=0;
			}
			fscanf(file3,"%s",ch3);
		}	
		fclose(file5);		//closing the final output file after writing the binary equivalent code for every instruction.
		fclose(file3);		//closing the intermediate file.	
	}	
	return 0;
}

void getfilename(char *assem)
{
	printf("Enter the Assembly Language code file name:\n");
	scanf("%s",assem);
}

int substr(char str[],char search[])	//A utility function to check if a string is substring of other string or not (extraction of opcode).
{	
	int count1=0,count2,i,j,flag;
	while (str[count1]!='\0')
		count1++;
	count2=24;			//Count = 24,Since opcode length for all control transfer statements(namely JUMP,JEQ) is 24.  
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
	//printf("%d\n",flag);
	if (flag==1)
		return 1;
	else
		return 0;
}

/*************************************************************END OF CODE**********************************************************************/
