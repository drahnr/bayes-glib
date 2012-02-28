#include "bayes-glib/bayes-guess.h"

static void
test1 (void)
{
   BayesGuess *guess;

   guess = bayes_guess_new("english", 0.4567);
   g_assert_cmpstr("english", ==, bayes_guess_get_name(guess));
   g_assert_cmpint(0.4567, ==, bayes_guess_get_probability(guess));
   bayes_guess_unref(guess);
}

gint
main (gint   argc,
      gchar *argv[])
{
   g_test_init(&argc, &argv, NULL);
   g_type_init();
   g_test_add_func("/Guess/basic", test1);
   return g_test_run();
}
