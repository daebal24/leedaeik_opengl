#include "glut.h"
#include <stdio.h>
#include <math.h>

#define W_Size_X 800 // ó��ȭ�� ����
#define W_Size_Y 700 // ó��ȭ�� ����
#define MOUSEANGLE 0.004 // ī�޶� ȸ���ϴ�����
#define AXISMOVE 0.01 // ī�޶� WSAD �����̴� ����
#define GROUNDSIZE 20 // �� �� �� ����

GLfloat G_width = 0.0, G_height = 0.0;

//���콺 ����, ������ ��ư üũ
bool rightmouseon = false;
bool leftmouseon = false;
float upordown = 0;

//ī�޶� ����
float angle = 0.0, angle2 = 0.0; // angle�� �¿�, angle2�� ����
float checkingangle = 0.0, checkingangle2 = 0.0;
float eyex = 0, eyey = 1.75, eyez = 5, lx = 0.0f, ly = 0.0f, lz = -1.0f;
float nowmousex = 0, nowmousey = 0;

float fov = 50;
int xmove = 0, ymove = 0, zmove = 0;

//������Ʈ ����
float objx = 0.0, objy = 0.0, objz = 0.0;
bool drawon = false;
GLuint draw_list;
GLuint draw_list_ground;
float objsize = 0.0;
float objrotate = 0.0;
char objectclick = '0';
char objrotateswitch='y';
float rotate_x = 0, rotate_y = 0, rotate_z = 0;
//������Ʈ gllist��
int listcount = 1;


//��������
float ground_y[GROUNDSIZE][GROUNDSIZE] = {0,};
bool firsttime = true;
int ground_y_switch = -1;

//������Ʈ ����
GLfloat Colors[3];
GLfloat c1 = 0.0, c2 = 0.0, c3 = 0.0;
GLfloat* pColor = Colors;
GLfloat GroundColors[GROUNDSIZE][GROUNDSIZE][3] = { 1, };


//ī�޶� �����¿��̵�
void zmovefunc(int i)
{
    eyex = eyex + i * (lx) *AXISMOVE;
    eyey = eyey + i * (ly) *AXISMOVE;
    eyez = eyez + i * (lz) *AXISMOVE;
    glLoadIdentity();
    gluLookAt(eyex, eyey, eyez, eyex + lx, eyey + ly, eyez + lz, 0.0f, 1.0f, 0.0f);
}
void xmovefunc(int i)
{
    eyex = eyex + i * (lz) *AXISMOVE;
    eyez = eyez + i * -(lx) *AXISMOVE;
    glLoadIdentity();
    gluLookAt(eyex, eyey, eyez, eyex + lx, eyey + ly, eyez + lz, 0.0f, 1.0f, 0.0f);
}

//ī�޶� ȸ��
void camrotate_horizon()
{
    lx = sin(angle);
    lz = -cos(angle);
    glLoadIdentity();
    gluLookAt(eyex, eyey, eyez, eyex + lx, eyey + ly, eyez + lz, 0.0f, 1.0f, 0.0f);
}
void camrotate_vertical()
{
    if (angle2 >= 1.57) { angle2 = angle2-3.14; }
    if (angle2 <= -1.57) { angle2 = -angle2+3.14; }

    ly = -sin(angle2);
    glLoadIdentity();
    gluLookAt(eyex, eyey, eyez, eyex + lx, eyey + ly, eyez + lz, 0.0f, 1.0f, 0.0f);
}

//�и��� �ڵ�, ������Ʈ ����
void object_to_draw()
{
    glPushMatrix();

    glRotatef(rotate_x, 1, 0, 0);
    glRotatef(rotate_y, 0, 1, 0);
    glRotatef(rotate_z, 0, 0, 1);

    switch (objectclick)
    {
    case '1': glutWireTeapot(0.1+objsize*0.01); break;
    case '2': glutSolidCube(0.1+objsize * 0.01); break;
    case '3': glutWireSphere(0.1+objsize * 0.01, 100, 100); break;
    }
    glPopMatrix();
}

//������Ʈ �׸���
void draw()
{
    listcount++;
    GLuint drawDL;
    drawDL = glGenLists(listcount);
    glNewList(drawDL, GL_COMPILE);

    glCallList(draw_list);
    glPushMatrix();
    glTranslatef(eyex + lx, eyey + ly, eyez + lz);
    glColor3fv(pColor);
    object_to_draw();
    glPopMatrix();

    glEndList();
    draw_list = drawDL;
}


// �� �� ���� ���ݾ� �ٸ���
void groundcolorchange(int n, int j, int sign)
{
    //����̸� ��������� �ú��̸� �ݴ��
    if (GroundColors[n - 1][j - 1][0] == 0 && GroundColors[n - 1][j - 1][1] == 0 && GroundColors[n - 1][j - 1][2] == 0) sign *= -1;
    GroundColors[n - 1][j - 1][0] += sign * 0.1;
    GroundColors[n - 1][j - 1][1] += sign * 0.1;
    GroundColors[n - 1][j - 1][2] += sign * 0.1;

    glColor3f(GroundColors[n - 1][j - 1][0], GroundColors[n - 1][j - 1][1], GroundColors[n - 1][j - 1][2]);
}

//���������� ���� �׸���
void background()
{
    //�ٴ�
    for (int n = 1; n < GROUNDSIZE; n++)
    {
        for (int j = 1; j < GROUNDSIZE; j++)
        {

            if (ground_y[n - 1][j - 1] != 0 && n != 1 && n != GROUNDSIZE && j != 1 && j != GROUNDSIZE)
            {
               
                glPushAttrib(GL_ALL_ATTRIB_BITS);
                glPushMatrix();
                glColor3f(GroundColors[n - 1][j - 1][0], GroundColors[n - 1][j - 1][1], GroundColors[n - 1][j - 1][2]);
                glBegin(GL_TRIANGLE_STRIP);
                glVertex3f((n) * 0.1f, ground_y[n - 1][j - 1], (j) * 0.1f);
                glVertex3f((n - 1) * 0.1f, ground_y[n - 2][j - 1], (j) * 0.1f);
                glVertex3f((n - 1) * 0.1f, ground_y[n - 2][j - 2], (j - 1) * 0.1f);
                glEnd();

                glBegin(GL_TRIANGLE_STRIP);
                glVertex3f((n) * 0.1f, ground_y[n - 1][j - 1], (j) * 0.1f);
                glVertex3f((n - 1) * 0.1f, ground_y[n - 2][j - 1], (j + 0) * 0.1f);
                glVertex3f((n - 1) * 0.1f, ground_y[n - 2][j - 0], (j + 1) * 0.1f);
                glEnd();

                groundcolorchange(n, j, -1);
                glBegin(GL_TRIANGLE_STRIP);
                glVertex3f((n) * 0.1f, ground_y[n - 1][j - 1], (j) * 0.1f);
                glVertex3f((n + 1) * 0.1f, ground_y[n - 0][j - 0], (j + 1) * 0.1f);
                glVertex3f((n - 0) * 0.1f, ground_y[n - 1][j - 0], (j + 1) * 0.1f);            
                glEnd();

                glBegin(GL_TRIANGLE_STRIP);
                glVertex3f((n) * 0.1f, ground_y[n - 1][j - 1], (j) * 0.1f);
                glVertex3f((n - 1) * 0.1f, ground_y[n - 2][j - 0], (j + 1) * 0.1f);
                glVertex3f((n - 0) * 0.1f, ground_y[n - 1][j - 0], (j + 1) * 0.1f);
                glEnd();

                groundcolorchange(n, j, 1);
                glBegin(GL_TRIANGLE_STRIP);
                glVertex3f((n) * 0.1f, ground_y[n - 1][j - 1], (j) * 0.1f);
                glVertex3f((n + 1) * 0.1f, ground_y[n - 0][j - 0], (j + 1) * 0.1f);
                glVertex3f((n + 1) * 0.1f, ground_y[n - 0][j - 1], (j - 0) * 0.1f);                 
                glEnd();

                glBegin(GL_TRIANGLE_STRIP);//
                glVertex3f((n) * 0.1f, ground_y[n - 1][j - 1], (j) * 0.1f);
                glVertex3f((n + 1) * 0.1f, ground_y[n][j - 1], (j) * 0.1f);
                glVertex3f((n + 1) * 0.1f, ground_y[n - 0][j - 2], (j - 1) * 0.1f);
                glEnd();

                groundcolorchange(n, j, -1);
                glBegin(GL_TRIANGLE_STRIP);
                glVertex3f((n) * 0.1f, ground_y[n - 1][j - 1], (j) * 0.1f);
                glVertex3f((n + 1) * 0.1f, ground_y[n - 0][j - 2], (j - 1) * 0.1f);
                glVertex3f((n + 0) * 0.1f, ground_y[n - 1][j - 2], (j - 1) * 0.1f);
                glEnd();

                glBegin(GL_TRIANGLE_STRIP);
                glVertex3f((n) * 0.1f, ground_y[n - 1][j - 1], (j) * 0.1f);
                glVertex3f((n + 0) * 0.1f, ground_y[n - 1][j - 2], (j - 1) * 0.1f);
                glVertex3f((n - 1) * 0.1f, ground_y[n - 2][j - 2], (j - 1) * 0.1f);
                glEnd();

                glPopMatrix();
                glPopAttrib();
                groundcolorchange(n, j, 1);
            }
            else//���� ���̰� 0�̸�
            {
            glPushMatrix();
            glColor3f(GroundColors[n][j][0], GroundColors[n][j][1], GroundColors[n][j][2]);
            glBegin(GL_QUADS);
            glVertex3f(n * 0.1f, 0, j * 0.1f);
            glVertex3f(n * 0.1f, 0, (j + 1) * 0.1f);
            glVertex3f((n + 1) * 0.1f, 0, (j + 1) * 0.1f);
            glVertex3f((n + 1) * 0.1f, 0, j * 0.1f);
            glEnd();


            //���ڼ�
            glColor3f(0.5f, 0.5f, 0.5f);
            glBegin(GL_LINE_STRIP);
            glVertex3f(n * 0.1f, 0, j * 0.1f);
            glVertex3f(n * 0.1f, 0, (j + 1) * 0.1f);
            glVertex3f((n + 1) * 0.1f, 0, (j + 1) * 0.1f);
            glVertex3f((n + 1) * 0.1f, 0, j * 0.1f);
            glEnd();

            }
        }
    }

    glPopMatrix();

    //���� ������Ʈ
    glPushMatrix();
    glTranslatef(0, 1.7, 0);
    glColor3f(0.8, 0.8, 0.4);
    glutWireSphere(0.2, 100, 100);
    //glutWireTeapot(0.3);
    glPopMatrix();


}

//��ũ��
void MyDisplay()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, G_width*0.75, G_height);
    gluPerspective(fov, 1.0f * G_width / G_height, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyex, eyey, eyez, eyex + lx, eyey + ly, eyez + lz, 0.0f, 1.0f, 0.0f);


    //ī�޶� �̵� �� ȸ��
    if (zmove != 0)
    {
        zmovefunc(zmove);
    }
    if (xmove != 0)
    {
        xmovefunc(xmove);
    }
    if (checkingangle != 0)
    {
        angle += checkingangle;
        camrotate_horizon();
    }
    if (checkingangle2 != 0)
    {
        angle2 += checkingangle2;
        camrotate_vertical();
    }

    //���� ��ũ��
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.8, 0.8, 1.0, 1.0); // ����
    glCallList(draw_list_ground);
    glCallList(draw_list);
    
        //���� �ü���ġ
        glPushMatrix();
        glColor3fv(pColor);
        glTranslatef(eyex + lx, eyey + ly, eyez + lz);

        object_to_draw();
        glColor3f(0.4, 0.4, 0.4); glutWireSphere(0.005, 100, 100);// ���콺������
                
        glPopMatrix();


    //���⼭ ���� �޴� ��ũ�� ��
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(G_width * 0.75, 0, G_width * 0.25, G_height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 0, 0, 0, -1.0f, 0.0f, 1.0f, 0.0f);


    //�޴���ũ��-������Ʈ1
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(-0.7,0.9,0);
    glBegin(GL_QUADS);
    glVertex3f(-0.2, -0.07, 0);
    glVertex3f(-0.2, 0.07, 0);
    glVertex3f(0.2, 0.07, 0);
    glVertex3f(0.2, -0.07, 0);
    glEnd();
    glColor3fv(pColor);
    glutWireTeapot(0.08);
    glPopMatrix();
    if ((nowmousex >= (612.0 / 800.0) * G_width && nowmousey <= (648.0 / 800.0) * G_width) && (nowmousey >= (12.0 / 700.0) * G_height) && (nowmousey <= (56.0 / 700.0) * G_height) && leftmouseon)
    {
        objectclick = '1';
        rotate_x = 0, rotate_y = 0, rotate_z = 0; // ���� �ʱ�ȭ
        
    }

    //�޴���ũ��-������Ʈ2
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(-0.2, 0.9, 0);
    glBegin(GL_QUADS);
    glVertex3f(-0.2, -0.07, 0);
    glVertex3f(-0.2, 0.07, 0);
    glVertex3f(0.2, 0.07, 0);
    glVertex3f(0.2, -0.07, 0);
    glEnd();
    glColor3fv(pColor);
    glutSolidCube(0.1);
    glPopMatrix();
    if ((nowmousex >= 660.0 / 800.0 * G_width && nowmousey <= 697.0 / 800.0 * G_width) && (nowmousey >= 12.0 / 700.0 * G_height && nowmousey <= 56.0 / 700.0 * G_height) && leftmouseon)
    {
        objectclick = '2';
        rotate_x = 0, rotate_y = 0, rotate_z = 0; // ���� �ʱ�ȭ
    }

    //�޴���ũ��-������Ʈ3
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(0.3, 0.9, 0);
    glBegin(GL_QUADS);
    glVertex3f(-0.2, -0.07, 0);
    glVertex3f(-0.2, 0.07, 0);
    glVertex3f(0.2, 0.07, 0);
    glVertex3f(0.2, -0.07, 0);
    glEnd();
    glColor3fv(pColor);
    glutWireSphere(0.05,10,100);
    glPopMatrix();
    if ((nowmousex >= 710.0 / 800.0 * G_width && nowmousey <= 749.0 / 800.0 * G_width) && (nowmousey >= 12.0 / 700.0 * G_height && nowmousey <= 56.0 / 700.0 * G_height) && leftmouseon)
    {
        objectclick = '3';
        rotate_x = 0, rotate_y = 0, rotate_z = 0; // ���� �ʱ�ȭ
    }

    //�޴���ũ�� - �������
    glPushMatrix();
    glTranslatef(-0.7, 0.6, 0);

    glColor3fv(pColor);
    glBegin(GL_QUADS);
    glVertex3f(-0.19, -0.09, 0);
    glVertex3f(-0.19, 0.09, 0);
    glVertex3f(0.49, 0.09, 0);
    glVertex3f(0.49, -0.09, 0);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex3f(-0.2, -0.1, 0);
    glVertex3f(-0.2, 0.1, 0);
    glVertex3f(0.5, 0.1, 0);
    glVertex3f(0.5, -0.1, 0);
    glEnd();
    glPopMatrix();

    //������
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f);
    glTranslatef(0.2, 0.67, 0);
    glBegin(GL_QUADS);
    glVertex3f(-0.2, -0.03, 0);
    glVertex3f(-0.2, 0.03, 0);
    glVertex3f(0.6, 0.03, 0);
    glVertex3f(0.6, -0.03, 0);
    glEnd();
    glPopMatrix();
    if ((nowmousex >= 700.0 /800.0 *G_width && nowmousey <= 780.0 /800.0 * G_width) && (nowmousey >= 105.0 /700.0 *G_height && nowmousey <= 125.0 / 700.0 * G_height) && leftmouseon)
    {
        c1 = (nowmousex - 700.0 / 800.0 * G_width) / (0.1 * G_width);
        //printf("%f\n",c1);
        Colors[0] = c1;
    }

    //�����
    glPushMatrix();
    glColor3f(0.0f, 1.0f, 0.0f);
    glTranslatef(0.2, 0.6, 0);
    glBegin(GL_QUADS);
    glVertex3f(-0.2, -0.03, 0);
    glVertex3f(-0.2, 0.03, 0);
    glVertex3f(0.6, 0.03, 0);
    glVertex3f(0.6, -0.03, 0);
    glEnd();
    glPopMatrix();
    if ((nowmousex >= 700.0 / 800.0 * G_width && nowmousey <= 780.0 / 800.0 * G_width) && (nowmousey >= 130.0 / 700.0 * G_height && nowmousey <= 150.0 / 700.0 * G_height) && leftmouseon)
    {
        c2 = (nowmousex - 700.0 / 800.0 * G_width) / (0.1 * G_width);
        Colors[1] = c2;
    }

    //�Ķ���
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 1.0f);
    glTranslatef(0.2, 0.53, 0);
    glBegin(GL_QUADS);
    glVertex3f(-0.2, -0.03, 0);
    glVertex3f(-0.2, 0.03, 0);
    glVertex3f(0.6, 0.03, 0);
    glVertex3f(0.6, -0.03, 0);
    glEnd();
    glPopMatrix();
    if ((nowmousex >= 700.0 / 800.0 * G_width && nowmousey <= 780.0 / 800.0 * G_width) && (nowmousey >= 155.0 / 700.0 * G_height && nowmousey <= 175.0 / 700.0 * G_height) && leftmouseon)
    {
        c3 = (nowmousex - 700.0 / 800.0 * G_width) / (0.1 * G_width);
        Colors[2] = c3;
    }

    //�޴���ũ��
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS); // ����
    glVertex3f(-1, -1.0f, 0);
    glVertex3f(-1, 1.0f, 0);
    glVertex3f(1, 1.0f, 0);
    glVertex3f(1, -1.0f, 0);
    glEnd();
    glPopMatrix();


    glutSwapBuffers();
}

//ȭ�� ������ ������ �ݿ�
void MyReshape(int width, int height)
{
    G_width = (GLfloat)width;
    G_height = (GLfloat)height;
}

//���� �׸��� �۾�, ù �۾��� E,CŰ �۾� ó��
void groundredraw()
{
    GLuint drawDL;
    drawDL = glGenLists(1);
    objx *= 10; objz *= 10; //

    if (firsttime)
    {
        glNewList(drawDL, GL_COMPILE);
        background();
        glEndList();
        draw_list_ground = drawDL;
        firsttime = false;
    }

    else if (objx >= 0 && objx < GROUNDSIZE && objz >= 0 && objz < GROUNDSIZE)
    {
        
        //��������
        GroundColors[int(objx)][int(objz)][0] = Colors[0];
        GroundColors[int(objx)][int(objz)][1] = Colors[1];
        GroundColors[int(objx)][int(objz)][2] = Colors[2];

        //��������        
        glNewList(drawDL, GL_COMPILE);
        ground_y[int(objx)][int(objz)] += upordown * 0.1;
        background();

        glEndList();
        draw_list_ground = drawDL;
        //printf("drawn\n");
    }
    else
    {
        glDeleteLists(drawDL, 1); objx /= 10; objz /= 10;
    }
}

//���� �׸��� �۾�, ù �۾��� R,VŰ �۾� ó��(���, ����ȭ)
void groundreddraw_high_ground()
{
    GLuint drawDL;
    drawDL = glGenLists(1);
    objx *= 10; objz *= 10;

    //������ ����
    int max_n = 3, max_j = 3;
    if (objx > GROUNDSIZE-3) { max_n = GROUNDSIZE - objx; }
    if (objz > GROUNDSIZE-3) { max_j = GROUNDSIZE - objz; }

    if (objx >= 0 && objx < GROUNDSIZE && objz >= 0 && objz < GROUNDSIZE)
    {
        //��������
        for(int n = 0; n< max_n;n++)
        for(int j = 0; j< max_j;j++)
        {
        GroundColors[int(objx)+n][int(objz)+j][0] = Colors[0];
        GroundColors[int(objx)+n][int(objz)+j][1] = Colors[1];
        GroundColors[int(objx)+n][int(objz)+j][2] = Colors[2];
        }
        //��������
        glNewList(drawDL, GL_COMPILE);
        for(int n = 0; n< max_n;n++)
            for (int j = 0; j < max_j; j++)
            {
                ground_y[int(objx) + n][int(objz) + j] = ground_y_switch*0.1;
            }
        background();

        glEndList();
        draw_list_ground = drawDL;
        //printf("drawn\n");
    }
    else
    glDeleteLists(drawDL, 1);

}

//Ű���� ����Ű - ������Ʈ ������
void MySpecialKey(int Key, int X, int Y) //Ű�������Ű
{
    switch (Key)
    {
    case GLUT_KEY_UP: ++objsize; break;
    case GLUT_KEY_DOWN: --objsize; break;
    case GLUT_KEY_LEFT: objrotate = -1; break;
    case GLUT_KEY_RIGHT: objrotate = 1; break;
    }
    if (objrotate == 360) objrotate = 0;

    switch (objrotateswitch)
    {
    case 'x': rotate_x += objrotate; break;
    case 'y': rotate_y += objrotate; break;
    case 'z': rotate_z += objrotate; break;
    }
    objrotate = 0;
    //object_to_draw���� ó��
}

//���� Ű
void MyKeyboard(unsigned char Key, int X, int Y)
{
    //ȭ������ wsad�� ī�޶� �̵�
    if (rightmouseon)
    {
        switch (Key)
        {
        case 'w': zmove = 1; break;
        case 's': zmove = -1; break;
        case 'd': xmove = -1; break;
        case 'a': xmove = 1; break;
        case 'e': upordown = 1; ground_y_switch = 1; break;
        case 'c': upordown = -1; ground_y_switch = 1; break;
        case 'r': ground_y_switch = 5; break;
        case 'v': ground_y_switch = 0; break;
        }

    }
    switch (Key)
    {
    case 'Q': case 'q': case 27: //'esc' Ű�� �ƽ�Ű �ڵ� ��
        exit(0); break;
    //������������ ������Ʈ ���� ���ϰ�
    case 'e': 
    case 'c': 
    case 'r': 
    case 'v': objectclick = 0; break;
    case ',': objrotateswitch = 'x'; break;
    case '.': objrotateswitch = 'y'; break;
    case '/': objrotateswitch = 'z'; break;
    }
}
//Ű���忡�� �� ���� ��
void Release_MyKeyboard(unsigned char Key, int X, int Y)
{
    //ȭ������ wsad�� ī�޶� �̵�
    switch (Key)
    {
    case 'w':
    case 's': zmove = 0; break;
    case 'd':
    case 'a': xmove = 0; break;
    case 'e': 
    case 'c': upordown = 0; break;
    case 'r': 
    case 'v': ground_y_switch = -1; break;
    }
}

//���콺 Ŭ��
void MyMouseClick(GLint Button, GLint State, GLint X, GLint Y)
{

    //���콺 ������ ��ư
    if (Button == GLUT_RIGHT_BUTTON && State == GLUT_DOWN)
    {
        rightmouseon = true;

        nowmousex = X;
        nowmousey = Y;

    }
    if (Button == GLUT_RIGHT_BUTTON && State == GLUT_UP)
    {
        rightmouseon = false;
        checkingangle = 0; checkingangle2 = 0;
    }
                                                                                                                                            
    //���콺 ���� ��ư
    if (Button == GLUT_LEFT_BUTTON && State == GLUT_DOWN)
    {
        nowmousex = X;
        nowmousey = Y;
        leftmouseon = true;

        if (X / G_width <= 0.75) // ���� ��ũ��������
        {

        //objx = X, objy = Y, objz = eyez + ly; // ȭ�� ����� ���� ���콺�� ����Ű�� ��ǥ���� �ٲ�
        //eyex, eyey, eyez�� �������� �ϰ� z���� +1. eye�������� z 1ĭ �����̴� ���̸� zmove(1)�� ������ //((Y - (0.5 * G_height)) / (0.5 * G_height))
            
            objy = 0; //-eyey/ly ��ŭ �̵�
            objx = eyex + (-eyey / ly) * (lx);
            objz = eyez + (-eyey / ly) * (lz);
            

            //x�� �̵�. x���� �ݴ�� ������. x�� ��ǥ ���ϱ��  x-(width/2) / (width/2)
            //objx = objx + -(X / G_width - 0.5) * (lz);
            //objz = objz + -(X / G_width - 0.5) * -(lx); 
           
            if (objectclick != 0)
            draw();
            if(ground_y_switch == 1)
            groundredraw();
            if(ground_y_switch == 5 || ground_y_switch == 0)
            groundreddraw_high_ground();

        }

    }
    if (Button == GLUT_LEFT_BUTTON && State == GLUT_UP)
    {
        leftmouseon = false;
    }

}
//���콺�� �巡�� �Ǵ� ����
void MyMouseMove(GLint X, GLint Y) // �巡���ϸ� �ߵ�
{
    if (X / G_width <= 0.75) // ���� ��ũ��������
    {
        if (rightmouseon) // ������ ��ư �巡�׸�
        {

            //���콺 ������ ��ư �������·�
            //�񱳰��� ���콺 �ΰ���

            //ī�޶� �¿� ȸ��
            if ((X - nowmousex) > 20)
                checkingangle = MOUSEANGLE;
            else if ((X - nowmousex) < -20)
                checkingangle = -MOUSEANGLE;

            //ī�޶� ���� ȸ��
            if ((Y - nowmousey) > 20)
                checkingangle2 = MOUSEANGLE;
            else if ((Y - nowmousey) < -20)
                checkingangle2 = -MOUSEANGLE;
        }
    }
}

//ó�� �ʱ�ȭ�۾�
void init()
{
    Colors[0] = c1; Colors[1] = c2; Colors[2] = c3;
    for(int i = 0; i< GROUNDSIZE; i++)
        for (int j = 0; j < GROUNDSIZE; j++)
        {
            GroundColors[i][j][0] = 1; 
            GroundColors[i][j][1] = 1; 
            GroundColors[i][j][2] = 1;
        }

    glEnable(GL_DEPTH_TEST);

    groundredraw();


}

//�����Լ�
int main(int argc, char** argv) {
    glutInit(&argc, argv); // ������ �ü���� ���� ����
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);	// ���÷��� ��� ����
    glutInitWindowPosition(120, 55); //������ ��ġ ����
    glutInitWindowSize(W_Size_X, W_Size_Y); //������ ũ�� ����
    glutCreateWindow("OG Project");

    init();

    glutSpecialFunc(MySpecialKey);
    glutKeyboardFunc(MyKeyboard);
    glutKeyboardUpFunc(Release_MyKeyboard);
    glutMouseFunc(MyMouseClick);
    glutMotionFunc(MyMouseMove);

    glutDisplayFunc(MyDisplay);
    glutIdleFunc(MyDisplay);

    glutReshapeFunc(MyReshape);

    glutMainLoop();
    return 0;
}