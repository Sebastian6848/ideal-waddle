#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX_LINE 1024

/*定义基站结构体*/
/*按分块方法存储数据*/
/*纸面X(-700,110000),Y(-200,40000),每个区块长900，宽600，共8241个*/
typedef struct Station
{
	int X_axis[10];
	int Y_axis[10];
	int Type[10];
	float Span[10];
	int No[10];
	int tag;//表示该区块已存储站点个数 
}St;

typedef struct path
{
	int Xa;
	int Ya;
	int Xb;
	int Yb;
	int Speed;
	int Hour;
	int Minit;
}Path;

typedef struct Wz
{
	int Xa;
	int Ya;
	int Xb;
	int Yb;
	int Speed;
	int Hour;
	int Minit;
	int No;
}W_Path;

/*目录*/
void myMenu()
{
	printf(" * * * * * * * * * 菜     单 * * * * * * * * * *\n");
	printf("     1 任务1：显示首个及最后一个区域中的基站数据                  \n");
	printf("     2 任务2：显示首个及最后一个区域旁边的基站数据                \n");
	printf("     3 任务3：给定坐标，求关联基站                     \n");
	printf("     4 任务4：给定坐标，求基站                                     \n");
	printf("     5 任务5：给定移动终端的移动轨迹，计算需要不断切换到哪些基站进行通信                                    \n");
	printf("     6 拓展1：分析进入和离开首个基站信号有效范围的时间         \n");
	printf("     7 拓展2: 分析第3段、第6段开始阶段切换基站信号重叠区，计算通过重叠区的时间长度\n") ;
	printf("     10 退出程序\n") ;
	printf(" * * * * * * * * * * * * * * * * * * * * * * * *\n");
}

/*输入基站数据*/
void input_block(St Site[],const char* file)
{
	int ax=1, se=0;;
/*文本按行读取*/
    char c[MAX_LINE];  /*缓冲区*/
    FILE *fp;          /*文件指针*/      
    int lens;
    if((fp = fopen(file,"r")) == NULL)
    {
        perror("fail to read");
        exit (1) ;
    }
      
/*循环输入*/    
    while(fgets(c,MAX_LINE,fp) != NULL)
    {	    
        lens = strlen(c);
        c[lens-1] = '\0';  /*去掉换行符*/
        char x1[10],y1[10];
        int len,flag;
        len = strlen(c);
        
/*结束判断条件*/        
        if(c[0]== 'J')   continue;
        if(c[0]== '-' &&c[1]== '1' && c[2]==',' && c[3]=='-' &&c[4]=='1')  break;

/*分离横坐标 x2*/ 
        int i,j=0;
        for(i=0;i<len;i++)
        {
    	    if(c[i]==',')	 
			{
				x1[j] = '\0';
				x1[j+1] = '\0';
				x1[j+2] = '\0';
				break; 
			}
    	    else 
    	    {
    		    x1[j] = c[i];
    		    //printf("%c ",x1[j]);
    		    j++;
		    }
	    }
        int x2= atoi(x1);
        int perm_X = (x2+700) / 900 +1; 
        //printf("%d\n",x2); 
        
/*分离纵坐标 y2*/   
        int m,k=0;
        for(m=i+1;m<len;m++)
        {
    	    if(c[m]==',')	 
			{
				y1[k] = '\0';
				y1[k+1] = '\0';
				y1[k+2] = '\0';
				break; 
			}
    	    else
    	    {
    		    y1[k] = c[m];
    		    k++;
		    }
	    }
	    int y2 = atoi(y1);
	    int perm_Y = (y2+200) / 600 +1; 
	    //printf("%d\n",y2);
	    
/*计算所处区块编号*/
        int perm,perm_V,perm_W;
		perm = (perm_Y-1)* 123 + perm_X; 

/*判断基站类型*/	
	    char dest[4];
	    char chengqu[10] = "城区",xiangzhen[10] = "乡镇",gaosu[10] = "高速";
	    strncpy(dest,c+m+1,4);
    	if(strcmp(dest,chengqu)== 0)  flag=1;
        else if(strcmp(dest,xiangzhen)== 0) flag=2;
    	else if(strcmp(dest,gaosu)== 0) flag=3;
    	
    	//printf("%d ",flag);
    	
/*判断信号强度 ins*/	
	    if(c[m+6]==' ') m+=1;
        char intension[10] ;
	    int a,b=0;
	    for(a=m+6;a<len;a++)
	    {
		    if(c[a]==',')  break;
		    else
		    {
    	        intension[b] = c[a];
    	        b++;
		    }
	    }
	    float ins = atof(intension);
	    for(int ak = 0;ak <=10;ak++)
	    {
	        intension[ak] = '\0';	    	
		}
	    //printf("%.4f \n",ins); 
/*分离基站编号*/
        int c1,d=0;
        char num[10];
        for(c1=a+1;c1<len;c1++)
        {
    	    if(c[c1]>='0' && c[c1]<='9')
    	    {
    		    num[d] = c[c1];
    		    d++;
		    }
		    if(c[m]=='\n')	    break;
	    }
	    int no = atoi(num);
	    for(int ak = 0;ak <=10;ak++)
	    {
	        num[ak] = '\0';	    	
		}
	    //printf("%d\n",no);
/*存储*/
        Site[perm].tag+=1;
		se = Site[perm].tag;
        Site[perm].X_axis[se] = x2;
        Site[perm].Y_axis[se] = y2;
        Site[perm].Type[se] = flag;
		Site[perm].Span[se] = ins;
		Site[perm].No[se] = no;			
    }

}

/*输入轨迹数据*/
void input_path(Path Track[],const char* fileman)
{

	FILE *fp1;
    int lines = 0;
    int Xa,Ya,Xb,Yb,Speed,Hour,Minit;
    char YD[3];
    
    if((fp1 = fopen(fileman, "r")) == NULL)
        printf("\aFile open failed.\n");
    else
    {   fscanf(fp1,"%s",YD);
        while(fscanf(fp1,"%d,%d,%d,%d,%d,%d,%d\n",&Xa,&Ya,&Xb,&Yb,&Speed,&Hour,&Minit) == 7)
        {
		    lines++;
        	Track[lines].Xa = Xa;Track[lines].Ya = Ya;Track[lines].Xb = Xb;Track[lines].Yb = Yb;
			Track[lines].Speed = Speed;Track[lines].Hour = Hour;Track[lines].Minit = Minit;
            //printf("%d ,%d ,%d ,%d ,%d ,%d ,%d\n",Xa,Ya,Xb,Yb,Speed,Hour,Minit); 
        }
        fclose(fp1); 
    }
}

/*输入伪基站数据*/
void input_wz(W_Path Track[],const char* fileman1)
{

	FILE *fp2;
    int lines = 0;
    int Xa,Ya,Xb,Yb,Speed,Hour,Minit,No;
    char YD[3];
    
    if((fp2 = fopen(fileman1, "r")) == NULL)
        printf("\aFile open failed.\n");
    else
    {   fscanf(fp2,"%s",YD);
        while(fscanf(fp2,"%d,%d,%d,%d,%d,%d,%d,%d\n",&Xa,&Ya,&Xb,&Yb,&Speed,&Hour,&Minit,&No) == 8)
        {
		    lines++;
        	Track[lines].Xa = Xa;Track[lines].Ya = Ya;Track[lines].Xb = Xb;Track[lines].Yb = Yb;
			Track[lines].Speed = Speed;Track[lines].Hour = Hour;Track[lines].Minit = Minit;Track[lines].No = No;
            //printf("%d ,%d ,%d ,%d ,%d ,%d ,%d\n",Xa,Ya,Xb,Yb,Speed,Hour,Minit); 
        }
        fclose(fp2); 
    }
}
/*给定区块，输出基站数据*/
void output_block1(St Site[],int prum,int a) 
{
	if(Site[prum].tag == 0) printf("该区域无基站\n"); 
	else
	{
		int i=1;
			printf("基站编号为：%d\n",Site[prum].No[a]);
			printf("坐标为：%d,%d\n",Site[prum].X_axis[a],Site[prum].Y_axis[a]);
		switch(Site[prum].Type[a])
	    {
	    	case 1:	    printf("类型是：城区\n");break;
			case 2:     printf("类型是：乡镇\n");break;
			case 3:     printf("类型是：高速\n");break; 
		}
        printf("强度为：%.4f\n",Site[prum].Span[a]);
	}
}

void output_block2(St Site[],int prum) 
{
	if(Site[prum].tag == 0) printf("该区域无基站\n"); 
	else
	{
		int i=1;
		while(i<=Site[prum].tag)
		{
			printf("基站%d编号为：%d\n",i,Site[prum].No[i]);
			printf("坐标为：%d,%d\n",Site[prum].X_axis[i],Site[prum].Y_axis[i]);
		switch(Site[prum].Type[i])
	    {
	    	case 1:	    printf("类型是：城区\n");break;
			case 2:     printf("类型是：乡镇\n");break;
			case 3:     printf("类型是：高速\n");break; 
		}
        printf("强度为：%.4f\n",Site[prum].Span[i]);
			i++; 
		}
	}
}

void output_block3(St Site[],int prum,int a)
{
	if(Site[prum].tag == 0) printf("该区域无基站\n"); 
	else
	{
		printf("基站编号为：%d ->",Site[prum].No[a]);
	}
} 
/*判断区块内是否有基站*/
int identify(St Site[],int prum) 
{
	if(Site[prum].tag == 0)
	{
		return 0;
	}
	else
	    return 1;
}
/*输出西北角及东南角区块的基站信息*/
void output1(St Site[])
{
	/*西北角*/
	if(Site[1].X_axis[1] == 0) printf("西北角第1个区块无基站\n");
	else
	{
		printf("西北角第1个区块有基站\n");
	    output_block2(Site,1);
	}
	printf("----------------------------------------------------\n");
	/*东南角*/
	if(Site[8241].X_axis[1] == 0) printf("东南角最后1个区块无基站\n");
	else
	{
		printf("东南角最后1个区块有基站\n");
	    output_block2(Site,8241);
	}
	printf("----------------------------------------------------\n");
}

/*输出首个及最后一个区域旁边的基站数据*/
void output2(St Site[]) 
{
	/*西北角东侧*/
	if(Site[2].X_axis[1] == 0) printf("西北角东侧无基站\n"); 
	else
	{
		printf("西北角东侧有基站\n"); 
		output_block2(Site,2);
	}
	printf("----------------------------------------------------\n");
	/*西北角南侧*/
	if(Site[124].X_axis[1] == 0) printf("西北角南侧无基站\n"); 
	else
	{
		printf("西北角南侧有基站\n"); 
		output_block2(Site,124);
	}
	printf("----------------------------------------------------\n");
	/*东南角西北侧*/
	if(Site[8117].X_axis[1] == 0) printf("东南角西北侧无基站\n"); 
	else
	{
		printf("东南角西北侧有基站\n"); 
		output_block2(Site,8117);
	}
	printf("----------------------------------------------------\n");
	/*东南角西北侧的北侧*/
	if(Site[7994].X_axis[1] == 0) printf("东南角西北侧的北侧无基站\n"); 
	else
	{
		printf("东南角西北侧的北侧有基站\n"); 
		output_block2(Site,7994);
	}
	printf("----------------------------------------------------\n");
}

/*两点间的距离*/
double near(double x,double y,double x1,double y1)
{
	double a;
	a = pow(x-x1,2)+pow(y-y1,2);
	return a;
}
/*信号强度*/
double Signal_Strength(St Site[],int perm,int i,double x,double y)//第perm号区块中第i个基站 
{
	double distance = near(x,y,Site[perm].X_axis[i],Site[perm].Y_axis[i]); 
	/*判断基站类型*/
	double stand;
	
	if(Site[perm].Type[i] == 1)  stand = 90000.0;
	else if(Site[perm].Type[i] == 2)  stand = 1000000.0;
	else if(Site[perm].Type[i] == 3)  stand = 25000000.0;

	double strength = Site[perm].Span[i]*(stand/distance);
	return strength;
}

/*比较信号强度的大小*/
int Strength_Compare(double Strengths[],int num)//共num个基站需比较
{
	int i,hk=1;
	double max = Strengths[1];
    for(i=1;i<=num;i++)
	{
		if(max<Strengths[i])
		{
			max = Strengths[i];
			hk = i;//第hk个信号最强 
		}
	}
	return hk;	
} 


/*给定一个点（x,y），求某一区块与它信号强度最强的点*/
int Strengthest (St Site[],int perm,double x,double y)
{
	int i;
	double qiangdu[10];
	if(Site[perm].X_axis[1] == 0)	    return 0;
	for(i=1;i<=Site[perm].tag;i++)
		qiangdu[i] = Signal_Strength(Site,perm,i,x,y);
	int hok = Strength_Compare(qiangdu,i);
	return hok;	
}


/*求信号最强的基站*/
int find_Station2(St Site[],double x,double y) 
{
	int perm,perm_X,perm_Y;

	perm_X = (x+700) / 900 +1; 
	perm_Y = (y+200) / 600 +1;
    perm = (perm_Y-1)* 123 + perm_X;
	//printf("%d\n",perm);

    int i,j,k;
	int abc;
	double Strength[300];
	int kiko = perm - 1113;  
	//printf("%d\n",kiko);
	double max_Signal = 0.0;
	//printf("%f\n",max_Signal);已重置 
	int flagx =1,flagy =1,flag1 =1;
	for(j=1;j<=19;j++)
	{
		for(k=1;k<=13;k++)
		{
			if(kiko<=0) 
			{
				kiko+=1;
				continue;
			}
			for(i=1;i<=Site[kiko].tag;i++)
	        {
		          Strength[i] = Signal_Strength(Site,kiko,i,x,y);	//printf("%f ",Strength[i]);
	        }
	        abc = Strength_Compare(Strength,Site[kiko].tag);
	        //printf("%f\n",Strength[abc]);
	        if (Strength[abc] > max_Signal)
	        {
	        	max_Signal = Strength[abc];   //printf("%f ",max_Signal);//0.526730
	        	flagx = k;   //printf("%d ",flagx);//4
	        	flagy = j;   //printf("%d \n",flagy);//1
	        	flag1 = abc; //printf("%d \n",flag1);
	        	Strength[abc]=0;
	        	//k=0;j=0;
			}
			kiko+=1;
		}
		kiko+=110;
	}
    int No_perm = perm-1113+(flagx-1)+123*(flagy-1);
    //printf("%d\n",No_perm);
    //printf("%f\n",max_Signal);
    
    if(max_Signal > 1.0)
    {
    	//printf("%d\n",No_perm);
    	output_block1(Site,No_perm,flag1);
    	printf("--------------\n");
    	return Site[No_perm].No[flag1];
	}
	    
	else
	{
		printf("范围内无信号！\n");
		printf("--------------\n");
		return 0;
	}
	/*重置*/
	max_Signal = 0.0;    
	for(int abx=0;abx<=100;abx++)
    {
    	Strength[abx] = 0;
	}   
	x =0; y =0; 
}

int find_Station3(St Site[],double x,double y)//返回No 
{
	int perm,perm_X,perm_Y;

	perm_X = (x+700) / 900 +1; 
	perm_Y = (y+200) / 600 +1;
    perm = (perm_Y-1)* 123 + perm_X;
	//printf("%d\n",perm);//perm = 8160

    int i,j,k;
	int abc;
	double Strength[300],Strength_all[300];
	int kiko = perm - 1113;  
	//printf("%d\n",kiko);
	double max_Signal = 0.0;
	//printf("%f\n",max_Signal);已重置 
	int flagx =1,flagy =1,flag1 =1;
	for(j=1;j<=19;j++)
	{
		for(k=1;k<=13;k++)
		{
			if(kiko<=0) 
			{
				kiko+=1;
				continue;
			}
			for(i=1;i<=Site[kiko].tag;i++)
	        {
		          Strength[i] = Signal_Strength(Site,kiko,i,x,y);	//printf("%f ",Strength[i]);
	        }
	        abc = Strength_Compare(Strength,Site[kiko].tag);
	        //printf("%f\n",Strength[abc]);
	        if (Strength[abc] > max_Signal)
	        {
	        	max_Signal = Strength[abc];   //printf("%f ",max_Signal);//0.526730
	        	flagx = k;   //printf("%d ",flagx);//4
	        	flagy = j;   //printf("%d \n",flagy);//1
	        	flag1 = abc; //printf("%d \n",flag1);
	        	Strength[abc]=0;
	        	//k=0;j=0;
			}
			kiko+=1;
		}
		kiko+=110;
	}
    int No_perm = perm-1113+(flagx-1)+123*(flagy-1);
    //printf("%d\n",No_perm);
    //printf("%f\n",max_Signal);
    
    if(max_Signal > 1.0)
    {
    	//printf("%d\n",No_perm);
    	//output_block1(Site,No_perm,flag1);
    	//printf("--------------\n");
    	return Site[No_perm].No[flag1];
	}
	    
	else
	{
		//printf("范围内无信号！\n");
		//printf("--------------\n");
		return 0;
	}
	/*重置*/
	max_Signal = 0.0;    
	for(int abx=0;abx<=100;abx++)
    {
    	Strength[abx] = 0;
	}   
	x =0; y =0; 
}

int find_Station4(St Site[],double x,double y)//返回flag1 
{
	int perm,perm_X,perm_Y;

	perm_X = (x+700) / 900 +1; 
	perm_Y = (y+200) / 600 +1;
    perm = (perm_Y-1)* 123 + perm_X;
	//printf("%d\n",perm);//perm = 8160

    int i,j,k;
	int abc;
	double Strength[300],Strength_all[300];
	int kiko = perm - 1113;  
	//printf("%d\n",kiko);
	double max_Signal = 0.0;
	//printf("%f\n",max_Signal);已重置 
	int flagx =1,flagy =1,flag1 =1;
	for(j=1;j<=19;j++)
	{
		for(k=1;k<=13;k++)
		{
			if(kiko<=0) 
			{
				kiko+=1;
				continue;
			}
			for(i=1;i<=Site[kiko].tag;i++)
	        {
		          Strength[i] = Signal_Strength(Site,kiko,i,x,y);	//printf("%f ",Strength[i]);
	        }
	        abc = Strength_Compare(Strength,Site[kiko].tag);
	        //printf("%f\n",Strength[abc]);
	        if (Strength[abc] > max_Signal)
	        {
	        	max_Signal = Strength[abc];   //printf("%f ",max_Signal);//0.526730
	        	flagx = k;   //printf("%d ",flagx);//4
	        	flagy = j;   //printf("%d \n",flagy);//1
	        	flag1 = abc; //printf("%d \n",flag1);
	        	Strength[abc]=0;
	        	//k=0;j=0;
			}
			kiko+=1;
		}
		kiko+=110;
	}
    int No_perm = perm-1113+(flagx-1)+123*(flagy-1);
    //printf("%d\n",No_perm);
    //printf("%f\n",max_Signal);
    
    if(max_Signal > 1.0)
    {
    	return flag1;
	}
	    
	else
	{
		return 0;
	}
	/*重置*/
	max_Signal = 0.0;    
	for(int abx=0;abx<=100;abx++)
    {
    	Strength[abx] = 0;
	}   
	x =0; y =0; 
}

int find_Station5(St Site[],double x,double y)//返回No_perm 
{
	int perm,perm_X,perm_Y;

	perm_X = (x+700) / 900 +1; 
	perm_Y = (y+200) / 600 +1;
    perm = (perm_Y-1)* 123 + perm_X;
	//printf("%d\n",perm);//perm = 8160

    int i,j,k;
	int abc;
	double Strength[300],Strength_all[300];
	int kiko = perm - 1113;  
	//printf("%d\n",kiko);
	double max_Signal = 0.0;
	//printf("%f\n",max_Signal);已重置 
	int flagx =1,flagy =1,flag1 =1;
	for(j=1;j<=19;j++)
	{
		for(k=1;k<=13;k++)
		{
			if(kiko<=0) 
			{
				kiko+=1;
				continue;
			}
			for(i=1;i<=Site[kiko].tag;i++)
	        {
		          Strength[i] = Signal_Strength(Site,kiko,i,x,y);	//printf("%f ",Strength[i]);
	        }
	        abc = Strength_Compare(Strength,Site[kiko].tag);
	        //printf("%f\n",Strength[abc]);
	        if (Strength[abc] > max_Signal)
	        {
	        	max_Signal = Strength[abc];   //printf("%f ",max_Signal);//0.526730
	        	flagx = k;   //printf("%d ",flagx);//4
	        	flagy = j;   //printf("%d \n",flagy);//1
	        	flag1 = abc; //printf("%d \n",flag1);
	        	Strength[abc]=0;
	        	//k=0;j=0;
			}
			kiko+=1;
		}
		kiko+=110;
	}
    int No_perm = perm-1113+(flagx-1)+123*(flagy-1);
    //printf("%d\n",No_perm);
    //printf("%f\n",max_Signal);
    
    if(max_Signal > 1.0)
    {
    	return No_perm;
	}
	    
	else
	{
		return 0;
	}
	/*重置*/
	max_Signal = 0.0;    
	for(int abx=0;abx<=100;abx++)
    {
    	Strength[abx] = 0;
	}   
	x =0; y =0; 
}

void find_Station1(St Site[])
{
	int x,y,i;
	printf("请输入坐标：");
	scanf("%d %d",&x,&y) ;
	i = find_Station2(Site,x,y);
}


/*计算运动路径角度X轴*/
double angle_X(Path Track[],int Num)//Num为路径号
{
    int Y_abs = abs(Track[Num].Yb - Track[Num].Ya)/100;
	int X_abs = abs(Track[Num].Xb - Track[Num].Xa)/100;
	int Hyo_abs = (int)sqrt((double)pow(X_abs,2) + (double)pow(Y_abs,2));	
	double X_angle = (X_abs*1.0)/Hyo_abs;
	return X_angle;
} 

/*计算运动路径角度Y轴*/
double angle_Y(Path Track[],int Num)
{
	double Xk = angle_X(Track,Num);
	double Yk = sqrt((float)(1-pow(Xk,2)));
	return Yk;
}

double angle_X1(W_Path Track[],int Num)//Num为路径号
{
    int Y_abs = abs(Track[Num].Yb - Track[Num].Ya)/100;
	int X_abs = abs(Track[Num].Xb - Track[Num].Xa)/100;
	int Hyo_abs = (int)sqrt((double)pow(X_abs,2) + (double)pow(Y_abs,2));	
	double X_angle = (X_abs*1.0)/Hyo_abs;
	return X_angle;
} 

double angle_Y1(W_Path Track[],int Num)
{
	double Xk = angle_X1(Track,Num);
	double Yk = sqrt((float)(1-pow(Xk,2)));
	return Yk;
}
/*计算移动20m的时间*/
double Time(Path Track[],int Num)
{
	float Sp = (Track[Num].Speed*1000.0)/3600;
	float time = 20.0/Sp;
	//printf("%f ",time);
	return time;
}

/*通过线路的总时间*/
int Time_all(Path Track[],int Num)
{
	int Y_abs = abs(Track[Num].Yb - Track[Num].Ya);//printf("%d ",Y_abs);
	int X_abs = abs(Track[Num].Xb - Track[Num].Xa);//printf("%d ",X_abs);
	int Path_all = (int)sqrt((float)(pow(X_abs,2)+pow(Y_abs,2)));//printf("%d ",Path_all);
	int time_all = Path_all/20;
	return time_all;
}

/*伪基站移动路径*/
int W_Station(W_Path W_Track[],int Num,double foot,double x,double y,int ab_i,int Minit)//步长用时 
{
	double ab_j = (double)(ab_i-(W_Track[Num].Minit - Minit)*60.0/foot);//printf("%lf ",ab_j);
	if(ab_j>=0)
	{
		double X_dic,Y_dic,X,Y;
		/*单次移动长度*/
	double Sp = (W_Track[Num].Speed*1000.0)/3600;  //printf("%lf ",Sp);
    double lucheng = Sp*foot; //printf("%lf ",lucheng);
	if(W_Track[Num].Xb - W_Track[Num].Xa < 0)
	    X_dic = -1.0* angle_X1(W_Track,Num)*(lucheng)-1; 
	else     
	    X_dic = angle_X1(W_Track,Num)*lucheng;
	    
	if(W_Track[Num].Yb - W_Track[Num].Ya < 0)
	    Y_dic = -1.0* angle_Y1(W_Track,Num)*(lucheng)-1; 
	else
        Y_dic = angle_Y1(W_Track,Num)*lucheng;
        
	//printf("%lf ",Sp); printf("%lf ",lucheng); printf("%lf ",X_dic);     printf("%lf ",Y_dic);
	X = X_dic*ab_j + W_Track[Num].Xa;
	Y = Y_dic*ab_j + W_Track[Num].Ya;  //printf("%lf ",X); printf("%lf ",Y);
    double distance = pow(X-x,2) + pow(Y-y,2); 
	if(distance<=1600.0)
	{
		return 1;
	}    
	else
	{
	    return 0;
	}
	}    
}

/*寻找关联基站*/
void Correspond_Station(Path Track[],W_Path W_Track[],St Site[],int Num)
{
	double x,y;
	int No_flag1=999999,No_flag2=999999;
	int perm6;
	x = Track[Num].Xa*1.0;  y = Track[Num].Ya*1.0; 	//printf("%d ",x);
	double X_dic,Y_dic;
	/*单次移动长度*/
	if(Track[Num].Xb - Track[Num].Xa < 0)
	    X_dic = -1 * angle_X(Track,Num)*20.0;  
	else     
	    X_dic = angle_X(Track,Num)*20;
	    
	if(Track[Num].Yb - Track[Num].Ya < 0)
	    Y_dic = angle_Y(Track,Num)*(-20);  
	else
        Y_dic = angle_Y(Track,Num)*20;
    //printf("%f ",X_dic);printf("%f ",Y_dic);
	
	double time_per = Time(Track,Num);//printf("%f\n",time_per); 
	int Minit_now,Hour_now;
	double Second_now;
	int time_total = Time_all(Track,Num) ;
	
	int ab_i,ab_j;	
	int Hour_first = Track[Num].Hour ;
    int Minit_first = Track[Num].Minit;
    int shuliang = 0,jio=0;

    //printf("%d ",time_total);
	for(ab_i = 0;ab_i<=time_total;ab_i++)
	{	
	    //printf("x = %lf y = %lf ab_i =%d\n",x,y,ab_i);
			 
	    int kok = find_Station3(Site,x,y);
	    for(int nv=1;nv<=5;nv++)
	    {
	    	if (W_Station(W_Track,nv,time_per,x,y,ab_i,Minit_first) ==1)
	    	    jio = 1;
	        else jio =0;
		}
	        
	    if(kok!=No_flag1 || jio==1)
	    {
	    	shuliang++; printf("%d\n",shuliang);
	    	
	    	int Hour_f = Hour_first+(Minit_first*60 + (int)(ab_i*time_per))/3600;
	    	printf("%d:",Hour_f);
	    	
	    	if((int)(Minit_first + (ab_i * time_per)/60) < 60)
			    printf("%d \n",(int)(Minit_first + (ab_i * time_per)/60));
	    	else 
			    printf("%d \n",((int)(Minit_first + (ab_i * time_per)/60)) % 60);  
		for(int num=1;num<=5;num++)		
		{
	        if(W_Station(W_Track,num,time_per,x,y,ab_i,Minit_first)==1)
            {
        	    printf("伪基站:%d\n",W_Track[num].No);
        	    printf("--------------\n");
        	    No_flag1 = W_Station(W_Track,num,time_per,x,y,ab_i,Minit_first);
        	    continue;
		    }		     
	    }
	    	No_flag1 = find_Station2(Site,x,y);
	    
		}
		x += X_dic;
		y += Y_dic;		
	}
	
}

/*生成路径*/
void find_Path(Path Track[],W_Path W_Track[],St Site[]) 
{
	int Num;
	printf("请输入查询线路号（1-12）：",Num);
	scanf("%d",&Num);
	Correspond_Station(Track,W_Track,Site,Num);
}

/*迭代求进入基站的时间*/
void Time_Station(Path Track[],St Site[])
{
	double x,y,X_half,Y_half,X_kk,Y_kk;
	int Num =1;
	int No_flag1=999999,No_flag2=999999;
	int perm6;
	x = Track[Num].Xa*1.0;  y = Track[Num].Ya*1.0; 	//printf("%d ",x);
	double X_dic,Y_dic;
	/*单次移动长度*/
	if(Track[Num].Xb - Track[Num].Xa < 0)
	    X_dic = -1 * angle_X(Track,Num)*20.0;  
	else     
	    X_dic = angle_X(Track,Num)*20;
	    
	if(Track[Num].Yb - Track[Num].Ya < 0)
	    Y_dic = angle_Y(Track,Num)*(-20);  
	else
        Y_dic = angle_Y(Track,Num)*20;
    //printf("%f ",X_dic);printf("%f ",Y_dic);
	
	double time_per = Time(Track,Num);//printf("%f\n",time_per); 
	int Minit_now,Hour_now;
	double Second_now;
	double dic_x,dic_y,sec;
	int time_total = Time_all(Track,Num) ;
	
	int ab_i,ab_j,ab_k,half;	
	int Hour_first = Track[Num].Hour ;
    int Minit_first = Track[Num].Minit;
    int shuliang = 0;

    //printf("%d ",time_total);
	for(ab_i = 0;ab_i<=time_total;ab_i++)
	{		    
	    int kok = find_Station3(Site,x,y);//0
	    if(kok!=No_flag1 )
	    {	
	    
	    	shuliang++; //printf("%d\n",shuliang);	
			
			if(kok!=0)
			{
				printf("进入首个基站：%d ",kok);
				dic_x=0.5*X_dic;
				dic_y=0.5*Y_dic;
				X_half = x-dic_x;
			    Y_half = y-dic_y;
				for(ab_k=1;ab_k<=8;ab_k++)
				{
				    //printf("%lf %lf",X_half,Y_half);
				    half = find_Station3(Site,X_half,Y_half);
				    //printf("%d",half);
				    if(half!=0)
				    {
				    	X_kk = X_half;Y_kk = Y_half;
					    X_half = X_half-0.5*dic_x;
						Y_half = Y_half-0.5*dic_y;
						dic_x = 0.5*dic_x;
						dic_y = 0.5*dic_y; 
						//printf("%lf %lf \n",X_half,Y_half);   
				    }
					else if(half==0)
					{
						X_kk = X_half;Y_kk = Y_half;
						X_half = X_half+0.5*dic_x;
						Y_half = Y_half+0.5*dic_y;
						dic_x = 0.5*dic_x;
						dic_y = 0.5*dic_y;   
						//printf("%lf %lf \n",X_half,Y_half);
					}   
				}
                if(half!=0)
                {
                	X_half +=dic_x;
                	Y_half +=dic_y;
				}
				else if(half==0)
				{
					X_half -=dic_x;
                	Y_half -=dic_y;
				}
				//printf("%lf %lf ",X_half,Y_half);
				sec = sqrt(near(X_half,Y_half,(double)Track[Num].Xa,(double)Track[Num].Ya) );
				//printf("%lf ",sec);
				Second_now = sec/(Track[Num].Speed*1000.0/3600); 
				printf("时间为："); 
				int Hour_f = Hour_first+(Minit_first*60 + (int)(Second_now))/3600;
	    	    printf("%d:",Hour_f);	
				
				if((int)(Minit_first + (Second_now)/60) < 60)
			        printf("%d:",(int)(Minit_first + (Second_now)/60));
	    	    else 
			        printf("%d:",((int)(Minit_first + (Second_now)/60)) % 60); 
				
				printf("%.1lf\n",Second_now);
				printf("-------------\n");  
				break;
			}
	    	No_flag1 = find_Station3(Site,x,y);
		}
		x += (int)X_dic;
		y += (int)Y_dic;		
	}
	
}

/*迭代求离开基站的时间*/
void Time_Station1(Path Track[],St Site[])
{
	double x,y,X_half,Y_half;
	int Num =1;
	int No_flag1=999999,No_flag2=999999;
	int perm6;
	x = Track[Num].Xa*1.0;  y = Track[Num].Ya*1.0; 	//printf("%d ",x);
	double X_dic,Y_dic;
	/*单次移动长度*/
	if(Track[Num].Xb - Track[Num].Xa < 0)
	    X_dic = -1 * angle_X(Track,Num)*20.0;  
	else     
	    X_dic = angle_X(Track,Num)*20;
	    
	if(Track[Num].Yb - Track[Num].Ya < 0)
	    Y_dic = angle_Y(Track,Num)*(-20);  
	else
        Y_dic = angle_Y(Track,Num)*20;
    //printf("%f ",X_dic);printf("%f ",Y_dic);
	
	double time_per = Time(Track,Num);//printf("%f\n",time_per); 
	int Minit_now,Hour_now;
	double Second_now;
	double dic_x,dic_y,sec;
	int time_total = Time_all(Track,Num) ;
	
	int ab_i,ab_j,ab_k;
	double half;	
	int Hour_first = Track[Num].Hour ;
    int Minit_first = Track[Num].Minit;
    int shuliang = 0,koc=0;
    double flag2,perm;
    //printf("%d ",time_total);
	for(ab_i = 0;ab_i<=time_total;ab_i++)
	{		    
	    int kok = find_Station3(Site,x,y);//0
	    if(kok!=No_flag1 )
	    {	
	    
	    	shuliang++; //printf("%d\n",shuliang);	
	    	if(shuliang==1)
	    	{
	    		koc = find_Station3(Site,x,y);
	    	    flag2 = find_Station4(Site,x,y);
                perm = find_Station5(Site,x,y);
	    		//printf("%d ",koc);
			}
	    	 
			
			if(kok!=koc)
			{
				printf("离开首个基站：%d ",koc);
				dic_x=0.5*X_dic;
				dic_y=0.5*Y_dic;
				X_half = x-dic_x;
			    Y_half = y-dic_y;
				for(ab_k=1;ab_k<=8;ab_k++)
				{
				    //printf("%lf %lf",X_half,Y_half);
				    half = Signal_Strength(Site,perm,flag2,X_half,Y_half);
				    //printf("%lf ",half);
				    if(half>=1.0)
				    {
					    X_half = X_half+0.5*dic_x;
						Y_half = Y_half+0.5*dic_y;
						dic_x = 0.5*dic_x;
						dic_y = 0.5*dic_y; 
						//printf("%lf %lf \n",X_half,Y_half);   
				    }
					else if(half<=1.0)
					{
						X_half = X_half-0.5*dic_x;
						Y_half = Y_half-0.5*dic_y;
						dic_x = 0.5*dic_x;
						dic_y = 0.5*dic_y;   
						//printf("%lf %lf \n",X_half,Y_half);
					}   
				}
                if(half<=1.0)
                {
                	X_half -=dic_x;
                	Y_half -=dic_y;
				}
				else if(half>1.0)
				{
					X_half +=dic_x;
                	Y_half +=dic_y;
				}
				//printf("%lf %lf ",X_half,Y_half);
				sec = sqrt(near(X_half,Y_half,(int)Track[Num].Xa,(int)Track[Num].Ya) );
				//printf("%lf ",sec);
				Second_now = sec/(Track[Num].Speed*1000.0/3600);
				//printf("%d\n",ab_i);
				printf("时间为："); 
				int Hour_f = Hour_first+(Minit_first*60 + (int)(Second_now))/3600;
	    	    printf("%d:",Hour_f);
					
				
				if((int)(Minit_first + (Second_now)/60) < 60)
			        printf("%d:",(int)(Minit_first + (Second_now)/60));
	    	    else 
			        printf("%d:",((int)(Minit_first + (Second_now)/60)) % 60);  	  
				
				printf("%.1lf\n",Second_now);
				printf("-------------\n"); 
				break;
			}
	    	No_flag1 = find_Station3(Site,x,y);
		}
		x += (int)X_dic;
		y += (int)Y_dic;		
	}
}

/*计算通过重叠区的时间长度*/
double overlap1(Path Track[],St Site[],int Num) 
{	
	double x,y;
	int No_flag1=999999,No_flag2=999999;
	x = Track[Num].Xa*1.0;  y = Track[Num].Ya*1.0; 	//printf("%d ",x);
	double X_dic,Y_dic;
	/*单次移动长度*/
	if(Track[Num].Xb - Track[Num].Xa < 0)
	    X_dic = -1 * angle_X(Track,Num)*20.0;  
	else     
	    X_dic = angle_X(Track,Num)*20;
	    
	if(Track[Num].Yb - Track[Num].Ya < 0)
	    Y_dic = angle_Y(Track,Num)*(-20);  
	else
        Y_dic = angle_Y(Track,Num)*20;
    //printf("%f ",X_dic);printf("%f ",Y_dic);
	
	double time_per = Time(Track,Num);//printf("%f\n",time_per); 
	double Second_now;
	int time_total = Time_all(Track,Num) ;
	
	int ab_i,ab_j;	
    int shuliang = 0;

	for(ab_i = 0;ab_i<=time_total;ab_i++)
	{		    
	    int kok = find_Station3(Site,x,y);
	    if(kok!=No_flag1)
	    {	
	    	shuliang++; //printf("%d\n",shuliang);
	    	if(kok!=0)
	    		break;
	    	No_flag1 = find_Station3(Site,x,y);
	    
		}
		x += X_dic;
		y += Y_dic;	
	}
	//printf("%lf %lf\n",X_dic,Y_dic);
	//printf("%lf %lf\n",x,y);
	int flag,perm;
	flag = find_Station4(Site,x,y);
    perm = find_Station5(Site,x,y);
    printf("首个基站为:%d号\n",find_Station3(Site,x,y));
	float SS = Signal_Strength(Site,perm,flag,x,y);
	//output_block1(Site,perm,flag);
	//printf("%lf",SS); 
	while(SS>=1)
	{
	    //printf("%lf %lf ",x,y);
	    SS = Signal_Strength(Site,perm,flag,x,y);
		x += X_dic;
		y += Y_dic;	
		//printf("%lf\n",SS); 		
	}
    double X_half,Y_half;
    X_half = x-X_dic;Y_half = y-Y_dic;
    double dic_x,dic_y,half,sec;
    int ab_k;
    
    //printf("%lf %lf\n",X_half,Y_half);
	dic_x=0.5*X_dic;
	dic_y=0.5*Y_dic;
	//printf("%lf %lf\n",dic_x,dic_y);
	X_half -=dic_x;
    Y_half -=dic_y; 
    //printf("%lf %lf\n",X_half,Y_half);
    
				for(ab_k=1;ab_k<=8;ab_k++)
				{
				    //printf("%lf %lf",X_half,Y_half);
				    half = Signal_Strength(Site,perm,flag,X_half,Y_half);
				    //printf("%lf\n",half);
				    
				    if(half>=1)
				    {
					    X_half = X_half+0.5*dic_x;
						Y_half = Y_half+0.5*dic_y;
						dic_x = 0.5*dic_x;
						dic_y = 0.5*dic_y; 
						//printf("%lf %lf \n",X_half,Y_half);   
				    }
					else if(half<1)
					{
						X_half = X_half-0.5*dic_x;
						Y_half = Y_half-0.5*dic_y;
						dic_x = 0.5*dic_x;
						dic_y = 0.5*dic_y;   
					    //printf("%lf %lf \n",X_half,Y_half);
					}   
				}
                if(half<1)
                {
                	X_half -=dic_x;
                	Y_half -=dic_y;
				}
				else if(half>=1)
				{
					X_half +=dic_x;
                	Y_half +=dic_y;
				}
	sec = sqrt(near(X_half,Y_half,(double)Track[Num].Xa,(double)Track[Num].Ya) );
	//printf("%lf ",sec);
	Second_now = sec/(Track[Num].Speed*1000.0/3600); 
	printf("离开基站的时间为:%lf s\n",Second_now);	
	return Second_now;            
}

double overlap2(Path Track[],St Site[],int Num) 
{
	double x,y;
	int No_flag1=999999,No_flag2=999999;
	x = Track[Num].Xa*1.0;  y = Track[Num].Ya*1.0; 	//printf("%d ",x);
	double X_dic,Y_dic;
	/*单次移动长度*/
	if(Track[Num].Xb - Track[Num].Xa < 0)
	    X_dic = -1 * angle_X(Track,Num)*20.0;  
	else     
	    X_dic = angle_X(Track,Num)*20;
	    
	if(Track[Num].Yb - Track[Num].Ya < 0)
	    Y_dic = angle_Y(Track,Num)*(-20);  
	else
        Y_dic = angle_Y(Track,Num)*20;
    //printf("%f ",X_dic);printf("%f ",Y_dic);
	
	double time_per = Time(Track,Num);//printf("%f\n",time_per); 
	double Second_now;
	int time_total = Time_all(Track,Num) ;
	
	int ab_i,ab_j;	
    int shuliang = 0;

	for(ab_i = 0;ab_i<=time_total;ab_i++)
	{		    
	    int kok = find_Station3(Site,x,y);
	    //printf("%d ",kok);
	    if(kok!=No_flag1)
	    {	
	    	if(kok!=0) shuliang++; //printf("%d\n",shuliang);
	    	if(shuliang == 2)
	    		break;
	    	No_flag1 = find_Station3(Site,x,y);
		}
		x += X_dic;
		y += Y_dic;	
	}
	//printf("%lf %lf\n",x,y);
	int flag,perm;
	flag = find_Station4(Site,x,y);
    perm = find_Station5(Site,x,y);
    printf("第二个基站为:%d号\n",find_Station3(Site,x,y));
    //printf("%d %d\n",perm,flag);
	//output_block1(Site,perm,flag);
	x = Track[Num].Xa*1.0;  y = Track[Num].Ya*1.0;
	float SS = Signal_Strength(Site,perm,flag,x,y);
	while(SS < 1.0)
	{
	    //printf("%lf %lf ",x,y);
	    SS = Signal_Strength(Site,perm,flag,x,y);
		x += X_dic;
		y += Y_dic;	
		//printf("%lf\n",SS); 		
	}
    double X_half,Y_half;
    X_half = x-X_dic;Y_half = y-Y_dic;
    double dic_x,dic_y,half,sec;
    int ab_k;
    //printf("%lf %lf\n",X_half,Y_half);
	dic_x=0.5*X_dic;
	dic_y=0.5*Y_dic;
	//printf("%lf %lf\n",dic_x,dic_y);
	X_half -=dic_x;
    Y_half -=dic_y; 
    //printf("%lf %lf\n",X_half,Y_half);
    
				for(ab_k=1;ab_k<=8;ab_k++)
				{
				    //printf("%lf %lf ",X_half,Y_half);
				    half = Signal_Strength(Site,perm,flag,X_half,Y_half);
				    //printf("%lf\n",half);
				    
				    if(half>=1)
				    {
					    X_half = X_half-0.5*dic_x;
						Y_half = Y_half-0.5*dic_y;
						dic_x = 0.5*dic_x;
						dic_y = 0.5*dic_y; 
						//printf("%lf %lf \n",X_half,Y_half);   
				    }
					else if(half<1)
					{
						X_half = X_half+0.5*dic_x;
						Y_half = Y_half+0.5*dic_y;
						dic_x = 0.5*dic_x;
						dic_y = 0.5*dic_y;   
						//printf("%lf %lf \n",X_half,Y_half);
					}   
				}
                if(half<1)
                {
                	X_half +=dic_x;
                	Y_half +=dic_y;
				}
				else if(half>=1)
				{
					X_half -=dic_x;
                	Y_half -=dic_y;
				}
    //printf("%lf %lf \n",X_half,Y_half);

	sec = sqrt(near(X_half,Y_half,(double)Track[Num].Xa,(double)Track[Num].Ya) );
	if((X_half < Track[Num].Xa && X_dic > 0 )||(X_half > Track[Num].Xa && X_dic < 0 )||((Y_half > Track[Num].Ya && Y_dic < 0 ))||(Y_half < Track[Num].Ya && Y_dic > 0 )) 
    {
    	sec =-1*sec;
	}
	//printf("%lf ",sec);
	Second_now = sec/(Track[Num].Speed*1000.0/3600); 
	printf("进入基站的时间为:%lf s\n",Second_now);	
	return Second_now;            
}

void overlap(Path Track[],St Site[])
{
	int Num;
	printf("请输入查询的线路号：");
	scanf("%d",&Num);
	double x1 = overlap1(Track,Site,Num);
	double x2 = overlap2(Track,Site,Num); 
	printf("重叠时间为:%.1lf s\n",x1-x2);
	printf("-------------\n"); 
}

int main()
{
/*为结构体开辟空间*/ 
	St *Site;
	Site=(St*)malloc(10000*sizeof(St));
	
	Path *Track;
	Track=(Path*)malloc(20*sizeof(Path));
	
	W_Path *W_Track;
	W_Track=(W_Path*)malloc(20*sizeof(W_Path));
/*存储基站信息*/
    input_block(Site,"D:\\桌面\\测试数据0303\\jz001.txt");
	input_block(Site,"D:\\桌面\\测试数据0303\\jz002.txt");	

/*存储移动轨迹信息*/        
    input_path(Track,"D:\\桌面\\测试数据0303\\yd001.txt");
    input_wz(W_Track,"D:\\桌面\\测试数据0303\\wz001.txt");

    myMenu(); 
    int select,kas,kos;
    while(1)
    {
    	printf("请输入所要实现的功能编号："); 
		scanf("%d",&select); 
		switch(select)
		{
		case 1: 
		    output1(Site);/*任务1：显示首个及最后一个区域中的基站数据*/
			break;
		case 2: 
		    output2(Site);/*任务2：显示首个及最后一个区域旁边的基站数据*/
			break;
		case 3:
		    find_Station1(Site);/*任务3 4：给定坐标，求基站*/
			break;
		case 4: 
		    find_Station1(Site);/*任务3 4：给定坐标，求基站*/
			break;
		case 5: 
		    find_Path(Track,W_Track,Site);/*任务5：绘制路径*/
			break;
		case 6:
		    Time_Station(Track,Site);/*拓展1*/
			Time_Station1(Track,Site);/*拓展1*/	
		    break;
		case 7:
			overlap(Track,Site);/*拓展2*/
		    break; 	   
		case 10:
		    printf("已退出程序，感谢您的使用！");
			return 0;	
		default:
			printf("输入不正确，应该输入1-10之间的数。\n");
			break;
        }       
    }
    
    system("pause");
    return 0;
}    










    

