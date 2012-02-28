/* bayes-guess.c
 *
 * Copyright (C) 2012 Christian Hergert <chris@dronelabs.com>
 *
 * This file is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This file is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "bayes-guess.h"

struct _BayesGuess
{
   volatile gint ref_count;
   gdouble probability;
   gchar *name;
};

BayesGuess *
bayes_guess_new (const gchar *name,
                 gdouble      probability)
{
   BayesGuess *guess;

   guess = g_slice_new0(BayesGuess);
   guess->ref_count = 1;
   guess->name = g_strdup(name);
   guess->probability = probability;

   return guess;
}

BayesGuess *
bayes_guess_ref (BayesGuess *guess)
{
   g_return_val_if_fail(guess != NULL, NULL);
   g_return_val_if_fail(guess->ref_count > 0, NULL);

   g_atomic_int_inc(&guess->ref_count);
   return guess;
}

void
bayes_guess_unref (BayesGuess *guess)
{
   g_return_if_fail(guess != NULL);
   g_return_if_fail(guess->ref_count > 0);

   if (g_atomic_int_dec_and_test(&guess->ref_count)) {
      g_free(guess->name);
      g_slice_free(BayesGuess, guess);
   }
}

const gchar *
bayes_guess_get_name (BayesGuess *guess)
{
   g_return_val_if_fail(guess, NULL);
   return guess->name;
}

gdouble
bayes_guess_get_probability (BayesGuess *guess)
{
   g_return_val_if_fail(guess, 0.0);
   return guess->probability;
}

GType
bayes_guess_get_type (void)
{
   static gsize initialized = FALSE;
   static GType type_id;

   if (g_once_init_enter(&initialized)) {
      type_id = g_boxed_type_register_static("BayesGuess",
                                             (GBoxedCopyFunc)bayes_guess_ref,
                                             (GBoxedFreeFunc)bayes_guess_unref);
      g_once_init_leave(&initialized, TRUE);
   }

   return type_id;
}
