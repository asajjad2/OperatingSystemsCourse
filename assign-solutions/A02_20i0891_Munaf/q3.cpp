#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
using namespace std;

int CHECK = 0;
int fdEmplys[10][2];
char tempStr[256];

void checkBuffer(int i)
{
    i--;
    read ( fdEmplys[i][0] , tempStr ,sizeof(tempStr));
	close(fdEmplys[i][0]);
	
    cout<<"Employee# "<< i+1 <<"    "<<tempStr<<endl;
	if ( tempStr[0]=='Y' || tempStr[0]=='y' )
	{
		CHECK++;
	}
	
}

void askEmployee(int i)
{
    i--;
    cout<<"\nEmployee# "<< i+1 <<"\nIs the meeting time suitable for you?"<<endl;
    cout<<"Y or N: ";
    char temp;
    cin>>temp;
    
    tempStr[0]=temp;

    write ( fdEmplys[i][1] , tempStr ,sizeof(tempStr));
    close(fdEmplys[i][1]);
		
	sleep(1);
}

int main()
{
    int fd [2] ; 
    pipe(fd);
    pid_t childpid ;
    char timeA[100];
    
	    
	childpid = fork () ;
	    
    if ( childpid > 0 ) 
	{	
		string str = "Meeting Timings: ";
		
        string  meeting_time;
        
        cout<<"Enter Meeting Timings: ";
        cin>>meeting_time;
        
        str += meeting_time;

        for(int i =0;i<str.length(); i++)
            timeA[i] = str[i];
        
        write ( fd [1] , timeA ,sizeof(timeA));
		close(fd [1]);
	   
		wait(NULL);

        for(int i=0; i < 10; i++)
        {
            checkBuffer(i);
        }

		cout << "Available employees count: " << CHECK << endl;
		
		if (CHECK>5)
		{
			cout << "Meeting scheduled at " << meeting_time << endl;
			return 0;
		}
		else
		{
		    cout << "Due to the unavailiblity of most employees meeting is not scheduled!\n";
		}

	    exit(0);
	}
	else
	{
	    read ( fd[0] , timeA , sizeof (timeA));
		close(fd[0]);
	        	
		cout << endl << timeA << endl;
		
        askEmployee(1);
        cin.ignore();
		
		//cout << "abcd testing\n";
	        
		pid_t p2;
		p2 = fork();
		if ( p2 > 0 )
		{
			wait(NULL);
			askEmployee(5);
            cin.ignore();
	       
	        //cout << "new testing\n";
			pid_t p3;
			p3 = fork();
			if ( p3 > 0 )
			{
				wait(NULL);
			    askEmployee(7);
                cin.ignore();
			    //cout << "what testing\n";
	       
				pid_t p4;
				p4 = fork();
				if ( p4 > 0 )
				{
					wait(NULL);
					askEmployee(9);
                    cin.ignore();
	       
					pid_t p5;
					p5 = fork();
					if ( p5 > 0 )
					{
						askEmployee(10);
                        cin.ignore();
	       
					}	
				}
				else
				{
					askEmployee(8);
                    cin.ignore();
	      
				}
			}
			else
			{
				askEmployee(6);
                cin.ignore();
	     
			}		

		}
		else
		{
            sleep(1);
			askEmployee(2);
            cin.ignore();
	     
			pid_t p6;
			p6 = fork();
			if ( p6 > 0 )
			{
				askEmployee(4);
                cin.ignore();
	     		
			}			
			else
			{
				askEmployee(3);
                cin.ignore();
	     
			}	

		}
	}

return 0;
}
