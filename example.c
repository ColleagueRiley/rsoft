#define RGFW_IMPLEMENTATION
#define RGFW_BUFFER

#include "RGFW.h"

#define RSoft_rect RGFW_rect
#define RSoft_area RGFW_area
#define RSoft_point RGFW_point

#define RSOFT_IMPLEMENTATION
#include "rsoft.h"

int main(void) {
    RGFW_window* win = RGFW_createWindow("Basic buffer example", RGFW_RECT(0, 0, 800, 800), RGFW_CENTER | RGFW_TRANSPARENT_WINDOW);
    
    RSoft_setBufferSize(RGFW_getScreenSize());
    RSoft_setCanvasSize(RGFW_AREA(win->r.w, win->r.h));
		
	float angle = 0;
	i8 running = 1;
    while (running) {
        while (RGFW_window_checkEvent(win)) {
            if (win->event.type == RGFW_quit || RGFW_isPressed(win, RGFW_Escape)) {
                running = 0;
                break;
			}

			if (win->event.type == RGFW_windowResized)
				RSoft_setCanvasSize(RGFW_AREA(win->r.w, win->r.h));
        } 

		RSoft_clear(win->buffer, (u8[4]){0, 0, 255, 15});

		RSoft_matrix m = RSoft_initMatrix();
		m = RSoft_translateMatrix(m, RSOFT_VECTOR2D(-500, -450));
		m = RSoft_rotateMatrix(m, angle, 1, 0, 1);
		m = RSoft_translateMatrix(m, RSOFT_VECTOR3D(0, 0, 100));

		RSoft_setMatrix(m);

		RSoft_vector v1 = RSOFT_VECTOR2D(600, 400);	
		RSoft_vector v2 = RSOFT_VECTOR2D(400, 400);	
		RSoft_vector v3 = RSOFT_VECTOR2D(500, 500);	
		//printf("%f %f\n", v.x, v.y);
		RSoft_drawTriangleF(win->buffer, (RSoft_vector[3]){v1, v2, v3}  , (u8[4]){0, 255, 0, 255});
		//RSoft_drawRectF(win->buffer, RSOFT_RECTF(200, 200, 200, 200), (u8[4]){0, 255, 0, 255}); 
			
		//angle = 1;
		angle++;
		RGFW_window_swapBuffers(win);
		RGFW_window_checkFPS(win, 60);
    }

    RGFW_window_close(win);
}

