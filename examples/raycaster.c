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
    RGFW_window* win = RGFW_createWindow("Raycaster example", RGFW_RECT(0, 0, 1100, 800), RGFW_CENTER);
    
    RSoft_setBufferSize(RGFW_getScreenSize());
    RSoft_setCanvasSize(RGFW_AREA(win->r.w, win->r.h));

	int w, h, c;
	u8* buffer = stbi_load("wall.jpg", &w, &h, &c, 4);

	const u32 map_width = 20;
	const u32 map_height = 20;
	
	u8 map[] = {
		1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
		1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1,
		1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
		1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
		1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
		1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	};

	float angle = 0;
	float fov = 120;
	float precision = 64; 

	RSoft_vector player = RSOFT_VECTOR2D(5, 7);

	RGFW_window_mouseHold(win, RGFW_AREA(0, 0));
	RGFW_window_showMouse(win, 0);

	i8 running = 1;    
	while (running) {
        while (RGFW_window_checkEvent(win)) {
            if (win->event.type == RGFW_quit || RGFW_isPressed(win, RGFW_Escape)) {
                running = 0;
                break;
			}

			if (win->event.type == RGFW_mousePosChanged) {
				angle += (win->event.point.x / 15);	
			}

			if (win->event.type == RGFW_windowResized)
				RSoft_setCanvasSize(RGFW_AREA(win->r.w, win->r.h));
        }
		
		if (RGFW_isPressed(win, RGFW_w)) {
			RSoft_vector next = RSOFT_VECTOR2D(player.x + cos(angle * DEG2RAD) / 20, 
											   player.y + sin(angle * DEG2RAD) / 20);

			if (map[(size_t)((size_t)next.y * map_width) + (size_t)next.x] == 0)
				player = next;
		}
		if (RGFW_isPressed(win, RGFW_s)){
			RSoft_vector next = RSOFT_VECTOR2D(player.x - cos(angle * DEG2RAD) / 20, 
											   player.y - sin(angle * DEG2RAD) / 20);
			if (map[(size_t)((size_t)next.y * map_width) + (size_t)next.x] == 0)
				player = next;
		}

		if (RGFW_isPressed(win, RGFW_a)) {
			RSoft_vector next = RSOFT_VECTOR2D(player.x - cos((angle + 90) * DEG2RAD) / 20, 
											   player.y - sin((angle + 90) * DEG2RAD) / 20);

			if (map[(size_t)((size_t)next.y * map_width) + (size_t)next.x] == 0)
				player = next;
		}

		if (RGFW_isPressed(win, RGFW_d)) {
			RSoft_vector next = RSOFT_VECTOR2D(player.x + cos((angle + 90) * DEG2RAD) / 20, 
											   player.y + sin((angle + 90) * DEG2RAD) / 20);

			if (map[(size_t)((size_t)next.y * map_width) + (size_t)next.x] == 0)
				player = next;
		}

		if (RGFW_isPressed(win, RGFW_Left))
			angle -= 5;	
		if (RGFW_isPressed(win, RGFW_Right))
			angle += 5;
	

		RSoft_setTexture(buffer, RSOFT_RECT(0, 0, 1, 1), RSOFT_AREA(w, h));
		RSoft_clear(win->buffer, (u8[4]){30, 130, 180, 255});
		RSoft_drawRectF(win->buffer, RSOFT_RECTF(0, win->r.h / 3, win->r.w, win->r.h), (u8[4]){10, 100, 10, 255});	

		float rayAngle = angle - (fov / 2);
		for (size_t i = 0; i < win->r.w; i++) {
			RSoft_vector ray = player;
			rayAngle += (fov / win->r.w);

			while (map[(size_t)((size_t)ray.y * map_width) + (size_t)ray.x] == 0) {
				ray.x += cos(rayAngle * DEG2RAD) / precision;
				ray.y += sin(rayAngle * DEG2RAD) / precision;
			}


			if (RGFW_isPressed(win, RGFW_Tab)) {
				RSoft_drawLineF(win->buffer, RSOFT_VECTOR2D(player.x * 50, player.y * 50), RSOFT_VECTOR2D(ray.x * 50, ray.y * 50),  (u8[4]){200, 100, 20, 255});
				continue;
			}

			float dist = (sqrt(pow(player.x - ray.x, 2) + pow(player.y - ray.y, 2)));
			dist *= cos((rayAngle - angle) * DEG2RAD);
			dist += 1;
			
			float height = (win->r.h / 1) / dist;	


			RSoft_setTexture(buffer, RSOFT_RECT(cos((rayAngle) * DEG2RAD) * w, 100, 1, height), RSOFT_AREA(w, h));
			RSoft_drawRectF(win->buffer, RSOFT_RECTF(i, win->r.h / 3.5, 1, height), (u8[4]){255, 255, 255, 255});	
		}
			

		if (RGFW_isPressed(win, RGFW_Tab)) {
			for (size_t y = 0; y < map_height; y++) {
				for (size_t x = 0; x < map_width; x++) {
					if (map[(y * map_width) + x])
						RSoft_drawRect(win->buffer, RSOFT_RECT(x * 50, y * 50, 50, 50), (u8[4]){255, 255, 255, 255});	
				}
			}

			RSoft_drawRect(win->buffer, RSOFT_RECT((player.x * 50) - 10, (player.y * 50) - 10, 20, 20), (u8[4]){255, 0, 0, 255});	
		}

		RGFW_window_swapBuffers(win);
    }
	
	free(buffer);
    RGFW_window_close(win);
}

