/* bayes-guess.h
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

#ifndef BAYES_GUESS_H
#define BAYES_GUESS_H

#include <glib-object.h>

G_BEGIN_DECLS

#define BAYES_TYPE_GUESS (bayes_guess_get_type())

typedef struct _BayesGuess BayesGuess;

const gchar *bayes_guess_get_name        (BayesGuess *guess);
gdouble      bayes_guess_get_probability (BayesGuess *guess);
GType        bayes_guess_get_type        (void) G_GNUC_CONST;
BayesGuess  *bayes_guess_new             (const gchar *name,
                                          gdouble      probability);
BayesGuess  *bayes_guess_ref             (BayesGuess *guess);
void         bayes_guess_unref           (BayesGuess *guess);

G_END_DECLS

#endif /* BAYES_GUESS_H */

