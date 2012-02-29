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

/**
 * SECTION:bayes-guess
 * @title: BayesGuess
 * @short_description: An immutable structure containing a probability.
 * @see_also: bayes_guess_get_name() bayes_guess_get_probability()
 *
 * #BayesGuess represents a guess at the classification of given input
 * data. The guess also includes the probability calculated for the input
 * being the classification. The guess structure contains the classification
 * name and a probability between 0.0 and 1.0.
 *
 * The #BayesGuess structure is a reference counted #GBoxed type. You can
 * reference the structure with bayes_guess_ref() and free the structure
 * with bayes_guess_unref().
 *
 */

struct _BayesGuess
{
   volatile gint ref_count;
   gdouble probability;
   gchar *name;
};

/**
 * bayes_guess_new:
 * @name: (in): The name of the classification.
 * @probability: (in): The probability of the classification.
 *
 * Creates a new #BayesGuess that is the probability of a given
 * classification.
 *
 * Returns: (transfer full): A newly allocated #BayesGuess.
 */
BayesGuess *
bayes_guess_new (const gchar *name,
                 gdouble      probability)
{
   BayesGuess *guess;

   g_return_val_if_fail(name, NULL);

   guess = g_slice_new0(BayesGuess);
   guess->ref_count = 1;
   guess->name = g_strdup(name);
   guess->probability = CLAMP(probability, 0.0, 1.0);

   return guess;
}

/**
 * bayes_guess_ref:
 * @guess: (in): A #BayesGuess.
 *
 * Increments the reference count of @guess by one.
 *
 * Returns: The instance provided, @guess.
 */
BayesGuess *
bayes_guess_ref (BayesGuess *guess)
{
   g_return_val_if_fail(guess != NULL, NULL);
   g_return_val_if_fail(guess->ref_count > 0, NULL);

   g_atomic_int_inc(&guess->ref_count);
   return guess;
}

/**
 * bayes_guess_unref:
 * @guess: (in): A #BayesGuess.
 *
 * Decrements the reference count of @guess by one. Once the reference count
 * reaches zero, the structure and allocated resources are released.
 */
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

/**
 * bayes_guess_get_name:
 * @guess: (in): A #BayesGuess.
 *
 * Retrieves the classification name for which this guess represents.
 *
 * Returns: A string which should not be modified or freed.
 */
const gchar *
bayes_guess_get_name (BayesGuess *guess)
{
   g_return_val_if_fail(guess, NULL);
   return guess->name;
}

/**
 * bayes_guess_get_probability:
 * @guess: (in): A #BayesGuess.
 *
 * Retrieves the probability that the input data matches the classification
 * this guess represents.
 *
 * Returns: A #gdouble between 0.0 and 1.0.
 */
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
