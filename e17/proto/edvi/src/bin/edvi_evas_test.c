#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <Evas.h>
#include <Ecore.h>
#include <Ecore_Evas.h>

#include "Edvi.h"

#include "config.h"

#if HAVE___ATTRIBUTE__
#define __UNUSED__ __attribute__((unused))
#else
#define __UNUSED__
#endif

#define DEFAULT_POLLING_INTERVAL   30      /*instructions/polling*/


static void app_resize(Ecore_Evas *ee);
static int app_signal_exit(void *data, int ev_type, void *ev);
static void app_delete_request(Ecore_Evas *ee);


void
_evas_object_bg_set (Evas_Object *o, int r, int g, int b)
{
  unsigned int *m;
  int           w, h;
  int           i, j;

  evas_object_image_size_get (o, &w, &h);
  evas_object_image_fill_set (o, 0, 0, w, h);
  m = (unsigned int *)evas_object_image_data_get (o, 1);
  if (!m)
    return;

  for (j = 0; j < h; j++) {
    for (i = 0; i < w; i++) {
      m[j * w + i] = (255 << 24) | (r << 16) | (g << 8) | b;
    }
  }
  evas_object_image_data_update_add (o, 0, 0, w, h);
  evas_object_resize (o, w, h);
}

int
main (int argc, char * argv[])
{
  Edvi_Device   *device;
  Edvi_Property *property;
  Edvi_Document *document;
  Edvi_Page     *page;
  Ecore_Evas    *ee;
  Evas          *evas;
  Evas_Object   *o;
  char          *param_kpathsea_mode  = "cx";
  int            page_number;

  if (argc < 3) {
    printf ("\nUsage: %s file.dvi page_number\n\n", argv[0]);
    return EXIT_FAILURE;
  }

  printf ("[DVI] version : %s\n", edvi_version_get ());

  if (!edvi_init (300, param_kpathsea_mode, 4,
                  1.0, 1.0,
                  0, 255, 255, 255, 0, 0, 0)) {
    return EXIT_FAILURE;
  }

  device = edvi_device_new (edvi_dpi_get (), edvi_dpi_get ());
  if (!device) {
    goto shutdown;
  }

  property = edvi_property_new ();
  if (!property) {
    goto free_device;
  }
  edvi_property_property_set (property, EDVI_PROPERTY_DELAYED_FONT_OPEN);

  document = edvi_document_new (argv[1], device, property);
  if (!document) {
    goto free_property;
  }

  sscanf (argv[2], "%d", &page_number);
  page = edvi_page_new (document, page_number);
  if (!page) {
    goto free_document;
  }

  /* evas stuff */
  if (!evas_init()) {
    goto free_page;
  }
  if (!ecore_init()) {
    goto shutdown_evas;
  }

  if (!ecore_evas_init()) {
    goto shutdown_ecore;
  }

  ee = ecore_evas_software_x11_new (NULL, 0,  0, 0, 600, 850);
  ecore_event_handler_add (ECORE_EVENT_SIGNAL_EXIT, app_signal_exit, NULL);
  ecore_evas_callback_delete_request_set (ee, app_delete_request);
  ecore_evas_title_set (ee, "Evas Dvi Test");
  ecore_evas_name_class_set (ee, "evas_dvi_test", "test_evas_dvi");
  ecore_evas_callback_resize_set (ee, app_resize);
  ecore_evas_show (ee);

  evas = ecore_evas_get (ee);

  o = evas_object_image_add (evas);
  evas_object_move (o, 0, 0);
  evas_object_image_size_set (o, edvi_page_width_get (page), edvi_page_height_get (page));
  _evas_object_bg_set (o, 255, 255, 255);
  edvi_page_render (page, device, o);
  evas_object_show (o);
  ecore_evas_resize (ee, edvi_page_width_get (page), edvi_page_height_get (page));

  ecore_main_loop_begin ();

  ecore_evas_shutdown ();
  ecore_shutdown ();
  evas_shutdown ();
  edvi_page_delete (page);
  edvi_document_delete (document);
  edvi_property_delete (property);
  edvi_device_delete (device);
  edvi_shutdown ();

  return EXIT_SUCCESS;

 shutdown_ecore:
  ecore_shutdown ();
 shutdown_evas:
  evas_shutdown ();
 free_page:
  edvi_page_delete (page);
 free_document:
  edvi_document_delete (document);
 free_property:
  edvi_property_delete (property);
 free_device:
  edvi_device_delete (device);
 shutdown:
  edvi_shutdown ();

  return EXIT_FAILURE;
}

static void
app_resize(Ecore_Evas *ee)
{
   Evas_Coord w, h;
   Evas *evas;

   evas = ecore_evas_get(ee);
   evas_output_viewport_get(evas, NULL, NULL, &w, &h);
/*    bg_resize(w, h); */
}

static int
app_signal_exit(void *data __UNUSED__, int ev_type __UNUSED__, void *ev __UNUSED__)
{
   ecore_main_loop_quit();
   return 1;
}

static void
app_delete_request(Ecore_Evas *ee __UNUSED__)
{
   ecore_main_loop_quit();
}
