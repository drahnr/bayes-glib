/* bayes-storage.c
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

#include "bayes-storage.h"

/**
 * bayes_storage_add_token_count:
 * @storage: (in): A #BayesStorage.
 * @class_name: (in): The classification to store the token in.
 * @token: (in): The token to add.
 * @count: (in): The count of times @token was found.
 *
 * This function will add a token to the storage, applying it to the given
 * classification. @count should indiciate the number of times that the
 * token was found.
 */
void
bayes_storage_add_token_count (BayesStorage *storage,
                               const gchar  *class_name,
                               const gchar  *token,
                               guint         count)
{
   g_return_if_fail(BAYES_IS_STORAGE(storage));
   g_return_if_fail(class_name);
   g_return_if_fail(token);
   g_return_if_fail(count);

   BAYES_STORAGE_GET_INTERFACE(storage)->
      add_token_count(storage, class_name, token, count);
}

/**
 * bayes_storage_add_token:
 * @storage: (in): A #BayesStorage.
 * @class_name: (in): The classification to store the token in.
 * @token: (in): The token to add.
 *
 * This function will add a token to the storage, applying it to the given
 * classification.
 */
void
bayes_storage_add_token (BayesStorage *storage,
                         const gchar  *class_name,
                         const gchar  *token)
{
   g_return_if_fail(BAYES_IS_STORAGE(storage));
   g_return_if_fail(class_name);
   g_return_if_fail(token);

   BAYES_STORAGE_GET_INTERFACE(storage)->
      add_token_count(storage, class_name, token, 1);
}

/**
 * bayes_storage_get_class_names:
 * @storage: (in): A #BayesStorage.
 *
 * Retrieves the names of the classifications trained in this storage
 * instance.
 *
 * Returns: (transfer full): A #GStrv of class names.
 */
gchar **
bayes_storage_get_class_names (BayesStorage *storage)
{
   g_return_val_if_fail(BAYES_IS_STORAGE(storage), NULL);
   return BAYES_STORAGE_GET_INTERFACE(storage)->get_class_names(storage);
}

/**
 * bayes_storage_get_token_count:
 * @storage: (in): A #BayesStorage.
 * @class_name: (in) (allow-none): The classification or %NULL for all.
 * @token: (in) (allow-none): The token or %NULL for all.
 *
 * Retrieves the number of times @token has been found in the training
 * data. If @token is %NULL, the count of all items in the classification
 * will be retrieved. If @class_name is %NULL, then the count of all the
 * instances of @token in the all the classifications.
 *
 * Returns: A #guint containing the count of all items.
 */
guint
bayes_storage_get_token_count (BayesStorage *storage,
                               const gchar  *class_name,
                               const gchar  *token)
{
   g_return_val_if_fail(BAYES_IS_STORAGE(storage), 0);
   g_return_val_if_fail(class_name || token, 0);

   return BAYES_STORAGE_GET_INTERFACE(storage)->
      get_token_count(storage, class_name, token);
}

/**
 * bayes_storage_get_token_probability:
 * @storage: (in): A #BayesStorage.
 * @class_name: (in): The classification.
 * @token: (in): The desired token.
 *
 * Checks to see the probability of a token being a given classification.
 *
 * Returns: A #gdouble between 0.0 and 1.0 containing the probability.
 */
gdouble
bayes_storage_get_token_probability (BayesStorage *storage,
                                     const gchar  *class_name,
                                     const gchar  *token)
{
   g_return_val_if_fail(BAYES_IS_STORAGE(storage), 0.0);
   g_return_val_if_fail(class_name, 0.0);
   g_return_val_if_fail(token, 0.0);

   return BAYES_STORAGE_GET_INTERFACE(storage)->
      get_token_probability(storage, class_name, token);
}

GType
bayes_storage_get_type (void)
{
   static GType type_id = 0;

   if (g_once_init_enter((gsize *)&type_id)) {
      GType _type_id;
      const GTypeInfo g_type_info = {
         sizeof(BayesStorageIface),
         NULL, /* base_init */
         NULL, /* base_finalize */
         NULL, /* class_init */
         NULL, /* class_finalize */
         NULL, /* class_data */
         0,    /* instance_size */
         0,    /* n_preallocs */
         NULL, /* instance_init */
         NULL  /* value_vtable */
      };

      _type_id = g_type_register_static(G_TYPE_INTERFACE,
                                        "BayesStorage",
                                        &g_type_info,
                                        0);
      g_type_interface_add_prerequisite(_type_id, G_TYPE_OBJECT);
      g_once_init_leave((gsize *)&type_id, _type_id);
   }

   return type_id;
}
