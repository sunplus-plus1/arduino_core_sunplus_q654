#include "Arduino.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "lvgl.h"
#ifdef __cplusplus
} /*extern "C"*/
#endif
#include "demos/widgets/lv_demo_widgets.h"

#include <TFT_eSPI.h>
#define HOR_RES 320
#define VER_RES 240


lv_color_t test_fb[HOR_RES * VER_RES]; /* TODO: FIXME */
static lv_color_t disp_buf[HOR_RES * VER_RES];

TFT_eSPI tft = TFT_eSPI(HOR_RES, VER_RES); /* TFT instance */
/* TODO: FIXME */
static void disp_flush_cb(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{

    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

	tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
	//tft.pushPixelsDMA( ( uint16_t * )&color_p->full, w * h);
	tft.pushColors( ( uint16_t * )&color_p->full, w * h,true);
    tft.endWrite();

	lv_disp_flush_ready(disp_drv);
}

static void hal_init(void)
{
	static lv_disp_draw_buf_t draw_buf;

	lv_disp_draw_buf_init(&draw_buf, disp_buf, NULL, HOR_RES * VER_RES);

	static lv_disp_drv_t disp_drv;
	lv_disp_drv_init(&disp_drv);
	disp_drv.draw_buf = &draw_buf;
	disp_drv.flush_cb = disp_flush_cb;
	disp_drv.hor_res = HOR_RES;
	disp_drv.ver_res = VER_RES;
	lv_disp_drv_register(&disp_drv);


}

void setup()
{
	tft.begin();          /* TFT init */
    tft.setRotation( 1 ); /* Landscape orientation, flipped */
	
	lv_init();
	hal_init();

    lv_obj_t *label = lv_label_create( lv_scr_act() );
    lv_label_set_text( label, "Hello ,Arduino !!!!" );
    lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );
	
//	lv_demo_widgets();
}

void loop()
{
	lv_timer_handler();
}
