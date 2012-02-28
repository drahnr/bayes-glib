/* bayes-classifier.c
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

#include "bayes-classifier.h"
#include "bayes-guess.h"
#include "bayes-storage-memory.h"

G_DEFINE_TYPE(BayesClassifier, bayes_classifier, G_TYPE_OBJECT)

struct _BayesClassifierPrivate
{
   BayesStorage *storage;

   BayesTokenizer token_func;
   gpointer       token_user_data;
   GDestroyNotify token_notify;
};

enum
{
   PROP_0,
   PROP_STORAGE,
   LAST_PROP
};

static GParamSpec *gParamSpecs[LAST_PROP];
static GRegex     *gWordRegex;

static gchar **
bayes_classifier_tokenize (BayesClassifier *classifier,
                           const gchar     *text)
{
   BayesClassifierPrivate *priv;

   g_return_val_if_fail(BAYES_IS_CLASSIFIER(classifier), NULL);
   g_return_val_if_fail(text, NULL);

   priv = classifier->priv;
   return priv->token_func(classifier, text, priv->token_user_data);
}

/**
 * bayes_classifier_train:
 * @classifier: (in): A #BayesClassifier.
 * @class_name: (in): The classification for @text.
 * @text: (in): Text to tokenize and store for guessing.
 *
 * Tokenizes @text and stores the values under the classification named
 * @class_name. These are used by bayes_classifier_guess() to determine
 * the classification.
 */
void
bayes_classifier_train (BayesClassifier *classifier,
                        const gchar     *class_name,
                        const gchar     *text)
{
   BayesClassifierPrivate *priv;
   gchar **tokens;
   guint i;

   g_return_if_fail(BAYES_IS_CLASSIFIER(classifier));
   g_return_if_fail(class_name);
   g_return_if_fail(text);

   priv = classifier->priv;

   if ((tokens = bayes_classifier_tokenize(classifier, text))) {
      for (i = 0; tokens[i]; i++) {
         bayes_storage_add_token(priv->storage, class_name, tokens[i]);
      }
      g_strfreev(tokens);
   }
}

/**
 * bayes_classifier_guess:
 * @classifier: (in): A #BayesClassifier.
 * @text: (in): Text to tokenize and guess the classification.
 *
 * Tries to guess the classification of @text by tokenizing the text
 * and testing against the classifiers that have been trained.
 *
 * @accuracy is set with the accuracy of the guess.
 *
 * Returns: (transfer full) (element-type BayesGuess*): The guesses.
 */
GList *
bayes_classifier_guess (BayesClassifier *classifier,
                        const gchar     *text)
{
   GList *ret = NULL;
   gchar **tokens;

   g_return_val_if_fail(BAYES_IS_CLASSIFIER(classifier), NULL);
   g_return_val_if_fail(text, NULL);

   tokens = bayes_classifier_tokenize(classifier, text);

   /*
    * TODO: Uh, everything.
    */

   g_strfreev(tokens);

   return ret;
}

/**
 * bayes_classifier_get_storage:
 * @classifier: (in): A #BayesClassifier.
 *
 * Retrieves the #BayesStorage used for tokens by @classifier.
 *
 * Returns: (transfer none): A #BayesStorage.
 */
BayesStorage *
bayes_classifier_get_storage (BayesClassifier *classifier)
{
   g_return_val_if_fail(BAYES_IS_CLASSIFIER(classifier), NULL);
   return classifier->priv->storage;
}

/**
 * bayes_classifier_set_storage:
 * @classifier: (in): A #BayesClassifier.
 * @storage: (in) (allow-none): A #BayesStorage or %NULL.
 *
 * Sets the storage to use for tokens by the classifier.
 * If @storage is %NULL, then in memory storage will be used.
 */
void
bayes_classifier_set_storage (BayesClassifier *classifier,
                              BayesStorage    *storage)
{
   BayesClassifierPrivate *priv;

   g_return_if_fail(BAYES_IS_CLASSIFIER(classifier));
   g_return_if_fail(!storage || BAYES_IS_STORAGE(storage));

   priv = classifier->priv;

   g_clear_object(&priv->storage);
   priv->storage = storage ? g_object_ref(storage)
                           : bayes_storage_memory_new();
}

void
bayes_classifier_set_tokenizer (BayesClassifier *classifier,
                                BayesTokenizer   tokenizer,
                                gpointer         user_data,
                                GDestroyNotify   notify)
{
   BayesClassifierPrivate *priv;

   g_return_if_fail(BAYES_IS_CLASSIFIER(classifier));
   g_return_if_fail(tokenizer);

   priv = classifier->priv;

   if (priv->token_notify) {
      priv->token_notify(priv->token_user_data);
   }

   priv->token_func = tokenizer;
   priv->token_user_data = user_data;
   priv->token_notify = notify;
}

static gchar **
bayes_tokenizer_word (BayesClassifier *classifier,
                      const gchar     *text,
                      gpointer         user_data)
{
   GMatchInfo *match_info;
   gchar **ret = NULL;

   if (g_regex_match_all(gWordRegex, text, 0, &match_info)) {
      ret = g_match_info_fetch_all(match_info);
   }

   g_match_info_free(match_info);

   return ret;
}

/**
 * bayes_classifier_finalize:
 * @object: (in): A #BayesClassifier.
 *
 * Finalizer for a #BayesClassifier instance. Frees any resources held by
 * the instance.
 */
static void
bayes_classifier_finalize (GObject *object)
{
   G_OBJECT_CLASS(bayes_classifier_parent_class)->finalize(object);
}

/**
 * bayes_classifier_get_property:
 * @object: (in): A #GObject.
 * @prop_id: (in): The property identifier.
 * @value: (out): The given property.
 * @pspec: (in): A #ParamSpec.
 *
 * Get a given #GObject property.
 */
static void
bayes_classifier_get_property (GObject    *object,
                               guint       prop_id,
                               GValue     *value,
                               GParamSpec *pspec)
{
   BayesClassifier *classifier = BAYES_CLASSIFIER(object);

   switch (prop_id) {
   case PROP_STORAGE:
      g_value_set_object(value, bayes_classifier_get_storage(classifier));
      break;
   default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
   }
}

/**
 * bayes_classifier_set_property:
 * @object: (in): A #GObject.
 * @prop_id: (in): The property identifier.
 * @value: (in): The given property.
 * @pspec: (in): A #ParamSpec.
 *
 * Set a given #GObject property.
 */
static void
bayes_classifier_set_property (GObject      *object,
                               guint         prop_id,
                               const GValue *value,
                               GParamSpec   *pspec)
{
   BayesClassifier *classifier = BAYES_CLASSIFIER(object);

   switch (prop_id) {
   case PROP_STORAGE:
      bayes_classifier_set_storage(classifier, g_value_get_object(value));
      break;
   default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
   }
}

/**
 * bayes_classifier_class_init:
 * @klass: (in): A #BayesClassifierClass.
 *
 * Initializes the #BayesClassifierClass and prepares the vtable.
 */
static void
bayes_classifier_class_init (BayesClassifierClass *klass)
{
   GObjectClass *object_class;
   GError *error = NULL;

   object_class = G_OBJECT_CLASS(klass);
   object_class->finalize = bayes_classifier_finalize;
   object_class->get_property = bayes_classifier_get_property;
   object_class->set_property = bayes_classifier_set_property;
   g_type_class_add_private(object_class, sizeof(BayesClassifierPrivate));

   /**
    * BayesClassifier:storage:
    *
    * The "storage" property. The token state is stored using @storage.
    */
   gParamSpecs[PROP_STORAGE] =
      g_param_spec_object("storage",
                          _("Storage"),
                          _("The storage to use for tokens."),
                          BAYES_TYPE_STORAGE,
                          G_PARAM_READWRITE);
   g_object_class_install_property(object_class, PROP_STORAGE,
                                   gParamSpecs[PROP_STORAGE]);

   if (!(gWordRegex = g_regex_new("\\w+", 0, 0, &error))) {
      g_error("%s", error->message);
      g_error_free(error);
      g_assert(FALSE);
   }
}

/**
 * bayes_classifier_init:
 * @classifier: (in): A #BayesClassifier.
 *
 * Initializes the newly created #BayesClassifier instance.
 */
static void
bayes_classifier_init (BayesClassifier *classifier)
{
   classifier->priv =
      G_TYPE_INSTANCE_GET_PRIVATE(classifier,
                                  BAYES_TYPE_CLASSIFIER,
                                  BayesClassifierPrivate);
   classifier->priv->token_func = bayes_tokenizer_word;
   bayes_classifier_set_storage(classifier, NULL);
}
