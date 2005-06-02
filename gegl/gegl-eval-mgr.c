/*
 *   This file is part of GEGL.
 *
 *    GEGL is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    GEGL is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with GEGL; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Copyright 2003 Calvin Williamson
 *
 */

#include "config.h"

#include <glib-object.h>

#include "gegl-types.h"

#include "gegl-eval-mgr.h"
#include "gegl-eval-visitor.h"
#include "gegl-node.h"
#include "gegl-visitable.h"
#include "gegl-property.h"


static void gegl_eval_mgr_class_init (GeglEvalMgrClass *klass);
static void gegl_eval_mgr_init       (GeglEvalMgr      *self);


G_DEFINE_TYPE(GeglEvalMgr, gegl_eval_mgr, GEGL_TYPE_OBJECT)


static void
gegl_eval_mgr_class_init (GeglEvalMgrClass * klass)
{
}

static void
gegl_eval_mgr_init (GeglEvalMgr *self)
{
}

/**
 * gegl_eval_mgr_apply:
 * @self: a #GeglEvalMgr.
 * @root:
 * @property_name:
 *
 * Update this property.
 *
 **/
void
gegl_eval_mgr_apply (GeglEvalMgr * self,
                     GeglNode    *root,
                     const gchar *property_name)
{
  GeglVisitor *visitor;
  GeglProperty *property;

  g_return_if_fail (GEGL_IS_EVAL_MGR (self));
  g_return_if_fail (GEGL_IS_NODE (root));

  g_object_ref(root);

  property = gegl_node_get_property(root, property_name);

#if 0
  /* This part does the evaluation of the ops, depth first. */
  gegl_log_debug(__FILE__, __LINE__,"eval_mgr_apply",
                 "begin eval-compute for node: %s %p property: %s",
                 G_OBJECT_TYPE_NAME(root), root, property_name);
#endif

  visitor = g_object_new(GEGL_TYPE_EVAL_VISITOR, NULL);
  gegl_visitor_dfs_traverse(visitor, GEGL_VISITABLE(property));
  g_object_unref(visitor);

  g_object_unref(root);
}
