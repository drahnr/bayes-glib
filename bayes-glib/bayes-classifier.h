/* bayes-classifier.h
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

#ifndef BAYES_CLASSIFIER_H
#define BAYES_CLASSIFIER_H

#include <glib-object.h>

#include "bayes-storage.h"

G_BEGIN_DECLS

#define BAYES_TYPE_CLASSIFIER            (bayes_classifier_get_type())
#define BAYES_CLASSIFIER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), BAYES_TYPE_CLASSIFIER, BayesClassifier))
#define BAYES_CLASSIFIER_CONST(obj)      (G_TYPE_CHECK_INSTANCE_CAST ((obj), BAYES_TYPE_CLASSIFIER, BayesClassifier const))
#define BAYES_CLASSIFIER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  BAYES_TYPE_CLASSIFIER, BayesClassifierClass))
#define BAYES_IS_CLASSIFIER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BAYES_TYPE_CLASSIFIER))
#define BAYES_IS_CLASSIFIER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  BAYES_TYPE_CLASSIFIER))
#define BAYES_CLASSIFIER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  BAYES_TYPE_CLASSIFIER, BayesClassifierClass))

typedef struct _BayesClassifier        BayesClassifier;
typedef struct _BayesClassifierClass   BayesClassifierClass;
typedef struct _BayesClassifierPrivate BayesClassifierPrivate;

typedef gchar **(*BayesTokenizer) (BayesClassifier *classifier,
                                   const gchar     *text,
                                   gpointer         user_data);

struct _BayesClassifier
{
   GObject parent;

   /*< private >*/
   BayesClassifierPrivate *priv;
};

struct _BayesClassifierClass
{
   GObjectClass parent_class;
};

BayesStorage    *bayes_classifier_get_storage   (BayesClassifier *classifier);
GType            bayes_classifier_get_type      (void) G_GNUC_CONST;
GList           *bayes_classifier_guess         (BayesClassifier *classifier,
                                                 const gchar     *text);
BayesClassifier *bayes_classifier_new           (void);
void             bayes_classifier_set_storage   (BayesClassifier *classifier,
                                                 BayesStorage    *storage);
void             bayes_classifier_set_tokenizer (BayesClassifier *classifier,
                                                 BayesTokenizer   tokenizer,
                                                 gpointer         user_data,
                                                 GDestroyNotify   notify);
void             bayes_classifier_train         (BayesClassifier *classifier,
                                                 const gchar     *class_name,
                                                 const gchar     *text);

G_END_DECLS

#endif /* BAYES_CLASSIFIER_H */
