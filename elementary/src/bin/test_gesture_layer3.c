#include <Elementary.h>
#ifdef HAVE_CONFIG_H
#include "elementary_config.h"
#endif
#ifndef ELM_LIB_QUICKLAUNCH

/* We zoom out to this value so we'll be able to use map and have a nice
 * resolution when zooming in. */
#define BASE_ZOOM 0.8
#define MIN_ZOOM 0.4
/* The amount of zoom to do when "lifting" objects. */
#define LIFT_FACTOR 1.3
/* The base size of the shadow image. */
#define SHADOW_W 118
#define SHADOW_H 118
//#define RAD2DEG(x) ((x) * 57.295779513)

#define MOMENTUM_FRICTION 2000
#define ROTATE_MOMENTUM_FRICTION 30
#define ZOOM_MOMENTUM_FRICTION 8
#define TIMER_TICK 0.1

struct _Photo_Object {
     Evas_Object *ic, *shadow;
     Evas_Object *hit;
     Evas_Object *gl;

     /* 3 transit object to implement momentum animation */
     Elm_Transit *momentum;
     Elm_Transit *zoom_momentum;
     Ecore_Timer *rot_timer;
     double rot_tot_time;
     double rot_progress;
     /* bx, by - current wanted coordinates of the photo object.
      * bw, bh - original size of the "ic" object.
      * dx, dy - Used to indicate the distance between the center point
      * m_dx, m_dy - momentum delta to apply with momentum transit
      * where we put down our fingers (when started moving the item) to
      * the coords of the object, so we'll be able to calculate movement
      * correctly. */
     Evas_Coord bx, by, bw, bh, dx, dy, m_dx, m_dy;
     /* Because gesture layer only knows the amount of rotation/zoom we do
      * per gesture, we have to keep the current rotate/zoom factor and the
      * one that was before we started the gesture. */
     int base_rotate, rotate;  /* base - initial angle */
     double rot_momentum, zoom_mom;
     double zoom_mom_time;
     double base_zoom, zoom;
     double shadow_zoom;
};
typedef struct _Photo_Object Photo_Object;


/* This function applies the information from the Photo_Object to the actual
 * evas objects. Zoom/rotate factors and etc. */
static void
apply_changes(Photo_Object *po)
{
   Evas_Map *map;

   map = evas_map_new(4);
   evas_map_point_coord_set(map, 0, po->bx, po->by, 0);
   evas_map_point_coord_set(map, 1, po->bx + po->bw, po->by, 0);
   evas_map_point_coord_set(map, 2, po->bx + po->bw, po->by + po->bh, 0);
   evas_map_point_coord_set(map, 3, po->bx, po->by + po->bh, 0);
   evas_map_point_image_uv_set(map, 0, 0, 0);
   evas_map_point_image_uv_set(map, 1, po->bw, 0);
   evas_map_point_image_uv_set(map, 2, po->bw, po->bh);
   evas_map_point_image_uv_set(map, 3, 0, po->bh);
   evas_map_util_rotate(map, po->rotate,
         po->bx + po->bw / 2, po->by + po->bh /2);
   evas_map_util_zoom(map, po->zoom, po->zoom,
         po->bx + po->bw / 2, po->by + po->bh /2);
   evas_object_map_enable_set(po->ic, EINA_TRUE);
   evas_object_map_set(po->ic, map);

     {
        Evas_Map *shadow_map = evas_map_new(4);
        evas_map_point_coord_set(shadow_map, 0, po->bx, po->by, 0);
        evas_map_point_coord_set(shadow_map, 1, po->bx + po->bw, po->by, 0);
        evas_map_point_coord_set(shadow_map, 2,
              po->bx + po->bw, po->by + po->bh, 0);
        evas_map_point_coord_set(shadow_map, 3, po->bx, po->by + po->bh, 0);
        evas_map_point_image_uv_set(shadow_map, 0, 0, 0);
        evas_map_point_image_uv_set(shadow_map, 1, SHADOW_W, 0);
        evas_map_point_image_uv_set(shadow_map, 2, SHADOW_W, SHADOW_H);
        evas_map_point_image_uv_set(shadow_map, 3, 0, SHADOW_H);
        evas_map_util_rotate(shadow_map, po->rotate,
              po->bx + po->bw / 2, po->by + po->bh /2);
        evas_map_util_zoom(shadow_map, po->zoom * po->shadow_zoom,
              po->zoom * po->shadow_zoom,
              po->bx + (po->bw / 2), po->by + (po->bh / 2));
        evas_object_map_enable_set(po->shadow, EINA_TRUE);
        evas_object_map_set(po->shadow, shadow_map);
        evas_map_free(shadow_map);
     }

   /* Update the position of the hit box */
     {
        Evas_Coord minx, miny, maxx, maxy;
        int i;
        evas_object_polygon_points_clear(po->hit);
        evas_map_point_coord_get(map, 0, &minx, &miny, NULL);
        maxx = minx;
        maxy = miny;
        evas_object_polygon_point_add(po->hit, minx, miny);
        for (i = 1 ; i <= 3 ; i++)
          {
             Evas_Coord x, y;
             evas_map_point_coord_get(map, i, &x, &y, NULL);
             evas_object_polygon_point_add(po->hit, x, y);
             if (x < minx)
                minx = x;
             else if (x > maxx)
                maxx = x;

             if (y < miny)
                miny = y;
             else if (y > maxy)
                maxy = y;
          }
     }

   evas_object_raise(po->shadow);
   evas_object_raise(po->ic);
   evas_object_raise(po->hit);
   evas_map_free(map);
}

/* Zoom momentum animation */
static void
zoom_momentum_animation_operation(void *_po, Elm_Transit *transit __UNUSED__,
      double progress)
{
   Photo_Object *po = (Photo_Object *) _po;
   double time_prog = po->zoom_mom_time * progress;
   double zoom_fric = ZOOM_MOMENTUM_FRICTION;

   if (po->zoom_mom > 0)
     zoom_fric *= -1;

   /* Current = rot0 + (rotv0 * t) + (a * t^2 / 2) */
   po->zoom = po->base_zoom +
      ((po->zoom_mom * time_prog) +
      (zoom_fric * (time_prog * time_prog) / 2));
   printf("%f = %f + (%f + %f)\n", po->zoom, po->base_zoom,
         (po->zoom_mom * time_prog),
         (zoom_fric * (time_prog * time_prog) / 2));

   if (po->zoom < MIN_ZOOM)
     po->zoom = MIN_ZOOM;

   apply_changes(po);
}

static void
zoom_momentum_animation_end(void *_po, Elm_Transit *transit __UNUSED__)
{
   Photo_Object *po = (Photo_Object *) _po;
   po->base_zoom = po->zoom;
   po->zoom_momentum = NULL;
}

/* Rotate momentum animation */
static Eina_Bool
rotate_momentum_animation_operation(void *_po)
{
   Eina_Bool rc = ECORE_CALLBACK_RENEW;
   int deg_friction = ROTATE_MOMENTUM_FRICTION;
   Photo_Object *po = (Photo_Object *) _po;
   po->rot_progress += TIMER_TICK;
   if (po->rot_progress > po->rot_tot_time)
     {
        po->rot_timer = NULL;
        po->rot_progress = po->rot_tot_time;
        rc = ECORE_CALLBACK_CANCEL;
     }

   if (po->rot_momentum > 0)
     deg_friction *= -1;

   /* Current = rot0 + (rotv0 * t) + (a * t^2 / 2) */
   po->rotate = po->base_rotate -
      ((po->rot_momentum * po->rot_progress) +
      (deg_friction * (po->rot_progress * po->rot_progress) / 2));
   po->rotate = (po->rotate % 360);
   if (po->rotate < 0)
     po->rotate += 360;
   printf("%d = %d - (%f + %f)\n", po->rotate, po->base_rotate,
         (po->rot_momentum * po->rot_progress),
         (deg_friction * (po->rot_progress * po->rot_progress) / 2));

   if (rc == ECORE_CALLBACK_CANCEL)
     {
        po->base_rotate = po->rotate;
        printf("%s po->rotate=<%d>\n", __func__, po->rotate);
     }

   apply_changes(po);
   return rc;
}

/* Momentum animation */
static void
momentum_animation_operation(void *_po, Elm_Transit *transit __UNUSED__,
      double progress __UNUSED__)
{
   Photo_Object *po = (Photo_Object *) _po;
   po->bx += po->m_dx;
   po->by += po->m_dy;
   apply_changes(po);
}

static void
pic_obj_keep_inframe(void *_po)
{  /* Make sure middle is in the screen, if not, fix it. */
   /* FIXME: Use actual window sizes instead of the hardcoded
    * values */
   Photo_Object *po = (Photo_Object *) _po;

   Evas_Coord mx, my;
   mx = po->bx + (po->bw / 2);
   my = po->by + (po->bh / 2);
   if (mx < 0)
     po->bx = 0 - (po->bw / 2);
   else if (mx > 480)
     po->bx = 480 - (po->bw / 2);

   if (my < 0)
     po->by = 0 - (po->bw / 2);
   else if (my > 800)
     po->by = 800 - (po->bh / 2);
}

static void
momentum_animation_end(void *_po, Elm_Transit *transit __UNUSED__)
{
   Photo_Object *po = (Photo_Object *) _po;

   pic_obj_keep_inframe(po);
   apply_changes(po);

   po->momentum = NULL;
}

static Evas_Event_Flags
rotate_start(void *_po, void *event_info)
{
   Photo_Object *po = (Photo_Object *) _po;
   Elm_Gesture_Rotate_Info *p = (Elm_Gesture_Rotate_Info *) event_info;
   printf("rotate start <%d,%d> po->rotate=<%d> base=<%f> p->angle=<%f>\n", p->x, p->y, po->rotate,
         p->base_angle, p->angle);

   /* If there's an active animator, stop it */
   if (po->rot_timer)
     {
        po->base_rotate = po->rotate;
        ecore_timer_del(po->rot_timer);
        po->rot_timer = NULL;
     }

   return EVAS_EVENT_FLAG_NONE;
}

static Evas_Event_Flags
rotate_move(void *_po, void *event_info)
{
   Photo_Object *po = (Photo_Object *) _po;
   Elm_Gesture_Rotate_Info *p = (Elm_Gesture_Rotate_Info *) event_info;
   printf("rotate move <%d,%d> base=<%f> <%f> m=<%f>\n", p->x, p->y,
         p->base_angle, p->angle, p->momentum);
   po->rotate = po->base_rotate + (int) (p->angle - p->base_angle);

   if (po->rotate < 0)
      po->rotate += 360;
   apply_changes(po);
   return EVAS_EVENT_FLAG_NONE;
}

static Evas_Event_Flags
rotate_end(void *_po, void *event_info)
{
   Photo_Object *po = (Photo_Object *) _po;
   Elm_Gesture_Rotate_Info *r_info = (Elm_Gesture_Rotate_Info *) event_info;
   printf("rotate end <%d,%d> base=<%f> <%f> m=<%f>\n", r_info->x, r_info->y,
         r_info->base_angle, r_info->angle, r_info->momentum);
   if (po->rotate < 0)
      po->rotate += 360;

   po->base_rotate = po->rotate;

   /* Apply the rotate-momentum */
   po->rot_tot_time = fabs(r_info->momentum) / ROTATE_MOMENTUM_FRICTION;
   po->rot_momentum = r_info->momentum;
   po->rot_progress = 0.0;
   if (po->rot_momentum)
     {
        po->rot_timer = ecore_timer_add(TIMER_TICK, rotate_momentum_animation_operation, po);
     }
   return EVAS_EVENT_FLAG_NONE;
}

static Evas_Event_Flags
rotate_abort(void *_po, void *event_info)
{
   Photo_Object *po = (Photo_Object *) _po;
   Elm_Gesture_Rotate_Info *p = (Elm_Gesture_Rotate_Info *) event_info;
   printf("rotate abort <%d,%d> base=<%f> <%f>\n", p->x, p->y,
         p->base_angle, p->angle);
   po->base_rotate = po->rotate;
   if (po->rotate < 0)
      po->rotate += 360;

   return EVAS_EVENT_FLAG_NONE;
}

static Evas_Event_Flags
zoom_start(void *_po, void *event_info)
{
   Photo_Object *po = (Photo_Object *) _po;
   Elm_Gesture_Zoom_Info *p = (Elm_Gesture_Zoom_Info *) event_info;
   printf("zoom start <%d,%d> <%f>\n", p->x, p->y, p->zoom);

   /* If there's an active animator, stop it */
   if (po->zoom_momentum)
     {
        elm_transit_del(po->zoom_momentum);
        po->zoom_momentum = NULL;
     }

   return EVAS_EVENT_FLAG_NONE;
}

static Evas_Event_Flags
zoom_move(void *_po, void *event_info)
{
   Photo_Object *po = (Photo_Object *) _po;
   Elm_Gesture_Zoom_Info *p = (Elm_Gesture_Zoom_Info *) event_info;
   printf("zoom move <%d,%d> <%f> momentum=<%f>\n", p->x, p->y, p->zoom, p->momentum);
   po->zoom = po->base_zoom * p->zoom;
   apply_changes(po);
   return EVAS_EVENT_FLAG_NONE;
}

static Evas_Event_Flags
zoom_end(void *_po, void *event_info)
{
   Photo_Object *po = (Photo_Object *) _po;
   Elm_Gesture_Zoom_Info *p = (Elm_Gesture_Zoom_Info *) event_info;
   printf("zoom end/abort <%d,%d> <%f> momentum=<%f>\n", p->x, p->y,
         p->zoom, p->momentum);

   /* Apply the zoom-momentum or zoom out animator */
   double tot_time = fabs(p->momentum) / ZOOM_MOMENTUM_FRICTION;
   po->zoom_mom_time = tot_time;
   po->zoom_mom = p->momentum;
   po->base_zoom = po->zoom;
   if (po->zoom_mom)
     {
        po->zoom_momentum = elm_transit_add();
        elm_transit_duration_set(po->zoom_momentum,
              tot_time);
        elm_transit_effect_add(po->zoom_momentum,
              zoom_momentum_animation_operation, po,
              zoom_momentum_animation_end);
        elm_transit_go(po->zoom_momentum);
     }

   return EVAS_EVENT_FLAG_NONE;
}

static Evas_Event_Flags
momentum_start(void *_po, void *event_info)
{
   Photo_Object *po = (Photo_Object *) _po;
   Elm_Gesture_Momentum_Info *p = (Elm_Gesture_Momentum_Info *) event_info;
   printf("momentum_start po->rotate=<%d> <%d,%d>\n", po->rotate, p->x2, p->y2);

   /* If there's an active animator, stop it */
   if (po->momentum)
     {
        elm_transit_del(po->momentum);
        po->momentum = NULL;
     }

   po->dx = p->x2 - po->bx;
   po->dy = p->y2 - po->by;
   apply_changes(po);

   return EVAS_EVENT_FLAG_NONE;
}

static Evas_Event_Flags
momentum_move(void *_po, void *event_info)
{
   Photo_Object *po = (Photo_Object *) _po;
   Elm_Gesture_Momentum_Info *p = (Elm_Gesture_Momentum_Info *) event_info;
   printf("momentum move <%d,%d>\n", p->x2, p->y2);

   po->bx = p->x2 - po->dx;
   po->by = p->y2 - po->dy;
   apply_changes(po);

   return EVAS_EVENT_FLAG_NONE;
}

static Evas_Event_Flags
momentum_end(void *_po, void *event_info)
{
   Photo_Object *po = (Photo_Object *) _po;
   Elm_Gesture_Momentum_Info *p = (Elm_Gesture_Momentum_Info *) event_info;
   printf("momentum end po->rotate=<%d> <%d,%d> <%d,%d>\n", po->rotate, p->x2, p->y2, p->mx, p->my);
   pic_obj_keep_inframe(po);
   apply_changes(po);
   po->m_dx = p->mx / 200;
   po->m_dy = p->my / 200;

   po->momentum = elm_transit_add();
   double tot_time = sqrt((p->mx * p->mx) + (p->my * p->my))
      / MOMENTUM_FRICTION;
   printf("%s tot_time=<%f>\n", __func__, tot_time);
   elm_transit_duration_set(po->momentum, tot_time);
   elm_transit_effect_add(po->momentum, momentum_animation_operation, po,
         momentum_animation_end);
   elm_transit_go(po->momentum);
   return EVAS_EVENT_FLAG_NONE;
}

static Evas_Event_Flags
momentum_abort(void *_po, void *event_info)
{
   Photo_Object *po = (Photo_Object *) _po;
   Elm_Gesture_Momentum_Info *p = (Elm_Gesture_Momentum_Info *) event_info;
   printf("momentum abort <%d,%d> <%d,%d>\n", p->x2, p->y2, p->mx, p->my);
   pic_obj_keep_inframe(po);
   apply_changes(po);

   return EVAS_EVENT_FLAG_NONE;
}

static void
_win_del_req(void *data, Evas_Object *obj __UNUSED__,
      void *event_info __UNUSED__)
{
   Photo_Object **photo_array = (Photo_Object **) data;

   if (!photo_array)
      return;

   /* The content of the photo object is automatically deleted when the win
    * is deleted. */
   for ( ; *photo_array ; photo_array++)
      free(*photo_array);

   free(data);
}


static Photo_Object *
photo_object_add(Evas_Object *parent, Evas_Object *ic, const char *icon,
      Evas_Coord x, Evas_Coord y, Evas_Coord w, Evas_Coord h, int angle)
{
   char buf[PATH_MAX];
   Photo_Object *po;
   po = calloc(1, sizeof(*po));
   po->base_zoom = po->zoom = BASE_ZOOM;

   if (ic)
     {
        po->ic = ic;
     }
   else
     {
        po->ic = elm_icon_add(parent);
        elm_icon_file_set(po->ic, icon, NULL);
     }

   po->bx = x;
   po->by = y;
   po->bw = w;
   po->bh = h;

   /* Add shadow */
     {
        po->shadow = elm_icon_add(po->ic);
        snprintf(buf, sizeof(buf), "%s/images/pol_shadow.png", elm_app_data_dir_get());
        elm_icon_file_set(po->shadow, buf, NULL);
        evas_object_resize(po->shadow, SHADOW_W, SHADOW_H);
        evas_object_show(po->shadow);
     }

   po->hit = evas_object_polygon_add(evas_object_evas_get(parent));
   evas_object_precise_is_inside_set(po->hit, EINA_TRUE);
   evas_object_repeat_events_set(po->hit, EINA_TRUE);
   evas_object_color_set(po->hit, 0, 0, 0, 0);

   evas_object_resize(po->ic, po->bw, po->bh);
   evas_object_show(po->ic);

   evas_object_show(po->hit);

   po->gl = elm_gesture_layer_add(po->ic);
   elm_gesture_layer_hold_events_set(po->gl, EINA_TRUE);
   elm_gesture_layer_attach(po->gl, po->hit);

   /* FIXME: Add a po->rotate start so we take the first angle!!!! */
   elm_gesture_layer_cb_set(po->gl, ELM_GESTURE_MOMENTUM,
         ELM_GESTURE_STATE_START, momentum_start, po);
   elm_gesture_layer_cb_set(po->gl, ELM_GESTURE_MOMENTUM,
         ELM_GESTURE_STATE_MOVE, momentum_move, po);
   elm_gesture_layer_cb_set(po->gl, ELM_GESTURE_MOMENTUM,
         ELM_GESTURE_STATE_END, momentum_end, po);
   elm_gesture_layer_cb_set(po->gl, ELM_GESTURE_MOMENTUM,
         ELM_GESTURE_STATE_ABORT, momentum_abort, po);

   elm_gesture_layer_cb_set(po->gl, ELM_GESTURE_ZOOM,
         ELM_GESTURE_STATE_START, zoom_start, po);
   elm_gesture_layer_cb_set(po->gl, ELM_GESTURE_ZOOM,
         ELM_GESTURE_STATE_MOVE, zoom_move, po);
   elm_gesture_layer_cb_set(po->gl, ELM_GESTURE_ZOOM,
         ELM_GESTURE_STATE_END, zoom_end, po);
   elm_gesture_layer_cb_set(po->gl, ELM_GESTURE_ZOOM,
         ELM_GESTURE_STATE_ABORT, zoom_end, po);

   elm_gesture_layer_cb_set(po->gl, ELM_GESTURE_ROTATE,
         ELM_GESTURE_STATE_START, rotate_start, po);
   elm_gesture_layer_cb_set(po->gl, ELM_GESTURE_ROTATE,
         ELM_GESTURE_STATE_MOVE, rotate_move, po);
   elm_gesture_layer_cb_set(po->gl, ELM_GESTURE_ROTATE,
         ELM_GESTURE_STATE_END, rotate_end, po);
   elm_gesture_layer_cb_set(po->gl, ELM_GESTURE_ROTATE,
         ELM_GESTURE_STATE_ABORT, rotate_abort, po);

   po->rotate = po->base_rotate = angle;
   po->shadow_zoom = 1.3;

   apply_changes(po);
   return po;
}

void
test_gesture_layer3(void *data __UNUSED__, Evas_Object *obj __UNUSED__,
      void *event_info __UNUSED__)
{
   Evas_Coord w, h;
   Evas_Object *win, *bg;
   char buf[PATH_MAX];
   int ind = 0;
   Photo_Object **photo_array;
   photo_array = calloc(sizeof(*photo_array), 4);

   w = 480;
   h = 800;

   win = elm_win_add(NULL, "gesture-layer", ELM_WIN_BASIC);
   elm_win_title_set(win, "Gesture Layer");
   elm_win_autodel_set(win, EINA_TRUE);
   evas_object_resize(win, w, h);

   bg = elm_bg_add(win);
   snprintf(buf, sizeof(buf), "%s/images/wood_01.jpg", elm_app_data_dir_get());
   elm_bg_file_set(bg, buf, NULL);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   snprintf(buf, sizeof(buf), "%s/images/pol_sky.png", elm_app_data_dir_get());
   photo_array[ind++] = photo_object_add(win, NULL, buf, 50, 200, 365, 400, 0);

   photo_array[ind] = NULL;
   evas_object_smart_callback_add(win, "delete,request", _win_del_req,
         photo_array);
   evas_object_show(win);
}
#endif