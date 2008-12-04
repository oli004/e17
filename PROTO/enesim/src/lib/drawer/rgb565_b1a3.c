/* File generated by enesim_generator, don't modify it */
#include "enesim_common.h"
#include "Enesim.h"
#include "enesim_private.h"
#if 0
static void rgb565_b1a3_pt_color_blend(Enesim_Surface_Data *d, Enesim_Surface_Data *s, Enesim_Color color, Enesim_Surface_Data *m)
{
	uint16_t data0;
	uint8_t data1;
	rgb565_b1a3_from_argb(color, &data0, &data1);
	rgb565_b1a3_blend(d->rgb565_b1a3.plane0, d->rgb565_b1a3.plane1, d->rgb565_b1a3.pixel_plane1, data0, data1);
}
static void rgb565_b1a3_pt_pixel_blend_argb8888(Enesim_Surface_Data *d, Enesim_Surface_Data *s, Enesim_Color color, Enesim_Surface_Data *m)
{
	uint16_t data0;
	uint8_t data1;
	unsigned int argb;
	argb8888_to_argb(&argb, *(s->argb8888.plane0));
	rgb565_b1a3_from_argb(argb, &data0, &data1);
	rgb565_b1a3_blend(d->rgb565_b1a3.plane0, d->rgb565_b1a3.plane1, d->rgb565_b1a3.pixel_plane1, data0, data1);
}
static void rgb565_b1a3_pt_pixel_blend_argb8888_unpre(Enesim_Surface_Data *d, Enesim_Surface_Data *s, Enesim_Color color, Enesim_Surface_Data *m)
{
	uint16_t data0;
	uint8_t data1;
	unsigned int argb;
	argb8888_unpre_to_argb(&argb, *(s->argb8888_unpre.plane0));
	rgb565_b1a3_from_argb(argb, &data0, &data1);
	rgb565_b1a3_blend(d->rgb565_b1a3.plane0, d->rgb565_b1a3.plane1, d->rgb565_b1a3.pixel_plane1, data0, data1);
}
static void rgb565_b1a3_pt_pixel_blend_rgb565_b1a3(Enesim_Surface_Data *d, Enesim_Surface_Data *s, Enesim_Color color, Enesim_Surface_Data *m)
{
	uint16_t data0;
	uint8_t data1;
	unsigned int argb;
	rgb565_b1a3_to_argb(&argb, *(s->rgb565_b1a3.plane0), *(s->rgb565_b1a3.plane1), s->rgb565_b1a3.pixel_plane1);
	rgb565_b1a3_from_argb(argb, &data0, &data1);
	rgb565_b1a3_blend(d->rgb565_b1a3.plane0, d->rgb565_b1a3.plane1, d->rgb565_b1a3.pixel_plane1, data0, data1);
}
static void rgb565_b1a3_sp_color_blend(Enesim_Surface_Data *d, unsigned int len, Enesim_Surface_Data *s, Enesim_Color color, Enesim_Surface_Data *m)
{
	Enesim_Surface_Data dtmp, end;

	rgb565_b1a3_data_copy(d, &dtmp);
	rgb565_b1a3_data_offset(d, &end, len);
	while (dtmp.rgb565_b1a3.plane0 < end.rgb565_b1a3.plane0)
	{
		rgb565_b1a3_pt_color_blend(&dtmp, NULL, color, NULL);
		rgb565_b1a3_data_increment(&dtmp, 1);
	}
}
static void rgb565_b1a3_sp_pixel_blend_argb8888(Enesim_Surface_Data *d, unsigned int len, Enesim_Surface_Data *s, Enesim_Color color, Enesim_Surface_Data *m)
{
	Enesim_Surface_Data stmp, dtmp, end;

	rgb565_b1a3_data_copy(d, &dtmp);
	argb8888_data_copy(s, &stmp);
	rgb565_b1a3_data_offset(d, &end, len);
	while (dtmp.rgb565_b1a3.plane0 < end.rgb565_b1a3.plane0)
	{
		rgb565_b1a3_pt_pixel_blend_argb8888(&dtmp, &stmp, 0, NULL);
		argb8888_data_increment(&stmp, 1);
		rgb565_b1a3_data_increment(&dtmp, 1);
	}
}
static void rgb565_b1a3_sp_pixel_blend_argb8888_unpre(Enesim_Surface_Data *d, unsigned int len, Enesim_Surface_Data *s, Enesim_Color color, Enesim_Surface_Data *m)
{
	Enesim_Surface_Data stmp, dtmp, end;

	rgb565_b1a3_data_copy(d, &dtmp);
	argb8888_unpre_data_copy(s, &stmp);
	rgb565_b1a3_data_offset(d, &end, len);
	while (dtmp.rgb565_b1a3.plane0 < end.rgb565_b1a3.plane0)
	{
		rgb565_b1a3_pt_pixel_blend_argb8888_unpre(&dtmp, &stmp, 0, NULL);
		argb8888_unpre_data_increment(&stmp, 1);
		rgb565_b1a3_data_increment(&dtmp, 1);
	}
}
static void rgb565_b1a3_sp_pixel_blend_rgb565_b1a3(Enesim_Surface_Data *d, unsigned int len, Enesim_Surface_Data *s, Enesim_Color color, Enesim_Surface_Data *m)
{
	Enesim_Surface_Data stmp, dtmp, end;

	rgb565_b1a3_data_copy(d, &dtmp);
	rgb565_b1a3_data_copy(s, &stmp);
	rgb565_b1a3_data_offset(d, &end, len);
	while (dtmp.rgb565_b1a3.plane0 < end.rgb565_b1a3.plane0)
	{
		rgb565_b1a3_pt_pixel_blend_rgb565_b1a3(&dtmp, &stmp, 0, NULL);
		rgb565_b1a3_data_increment(&stmp, 1);
		rgb565_b1a3_data_increment(&dtmp, 1);
	}
}
static void rgb565_b1a3_pt_color_fill(Enesim_Surface_Data *d, Enesim_Surface_Data *s, Enesim_Color color, Enesim_Surface_Data *m)
{
	uint16_t data0;
	uint8_t data1;
	rgb565_b1a3_from_argb(color, &data0, &data1);
	rgb565_b1a3_fill(d->rgb565_b1a3.plane0, d->rgb565_b1a3.plane1, d->rgb565_b1a3.pixel_plane1, data0, data1);
}
static void rgb565_b1a3_pt_pixel_fill_argb8888(Enesim_Surface_Data *d, Enesim_Surface_Data *s, Enesim_Color color, Enesim_Surface_Data *m)
{
	uint16_t data0;
	uint8_t data1;
	unsigned int argb;
	argb8888_to_argb(&argb, *(s->argb8888.plane0));
	rgb565_b1a3_from_argb(argb, &data0, &data1);
	rgb565_b1a3_fill(d->rgb565_b1a3.plane0, d->rgb565_b1a3.plane1, d->rgb565_b1a3.pixel_plane1, data0, data1);
}
static void rgb565_b1a3_pt_pixel_fill_argb8888_unpre(Enesim_Surface_Data *d, Enesim_Surface_Data *s, Enesim_Color color, Enesim_Surface_Data *m)
{
	uint16_t data0;
	uint8_t data1;
	unsigned int argb;
	argb8888_unpre_to_argb(&argb, *(s->argb8888_unpre.plane0));
	rgb565_b1a3_from_argb(argb, &data0, &data1);
	rgb565_b1a3_fill(d->rgb565_b1a3.plane0, d->rgb565_b1a3.plane1, d->rgb565_b1a3.pixel_plane1, data0, data1);
}
static void rgb565_b1a3_pt_pixel_fill_rgb565_b1a3(Enesim_Surface_Data *d, Enesim_Surface_Data *s, Enesim_Color color, Enesim_Surface_Data *m)
{
	uint16_t data0;
	uint8_t data1;
	unsigned int argb;
	rgb565_b1a3_to_argb(&argb, *(s->rgb565_b1a3.plane0), *(s->rgb565_b1a3.plane1), s->rgb565_b1a3.pixel_plane1);
	rgb565_b1a3_from_argb(argb, &data0, &data1);
	rgb565_b1a3_fill(d->rgb565_b1a3.plane0, d->rgb565_b1a3.plane1, d->rgb565_b1a3.pixel_plane1, data0, data1);
}
static void rgb565_b1a3_sp_color_fill(Enesim_Surface_Data *d, unsigned int len, Enesim_Surface_Data *s, Enesim_Color color, Enesim_Surface_Data *m)
{
	Enesim_Surface_Data dtmp, end;

	rgb565_b1a3_data_copy(d, &dtmp);
	rgb565_b1a3_data_offset(d, &end, len);
	while (dtmp.rgb565_b1a3.plane0 < end.rgb565_b1a3.plane0)
	{
		rgb565_b1a3_pt_color_fill(&dtmp, NULL, color, NULL);
		rgb565_b1a3_data_increment(&dtmp, 1);
	}
}
static void rgb565_b1a3_sp_pixel_fill_argb8888(Enesim_Surface_Data *d, unsigned int len, Enesim_Surface_Data *s, Enesim_Color color, Enesim_Surface_Data *m)
{
	Enesim_Surface_Data stmp, dtmp, end;

	rgb565_b1a3_data_copy(d, &dtmp);
	argb8888_data_copy(s, &stmp);
	rgb565_b1a3_data_offset(d, &end, len);
	while (dtmp.rgb565_b1a3.plane0 < end.rgb565_b1a3.plane0)
	{
		rgb565_b1a3_pt_pixel_fill_argb8888(&dtmp, &stmp, 0, NULL);
		argb8888_data_increment(&stmp, 1);
		rgb565_b1a3_data_increment(&dtmp, 1);
	}
}
static void rgb565_b1a3_sp_pixel_fill_argb8888_unpre(Enesim_Surface_Data *d, unsigned int len, Enesim_Surface_Data *s, Enesim_Color color, Enesim_Surface_Data *m)
{
	Enesim_Surface_Data stmp, dtmp, end;

	rgb565_b1a3_data_copy(d, &dtmp);
	argb8888_unpre_data_copy(s, &stmp);
	rgb565_b1a3_data_offset(d, &end, len);
	while (dtmp.rgb565_b1a3.plane0 < end.rgb565_b1a3.plane0)
	{
		rgb565_b1a3_pt_pixel_fill_argb8888_unpre(&dtmp, &stmp, 0, NULL);
		argb8888_unpre_data_increment(&stmp, 1);
		rgb565_b1a3_data_increment(&dtmp, 1);
	}
}
static void rgb565_b1a3_sp_pixel_fill_rgb565_b1a3(Enesim_Surface_Data *d, unsigned int len, Enesim_Surface_Data *s, Enesim_Color color, Enesim_Surface_Data *m)
{
	Enesim_Surface_Data stmp, dtmp, end;

	rgb565_b1a3_data_copy(d, &dtmp);
	rgb565_b1a3_data_copy(s, &stmp);
	rgb565_b1a3_data_offset(d, &end, len);
	while (dtmp.rgb565_b1a3.plane0 < end.rgb565_b1a3.plane0)
	{
		rgb565_b1a3_pt_pixel_fill_rgb565_b1a3(&dtmp, &stmp, 0, NULL);
		rgb565_b1a3_data_increment(&stmp, 1);
		rgb565_b1a3_data_increment(&dtmp, 1);
	}
}
Enesim_Drawer rgb565_b1a3_drawer = {
	.sp_color[ENESIM_BLEND] = rgb565_b1a3_sp_color_blend,
	.pt_color[ENESIM_BLEND] = rgb565_b1a3_pt_color_blend,
	.sp_pixel[ENESIM_BLEND][ENESIM_SURFACE_ARGB8888] = rgb565_b1a3_sp_pixel_blend_argb8888,
	.pt_pixel[ENESIM_BLEND][ENESIM_SURFACE_ARGB8888] = rgb565_b1a3_pt_pixel_blend_argb8888,
	.sp_pixel[ENESIM_BLEND][ENESIM_SURFACE_ARGB8888_UNPRE] = rgb565_b1a3_sp_pixel_blend_argb8888_unpre,
	.pt_pixel[ENESIM_BLEND][ENESIM_SURFACE_ARGB8888_UNPRE] = rgb565_b1a3_pt_pixel_blend_argb8888_unpre,
	.sp_pixel[ENESIM_BLEND][ENESIM_SURFACE_RGB565_XA5] = NULL,
	.pt_pixel[ENESIM_BLEND][ENESIM_SURFACE_RGB565_XA5] = NULL,
	.sp_pixel[ENESIM_BLEND][ENESIM_SURFACE_RGB565_B1A3] = rgb565_b1a3_sp_pixel_blend_rgb565_b1a3,
	.pt_pixel[ENESIM_BLEND][ENESIM_SURFACE_RGB565_B1A3] = rgb565_b1a3_pt_pixel_blend_rgb565_b1a3,
	.sp_color[ENESIM_FILL] = rgb565_b1a3_sp_color_fill,
	.pt_color[ENESIM_FILL] = rgb565_b1a3_pt_color_fill,
	.sp_pixel[ENESIM_FILL][ENESIM_SURFACE_ARGB8888] = rgb565_b1a3_sp_pixel_fill_argb8888,
	.pt_pixel[ENESIM_FILL][ENESIM_SURFACE_ARGB8888] = rgb565_b1a3_pt_pixel_fill_argb8888,
	.sp_pixel[ENESIM_FILL][ENESIM_SURFACE_ARGB8888_UNPRE] = rgb565_b1a3_sp_pixel_fill_argb8888_unpre,
	.pt_pixel[ENESIM_FILL][ENESIM_SURFACE_ARGB8888_UNPRE] = rgb565_b1a3_pt_pixel_fill_argb8888_unpre,
	.sp_pixel[ENESIM_FILL][ENESIM_SURFACE_RGB565_XA5] = NULL,
	.pt_pixel[ENESIM_FILL][ENESIM_SURFACE_RGB565_XA5] = NULL,
	.sp_pixel[ENESIM_FILL][ENESIM_SURFACE_RGB565_B1A3] = rgb565_b1a3_sp_pixel_fill_rgb565_b1a3,
	.pt_pixel[ENESIM_FILL][ENESIM_SURFACE_RGB565_B1A3] = rgb565_b1a3_pt_pixel_fill_rgb565_b1a3,
};
#endif
