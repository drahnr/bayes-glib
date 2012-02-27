/* bayes-storage-memory.h
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

#ifndef BAYES_STORAGE_MEMORY_H
#define BAYES_STORAGE_MEMORY_H

#include "bayes-storage.h"

G_BEGIN_DECLS

#define BAYES_TYPE_STORAGE_MEMORY            (bayes_storage_memory_get_type())
#define BAYES_STORAGE_MEMORY(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), BAYES_TYPE_STORAGE_MEMORY, BayesStorageMemory))
#define BAYES_STORAGE_MEMORY_CONST(obj)      (G_TYPE_CHECK_INSTANCE_CAST ((obj), BAYES_TYPE_STORAGE_MEMORY, BayesStorageMemory const))
#define BAYES_STORAGE_MEMORY_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  BAYES_TYPE_STORAGE_MEMORY, BayesStorageMemoryClass))
#define BAYES_IS_STORAGE_MEMORY(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BAYES_TYPE_STORAGE_MEMORY))
#define BAYES_IS_STORAGE_MEMORY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  BAYES_TYPE_STORAGE_MEMORY))
#define BAYES_STORAGE_MEMORY_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  BAYES_TYPE_STORAGE_MEMORY, BayesStorageMemoryClass))

typedef struct _BayesStorageMemory        BayesStorageMemory;
typedef struct _BayesStorageMemoryClass   BayesStorageMemoryClass;
typedef struct _BayesStorageMemoryPrivate BayesStorageMemoryPrivate;

struct _BayesStorageMemory
{
   GObject parent;

   /*< private >*/
   BayesStorageMemoryPrivate *priv;
};

struct _BayesStorageMemoryClass
{
   GObjectClass parent_class;
};

GType         bayes_storage_memory_get_type (void) G_GNUC_CONST;
BayesStorage *bayes_storage_memory_new      (void);

G_END_DECLS

#endif /* BAYES_STORAGE_MEMORY_H */
