#define RGFW_IMPLEMENTATION
#define RGFW_BUFFER

#include "RGFW.h"

#define RSoft_rect RGFW_rect
#define RSoft_area RGFW_area
#define RSoft_point RGFW_point

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define RSOFT_IMPLEMENTATION
#include "RSoft.h"

RSoft_matrix rotateAroundCenter(RSoft_vector center, float angle) {
	RSoft_matrix m = RSoft_initMatrix();
	m = RSoft_translateMatrix(m, RSOFT_VECTOR2D(-center.x, -center.y));
	m = RSoft_rotateMatrix(m, angle, 1, 1, 1);
	m = RSoft_translateMatrix(m, center);
	return m;
}

int main(void) {
    RGFW_window* win = RGFW_createWindow("Textures example", RGFW_RECT(0, 0, 800, 800), RGFW_CENTER | RGFW_TRANSPARENT_WINDOW);
    
    RSoft_setBufferSize(RGFW_getScreenSize());
    RSoft_setCanvasSize(RGFW_AREA(win->r.w, win->r.h));

	int w, h, c;
	u8* buffer = stbi_load("tex.jpg", &w, &h, &c, 4);

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
		RSoft_vector v1 = RSOFT_VECTOR2D(700, 100);	
		RSoft_vector v2 = RSOFT_VECTOR2D(500, 100);	
		RSoft_vector v3 = RSOFT_VECTOR2D(600, 200);

		RSoft_setTexture(buffer, RSOFT_RECT(0, 100, 200, 100), RSOFT_AREA(w, h));
		RSoft_setMatrix(rotateAroundCenter(RSOFT_VECTOR2D(500, 150), angle));
		RSoft_drawTriangleF(win->buffer, (RSoft_vector[3]){v1, v2, v3}, (u8[4]){255, 225, 225, 255});

		RSoft_setTexture(buffer, RSOFT_RECT(0, 100, 200, 200), RSOFT_AREA(w, h));
		RSoft_setMatrix(rotateAroundCenter(RSOFT_VECTOR2D(250, 250), angle));
		RSoft_drawRectF(win->buffer, RSOFT_RECTF(150, 150, 200, 200), (u8[4]){255, 225, 225, 255}); 
		
		RSoft_setTexture(buffer, RSOFT_RECT(0, 0, 100, 100), RSOFT_AREA(w, h));
		
		RSoft_setMatrix(rotateAroundCenter(RSOFT_VECTOR2D(200, 600), angle));
		RSoft_drawPolygonF(win->buffer, RSOFT_RECTF(200, 600, 100, 100), 6, (u8[4]){255, 225, 225, 255});
		
		RSoft_setMatrix(rotateAroundCenter(RSOFT_VECTOR2D(500, 600), angle));
		RSoft_drawPolygonF(win->buffer, RSOFT_RECTF(500, 600, 100, 100), 360, (u8[4]){255, 225, 225, 255});

		angle++;
		RGFW_window_swapBuffers(win);
    }
	
	free(buffer);
    RGFW_window_close(win);
}

