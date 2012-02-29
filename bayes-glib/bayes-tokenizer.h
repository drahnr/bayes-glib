/* bayes-tokenizer.h
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

#ifndef BAYES_TOKENIZER_H
#define BAYES_TOKENIZER_H

#include <glib.h>

G_BEGIN_DECLS

/**
 * BayesTokenizer:
 * @text: (in): The text to tokenize.
 * @user_data: (in): User data provided during registration.
 *
 * #BayesTokenizer is a callback that can be used to tokenize
 * a piece of text into individual tokens. This is used by a
 * #BayesClassifier to convert the input text into a form that
 * may be used when training or guessing probabilities.
 *
 * The caller is responsible for freeing the result with
 * g_strfreev().
 *
 * Returns: (transfer full): A newly allocated #GStrv.
 */
typedef gchar **(*BayesTokenizer) (const gchar *text,
                                   gpointer     user_data);

gchar **bayes_tokenizer_word (const gchar *text,
                              gpointer     user_data);


G_END_DECLS

#endif /* BAYES_TOKENIZER_H */
