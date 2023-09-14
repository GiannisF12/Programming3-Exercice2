#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

struct flight{
    int flight_id;
    char from_city[10];
    char to_city[10];
    int available_seats;
    int price;
};

struct flightinfo{
	int Reserves,Failed,Success;
};

int main(int argc, char *argv[])
{
    int i;
    struct flight flights[10]=
	{
    		{0,"city1","city2",(rand() % 5)+1,(rand() % (300-20+1))+20},
        	{1,"city2","city3",(rand() % 5)+1,(rand() % (300-20+1))+20},
        	{2,"city3","city4",(rand() % 5)+1,(rand() % (300-20+1))+20},
        	{3,"city4","city5",(rand() % 5)+1,(rand() % (300-20+1))+20},
           	{4,"city5","city6",(rand() % 5)+1,(rand() % (300-20+1))+20},
           	{5,"city6","city7",(rand() % 5)+1,(rand() % (300-20+1))+20},
        	{6,"city7","city8",(rand() % 5)+1,(rand() % (300-20+1))+20},
        	{7,"city8","city9",(rand() % 5)+1,(rand() % (300-20+1))+20},
        	{8,"city9","city10",(rand() % 5)+1,(rand() % (300-20+1))+20},
        	{9,"city10","city1",(rand() % 5)+1,(rand() % (300-20+1))+20},

	};
    struct flightinfo fl_info[10];
    
    for(i=0;i<10;i++){
    	fl_info[i].Reserves = 0;
    	fl_info[i].Failed = 0;
    	fl_info[i].Success = 0;
    }
	
    int client;   	
    int Res=0;
    int success=0;
    int Profit=0;
    int fd1[2],fd2[2];
    
    
    for(i=0;i<10;i++)
    {
    	srand(time(NULL));
    	sleep(1);
    	pipe(fd1);
        pipe(fd2);
    	client = fork();
        if(client == 0)
        {
        	int flight,ticket_number;
        	close(fd1[0]);
        	close(fd2[0]);
            ticket_number=(rand() % (3-1+1))+1;
            flight=(rand() % (10));
            printf("%d\n",flight);
            write(fd1[1], &ticket_number, sizeof(ticket_number)); 
            write(fd2[1], &flight, sizeof(flight));
            close(fd1[1]);
            close(fd2[1]);
            exit(1);
        }
        else
        {
        	int flight=0,ticket_number=0;
        	read(fd1[0], &ticket_number, sizeof(ticket_number)); 
        	read(fd2[0], &flight, sizeof(flight));
        	Res = Res + ticket_number; //gia na broume ta sinolika aithmata krathshs
        	fl_info[flight].Reserves = fl_info[flight].Reserves + ticket_number; //count prospathies kratisewn
        	close(fd1[0]);
            close(fd2[0]);
            close(fd1[1]);
            close(fd2[1]);
        	if(flights[flight].available_seats >= ticket_number) // an eparkoun oi diathesimes theseis
			{
        		fl_info[flight].Success =  fl_info[flight].Success + ticket_number; //gia kathe ptisi
        		success = success + ticket_number; // gia oles tis ptiseis
        		Profit = Profit + ticket_number*flights[flight].price;
        		flights[flight].available_seats = flights[flight].available_seats - ticket_number; //meiwsh diathesimwn thesewn giati exoun kleistei kapoies
        	}
			else
			{
        		fl_info[flight].Success =  fl_info[flight].Success + flights[flight].available_seats; //kleinei oses theseis uparxoun apominei
        		success = success + flights[flight].available_seats;
        		Profit = Profit + flights[flight].available_seats*flights[flight].price;
        		flights[flight].available_seats = 0;
        	}
        	
        }
   }
   for(i=0;i<10;i++)
   {
	fl_info[i].Failed = fl_info[i].Reserves - fl_info[i].Success;
    printf("\n From %s To %s",flights[i].from_city,flights[i].to_city);
   	printf("\n With Flights Reserves :%d",fl_info[i].Reserves);
   	printf("\n Success Flight Reserves:%d",fl_info[i].Success);
   	printf("\n Failed Flight Reserves:%d",fl_info[i].Failed);
    }
   
   printf("\n synolika aithmata krathshs:%d",Res);
   printf("\n synolikes epityxhs krathseis :%d",success);
   printf("\n synolika kerdh etairias:%d\n",Profit);
       
    
	return 0;
}
