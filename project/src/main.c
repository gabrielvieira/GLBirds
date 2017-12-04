#include "globals.h"
#include "gl.h"
#include <math.h> 


#define PI 3.14159265
// Not really much to say about the stuff in here. Hopefully it's self explanatory.

Globals globals;

static float getDeltaTime()
{
	static int t1 = -1;

	if (t1 == -1)
		t1 = glutGet(GLUT_ELAPSED_TIME);
	int t2 = glutGet(GLUT_ELAPSED_TIME);
	float dt = (t2 - t1) / 1000.0f;
	t1 = t2;
	return dt;
}

static void updateKeyInt(int key, bool state)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		globals.playerControls.up = state;
		break;

	case GLUT_KEY_DOWN:
		globals.playerControls.down = state;
		break;

	case GLUT_KEY_LEFT:
		globals.playerControls.left = state;
		break;

	case GLUT_KEY_RIGHT:
		globals.playerControls.right = state;
		break;

	default:
		break;
	}
}

static void updateKeyChar(unsigned char key, bool state)
{
	switch (key)
	{
	case 'w':
		globals.playerControls.upSpeed = state;
		break;

	case 's':
		globals.playerControls.downSpeed = state;
		break;

	case 'a':
		globals.playerControls.upRot = state;
		break;

	case 'd':
		globals.playerControls.downRot = state;
		break;

	default:
		break;
	}
}

float getAngle2 (float x1,float y1,float z1,float x2,float y2,float z2)
{
    float dist=sqrt(pow(x1 - x2,2) + pow(y1 - y2,2) + pow(z1 - z2,2));
    float dist2=sqrt(pow(x1 - x2,2) + pow(z1 - z2,2));
    return acos(dist2/dist)*180/3.1415926;
}

void displayOSD()
{
  char buffer[30];
  char *bufp;
  int w, h;
    
  glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  /* Set up orthographic coordinate system to match the 
     window, i.e. (0,0)-(w,h) */
  w = glutGet(GLUT_WINDOW_WIDTH);
  h = glutGet(GLUT_WINDOW_HEIGHT);
  glOrtho(0.0, w, 0.0, h, -1.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
		
		  /* Frame rate */
  glColor3f(0.0, 0.0, 0.0);
  glRasterPos2i(10, 120);
  if (globals.collision) {
	snprintf(buffer, sizeof buffer, "Colisao: True");
  }else {
	snprintf(buffer, sizeof buffer, "Colisao: False");
  }
    for (bufp = buffer; *bufp; bufp++)
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24 , *bufp);


  static float scale = 0.5f;
	vec3f dir = mulVec3f(globals.player.forward, globals.player.jumpSpeed);
	if (globals.player.vel.x != 0 || globals.player.vel.y != 0)
		dir = globals.player.vel;

   
	float x1 = globals.player.pos.x;
	float x2 = globals.player.pos.x + dir.x * scale;

	float y1 = globals.player.pos.y;
	float y2 = globals.player.pos.y + dir.y * scale;

	float z1 = globals.player.pos.z;
	float z2 = globals.player.pos.z + dir.z * scale;

  float t = getAngle2 ( x1, y1, z1, x2, y2, z2);

  glColor3f(0.0, 0.0, 0.0);
  glRasterPos2i(10, 100);
snprintf(buffer, sizeof buffer, "Angulo inicial %f", t);
  for (bufp = buffer; *bufp; bufp++)
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24 , *bufp);

	  /* Frame rate */
  glColor3f(0.0, 0.0, 0.0);
  glRasterPos2i(10, 80);
  snprintf(buffer, sizeof buffer, "velocidade inicial %f", fabs(globals.player.jumpSpeed));
  for (bufp = buffer; *bufp; bufp++)
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24 , *bufp);


  /* Frame rate */
  glColor3f(0.0, 0.0, 0.0);
  glRasterPos2i(10, 60);
  snprintf(buffer, sizeof buffer, "velocidadeX %f", fabs(globals.player.vel.x));
  for (bufp = buffer; *bufp; bufp++)
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24 , *bufp);

  /* Time per frame */
  glColor3f(0.0, 0.0, 0.0);
  glRasterPos2i(10, 40);
  snprintf(buffer, sizeof buffer, "velocidadeY %f", globals.player.vel.y);
//   printf()
  for (bufp = buffer; *bufp; bufp++)
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24 , *bufp);


  /* Frame rate */
  glColor3f(0.0, 0.0, 0.0);
  glRasterPos2i(10, 20);
  snprintf(buffer, sizeof buffer, "velocidadeZ %f", fabs(globals.player.vel.z));
  for (bufp = buffer; *bufp; bufp++)
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24 , *bufp);
// printf("test\n");
  /* Pop modelview */
  glPopMatrix();  
  glMatrixMode(GL_PROJECTION);

  /* Pop projection */
  glPopMatrix();  
  glMatrixMode(GL_MODELVIEW);

  /* Pop attributes */
  glPopAttrib();
}


static void render(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	uploadProjection(&globals.camera);
	uploadModelview(&globals.camera);

	renderLight(&globals.light);
	renderTerrain(&globals.terrain, &globals.debugControls);
	
	renderPlayer(&globals.player, &globals.debugControls);
	renderParabola(&globals.parabola, &globals.debugControls);

	renderPig(&globals.pig, &globals.debugControls);

	
	displayOSD();
	// renderRoad(&globals.road, &globals.debugControls);
	// renderRiver(&globals.river, &globals.debugControls);

	glutSwapBuffers();
}

static void collisionDetection(void) {

	if( (fabs(globals.player.pos.x - globals.pig.pos.x) <= 0.99) && (fabs(globals.player.pos.z - globals.pig.pos.z) <= 0.99) ){
		globals.collision = 1;
	}else {
		globals.collision = 0;
	}
}


static void update(void)
{
	float dt = getDeltaTime();

	updatePlayer(&globals.player, &globals.playerControls, dt);

	// updateRoad(&globals.road, dt);
	// updateRiver(&globals.river, dt);
	collisionDetection();

	if (globals.player.pos.y <= 0)
		globals.parabola.pos = globals.player.pos;

	
	glutPostRedisplay();
}

static void reshape(int width, int height)
{
	globals.camera.right = width;
	globals.camera.height = height;

	render();
}

static void mouseMove(int x, int y)
{
	static int prevX, prevY;

	int dx = x - prevX;
	int dy = y - prevY;

	if (globals.cameraControls.rotating)
		updateRotation(&globals.camera, dy, dx);

	if (globals.cameraControls.zooming)
		updateZoom(&globals.camera, dy);

	prevX = x;
	prevY = y;
}

static void mouseDown(int button, int state, int x, int y)
{
	UNUSED(x);
	UNUSED(y);

	if (button == GLUT_LEFT_BUTTON)
		globals.cameraControls.rotating = state == GLUT_DOWN;
	if (button == GLUT_RIGHT_BUTTON)
		globals.cameraControls.zooming = state == GLUT_DOWN;
}

static void keyboardDown(unsigned char key, int x, int y)
{
	UNUSED(x);
	UNUSED(y);

	switch (key)
	{
	case 27:
	case 'q':
		exit(EXIT_SUCCESS);
		break;

	// case 'p':
	// 	globals.debugControls.wireframeFlag = !globals.debugControls.wireframeFlag;
	// 	toggleWireframe(&globals.debugControls);
	// 	break;

	// case 'l':
	// 	globals.debugControls.lightingFlag = !globals.debugControls.lightingFlag;
	// 	toggleLighting(&globals.debugControls);
	// 	break;

	// case 'n':
	// 	globals.debugControls.normalFlag = !globals.debugControls.normalFlag;
	// 	break;

	// case 't':
	// 	globals.debugControls.textureFlag = !globals.debugControls.textureFlag;
	// 	toggleTexturing(&globals.debugControls);
	// 	break;

	// case 'o':
	// 	globals.debugControls.axisFlag = !globals.debugControls.axisFlag;
	// 	break;

	case ' ':
		jump(&globals.player);
		break;

	default:
		// Any keys that you want to handle repeats for (ie, holding the key instead of just pressing) should be handled from updateKeyChar.
		updateKeyChar(key, true);
		break;
	}
}

static void keyboardUp(unsigned char key, int x, int y)
{
	UNUSED(x);
	UNUSED(y);

	updateKeyChar(key, false);
}

static void keyboardSpecialDown(int key, int x, int y)
{
	UNUSED(x);
	UNUSED(y);

	updateKeyInt(key, true);
}

static void keyboardSpecialUp(int key, int x, int y)
{
	UNUSED(x);
	UNUSED(y);

	updateKeyInt(key, false);
}

static void cleanup(void)
{
	freeGlobals(&globals);
}

int main(int argc, char **argv)
{
	initCamera(&globals.camera, 0, 1024, 720);

	glutInit(&argc, argv);
	glutInitWindowSize(globals.camera.right, globals.camera.height);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("Projectile Motion");

	glutDisplayFunc(render);
	glutIdleFunc(update);
	glutReshapeFunc(reshape);
	glutMotionFunc(mouseMove);
	glutPassiveMotionFunc(mouseMove);
	glutMouseFunc(mouseDown);
	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(keyboardSpecialDown);
	glutSpecialUpFunc(keyboardSpecialUp);

	atexit(cleanup);

	initGlobals(&globals);

	glutMainLoop();

	return EXIT_SUCCESS;
}

