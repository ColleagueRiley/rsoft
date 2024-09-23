#define RGFW_IMPLEMENTATION
#define RGFW_BUFFER

#include "RGFW.h"

#define RSoft_rect RGFW_rect
#define RSoft_area RGFW_area
#define RSoft_point RGFW_point

#define RSOFT_IMPLEMENTATION
#include "RSoft.h"

#define RFONT_RENDER_LEGACY
#define RFONT_NO_OPENGL

#define RFONT_IMPLEMENTATION
#include "RFont.h"

#define MAX_ATLASES 5

struct {
	u32 color;
	u8** atlases;
	size_t atlasCount;
} RFont_info;

void RFont_render_init(void) { 
	RFont_info.atlases = (u8**)malloc(sizeof(u8**) * MAX_ATLASES);
	RFont_info.atlasCount = 0;
}

u32 RFont_create_atlas(u32 atlasWidth, u32 atlasHeight) {
	u32* atlas = (u32*)malloc((atlasWidth * atlasHeight * 4) + sizeof(u32));
	atlas[0] = atlasWidth;

	RFont_info.atlases[RFont_info.atlasCount] = (u8*)(&atlas[1]); 
	RFont_info.atlasCount++;
	
	return RFont_info.atlasCount - 1;
}

void RFont_bitmap_to_atlas(u32 atlas, u8* bitmap, float x, float y, float w, float h) {
	u8* buffer = RFont_info.atlases[atlas];
	u32 atlasWidth = *((u32*)RFont_info.atlases[atlas] - 1);
	
	RSoft_copyBuffer(buffer, atlasWidth, bitmap, RSOFT_RECT(x * atlasWidth, y * RFONT_ATLAS_HEIGHT, w, h));
}

void RFont_render_set_color(float r, float g, float b, float a) {
	u8 color[] = {r * 255, g * 255, b * 255, a * 255};
	RFont_info.color = *(u32*)color;
}

RGFW_window* win;

#define GET_NORMAL_X(x) ((x) + 1) * (win->r.w / 2)
#define GET_NORMAL_Y(y) (1.0 - (y)) * (win->r.h / 2)



void RFont_render_text(u32 atlas, float* verts, float* tcoords, size_t nverts) {
	for (size_t i = 0; i < nverts; i += 6) {
		RSoft_vector npoints[3] = {RSOFT_VECTOR2D(GET_NORMAL_X(verts[i]), GET_NORMAL_Y(verts[i + 1])), 
								   RSOFT_VECTOR2D(GET_NORMAL_X(verts[i + 2]), GET_NORMAL_Y(verts[i + 3])), 
								   RSOFT_VECTOR2D(GET_NORMAL_X(verts[i + 4]), GET_NORMAL_Y(verts[i + 5]))};
	
		/*printf("first: {%f, %f}, {%f, %f}, {%f, %f}\n", (verts[i]), (verts[i + 1]), 
								   verts[i + 2], (verts[i + 3]), 
									(verts[i + 4]), (verts[i + 5]));
		*/

		 	
		/*printf("last: {%f, %f}, {%f, %f}, {%f, %f}\n", GET_NORMAL_X(verts[i]), GET_NORMAL_Y(verts[i + 1]), 
								   GET_NORMAL_X(verts[i + 2]), GET_NORMAL_Y(verts[i + 3]), 
									GET_NORMAL_X(verts[i + 4]), GET_NORMAL_Y(verts[i + 5]));
	*/
		RSoft_drawRectF(win->buffer, RSOFT_RECTF(npoints[0].x, npoints[0].y, 
						npoints[2].x - npoints[0].x, abs(npoints[1].y - npoints[0].y)), (u8*)&RFont_info.color);
		
		printf("%f %f %f %f\n", npoints[0].x, npoints[0].y, 
						npoints[2].x - npoints[0].x, abs(npoints[1].y - npoints[0].y));
		//RSoft_drawTriangleF(win->buffer, npoints, (u8*)&RFont_info.color);
	}
}

void RFont_render_free(u32 atlas) {
	for (size_t i = 0; i < RFont_info.atlasCount; i++)
		free(((u32*)RFont_info.atlases[i] - 1));
	
	free(RFont_info.atlases);
	RFont_info.atlasCount = 0;
}

void RFont_render_legacy(u8 legacy) { }


int main(void) {
    win = RGFW_createWindow("Basic buffer example", RGFW_RECT(0, 0, 800, 800), RGFW_CENTER | RGFW_TRANSPARENT_WINDOW);
   
    RFont_init(win->r.w, win->r.h);
    RFont_font* font = RFont_font_init("DejaVuSans.ttf");

	RSoft_setBufferSize(RGFW_getScreenSize());
    RSoft_setCanvasSize(RGFW_AREA(win->r.w, win->r.h));
	
	i8 running = 1;    
	while (running) {
        while (RGFW_window_checkEvent(win)) {
            if (win->event.type == RGFW_quit || RGFW_isPressed(win, RGFW_Escape)) {
                running = 0;
                break;
			}

			if (win->event.type == RGFW_windowResized) {
				RSoft_setCanvasSize(RGFW_AREA(win->r.w, win->r.h));
				RFont_update_framebuffer(win->r.w, win->r.h);
			}
		} 

		RSoft_clear(win->buffer, (u8[4]){0, 0, 255, 15});
        RFont_set_color(0.0f, 1.0f, 0, 1.0f);

        RFont_draw_text(font, "a", 0, 0, 60);
	
		RGFW_window_swapBuffers(win);
    }

	RFont_font_free(font);
    RGFW_window_close(win);
}

