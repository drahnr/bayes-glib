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

void
bayes_storage_add_token (BayesStorage *storage,
                         const gchar  *class_name,
                         const gchar  *token,
                         guint         count)
{
   BAYES_STORAGE_GET_INTERFACE(storage)->add_token(storage,
                                                   class_name,
                                                   token,
                                                   count);
}

/**
 * bayes_storage_get_token:
 * @storage: (in): A #BayesStorage.
 * @class_name: (in): The name of the classification to lookup.
 * @token: (in) (allow-none): The token to check, or %NULL for all tokens.
 *
 * Retrieves the number of times @token has been found in the training
 * data. If @token is %NULL, the count of all items in the classification
 * will be retrieved.
 *
 * Returns: A #guint containing the count of all items.
 */
guint
bayes_storage_get_token (BayesStorage *storage,
                         const gchar  *class_name,
                         const gchar  *token)
{
   return BAYES_STORAGE_GET_INTERFACE(storage)->get_token(storage,
                                                          class_name,
                                                          token);
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
