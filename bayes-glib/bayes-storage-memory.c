/* bayes-storage-memory.c
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

#include <glib/gi18n.h>

#include "bayes-storage-memory.h"

static void bayes_storage_init (BayesStorageIface *iface);

G_DEFINE_TYPE_EXTENDED(BayesStorageMemory,
                       bayes_storage_memory,
                       G_TYPE_OBJECT,
                       0,
                       G_IMPLEMENT_INTERFACE(BAYES_TYPE_STORAGE,
                                             bayes_storage_init))

struct _BayesStorageMemoryPrivate
{
   GHashTable *classes;
};

/**
 * bayes_storage_memory_new:
 *
 * Creates a new #BayesStorageMemory instance.
 *
 * Returns: (transfer full): A #BayesStorageMemory.
 */
BayesStorage *
bayes_storage_memory_new (void)
{
   return g_object_new(BAYES_TYPE_STORAGE_MEMORY, NULL);
}

static void
bayes_storage_memory_add_token (BayesStorage *storage,
                                const gchar  *class_name,
                                const gchar  *token,
                                guint         count)
{
   BayesStorageMemoryPrivate *priv;
   BayesStorageMemory *memory = (BayesStorageMemory *)storage;
   GHashTable *class_hash;
   guint *token_count;

   g_return_if_fail(BAYES_IS_STORAGE_MEMORY(memory));
   g_return_if_fail(class_name);
   g_return_if_fail(token);

   priv = memory->priv;

   /*
    * Get the classification hashtable or create it if necessary.
    */
   if (!(class_hash = g_hash_table_lookup(priv->classes, class_name))) {
      class_hash = g_hash_table_new_full(g_str_hash, g_str_equal,
                                         g_free, g_free);
      g_hash_table_insert(priv->classes, g_strdup(class_name), class_hash);
   }

   /*
    * Get the container for the token count or create it if necessary.
    */
   if (!(token_count = g_hash_table_lookup(class_hash, token))) {
      token_count = g_new0(guint, 1);
      g_hash_table_insert(class_hash, g_strdup(token), token_count);
   }

   /*
    * Increment the count of the token.
    */
   *token_count += count;
}

static guint
bayes_storage_memory_get_token (BayesStorage *storage,
                                const gchar  *class_name,
                                const gchar  *token)
{
   BayesStorageMemoryPrivate *priv;
   BayesStorageMemory *memory = (BayesStorageMemory *)storage;
   GHashTable *class_hash;
   guint *token_count;

   g_return_val_if_fail(BAYES_IS_STORAGE_MEMORY(memory), 0);
   g_return_val_if_fail(class_name, 0);
   g_return_val_if_fail(token, 0);

   priv = memory->priv;

   if ((class_hash = g_hash_table_lookup(priv->classes, class_name))) {
      if ((token_count = g_hash_table_lookup(class_hash, token))) {
         return *token_count;
      }
   }

   return 0;
}

/**
 * bayes_storage_memory_finalize:
 * @object: (in): A #BayesStorageMemory.
 *
 * Finalizer for a #BayesStorageMemory instance. Frees any resources held by
 * the instance.
 */
static void
bayes_storage_memory_finalize (GObject *object)
{
   BayesStorageMemoryPrivate *priv = BAYES_STORAGE_MEMORY(object)->priv;

   g_hash_table_unref(priv->classes);

   G_OBJECT_CLASS(bayes_storage_memory_parent_class)->finalize(object);
}

/**
 * bayes_storage_memory_class_init:
 * @klass: (in): A #BayesStorageMemoryClass.
 *
 * Initializes the #BayesStorageMemoryClass and prepares the vtable.
 */
static void
bayes_storage_memory_class_init (BayesStorageMemoryClass *klass)
{
   GObjectClass *object_class;

   object_class = G_OBJECT_CLASS(klass);
   object_class->finalize = bayes_storage_memory_finalize;
   g_type_class_add_private(object_class, sizeof(BayesStorageMemoryPrivate));
}

/**
 * bayes_storage_memory_init:
 * @: (in): A #BayesStorageMemory.
 *
 * Initializes the newly created #BayesStorageMemory instance.
 */
static void
bayes_storage_memory_init (BayesStorageMemory *memory)
{
   memory->priv =
      G_TYPE_INSTANCE_GET_PRIVATE(memory,
                                  BAYES_TYPE_STORAGE_MEMORY,
                                  BayesStorageMemoryPrivate);
   memory->priv->classes =
      g_hash_table_new_full(g_str_hash,
                            g_str_equal,
                            g_free,
                            (GDestroyNotify)g_hash_table_unref);
}

static void
bayes_storage_init (BayesStorageIface *iface)
{
   iface->add_token = bayes_storage_memory_add_token;
   iface->get_token = bayes_storage_memory_get_token;
}
