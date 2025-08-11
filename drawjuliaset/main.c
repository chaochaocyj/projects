#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

#define FRAME_WIDTH		640
#define FRAME_HEIGHT	480

#define FRAME_BUFFER_DEVICE	"/dev/fb0"

int16_t frame[480][640];
int ida;
int idb;
int idc;
int ids;
char tnum [50] = "Team 8\n";
char mema [50] = "Chou, Pin-Yi\n";
char memb [50] = "Lin, Chih-An\n";
char memc [50] = "Chao, Yi-Ju\n";
int main()
{
	//RGB16


	int max_cX = -700;
	int min_cY = 270;

	int cY_step = -5;
	int cX = -700;	// x = -700~-700
	int cY;			// y = 400~270

	int fd;


	printf( "Function1: Name\n" );


	name(tnum,mema,memb,memc);

	printf( "Function2: ID\n" );


	id(ida,idb,idc,ids);

	printf( "Main Function:\n" );
	printf( "*****Print All*****\n" );
	printf( "%s", tnum );
	printf( "%d %s",ida,mema );
	printf( "%d %s",idb,memb );
	printf( "%d %s",idc,memc);
	printf( "ID Summation = %d\n",ids );
	printf( "*****End Print*****\n" );


	printf( "\n***** Please enter p to draw Julia Set animation *****\n" );
	// ���ݨϥΪ̿�J���T���O
	while(getchar()!='p') {}

	// �M���e��
	system( "clear" );

	// ���} Frame Buffer �w��˸m��Device Node�A�ǳƤ��᪺�X�ʵ{���I�s
	fd = open( FRAME_BUFFER_DEVICE, (O_RDWR | O_SYNC) );

	if( fd<0 )
	{	printf( "Frame Buffer Device Open Error!!\n" );	}
	else
	{
		for( cY=400 ; cY>=270; cY -= 5 ) {

			// �p��ثecX,cY�ѼƤU��Julia set�e��
			drawjuliaset( cY, frame);

			// �z�L�C��I/O�ާ@�I�sFrame Buffer���X�ʵ{��
			// (�N�e����Ƽg�JFrame Buffer)
			write( fd, frame, sizeof(int16_t)*307200 );

			// �����ɮ׾ާ@��m�̫ܳe�ݡA�H�K�U�@�����e�����s�g�J
			lseek( fd, 0, SEEK_SET );
		}


		//Dummy printout. Please refer to the specification of Project 1.

		printf( ".*.*.*.<:: Happy New Year ::>.*.*.*.\n" );
		printf( "%s", tnum );
		printf( "%d %s",ida,mema );
		printf( "%d %s",idb,memb );
		printf( "%d %s",idc,memc);

		// ���� Device Node�ɮסA�����X�ʵ{�����ϥ�
		close( fd );
	}

	// ���ݨϥΪ̿�J���T���O
	while(getchar()!='p') {}

	return 0;
}
