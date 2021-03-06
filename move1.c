#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <libplayerc/playerc.h>
#include "playerc.h"
int read()
	{
	int choice=0;
	FILE *in;
	char line[511];
	in = fopen("input.dat", "r");
	while(!feof(in))
		{
		fgets(line, 511 , in);
		if (strcmp("sentence1: <s> DIAL ONE </s>\n", line) == 0)
			{
			
			choice=1;
			break;
			}
	
		if (strcmp("sentence1: <s> DIAL TWO </s>\n", line) == 0)
			{
			choice=2;
			break;
			}
		if (strcmp("sentence1: <s> DIAL THREE </s>\n", line) == 0)
			{
			choice=3;
			break;
			}
		if (strcmp("sentence1: <s> DIAL FOUR </s>\n", line) == 0)
			{
			choice=4;
			break;		
			}
		if (strcmp("sentence1: <s> DIAL SEVEN </s>\n", line) == 0)
			{
			choice=7;
			break;
			}
		if (strcmp("sentence1: <s> DIAL EIGHT </s>\n", line) == 0)
			{
			choice=8;
			break;
			}
		if (strcmp("sentence1: <s> DIAL FIVE </s>\n", line) == 0)
			{
			choice=5;
			break;
			}
	
		}
		fclose(in);
		remove("input.dat");
		return choice;
	}


char* getFileCreationTime(char *path) 
	{
	struct stat attr;
    	stat(path, &attr);
    	return ctime(&attr.st_mtime);
	}

void timeDelay (int secs) 
	{
    	int retTime = time(0) + secs;     
    	while (time(0) < retTime);   
	}

int exists(const char *filename)
	{
   	struct stat buffer;   
   	return (stat (filename, &buffer) == 0);
	}
void move_straight(int x,playerc_client_t *client,playerc_position2d_t *position2d)
	{
	static int count = 1;
	int i = 0;
	while(i<1)
		{
        	// Wait for new data from server
        	playerc_client_read(client);
        	fprintf(stdout, "X: %3.2f, Y: %3.2f, Yaw: %3.2f \n",position2d->px, position2d->py, position2d->pa);
        	//Random walk is continued till finding first marker
        	playerc_position2d_set_cmd_vel(position2d, x , 0 , 0 , 1 );
        	sleep(1);
		playerc_position2d_set_cmd_vel(position2d, 0 , 0 , 0 , 0 );
		i+=1;
		}
	//playerc_position2d_set_cmd_vel(position2d, 0 , 0 , 0 , 1 );
	sleep(1);
	playerc_position2d_set_cmd_vel(position2d, 0 , 0 , 0 , 0 );
	}

int turn_left(playerc_client_t *client,playerc_position2d_t *position2d)
	{
        playerc_client_read(client);
    	fprintf(stdout, "X: %3.2f, Y: %3.2f, Yaw: %3.2f \n",position2d->px, position2d->py, position2d->pa);
    	playerc_position2d_set_cmd_vel(position2d, 0,0 , DTOR(91),1);
    	sleep(1);
    	playerc_position2d_set_cmd_vel(position2d, 0, 0, 0, 1);
    	sleep(1);
	}

int turn_right(playerc_client_t *client,playerc_position2d_t *position2d)

	{
	playerc_client_read(client);
    	fprintf(stdout, "X: %3.2f, Y: %3.2f, Yaw: %3.2f \n",position2d->px, position2d->py, position2d->pa);
    	playerc_position2d_set_cmd_vel(position2d, 0,0 , -1*DTOR(91),1);
    	sleep(1);
    	playerc_position2d_set_cmd_vel(position2d, 0, 0, 0, 1);
    	sleep(1);
	}

int rotate120(playerc_client_t *client,playerc_position2d_t *position2d)
	{
	playerc_client_read(client);
    	fprintf(stdout, "X: %3.2f, Y: %3.2f, Yaw: %3.2f \n",position2d->px, position2d->py, position2d->pa);
    	playerc_position2d_set_cmd_vel(position2d, 0,0 , -1*DTOR(123),1);
    	sleep(1);
    	playerc_position2d_set_cmd_vel(position2d, 0, 0, 0, 1);
    	sleep(1);
	}

int square(playerc_client_t *client,playerc_position2d_t *position2d)
	{
	playerc_client_read(client);
	move_straight(1,client,position2d);
	turn_left(client,position2d);	
	move_straight(1,client,position2d);
	turn_left(client,position2d);
	move_straight(1,client,position2d);
	turn_left(client,position2d);
	move_straight(1,client,position2d);
	turn_left(client,position2d);
	move_straight(1,client,position2d);
	sleep(1);
	}	


int triangle(playerc_client_t *client,playerc_position2d_t *position2d)
	{
	playerc_client_read(client);
	move_straight(1,client,position2d);
	rotate120(client,position2d);
	move_straight(1,client,position2d);	
	rotate120(client,position2d);
	move_straight(1,client,position2d);	
	sleep(1);
	}

int main(int argc, const char **argv)
	{
	while(1)
		{
		char time[100];
		strcpy(time,"V-Rec Robot");
		int i,count=0;
		playerc_client_t *client;
		playerc_position2d_t *position2d;
		int cycle, index=0;
		double dist,angle,fidAngle = 0,lineAngle=0, fidDist=0, prevYaw=0,posAngle=0;
		// Create a client and connect it to the server.
		client = playerc_client_create(NULL, "localhost", 6665);
		if (0 != playerc_client_connect(client))
			{
			fprintf(stdout,"Error!!");
			return -1;
			}
		// Create and subscribe to a position2d device.
		position2d = playerc_position2d_create(client, 0);
		if (playerc_position2d_subscribe(position2d, PLAYER_OPEN_MODE))
			return -1;
		int choice1,ch;
		playerc_client_read(client);
		if(exists("input.dat")
			{
			if(strcmp(time, getFileCreationTime("input.dat"))
				{
				choice1=read();
				switch(choice1)
					{
					case 1 : {move_straight(1,client,position2d);break;}
					case 2 : {turn_left(client,position2d);break;}
					case 3 : {turn_right(client,position2d);break;}
					case 4 : {move_straight(-1,client,position2d);break;}
					case 5 : {square(client,position2d);break;}
					case 6 : {triangle(client,position2d);break;}
					case 7 : break;
					default : {printf("Wrong Choice!!!!!\n"); break;}
					}
				strcpy(time, getFileCreationTime("input.dat"));
				playerc_client_read(client);
				playerc_position2d_set_cmd_vel(position2d,0,0,0,0);
				sleep(1);
				//time = getFileCreationTime("input.dat");
				}
			else
				{
				timeDelay(3);
				}
			}
		else
			{
			timeDelay(2)
			}
		}
	playerc_position2d_unsubscribe(position2d);
	playerc_position2d_destroy(position2d);
	playerc_client_disconnect(client);
	playerc_client_destroy(client);
	return 0;
	}





