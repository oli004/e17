#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <Ecore_X.h>

#include "ecore_suite.h"

/* FIXME: Currently disable these tests. They are useless ATM and they just
 * make buildbot complain. Once we add useful tests here we'll also bother
 * with getting X on the server. */
#undef HAVE_ECORE_X_XLIB

/* TODO: change to HAVE_ECORE_X when xcb implementation is done */
#ifdef HAVE_ECORE_X_XLIB

START_TEST(ecore_test_ecore_x_init)
{
   int ret;

   ret = ecore_x_init(NULL);
   fail_if(ret != 1);

   ret = ecore_x_shutdown();
   fail_if(ret != 0);
}
END_TEST

START_TEST(ecore_test_ecore_x_bell)
{
   int i;
   int ret;

   ret = ecore_x_init(NULL);
   fail_if(ret != 1);

   printf("You should hear 3 beeps now.\n");
   for (i = 0; i < 3; i++)
     {
	ret = ecore_x_bell(0);
	fail_if(ret != EINA_TRUE);
	ecore_x_sync();
	sleep(1);
     }

   ecore_x_shutdown();
}
END_TEST

#endif

void ecore_test_ecore_x(TCase *tc __UNUSED__)
{

/* TODO: change to HAVE_ECORE_X when xcb implementation is done */
#ifdef HAVE_ECORE_X_XLIB
   tcase_add_test(tc, ecore_test_ecore_x_init);
   tcase_add_test(tc, ecore_test_ecore_x_bell);
#endif
}
