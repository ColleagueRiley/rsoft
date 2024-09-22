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

RSoft_vector rotateAroundCenter(RSoft_vector v, RSoft_vector center, RSoft_vector player, float angle) {
	RSoft_matrix m = RSoft_initMatrix();
	m = RSoft_translateMatrix(m, RSOFT_VECTOR2D(-center.x, -center.y));
	m = RSoft_simpleRotateMatrix(m, angle);
	m = RSoft_translateMatrix(m, center);
	m = RSoft_translateMatrix(m, RSOFT_VECTOR3D(-player.x, -player.y, -player.z));
	return RSoft_applyMatrix(m, v);
}

void drawLine(RGFW_window* win, RSoft_vector center, RSoft_vector player, float angle, RSoft_vector v1, RSoft_vector v2) {
	v1 = rotateAroundCenter(v1, center, player, angle);
	v2 = rotateAroundCenter(v2, center, player, angle);

	RSoft_setMatrix(RSoft_initMatrix());

	if (RGFW_isPressed(win, RGFW_Tab)) {
		RSoft_drawLineF(win->buffer, v1, v2, (u8[4]){255, 225, 225, 255});
	} else {
		v1.x = (-(v1.x * 200)) / (v1.y - (center.y));
		v1.y = (v1.z * 200) / (v1.y - (center.y)) + center.y;	

		v2.x = (-(v2.x * 200)) / (v2.y - (center.y));	
		v2.y = (v2.z * 200) / (v2.y - (center.y)) + center.y;	
		
		if (v1.x >= 0 && v1.y >= 0 && v2.x >= 0 && v2.y >= 0) 
			RSoft_drawLineF(win->buffer, v1, v2, (u8[4]){255, 225, 225, 255});
	}
}

u8* buffer = NULL;
int w, h, c;

void drawWall(RGFW_window* win, RSoft_vector center, RSoft_vector player, float angle, RSoft_rectF rect) {
	for (float i = 0; i < rect.h; i += 0.1) {
		RSoft_setTexture(buffer, RSOFT_RECT(0, i, w, h), RSOFT_AREA(w, h));
		drawLine(win, center, player, angle, RSOFT_VECTOR3D(rect.x, rect.y, i), RSOFT_VECTOR3D(rect.x + rect.w, rect.y, i));
	}
}

int main(void) {
    RGFW_window* win = RGFW_createWindow("Basic buffer example", RGFW_RECT(0, 0, 800, 800), RGFW_CENTER | RGFW_TRANSPARENT_WINDOW);
    
    RSoft_setBufferSize(RGFW_getScreenSize());
    RSoft_setCanvasSize(RGFW_AREA(win->r.w, win->r.h));

	buffer = stbi_load("wall2.jpg", &w, &h, &c, 4);

	float angle = 0;
	float fov = 120;

	RSoft_vector center = RSOFT_VECTOR2D(win->r.w / 2, win->r.h / 2);

	RSoft_vector player = RSOFT_VECTOR3D(0, 0, 10);

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
			
		if (RGFW_isPressed(win, RGFW_w)) {
			RSoft_vector next = RSOFT_VECTOR3D(player.x - cos((angle + 90) * DEG2RAD), 
											   player.y - sin((angle + 90) * DEG2RAD),
											   player.z);

			player = next;
		}
		if (RGFW_isPressed(win, RGFW_s)){
			RSoft_vector next = RSOFT_VECTOR3D(player.x + cos((angle + 90) * DEG2RAD), 
											   player.y + sin((angle + 90) * DEG2RAD),
											   player.z);
			player = next;
		}

		if (RGFW_isPressed(win, RGFW_a)) {
			RSoft_vector next = RSOFT_VECTOR3D(player.x + cos((angle) * DEG2RAD), 
											   player.y + sin((angle) * DEG2RAD),
											   player.z);

			player = next;
		}

		if (RGFW_isPressed(win, RGFW_d)) {
			RSoft_vector next = RSOFT_VECTOR3D(player.x - cos((angle) * DEG2RAD), 
											   player.y - sin((angle) * DEG2RAD),
											   player.z);
			
			player = next;
		}

		if (RGFW_isPressed(win, RGFW_Left))
			angle += 0.5;	
		if (RGFW_isPressed(win, RGFW_Right))
			angle -= 0.5;

		if (RGFW_isPressed(win, RGFW_Up))
			player.z -= 5;	
		if (RGFW_isPressed(win, RGFW_Down))
			player.z += 5;

		RSoft_clear(win->buffer, (u8[4]){0, 0, 255, 255});
		RSoft_setMatrix(RSoft_initMatrix());
		
		if (RGFW_isPressed(win, RGFW_Tab)) {
			RSoft_matrix m = RSoft_initMatrix();
			m = RSoft_translateMatrix(m, RSOFT_VECTOR2D(-center.x, -center.y));
			m = RSoft_simpleRotateMatrix(m, angle - 90);
			m = RSoft_translateMatrix(m, center);
			RSoft_setMatrix(m);
			
			RSoft_drawPolygon(win->buffer, RSOFT_RECT(center.x - 10, center.y - 5, 20, 10), 3, (u8[4]){255, 0, 0, 255});
		}

		drawWall(win, center, player, angle, RSOFT_RECTF(200, 200, 200, 20));

		RGFW_window_swapBuffers(win);
    }
	
	free(buffer);
    RGFW_window_close(win);
}

