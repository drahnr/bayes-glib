/* bayes-storage.h
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

#ifndef BAYES_STORAGE_H
#define BAYES_STORAGE_H

#include <glib-object.h>

G_BEGIN_DECLS

#define BAYES_TYPE_STORAGE             (bayes_storage_get_type())
#define BAYES_STORAGE(o)               (G_TYPE_CHECK_INSTANCE_CAST((o),    BAYES_TYPE_STORAGE, BayesStorage))
#define BAYES_IS_STORAGE(o)            (G_TYPE_CHECK_INSTANCE_TYPE((o),    BAYES_TYPE_STORAGE))
#define BAYES_STORAGE_GET_INTERFACE(o) (G_TYPE_INSTANCE_GET_INTERFACE((o), BAYES_TYPE_STORAGE, BayesStorageIface))

typedef struct _BayesStorage      BayesStorage;
typedef struct _BayesStorageIface BayesStorageIface;

struct _BayesStorageIface
{
   GTypeInterface parent;

   /* interface methods */
   void    (*add_token_count)       (BayesStorage *storage,
                                     const gchar  *class_name,
                                     const gchar  *token,
                                     guint         count);
   gchar **(*get_class_names)       (BayesStorage *storage);
   guint   (*get_token_count)       (BayesStorage *storage,
                                     const gchar  *class_name,
                                     const gchar  *token);
   gdouble (*get_token_probability) (BayesStorage *storage,
                                     const gchar  *class_name,
                                     const gchar  *token);
};

void      bayes_storage_add_token             (BayesStorage *storage,
                                               const gchar  *class_name,
                                               const gchar  *token);
void      bayes_storage_add_token_count       (BayesStorage *storage,
                                               const gchar  *class_name,
                                               const gchar  *token,
                                               guint         count);
gchar   **bayes_storage_get_class_names       (BayesStorage *storage);
GType     bayes_storage_get_type              (void) G_GNUC_CONST;
guint     bayes_storage_get_token_count       (BayesStorage *storage,
                                               const gchar  *class_name,
                                               const gchar  *token);
gdouble   bayes_storage_get_token_probability (BayesStorage *storage,
                                               const gchar  *class_name,
                                               const gchar  *token);

G_END_DECLS

#endif /* BAYES_STORAGE_H */
