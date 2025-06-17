static GLint display_num;

GLubyte fontdata[95][12]; /* 白巧件玄及犯□正傘Ф迕т��E*/

void load_bitmapdata(void)
{
	int i,j;
	FILE *fp;
	unsigned int data[95][12];
	if((fp = fopen("./font/bitmap_font.dat", "r")) == 0){
		printf("Can't open file");
		exit(1);
	}
	for(i=0; i<95; i++){
		for(j=0; j<12; j++){
			fscanf(fp, "%x",&data[i][j]);
			fontdata[i][j] = (GLubyte)data[i][j];
		}
	}
	fclose(fp);
}

void MakeBitMapFont(void)
{
	int i;
	load_bitmapdata();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	display_num = glGenLists (128);
	for(i=32; i<127; i++){
		glNewList(display_num+i, GL_COMPILE);
		   glBitmap(8, 12, 0.0, 1.0, 10.0, 0.0, fontdata[i-32]);
		glEndList();
	}
}

void DrawString(char *str)
{
	int len;
	len = strlen(str);
	glListBase(display_num);
	glCallLists(len, GL_BYTE, str);
}

void DrawChar(char c)
{
	glListBase(display_num);
	glCallLists(1,GL_BYTE,&c);
}
