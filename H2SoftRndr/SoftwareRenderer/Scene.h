#ifndef SCENE_H
#define SCENE_H


#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include "Matrices.h" 
#include "Triangle.h"
#include "mouse.h"

/* creates a linked list of triangles, which will define our scene */
typedef struct TriangleList_t {
	Triangle *t;
	struct TriangleList_t *next;
} TriangleList;


/* implements the scene class */
class Scene {
	private:
		/* the scene class is basically a linked list of triangles */
		TriangleList *original_head;
		TriangleList *original_tail;


		/* Clipped Triangle values */
		TriangleList *clipped_head;
		TriangleList *clipped_tail;


		/* Test Triangle List */
		TriangleList *test_tr_head;
		TriangleList *test_tr_tail;


	public:
		Scene() {
			original_head = NULL;		original_tail = NULL;
			clipped_head = NULL; 		clipped_tail = NULL;
			test_tr_head = NULL; 		test_tr_tail = NULL;
		};

		/* destructor */
		~Scene(){
			destroyList(&original_head, &original_tail);
		};

		void destroyList(TriangleList **head_ptr, TriangleList **tail_ptr) {
			TriangleList *ptr, *ptr2;
			
			if (*head_ptr) {
				for (ptr = *head_ptr; ptr;) {
					ptr2 = ptr->next;

					/* delete my triangle */
					delete ptr->t;

					/* free the node */
					free(ptr);	
					ptr = ptr2;
				}
			}

			*head_ptr = NULL;
			*tail_ptr = NULL;

			return;
		};

		TriangleList *newNode(void) {
			TriangleList *node = (TriangleList*)malloc(sizeof(TriangleList));
			node->next = NULL;
			node->t = NULL;

			return node;
		};

		void addTriangle(TriangleList **head_ptr, TriangleList **tail_ptr, Triangle *tri) {
			if ((*head_ptr) == NULL) {
				(*head_ptr) = newNode();
				(*head_ptr)->t = tri;
				(*tail_ptr) = (*head_ptr);
			}
			else {
				(*tail_ptr)->next = newNode();
				(*tail_ptr) = (*tail_ptr)->next;
				(*tail_ptr)->t = tri;
			}

			return;
		};

		
		void addWall(float x1, float y1, float x2, float y2, int wall_height, Texture *tex) {
			Triangle *t1, *t2;
			Vertex v0(x1, y1, 0);
			Vertex v1(x2, y2, 0);
			Vertex v2(x2, y2, wall_height);
			Vertex v3(x1, y1, wall_height);

			t1 = new Triangle(&v0, &v1, &v2);
			t2 = new Triangle(&v2, &v3, &v0);
			
			/* set the texture coordinates for the wall */
			t1->setCoords(0, 0, 0);
			t1->setCoords(1, 1, 0);
			t1->setCoords(2, 1, 1);

			t2->setCoords(0, 1, 1);
			t2->setCoords(1, 0, 1);
			t2->setCoords(2, 0, 0);

			t1->setTexture(tex);
			t2->setTexture(tex);

			/* add these triangles to the list */
			addTriangle(&original_head, &original_tail, t1);
			addTriangle(&original_head, &original_tail, t2);
		};

		void addFloor(float x1, float y1, float x2, float y2, Texture *tex, int scene_width, int scene_height) {
			Triangle *t1, *t2;
			Vertex v0(x1, y2, 0);
			Vertex v1(x2, y2, 0);
			Vertex v2(x2, y1, 0);
			Vertex v3(x1, y1, 0);

			t1 = new Triangle(&v0, &v1, &v2);
			t2 = new Triangle(&v2, &v3, &v0);
			
			/* set the texture coordinates for the wall */
			t1->setCoords(0, 0, 0);
			t1->setCoords(1, scene_width/4.0, 0);
			t1->setCoords(2, scene_width/4.0, scene_height/4.0);

			t2->setCoords(0, scene_width/4.0, scene_height/4.0);
			t2->setCoords(1, 0, scene_height/4.0);
			t2->setCoords(2, 0, 0);

			t1->setTexture(tex);
			t2->setTexture(tex);

			/* add these triangles to the list */
			addTriangle(&original_head, &original_tail, t1);
			addTriangle(&original_head, &original_tail, t2);
		};

		/* Test with static triangles */
		void testRenderSceneOpenGL(void) {

			Texture ** tx = (Texture **)malloc(sizeof(Texture*) * 4);

			tx[0] = new Texture("wall1.tif");
			tx[1] = new Texture("wall4.tif");
			tx[2] = new Texture("wall6.tif");
			tx[3] = new Texture("monaLisa.tif");

			// Lets draw a fixed test triangle 
			Vertex v1_t = { -20.0f,80.0f,0.0f,1.0f };
			Vertex v2_t = { -30.0f,70.0f,0.0f,1.0f };
			Vertex v3_t = { -30.0f,60.0f,30.0f,1.0f };
			
			Triangle test(&v1_t, &v2_t, &v3_t);
			test.setCoords(0, 0, 0);
			test.setCoords(1, 1, 1);
			test.setCoords(2, 0, 1);
			test.setColor(0, 127, 0, 0); // color[0] = (150 * id) % 255; // rand() %
			test.setColor(1, 0, 255, 0);
			test.setColor(2, 127, 0, 0);
			test.setTexture(tx[0]);
			test.renderOpenGL();

			v1_t = { -20.0f,70.0f,20.0f,1.0f };
			v2_t = { -30.0f,60.0f,20.0f,1.0f };
			v3_t = { -30.0f,50.0f,0.0f,1.0f };
			Triangle test0(&v1_t, &v2_t, &v3_t);
			test0.setCoords(0, 1, 1);
			test0.setCoords(1, 0, 1);
			test0.setCoords(2, 0, 0);
			test0.setColor(0, 0, 127, 0);
			test0.setColor(1, 0, 0, 255);
			test0.setColor(2, 0, 127, 0);
			test0.setTexture(tx[1]);
			test0.renderOpenGL();

			v1_t = { -40.0f,80.0f,20.0f,1.0f };
			v2_t = { -70.0f,80.0f,20.0f,1.0f };
			v3_t = { -70.0f,80.0f,0.0f,1.0f };
			Triangle test1(&v1_t, &v2_t, &v3_t);
			test1.setCoords(0, 0, 0);
			test1.setCoords(1, 1, 1);
			test1.setCoords(2, 0, 1);
			test1.setColor(0, 0, 0, 127);
			test1.setColor(1, 255, 0, 0);
			test1.setColor(2, 0, 0, 127);
			test1.setTexture(tx[2]);
			test1.renderOpenGL();

			v1_t = { 0.0f,0.0f,1.0f,1.0f };
			v2_t = { 1.0f,1.0f,1.0f,1.0f };
			v3_t = { 0.0f,1.0f,1.0f,1.0f };
			Triangle test3(&v1_t, &v2_t, &v3_t);
			test3.setCoords(0, 0, 0);
			test3.setCoords(1, 1, 1);
			test3.setCoords(2, 0, 1);
			test3.setColor(0, 255, 0, 0);
			test3.setColor(1, 0, 255, 0);
			test3.setColor(2, 0, 0, 255);
			test3.setTexture(tx[3]);
			test3.renderOpenGL(); // Mixed

		}

		void renderSceneOpenGL(void) {
			TriangleList *ptr;

			/* now clear all the stuff */
			glClearColor(100/(float)255, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);

#ifdef DEBUG_TEST
			/* Test static triangles */
			testRenderSceneOpenGL();
#endif // DEBUG_TEST

#ifndef DEBUG_TEST
			/* now render the triangles in the list  */
			int count=0;
			for (ptr = original_head; ptr;) {
				count++;
				ptr->t->renderOpenGL();
				ptr = ptr->next;
			}
#endif // !DEBUG_TEST
			glDisable(GL_DEPTH_TEST);

			return;
		};	

		void testRenderSceneSoftware(void);
		void renderSceneSoftware(void);

		/* Clipping operations */
		float getPtToPlaneW(float v1, float v2, float w1, float w2, int isWPositive);
		float getAlphaToPlaneW(Vector4 v1, Vector4 v2, int isWPositive, int axis);
		Vector4 getIntersectionToPlaneW(Vector4 v1, Vector4 v2, int isWPositive, float a);
		Vector4 getTexCoordToPlaneW(float x1, float y1, float x2, float y2, int isWPositive, float a);
		int clipTriangleX(Triangle * tr);
		int clipTriangle(Triangle *tr);
};

void loadScene(char *name);
void setDefaults(void);
void sceneCleanup(void);


void setPerspectiveProjection(float eye_fov, float aspect_ratio, float near, float far);
void setModelviewMatrix(float *eye_pos, float eye_theta, float eye_phi);

#endif		/* SCENE_H */