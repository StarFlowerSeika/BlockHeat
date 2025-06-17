/* VC�p�̂���windows.h����include����K�v������ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>
#include <time.h>
#include <math.h>
#include "text_GL.h"

#define  FPS 30

#define _X 0
#define _Y 1
#define _Z 2

#define _LEFT 0
#define _MIDDLE 1
#define _RIGHT 2

/* �O���[�o���ϐ��錾 */
/* �x�N�g���^ */
typedef struct{
	GLfloat vector[3]; /* �x�N�g��[XYZ] */
	GLfloat norm;      /* �x�N�g���̑傫�� */
} Vector;

/* �}�e���A���i�ގ��j�^ */
typedef struct {
	GLfloat diffuse_rgba[4];     /* �g�U���ːF */
	GLfloat specular_rgba[4];    /* ���ʔ��ːF */
	GLfloat shininess_rate;      /* �P���̓x���� */
} Material;


/* ���@�̌^ */
typedef struct {
	Vector position;       /* �ʒu�x�N�g�� */
	Vector speed;          /* ���x�x�N�g�� */
	Vector direction;      /* �����x�N�g�� */
	GLfloat power;         /* �p���[ */
	Vector mobility;       /* �@���� */
	GLint life;            /* �c�@ */
	GLint balls;           /* �{�[���� */
	GLfloat hitwidth, hitwidth0[2], hitheight, hitdepth; /* �����蔻��T�C�Y */
	Material material;     /* �ގ� */
	int score;             /* �X�R�A */
	int highscore;         /* �n�C�X�R�A */
	GLint deadcount;       /* ���S�� */
	GLint newrecoad;       /* �V�L�^�t���O */
	GLint gamemode;        /* �Q�[�����[�h */
} Ship;

/* �{�[���̌^ */
typedef struct {
	Vector position;       /* �ʒu�x�N�g�� */
	Vector speed;          /* ���x�x�N�g�� */
	Vector addspeed;       /* �����x�x�N�g�� */
	Material material;     /* �ގ� */
	GLint tamaflag;        /* �e�̗L���t���O */
	GLint hitcount;        /* �q�b�g�� */
	GLint point;           /* ���_ */
	GLint hitflag[10][10];
	GLint time;            /* �o�ߎ��� */
} Tama;


/* �X�e�[�W�̌^ */
typedef struct {
	GLint block[10][10];
	GLint blocks;
	GLfloat left, right, top, bottom;
	GLint gravity_flag;    /* -1:������ 0:�Ȃ� 1:����� */
	GLfloat blockroll[10][10];           /* �u���b�N��] */
	GLfloat blockrollspeed[20];          /* �u���b�N��]���x */
	GLint blockpoint[20];                /* �u���b�N���_ */
	GLint enemy;
	char filename[16][16];   /* �t�@�C�������X�g */
	char filenamedata[16];   /* �X�e�[�W�t�@�C���� */
	GLint now;               /* ���݂̃X�e�[�W�ԍ� */
	GLint last;              /* �ŏI�X�e�[�W�ԍ� */
	GLint demotime;          /* �f������ */
	GLint display;
	GLfloat vmax[2];         /* �ő呬�x */
	GLint debugflag;
	GLfloat demoview[9];
	GLfloat gameview[9];
} Stage;

/* �{�^�����͏�� */
typedef struct {
	GLint Down[3];
	GLint Up[3];
} Button; 

Ship ship;
Tama tama[10],shot[10],hit[10],collision[10];
Stage stage;
Button mouse;
Material textmaterial;

static GLint display_num;


/* �O���[�o���ϐ��錾�����܂� */


/* �֐��錾 */
/* �L�����N�^�[������ */
void Initcharacter(void)
{
	int i,j;
	/* ���@�̏����� */
	for(i=0; i<3; i++){
		ship.position.vector[i] = 0.0;
		ship.speed.vector[i] = 0.0;
		ship.direction.vector[i] = 0.0;
   	}
	ship.position.vector[_X] = 10.0;
	ship.position.vector[_Y] = -10.0;
	ship.position.vector[_Z] = 0.0; 
	ship.position.norm = 0.0;
	ship.speed.norm = 0.0;
	ship.power = 0.1;
	ship.mobility.vector[0] = 0.1;
	ship.mobility.vector[1] = 0.1;
	ship.mobility.vector[2] = 0.1;
	ship.life = 10;
	ship.balls = 10;
	ship.hitwidth0[0] = 5.0;
	ship.hitwidth0[1] = 1.0;
	ship.hitheight = 1.0;
	ship.hitdepth = 0.5;
	ship.material.diffuse_rgba[0] = 0.7;
	ship.material.diffuse_rgba[1] = 1.0;
	ship.material.diffuse_rgba[2] = 0.7;
	ship.material.diffuse_rgba[3] = 1.0;
	ship.material.specular_rgba[0] = 1.0;
	ship.material.specular_rgba[1] = 1.0;
	ship.material.specular_rgba[2] = 1.0;
	ship.material.specular_rgba[3] = 1.0;
	ship.material.shininess_rate = 80.0;

	stage.vmax[0] = 0.9;
	stage.vmax[1] = 0.5;


	/* �e�̏����� */
	for(i=0; i<10; i++){
		tama[i].material.diffuse_rgba[0] = 0.1*i;
		tama[i].material.diffuse_rgba[1] = sin(0.314*i);
		tama[i].material.diffuse_rgba[2] = (10-i)*0.1;
		tama[i].material.diffuse_rgba[3] = 1.0;
		tama[i].material.specular_rgba[0] = 1.0;
		tama[i].material.specular_rgba[1] = 1.0;
		tama[i].material.specular_rgba[2] = 1.0;
		tama[i].material.specular_rgba[3] = 1.0;
		tama[i].material.shininess_rate = 40.0;
		tama[i].tamaflag = 0;
		hit[i].tamaflag=0;
		collision[i].tamaflag=0;
		for(j=0;j<10;j++){tama[i].hitflag[i][j] = 0;}
	}

	/* �e�L�X�g�p�}�e���A���̏����� */
	textmaterial.diffuse_rgba[0] = 1.0;
	textmaterial.diffuse_rgba[1] = 1.0;
	textmaterial.diffuse_rgba[2] = 1.0;
	textmaterial.diffuse_rgba[3] = 1.0;
	textmaterial.specular_rgba[0] = 1.0;
	textmaterial.specular_rgba[1] = 1.0;
	textmaterial.specular_rgba[2] = 1.0;
	textmaterial.specular_rgba[3] = 1.0;
	textmaterial.shininess_rate = 90.0;
	stage.gameview[0] = 9.5;
	stage.gameview[1] = 5.0;
	stage.gameview[2] = 10.0;
	stage.gameview[3] = 9.5;
	stage.gameview[4] = 0.0;
	stage.gameview[5] = 0.0;
	stage.gameview[6] = 0.0;
	stage.gameview[7] = 1.0;
	stage.gameview[8] = 0.0;
	for(i=0;i<9;i++){
		stage.demoview[i] = stage.gameview[i];
	}
	stage.demoview[2] = 30.0;
	stage.demotime = 200;
}

/* �O���[�o���ϐ������������܂� */

/* �֐���`�������� */

/* �E�B���h�E������ */
void window(void)
{
	auxInitDisplayMode(AUX_DOUBLE | AUX_RGBA | AUX_DEPTH);
	auxInitPosition(0,0,800,800);
	auxInitWindow("Block");
}


/* �t�@�C�������� */
void Initfile(void)
{
	FILE *fp;
	GLint i;
	if((fp = fopen("block_stage_filename.dat","r")) == 0){
		
		exit(0);
	}
	fscanf(fp,"%d",&stage.last);

	for(i=0;i<stage.last;i++){
		fscanf(fp,"%s",&stage.filename[i]);
	}
	fclose(fp);

	if((fp = fopen("block_rollspeed.dat","r")) == 0){
		
		exit(0);
	}
	for(i=0;i<=20;i++){
		fscanf(fp,"%f",&stage.blockrollspeed[i]);
	}
	fclose(fp);

}

/* �n�C�X�R�A�Ǎ� */
void loadhighscore(void)
{
	FILE *fp;

	char filename[2][40]={"block_normal_highscore.dat","block_tamayoke_highscore.dat"};
		if((fp = fopen(filename[ship.gamemode],"r")) == 0){
		
		exit(0);
	}
	fscanf(fp,"%d",&ship.highscore);
}

/* �n�C�X�R�A�ۑ� */
void savehighscore(void)
{
	FILE *fp;
	char filename[2][40]={"block_normal_highscore.dat","block_tamayoke_highscore.dat"};

	if((fp = fopen(filename[ship.gamemode],"w")) == 0){
		
		exit(0);
	}
	fprintf(fp,"%d",ship.highscore);
}

/* �X�e�[�W������ */
void Initstage(void)
{
	int i,j;
	FILE *fp;
	GLint block;

	stage.blocks = 0;
	stage.left = 0.0;
	stage.right = 20.0;
	stage.top = 15.0;
	stage.bottom = -10.0;
	stage.gravity_flag = 0;
	stage.enemy = 0;
	
	if((fp = fopen(stage.filename[stage.now],"r")) == 0){
		return;
	}

	for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			fscanf(fp,"%d\n",&stage.block[i][j]);
			block=stage.block[i][j];
			stage.blockroll[i][j] = 0;
			if(block>=1 && block<=10 || block==12 || block ==13){
				stage.blocks++;
			}
		}
	}

	
	fscanf(fp,"%d\n",&stage.gravity_flag);
	
	fscanf(fp,"%d\n",&stage.enemy);
	
	fclose(fp);
	
}

/* ���C�g�ݒ� */
void light_setting(void)
{
	GLfloat v[4] = { 1.0,1.0,1.0,1.0 }; /* RGBA�����F */
	glLightfv(GL_LIGHT0, GL_DIFFUSE, v);    /* �g�U���ݒ� */
	/* glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE); */ /* ���[�J���r���[�A�L�����i���g�p�j */
	glEnable(GL_LIGHTING); glEnable(GL_LIGHT0);  /* ���C�e�B���O�L���� */
	glEnable(GL_NORMALIZE);   /* �@���x�N�g�����K�� */
	glEnable(GL_DEPTH_TEST);  /* �f�v�X�e�X�g�L���� */
}

/* �����܂Ŋ֐���` */

/* ����ɽ���ؿ��� */
/* ���l�𕶎���Ƃ��ĕ`�� */
void Drawnumber(GLint num)
{
	char str[12]={'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'};
	GLint a=1000000000,i,b,j=0,flag=0;
	if(num != 0){
		for(i=0;i<10;i++){
			if(num>=a || flag){
				flag = 1;
				b = num / a;
				str[j] = '0'+(char)b;
				num -= b*a;
				j++;
			}
			a/=10;
		}
		str[j]='\0';
	}
	else{
		str[0] = '0';
	}
	DrawString(str);
}
/* ��ʏ�̊e��e�L�X�g�\�� */
void textsput(void)
{
	char string[50];
	GLint i,j;
	GLfloat px,py,fi;
	GLfloat v[][4] = {{1.0,1.0,1.0,1.0},{0.0,0.0,0.0,1.0}};
	glMaterialfv(GL_FRONT,GL_EMISSION, v[0]);
	
	glRasterPos3d(0,0,0);
	Drawnumber(ship.balls);
	/*
	for(i=1;i<=20;i++){
		sprintf(string,"%f",stage.rollspeed[i]);
		glRasterPos3f(0.0,(GLfloat)i,1.0);
		DrawString(string);
	}
	
	for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			glRasterPos3d(j,-i-1,0);
			Drawnumber(stage.block[i][j]);
		}
	}
	*/
	// �X�R�A�\��
	sprintf(string,"Score %7d",ship.score);
	glRasterPos3f(-40.0,50.0,1.0);
	DrawString(string);
	// �n�C�X�R�A�\��
	sprintf(string,"High Score %7d",ship.highscore);
	glRasterPos3f(-45.0,60.0,1.0);
	DrawString(string);
	// ���C�t�\��
	sprintf(string,"Life %2d",ship.life);
	glRasterPos3f(0,50.0,1.0);
	DrawString(string);

	px = -40.0;
	py = 45.0;
	fi = 0;
	if(stage.demotime==0){

		// �q�b�g���⓾�_�A�R���W�����̕\��
		for(i=0; i<10; i++){
			/* �q�b�g�� */
			if(hit[i].tamaflag == 1){
				sprintf(string,"%6d HIT",tama[i].hitcount);
				glRasterPos3f(hit[i].position.vector[_X]-2.0,hit[i].position.vector[_Y]-0.5,1.0);
				DrawString(string);
			}
			/* ���_ */
			else if(hit[i].tamaflag == 2 && tama[i].point){
				fi += 0.5;
				sprintf(string,"%6d Pts.",tama[i].point);
				glRasterPos3f(ship.position.vector[_X]-1.0,ship.position.vector[_Y]+0.75+fi,1.0);
				
				DrawString(string);
			}
			/* �q���W���� */
			if(collision[i].tamaflag == 1){
				sprintf(string,"Collision! %6d Pts.",tama[i].point);
				glRasterPos3f(px,py,1.0);
				DrawString(string);
				px += 3.0;
				py -= 3.7;
			}
		}
		// �c��u���b�N���\��
		sprintf(string,"Block %4d",stage.blocks);
		glRasterPos3f(20.0,30.0,1.0);
		DrawString(string);
		
		// ���S�񐔕\��
		sprintf(string,"Dead Count %d",ship.deadcount);
		glRasterPos3f(20.0,34.0,1.0);
		DrawString(string);
	}

	// �X�e�[�W�N���A�\��
	if(stage.blocks==0 && stage.now!=stage.last){
		sprintf(string,"\"%s\" Crear!",stage.filename[stage.now]);
		glRasterPos3f(5.0,0.0,1.0);
		DrawString(string);
	}
	else if(stage.blocks==0 && stage.now==stage.last){
		if(ship.newrecoad && ship.deadcount){
			savehighscore();
		}
		exit(0);
	}

	glMaterialfv(GL_FRONT,GL_EMISSION, v[1]);
}


/* �u���b�N�`�� */
void blockput(GLfloat x, GLfloat y, GLint block, GLfloat roll)
{
	GLfloat v[2][4],w[2][4]={{1.0,1.0,1.0,1.0},{0.0,0.0,0.0,1.0}};
	v[0][0]=x/10.0;
	v[0][1]=y/10.0;
	v[0][2]=1.0;
	v[0][3]=1.0;
	v[1][0]=1.0;
	v[1][1]=1.0;
	v[1][2]=1.0;
	v[1][3]=1.0;

	glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,v[0]);
	glMaterialfv(GL_FRONT,GL_SPECULAR,v[1]);
	glMaterialf(GL_FRONT,GL_SHININESS,64.0);
	glPushMatrix();
	glTranslatef(x,y,0);
	
    if(block<=10){
		  auxSolidBox(1.8,0.8,(GLfloat)(block/2.0));
	}
	else if(block>=11){
		switch(block){
			case 11: glPushMatrix();
			           glRotatef(roll,0.0,1.0,0.0);
			           auxSolidCube(1.3);
			         glPopMatrix();
			         break;
			case 12: glPushMatrix();
			           glRotatef(roll,0.0,0.0,1.0);
			           auxSolidTetrahedron(1.0);
			         glPopMatrix();
			         break;
			case 13: auxSolidCone(1.0,2.0);
			         break;
		}
		if(block>=14 && block<=20){
			glPushMatrix();
			  glRotatef(51.4*(float)(block-13)+roll,1.0,0.0,0.0);
			  auxWireBox(1.8,0.8,0.8);
			glPopMatrix();
			if(stage.demotime==0){
				glMaterialfv(GL_FRONT,GL_EMISSION, w[0]);
				glRasterPos3f(x/8.0-2.0,y/8.0,-1.0);
				Drawnumber((block-13)*10);
				glMaterialfv(GL_FRONT,GL_EMISSION, w[1]);
			}
		}
	}
	glPopMatrix();
}

/* �p�h���i���@�j�`�� */
void paddleput(void)
{
	GLfloat v[4]={1.0,1.0,1.0,1.0};
	glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,ship.material.diffuse_rgba);
	glMaterialfv(GL_FRONT,GL_SPECULAR,ship.material.specular_rgba);
	glMaterialf(GL_FRONT,GL_SHININESS,ship.material.shininess_rate);
	if(ship.gamemode==0){
		glPushMatrix();
		glTranslatef(ship.position.vector[_X],ship.position.vector[_Y],ship.position.vector[_Z]);
		auxSolidBox(ship.hitwidth,ship.hitheight,ship.hitdepth);
		glPopMatrix();
	}
	else if(ship.gamemode==1){
		glPushMatrix();
		glTranslatef(ship.position.vector[_X],ship.position.vector[_Y],ship.position.vector[_Z]);
		auxSolidBox(ship.hitwidth,ship.hitheight,ship.hitdepth);
		glPopMatrix();

		glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,v);
		glMaterialfv(GL_FRONT,GL_SPECULAR,v);
		glMaterialf(GL_FRONT,GL_SHININESS,0.0);
		glPushMatrix();
		glTranslatef(stage.right/2.0,stage.bottom-1.5,0.0);
		auxSolidBox(stage.right+3.0,1.0,0.5);
		glPopMatrix();


	}

}

/* �g�i�ǁj�`�� */
void wakuput(void)
{
	GLfloat v[4]={1.0,1.0,1.0,1.0};
	glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,v);
	glMaterialfv(GL_FRONT,GL_SPECULAR,v);
	glMaterialf(GL_FRONT,GL_SHININESS,60.0);
	glPushMatrix();
	  glTranslatef(stage.left-1.0,0.0,0.0);
	  auxSolidBox(1.0,30.0,1.0);
	glPopMatrix();
	glPushMatrix();
	  glTranslatef(stage.right+1.0,0.0,0.0);
	  auxSolidBox(1.0,30.0,1.0);
	glPopMatrix();
	glPushMatrix();
	  glTranslatef(stage.right/2.0,stage.top,0.0);
	  auxSolidBox(stage.right+3.0,1.0,1.0);
	glPopMatrix();
}

/* �{�[���`�� */
void ballput(void)
{
	GLint i;
	for(i=0;i<10;i++){
		if(tama[i].tamaflag == 1){
			glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,tama[i].material.diffuse_rgba);
			glMaterialfv(GL_FRONT,GL_SPECULAR,tama[i].material.specular_rgba);
			glMaterialf(GL_FRONT,GL_SHININESS,tama[i].material.shininess_rate);
			glPushMatrix();
			glTranslatef(tama[i].position.vector[_X],tama[i].position.vector[_Y],tama[i].position.vector[_Z]);
			auxSolidIcosahedron(0.5);
			glPopMatrix();
		}
	}
}

/* ���C���`�揈�� */
void CALLBACK display(void)
{
	GLint i,j,k;

	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			if(stage.block[i][j] > 0){
				stage.blockroll[i][j] += stage.blockrollspeed[stage.block[i][j]];
				if(stage.blockroll[i][j] > 360.0){
					stage.blockroll[i][j] -= 360.0;
				}
				else if(stage.blockroll[i][j] < -360.0){
					stage.blockroll[i][j] += 360.0;
				}
				blockput((GLfloat)j*2+1.0, (GLfloat)i, stage.block[i][j], stage.blockroll[i][j]);
			}
		}
	}

	if(ship.life > -10){
		paddleput();
	}
	ballput();

	wakuput();
  
	textsput();
   
#if 1
  {
    float tt = (float)timeGetTime();
    float ts = tt / (1000.0/(float)FPS);
    int tn = (int)((int)(ts+1) * (1000.0/(float)FPS));
    if(tn > (int)tt){
      Sleep(tn-(int)tt);
    }
  }
#endif
	glFlush();

	auxSwapBuffers();

}
/* ���C���`�揈�������܂� */


/* �E�B���h�E���T�C�Y���̏��� */
void CALLBACK Reshape(int w, int h)
{
	static GLfloat v[4]={1.0,-1.0,2.0,0.0}; /* �����θ��� */
	auxInitPosition(0,0,800,800);
 
	

	if(stage.demotime==0){
		glMatrixMode(GL_PROJECTION); glLoadIdentity();
		gluPerspective(120,(GLfloat)w/(GLfloat)h,1.0,40.0);
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
		gluLookAt(9.5,-5.0,10.0, 9.5,0.0,0.0, 0.0,1.0,0.0);
	}
	else if(stage.blocks>0 && stage.demotime>0){
		glMatrixMode(GL_PROJECTION); glLoadIdentity();
		gluPerspective(120,1.0,1.0,(GLfloat)(stage.demotime)+50.0);
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
		gluLookAt(9.5,-5.0,10.0+(GLfloat)(stage.demotime), 9.5,0.0,0.0, 0.0,1.0,0.0);
	}
	glLightfv(GL_LIGHT0,GL_POSITION,v);      /* ʿ�Ը��� */
	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	display();
}

/* �e�L�X�g�ړ��p�̏����� */
void init_movetext(Tama moji[],GLint i)
{
	moji[i].tamaflag = 1;
	moji[i].position.vector[_X] = tama[i].position.vector[_X];
	moji[i].position.vector[_Y] = tama[i].position.vector[_Y];
	moji[i].position.vector[_Z] = 0.0;
	moji[i].speed.vector[_X] = 0.0;
	moji[i].speed.vector[_Y] = 0.01;
	moji[i].speed.vector[_Z] = 0.0;
}


/* �{�[�����ˏ��� */
int ball_shot(GLfloat vector1[3],GLfloat vector2[3]){
	GLint i,j,flag=0;
	for(i=0;i<10;i++){
		if(tama[i].tamaflag == 0){
			for(j=0;j<10;j++){
				tama[i].hitflag[i][j] = 1;
				tama[i].hitflag[j][i] = 1;
			}
			tama[i].time=0;
			tama[i].tamaflag = 1;
			tama[i].position.vector[_X] = vector1[_X];
			tama[i].position.vector[_Y] = vector1[_Y];
			tama[i].position.vector[_Z] = vector1[_Z];
			tama[i].speed.vector[_X] = vector2[_X];
			tama[i].speed.vector[_Y] = vector2[_Y];
			tama[i].speed.vector[_Z] = 0.0;
			i = 10;
			flag=1;
		}
	}
	return flag;
}
/* �}�E�X���{�^���������̏��� */
void  CALLBACK mouse_left_down(AUX_EVENTREC *event)
{
	GLint i,j,no=0;
	GLfloat rr,dx,dy;
	GLfloat vector[3],position[3];
	if(stage.blocks && stage.demotime==0){
		for(j=0;j<10;j++){
			if(tama[j].tamaflag){
				dx = ship.position.vector[_X] - tama[j].position.vector[_X];
				dy = ship.position.vector[_Y]+1.0 - tama[j].position.vector[_Y];
				rr = dx*dx+dy*dy;
				if(rr<=1.0){
					no = 1;
				}
			}
		}
		vector[_X] = stage.vmax[ship.gamemode];
		vector[_Y] = stage.vmax[ship.gamemode];
		vector[_Z] = 0.0;


		if(ship.balls>0 && no == 0){
			for(i=_X;i<=_Z;i++){
				position[i]=ship.position.vector[i];
			}
			position[_Y] += 1.0;
			ship.balls --;
			ball_shot(position,vector);
		}
	}
	else if(stage.blocks && stage.demotime>0){
		mouse.Down[_LEFT] = 1;
		Reshape(1.0,1.0);
	}
	else if(stage.blocks==0 && stage.now!=stage.last){
		mouse.Down[_LEFT] = 1;
		/*
		stage.now++;
		Initcharacter();
		Initstage();
		*/
	}
	else if(stage.blocks==0 && stage.now==stage.last){
		exit(0);
	}
}

/* �}�E�X���{�^�����������̏��� */
void CALLBACK mouse_left_up(AUX_EVENTREC *event)
{
	mouse.Down[_LEFT]=0;
}

/* �}�E�X���{�^���������̏����i�Q�[�����[�h�ؑցj */
void CALLBACK mouse_middle_down(AUX_EVENTREC *event)
{
	if(ship.gamemode == 0){
	    ship.hitwidth = ship.hitwidth0[1];
	}
	else{
		ship.hitwidth = ship.hitwidth0[0];
	}
	ship.gamemode = !ship.gamemode;
}

/* �u���b�N������ */
void block_delete(GLint i, GLint x, GLint y)
{
	stage.blocks --;
	stage.block[y][x] = 0;
}
/* �q�b�g�����u���b�N�̓��_�v�Z */
void hit_score_make(GLint i,GLint x, GLint y,GLint pos)
{
	tama[i].hitcount ++;
	tama[i].point = (tama[i].hitcount+i) * pos;
	ship.score += tama[i].point;
	hit[i].tamaflag = 0;
	init_movetext(hit,i);
}

/* �u���b�N�Ƀq�b�g�������̏��� */
void block_hit(GLfloat fnx, GLfloat fny, GLfloat fbx, GLfloat fby, GLint i)
{
	GLint blo,blox,bloy,j,k,nx,ny,bx,by,hitflag=0;
	GLint bloM[3][3];
	GLint Mx,My,Mxs,Mxe,Mys,Mye;
	GLint flag[]={0,0,0,0,0,0,0,0,0};
	static GLfloat vector[3];
	GLfloat v[3];
	nx = (GLint)(fnx/2.0);
	ny = (GLint)fny;
	bx = (GLint)(fbx/2.0);
	by = (GLint)fby;
	stage.debugflag=0;
	if(nx>=0 && nx<10 && ny>=0 && ny<10){

		v[_X]=tama[i].speed.vector[_X];
		v[_Y]=tama[i].speed.vector[_Y];
		v[_Z]=tama[i].speed.vector[_Z];

		if(hit[i].tamaflag == 2){
			tama[i].hitcount = 0;
		}
		
		blo = stage.block[ny][nx];
		if(by != ny && bx != nx && blo>0){
			for(Mx=nx-1;Mx<=nx+1;Mx++){
				for(My=ny-1;My<=ny+1;My++){
					if(Mx>=0 && Mx<10 && My>=0 && My<10){
						bloM[My-ny+1][Mx-nx+1] = stage.block[My][Mx];
					}
					else if(Mx<0 || Mx>=10){
						bloM[My-ny+1][Mx-nx+1] = 11;
					}
					else if(My<0 || My>=10){
						bloM[My-ny+1][Mx-nx+1] = 0;
					}
				}
			}
			
			if(blo<=11 || blo>=14 && blo<=20){
				
				/* �u���b�N�̎��͂̃u���b�N�𒲂ׂ� */
				/*
				flag[0] = flag[0] || (bloM[1][0]>0 && bloM[1][2]>0 && bloM[0][1]>0 && bloM[2][1]>0);				
				*/
				flag[1] = ((bloM[0][1]>0 && bloM[1][2]>0) && bx>nx && by<ny);
				flag[1] = flag[1] || ((bloM[2][1]>0 && bloM[1][0]>0) && bx<nx && by>ny);
				flag[1] = flag[1] || ((bloM[2][1]>0 && bloM[1][2]>0) && bx>nx && by>ny);
				flag[1] = flag[1] || ((bloM[0][1]>0 && bloM[1][0]>0) && bx<nx && by<ny);
			    flag[2] = bloM[0][1]==0 && bloM[1][2]==0 && bx>nx && by<ny;
				flag[2] = flag[2] || (bloM[2][1]==0 && bloM[1][0]==0 && bx<nx && by>ny);
				flag[3] = bloM[2][1]==0 && bloM[1][2]==0 && bx>nx && by>ny;
				flag[3] = flag[3] || (bloM[0][1]==0 && bloM[1][0]==0 && bx<nx && by<ny);
				flag[4] = bloM[1][0]>0 && bx<nx;
				flag[4] = flag[4] || bloM[1][2]>0 && bx>nx;
				flag[4] = flag[4] && (bloM[0][1]==0 || bloM[2][1]==0);
				flag[5] = bloM[0][1]>0 && by<ny;
				flag[5] = flag[5] || bloM[2][1]>0 && by>ny;
				flag[5] = flag[5] && (bloM[1][0]==0 || bloM[1][2]==0);


				if(flag[1]){
					tama[i].speed.vector[_X] = -v[_X];
					tama[i].position.vector[_X] = fbx;		
					tama[i].speed.vector[_Y] = -v[_Y];
					tama[i].position.vector[_Y] = fby;		
					stage.debugflag=1;
				}
				else if(flag[2]){
					tama[i].speed.vector[_Y] = v[_X];
					tama[i].position.vector[_Y] = fby;
					tama[i].speed.vector[_X] = v[_Y];
					tama[i].position.vector[_X] = fbx;
				}				
				else if(flag[3]){
					tama[i].speed.vector[_Y] = -v[_X];
					tama[i].position.vector[_Y] = fby;
					tama[i].speed.vector[_X] = -v[_Y];
					tama[i].position.vector[_X] = fbx;
				}				
				else if(flag[4]){
					tama[i].speed.vector[_Y] = -v[_Y];
					tama[i].position.vector[_Y] = fby;		
				}
				else if(flag[5]){
					tama[i].speed.vector[_X] = -v[_X];
					tama[i].position.vector[_X] = fbx;		
				}
				
				if(blo<=10){
					hitflag=1;
					stage.block[ny][nx] -= (i/2+1);
					if(stage.block[ny][nx]<=0){
						block_delete(i,nx,ny);
					}
					hit_score_make(i,nx,ny,10);
				}
				else if(blo>=14 && blo<=20){
					hit_score_make(i,nx,ny,(blo-13)*10);
					stage.block[ny][nx] ++;
					if(stage.block[ny][nx] == 21){stage.block[ny][nx] = 17;}	
				}
			
			}		
			else {
				switch(blo){
					case 12 :
					if(ship.life>0){
						ship.life += ball_shot(tama[i].position.vector,tama[i].speed.vector);
						
						if(ship.life>(10+ship.balls)){
							ship.life = 10+ship.balls;
						}
					}				
					block_delete(i,nx,ny);
					hit_score_make(i,nx,ny,10);
					break;

					case 13 :
					if(ship.life>0){
						for(j=0;j<9;j++){
							vector[_X] = cos((float)j*6.28/9.0);
							vector[_Y] = sin((float)j*6.28/9.0);
							ship.life += ball_shot(tama[i].position.vector,vector);
							if(ship.life>(10+ship.balls)){
								ship.life = 10+ship.balls;
							}
						}
					}
					block_delete(i,nx,ny);
					hit_score_make(i,nx,ny,10);
					break;
				}
			}
		
		}
		else {
		
		bloy = stage.block[ny][bx];
		
			if(by != ny && bloy>0 && bx>=0 && bx<10){
				if(bloy<=11 || bloy>=14 && bloy<=20){		
					tama[i].speed.vector[_Y] = -v[_Y];
					if(bloy<=10){
						hitflag=1;
						stage.block[ny][bx] -= (i/2+1);
						if(stage.block[ny][bx]<=0){
							block_delete(i,bx,ny);
						}
						hit_score_make(i,bx,ny,10);
					}
					else if(bloy>=14 && bloy<=20){
						hit_score_make(i,bx,ny,(bloy-13)*10);
						stage.block[ny][bx] ++;
						if(stage.block[ny][bx] == 21){stage.block[ny][bx] = 17;}	
					}
					
				}		
				else {
					switch(bloy){
						case 12 :
						if(ship.life>0){
							ship.life += ball_shot(tama[i].position.vector,tama[i].speed.vector);
							
							if(ship.life>(10+ship.balls)){
								ship.life = 10+ship.balls;
							}
						}				
						block_delete(i,bx,ny);
						hit_score_make(i,bx,ny,10);
						break;
						
						case 13 :
						if(ship.life>0){
							for(j=0;j<9;j++){
								vector[_X] = cos((float)j*6.28/9.0);
								vector[_Y] = sin((float)j*6.28/9.0);
								ship.life += ball_shot(tama[i].position.vector,vector);
								
								if(ship.life>(10+ship.balls)){
									ship.life = 10+ship.balls;
								}
							}
						}
						block_delete(i,bx,ny);
						hit_score_make(i,bx,ny,10);
						break;
					}
				}
				tama[i].position.vector[_Y] = fby;
			}
			
			blox = stage.block[by][nx];	
			if(bx != nx && blox>0 && by>=0 && by<10){
				if(blox<=11 || blox>=14 && blox<=20){
					tama[i].speed.vector[_X] = -v[_X];
					tama[i].hitcount ++;
					if(blox<=10){
						hitflag=1;
						stage.block[by][nx] -= (i/2+1);
						if(stage.block[by][nx] <= 0){
							block_delete(i,nx,by);
						}
						hit_score_make(i,nx,by,10);
					}
					else if(blox>=14 && blox<=20){
						hit_score_make(i,nx,by,(blox-13)*10);
						stage.block[by][nx] ++;
						if(stage.block[by][nx] == 21){stage.block[by][nx] = 17;}
					}
				}
				else {
					switch(blox){
						case 12 :
						if(ship.life>0){
							ship.life += ball_shot(tama[i].position.vector,tama[i].speed.vector);
							
							if(ship.life>(10+ship.balls)){
								ship.life = 10+ship.balls;
							}
						}				
						block_delete(i,nx,by);
						hit_score_make(i,nx,by,10);
						
						break;
						
						case 13 :
						if(ship.life>0){
							for(j=0;j<9;j++){
								vector[_X] = cos((float)j*6.28/9.0);
								vector[_Y] = sin((float)j*6.28/9.0);
								ship.life += ball_shot(tama[i].position.vector,vector);
								
								if(ship.life>(10+ship.balls)){
									ship.life = 10+ship.balls;
								}
							}
						}
						block_delete(i,nx,by);
						hit_score_make(i,nx,by,10);
						break;
					}
				}
				tama[i].position.vector[_X] = fbx;
			}
		
	    }
		for(k=_X;k<=_Y;k++){
			if(tama[i].speed.vector[k]>stage.vmax[ship.gamemode]){
				tama[i].speed.vector[k]=stage.vmax[ship.gamemode];
			}
			else if(tama[i].speed.vector[k]<-stage.vmax[ship.gamemode]){
				tama[i].speed.vector[k]=-stage.vmax[ship.gamemode];
			}
		}
		for(k=_X;k<=_Y;k++){
			if(tama[i].speed.vector[k]>stage.vmax[ship.gamemode]){
				tama[i].speed.vector[k]=stage.vmax[ship.gamemode];
			}
			else if(tama[i].speed.vector[k]<-stage.vmax[ship.gamemode]){
				tama[i].speed.vector[k]=-stage.vmax[ship.gamemode];
			}
		}
	}
}

/* �d�͏��� */
void gravity()
{
	GLint i,j;
	GLfloat g_const=0.1;
	GLfloat rr,g,dx,dy,gx,gy;
	for(i=0;i<10;i++){
		tama[i].addspeed.vector[_X] = 0;
		tama[i].addspeed.vector[_Y] = 0;
	}
	for(i=0;i<9;i++){
		for(j=i+1;j<10;j++){
			if((i != j) && (tama[i].tamaflag == 1) && (tama[j].tamaflag == 1)){
				dx = tama[i].position.vector[_X] - tama[j].position.vector[_X];
				dy = tama[i].position.vector[_Y] - tama[j].position.vector[_Y];
				rr = dx*dx + dy*dy;
				if(rr>0.5){
					g = g_const/(rr+1.0);
					gx = g*((dx<0)-(dx>0));
					gy = g*((dy<0)-(dy>0));
					tama[i].addspeed.vector[_X] += gx;
					tama[i].addspeed.vector[_Y] += gy;
					tama[j].addspeed.vector[_X] += -gx;
					tama[j].addspeed.vector[_Y] += -gy;
				}
			}
		}
	}
}

/* ���x�x�N�g���̐i�s�����������v�Z */
void pass_speed(GLfloat vp[],GLfloat dx, GLfloat dy, GLfloat r, GLint index)
{
	GLfloat vx, vy, si, co, vbase;
	si = dy / r;
	co = dx / r;
	vx = tama[index].speed.vector[_X];
	vy = tama[index].speed.vector[_Y];
	vbase = vx*co + vy*si;
	vp[_X] = co * vbase;
	vp[_Y] = si * vbase;
}

/* ���x�x�N�g���̖@�������������v�Z */
void leave_speed(GLfloat vl[],GLfloat dx, GLfloat dy, GLfloat r, GLint index)
{
	GLfloat vx, vy, si, co, vbase;
	si = dy / r;
	co = dx / r;
	vx = tama[index].speed.vector[_X];
	vy = tama[index].speed.vector[_Y];
	vbase = vx*si - vy*co;
	vl[_X] = si * vbase;
	vl[_Y] = -co * vbase;
}

/* �{�[�����m�̏Փ˔���Ɣ��ˏ��� */
void ballhit(void)
{
	GLfloat r,rr,dx,dy, vp1[2],vp2[2],vl1[2],vl2[2],co,si;
	GLint i,j,k;
	static GLint hitflag[10][10];

	for(i=0;i<9;i++){
		for(j=i+1;j<10;j++){
			dx = tama[i].position.vector[_X] - tama[j].position.vector[_X];
			dy = tama[i].position.vector[_Y] - tama[j].position.vector[_Y];
			rr = dx*dx+dy*dy;
			if(rr<=1.0 && rr!=0 && (tama[i].hitflag[i][j])==0 && tama[i].tamaflag==1 && tama[j].tamaflag==1){
				init_movetext(collision,i);
				init_movetext(collision,j);
				collision[i].hitcount += 5;
				collision[j].hitcount += 5;

				tama[i].point = (collision[i].hitcount + collision[j].hitcount)*(i+1);
				ship.score += tama[i].point;

				tama[i].hitflag[i][j] = 1;
				
				r = sqrt(rr);
				pass_speed(vp1, dx, dy, r, i);
				pass_speed(vp2, dx, dy, r, j);

				leave_speed(vl1, dx, dy, r, i);
				leave_speed(vl2, dx, dy, r, j);
				
				tama[i].speed.vector[_X] = vl1[_X] + vp2[_X];
				tama[i].speed.vector[_Y] = vl1[_Y] + vp2[_Y];
				/*
				tama[i].position.vector[_X] += tama[i].speed.vector[_X];
				tama[i].position.vector[_Y] += tama[i].speed.vector[_Y];
				*/
				tama[j].speed.vector[_X] = vl2[_X] + vp1[_X];
				tama[j].speed.vector[_Y] = vl2[_Y] + vp1[_Y];
				/*
				tama[j].position.vector[_X] += tama[j].speed.vector[_X];
				tama[j].position.vector[_Y] += tama[j].speed.vector[_Y];
				*/
				for(k=_X;k<=_Y;k++){
					if(tama[i].speed.vector[k]>stage.vmax[ship.gamemode]){
						tama[i].speed.vector[k]=stage.vmax[ship.gamemode];
					}
					else if(tama[i].speed.vector[k]<-stage.vmax[ship.gamemode]){
						tama[i].speed.vector[k]=-stage.vmax[ship.gamemode];
					}
				}
			}
			else if(rr>1.0){
				tama[i].hitflag[i][j] = 0;
			}
		}
	}
}

/* �{�[���̈ړ����� */
void ballmove(void)
{
	GLint i,flag=0;
	GLfloat x,y,z,dx,nx,ny,bx,by,v;
	if(ship.life>0){
		ballhit();
		gravity();
	}
	
	for(i=0;i<10;i++){
		if(tama[i].tamaflag == 1){
			/* �{�[���̑��x�ω��i���Ԍo�߂ɂ������E�����j */
			tama[i].time ++;
			if(tama[i].time==500){
				tama[i].speed.vector[_X]/=2.0;
				tama[i].speed.vector[_Y]*=2.0;
			}
			else if(tama[i].time==1000){
				tama[i].speed.vector[_X]*=2.0;
				tama[i].speed.vector[_Y]/=2.0;
				tama[i].time=0;
			}				
			x = tama[i].position.vector[_X];
			y = tama[i].position.vector[_Y];
			bx = x;
			by = y;
			if(stage.gravity_flag != 0){
				tama[i].speed.vector[_X] += tama[i].addspeed.vector[_X] * stage.gravity_flag;
				tama[i].speed.vector[_Y] += tama[i].addspeed.vector[_Y] * stage.gravity_flag;
			}
			if(ship.life <= -10 && ship.gamemode == 0){
				tama[i].speed.vector[_Y] -= 0.02;
			}
			else if(ship.life > 0 && ship.gamemode == 0){
				tama[i].speed.vector[_Y] -= 0.02;
			}

			/* X�����̕ǔ��� */
			x += tama[i].speed.vector[_X];
			if(x<0.5 || x>19.5){
				tama[i].speed.vector[_X] *= -1;
				x = tama[i].position.vector[_X];
			}
			else{
				tama[i].position.vector[_X] = x;
			}
			
			/* Y�����̕ǔ���E�p�h������ */
			y += tama[i].speed.vector[_Y];
			dx = x-ship.position.vector[_X];
			if(y>14){
				tama[i].speed.vector[_Y] *= -1;
				y = tama[i].position.vector[_Y];
			}
			/* �p�h���ɓ��������ꍇ�̏����i�m�[�}�����[�h�j */
			else if(ship.gamemode==0 && ship.life>0  && abs(dx)<ship.hitwidth/1.7 && y<ship.position.vector[_Y]+1.0 && y>ship.position.vector[_Y]-2.0){
				tama[i].speed.vector[_Y] = 1.0;
				tama[i].speed.vector[_X] = 2.0 * dx/ship.hitwidth;
				tama[i].position.vector[_Y] = ship.position.vector[_Y]+1.0;
				tama[i].point = tama[i].hitcount*tama[i].hitcount*(10+i);
				ship.score += tama[i].point;
				hit[i].tamaflag = 2;
				/* tama[i].hitcount = 0; */
				collision[i].tamaflag = 0;
				collision[i].hitcount = 0;
			}
			/* �p�h���ɓ��������ꍇ�̏����i�e�������[�h�j */
			else if(ship.gamemode==1 && ship.life>0 && y<stage.bottom-1.0){
				tama[i].speed.vector[_Y] = 0.5;
				tama[i].position.vector[_Y] = stage.bottom-1.0;
				tama[i].point = tama[i].hitcount*(tama[i].hitcount+10)*(i+1);
				ship.score += tama[i].point;
				hit[i].tamaflag = 2;
				/* tama[i].hitcount = 0; */
				collision[i].tamaflag = 0;
				collision[i].hitcount = 0;
			}
			/* �p�h���ɒ��������ꍇ�i�e�������[�h�j */
			else if(ship.gamemode==1 && ship.life>0  && abs(dx)<0.5 && y<ship.position.vector[_Y]+0.25 && y>ship.position.vector[_Y]-0.25){
				ship.life=0;
			}
			/* �{�[�������ɗ������ꍇ�̏��� */
			else if(y<-17){
				tama[i].tamaflag = 0;
				ship.life--;
				tama[i].hitcount = 0;
				hit[i].tamaflag = 0;
				collision[i].tamaflag = 0;
				collision[i].hitcount = 0;
			}
			else{
				tama[i].position.vector[_Y] = y;
			}
			/* �u���b�N�Ƃ̓����蔻�� */
			block_hit(x,y,bx,by,i);
		}
	}
}
/* �����G�t�F�N�g�̏����� */
void initexplotion(void)
{
	GLint i;
	for(i=0;i<10;i++){
		tama[i].speed.vector[_X] = cos(i*0.62)*(sin(i*1.5)+0.5);
		tama[i].speed.vector[_Y] = sin(i*0.62)*(cos(i*1.5)+0.5)+0.4;
		tama[i].position.vector[_X] = ship.position.vector[_X];
		tama[i].position.vector[_Y] = ship.position.vector[_Y];
		tama[i].material.diffuse_rgba[0] = 1.0;
		tama[i].material.diffuse_rgba[1] = 0.3;
		tama[i].material.diffuse_rgba[2] = 0.0;
		tama[i].material.diffuse_rgba[3] = 1.0;	
		tama[i].tamaflag = 1;
	}
}

/* �A�C�h�����̏����i�}�E�X�ʒu�ɂ�鎩�@�ړ��Ȃǁj */
void  CALLBACK  idle_process(void)
{
	GLint x,y;
	GLfloat fx,fy;
	fx = (GLfloat)x;
	auxGetMouseLoc(&x,&y);
	fx = (GLfloat)x/40.0;
	fy = (GLfloat)(y-200.0)/40.0;    
	ship.position.vector[_X] = fx;
	if(ship.gamemode) ship.position.vector[_Y] = -fy;

	if(ship.position.vector[_X] < ship.hitwidth/2.0 - 0.5){
		ship.position.vector[_X] = ship.hitwidth/2.0 - 0.5;
	}
	else if(ship.position.vector[_X] > 20.5 - ship.hitwidth/2.0){
		ship.position.vector[_X] = 20.5 - ship.hitwidth/2.0;
	}
	if(ship.position.vector[_Y] < stage.bottom-0.5){
		ship.position.vector[_Y] = stage.bottom-0.5;
	}
	else if(ship.position.vector[_Y] >-1.0){
		ship.position.vector[_Y] = -1.0;
	}

	
	if(stage.blocks && stage.demotime==0){
	
		ballmove();
		if(ship.life<=0 && ship.life > -10){
			ship.material.diffuse_rgba[0] = -(GLfloat)ship.life;
			ship.life --;
			if(ship.life == -10){
				initexplotion();
				ship.deadcount++;
				if(ship.newrecoad){
					savehighscore();
				}
			}
		}
		if(ship.life == -20 || ship.life<=-10 && ship.gamemode==1){
			ship.score = 0;
			ship.newrecoad = 0;
			Initcharacter();
			Initstage();
			light_setting();
		}
		

		
		if(ship.newrecoad){
			ship.highscore = ship.score;
		}
		else if(ship.score>ship.highscore && ship.newrecoad==0){
			ship.newrecoad=1;
			ship.highscore = ship.score;
		}
	}	
	else if(stage.blocks>0 && stage.demotime>0){
		glMatrixMode(GL_PROJECTION); glLoadIdentity();
		gluPerspective(120,1.0,1.0,(GLfloat)(stage.demotime)+50.0);
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
		gluLookAt(9.5,-5.0,10.0+(GLfloat)(stage.demotime), 9.5,0.0,0.0, 0.0,1.0,0.0);
		stage.demotime -= (1 + mouse.Down[_LEFT]*3);
		if(stage.demotime<=0){
			stage.demotime = 0;
			glMatrixMode(GL_PROJECTION); glLoadIdentity();
			gluPerspective(120,1.0,1.0,40.0);
			glMatrixMode(GL_MODELVIEW); glLoadIdentity();
			gluLookAt(9.5,-5.0,10.0, 9.5,0.0,0.0, 0.0,1.0,0.0);
		}
	}
	else if(stage.blocks==0 && stage.demotime<200){
		glMatrixMode(GL_PROJECTION); glLoadIdentity();
		gluPerspective(120,1.0,1.0,(GLfloat)(stage.demotime)+50.0);
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
		gluLookAt(9.5,-5.0,10.0+(GLfloat)(stage.demotime), 9.5,0.0,0.0, 0.0,1.0,0.0);
		stage.demotime += (1 + mouse.Down[_LEFT]*3);
		if(stage.demotime>=200){
			stage.demotime = 200;
			stage.now++;
			Initcharacter();
			Initstage();
		}
	}

	display();
}
/* ���C���֐� */
void main(void)
{
	printf("Select Game Mode\n");
	printf("0.Normal Game\n");
	printf("1.Tameyoke Game\n");
	do{
		scanf("%d",&ship.gamemode);
	}while(ship.gamemode>1 || ship.gamemode<0);

	ship.score = 0;
	stage.now = 0;
	stage.display = 0;
	ship.deadcount = 0;
	window();
	Initcharacter();
	Initfile();
	ship.hitwidth = ship.hitwidth0[ship.gamemode];
	loadhighscore();

	Initstage();
	light_setting();

	glColor4f(1.0,1.0,1.0,1.0);

	stage.display = 1;

	MakeBitMapFont();	
	auxReshapeFunc(Reshape);	
	/* display(); */

	auxMouseFunc(AUX_LEFTBUTTON,AUX_MOUSEDOWN,mouse_left_down);
	auxMouseFunc(AUX_LEFTBUTTON,AUX_MOUSEUP,mouse_left_up);
	auxIdleFunc(idle_process);
	auxMainLoop(display);
}
