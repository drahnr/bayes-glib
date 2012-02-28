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

typedef struct
{
   GHashTable *tokens;
   guint count;
} Tokens;

struct _BayesStorageMemoryPrivate
{
   GHashTable *classes;
   Tokens *corpus;
};

static void
tokens_free (gpointer data)
{
   Tokens *tokens = data;

   if (tokens) {
      g_hash_table_unref(tokens->tokens);
      g_free(tokens);
   }
}

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
tokens_inc (Tokens      *tokens,
            const gchar *token,
            guint        count)
{
   guint *token_count;

   /*
    * Get the container for the token count or create it if necessary.
    */
   if (!(token_count = g_hash_table_lookup(tokens->tokens, token))) {
      token_count = g_new0(guint, 1);
      g_hash_table_insert(tokens->tokens, g_strdup(token), token_count);
   }

   /*
    * Increment the count of the token.
    */
   *token_count += count;
   tokens->count += count;
}

/**
 * bayes_storage_memory_add_token_count:
 * @storage: (in): A #BayesStorageMemory.
 * @class_name: (in): A string containing the classification.
 * @token: (in): The token to add.
 * @count: (in): The number of times to add @token.
 *
 * Adds @token to the classification @class_name @count times.
 */
static void
bayes_storage_memory_add_token_count (BayesStorage *storage,
                                      const gchar  *class_name,
                                      const gchar  *token,
                                      guint         count)
{
   BayesStorageMemoryPrivate *priv;
   BayesStorageMemory *memory = (BayesStorageMemory *)storage;
   Tokens *tokens;

   g_return_if_fail(BAYES_IS_STORAGE_MEMORY(memory));
   g_return_if_fail(class_name);
   g_return_if_fail(token);

   priv = memory->priv;

   /*
    * Get the classification hashtable or create it if necessary.
    */
   if (!(tokens = g_hash_table_lookup(priv->classes, class_name))) {
      tokens = g_new0(Tokens, 1);
      tokens->tokens = g_hash_table_new_full(g_str_hash, g_str_equal,
                                             g_free, g_free);
      g_hash_table_insert(priv->classes, g_strdup(class_name), tokens);
   }

   tokens_inc(tokens, token, count);
   tokens_inc(priv->corpus, token, count);
}

/**
 * bayes_storage_memory_get_token_count:
 * @storage: (in): A #BayesStorageMemory.
 * @class_name: (in): A string containing a classification.
 * @token: (in): A string containing the token.
 *
 * Gets the count of times @token was found in the classification named
 * @class_name.
 *
 * Returns: a #guint containing the count of @token in the classification.
 */
static guint
bayes_storage_memory_get_token_count (BayesStorage *storage,
                                      const gchar  *class_name,
                                      const gchar  *token)
{
   BayesStorageMemoryPrivate *priv;
   BayesStorageMemory *memory = (BayesStorageMemory *)storage;
   Tokens *tokens;
   guint *token_count;

   g_return_val_if_fail(BAYES_IS_STORAGE_MEMORY(memory), 0);

   priv = memory->priv;

   tokens = class_name ? g_hash_table_lookup(priv->classes, class_name)
                       : priv->corpus;
   if (tokens) {
      if (!token) {
         return tokens->count;
      } else if ((token_count = g_hash_table_lookup(tokens->tokens, token))) {
         return *token_count;
      }
   }

   return 0;
}

/**
 * bayes_storage_memory_get_token_probability:
 * @storage: (in): A #BayesStorageMemory.
 * @class_name: (in): A string containing the classification name.
 * @token: (in): The token to calculate.
 *
 * Calculates the probability that @token indicates the guess of
 * @class_name classification.
 *
 * Returns: A #gdouble between 0.0 and 1.0.
 */
static gdouble
bayes_storage_memory_get_token_probability (BayesStorage *storage,
                                            const gchar  *class_name,
                                            const gchar  *token)
{
   BayesStorageMemoryPrivate *priv;
   BayesStorageMemory *memory = (BayesStorageMemory *)storage;
   gdouble pool_count;
   gdouble them_count;
   gdouble tot_count;
   gdouble this_count;
   gdouble other_count;
   gdouble good_metric;
   gdouble bad_metric;
   gdouble f;
   Tokens *tokens;

   g_return_val_if_fail(BAYES_IS_STORAGE_MEMORY(memory), 0.0);
   g_return_val_if_fail(class_name, 0.0);
   g_return_val_if_fail(token, 0.0);

   priv = memory->priv;

   if (!(tokens = g_hash_table_lookup(priv->classes, class_name))) {
      return 0.0;
   }

   pool_count = tokens->count;
   them_count = MAX(priv->corpus->count - pool_count, 1);
   this_count = bayes_storage_get_token_count(storage, class_name, token);
   tot_count = bayes_storage_get_token_count(storage, NULL, token);
   other_count = tot_count - this_count;
   good_metric = (!pool_count) ? 1.0 : MIN(1.0, other_count / pool_count);
   bad_metric = MIN(1.0, this_count / them_count);
   f = bad_metric / (good_metric + bad_metric);

   if (ABS(f - 0.5) >= 0.1) {
       return MAX(0.0001, MIN(0.9999, f));
   }

   return 0.0;
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
   tokens_free(priv->corpus);

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
   Tokens *tokens;

   memory->priv =
      G_TYPE_INSTANCE_GET_PRIVATE(memory,
                                  BAYES_TYPE_STORAGE_MEMORY,
                                  BayesStorageMemoryPrivate);

   memory->priv->classes =
      g_hash_table_new_full(g_str_hash, g_str_equal,
                            g_free, tokens_free);

   tokens = g_new0(Tokens, 1);
   tokens->tokens = g_hash_table_new_full(g_str_hash, g_str_equal,
                                          g_free, g_free);
   memory->priv->corpus = tokens;
}

static void
bayes_storage_init (BayesStorageIface *iface)
{
   iface->add_token_count = bayes_storage_memory_add_token_count;
   iface->get_token_count = bayes_storage_memory_get_token_count;
   iface->get_token_probability = bayes_storage_memory_get_token_probability;
}
