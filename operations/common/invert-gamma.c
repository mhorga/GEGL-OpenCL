/* This file is an image processing operation for GEGL
 *
 * GEGL is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * GEGL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GEGL; if not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2006 Øyvind Kolås <pippin@gimp.org>
 */

#include "config.h"
#include <glib/gi18n-lib.h>


#ifdef GEGL_PROPERTIES

   /* no properties */

#else

#define GEGL_OP_POINT_FILTER
#define GEGL_OP_C_SOURCE invert-gamma.c

#include "gegl-op.h"
#include "opencl/invert-gamma.cl.h"

static GeglClRunData *cl_data = NULL;

static void
prepare (GeglOperation *operation)
{
  gegl_operation_set_format (operation, "input", babl_format ("R'G'B'A float"));
  gegl_operation_set_format (operation, "output", babl_format ("R'G'B'A float"));
}

static gboolean
process (GeglOperation       *op,
         void                *in_buf,
         void                *out_buf,
         glong                samples,
         const GeglRectangle *roi,
         gint                 level)
{
  gfloat *in  = in_buf;
  gfloat *out = out_buf;

  while (samples--)
    {
      out[0] = 1.0 - in[0];
      out[1] = 1.0 - in[1];
      out[2] = 1.0 - in[2];
      out[3] = in[3];

      in += 4;
      out+= 4;
    }
  return TRUE;
}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass            *operation_class;
  GeglOperationPointFilterClass *point_filter_class;

  operation_class    = GEGL_OPERATION_CLASS (klass);
  point_filter_class = GEGL_OPERATION_POINT_FILTER_CLASS (klass);

  operation_class->prepare        = prepare;
  operation_class->opencl_support = TRUE;
    
  point_filter_class->process    = process;
  point_filter_class->cl_process = cl_process;

  gegl_operation_class_set_keys (operation_class,
    "name"       , "gegl:invert-gamma",
    "title",      _("Invert in Perceptual space"),
    "categories" , "color",
    "description",
       _("Inverts the components (except alpha), the result is the "
         "corresponding \"negative\" image."),
    NULL);
}

static gboolean
cl_process(GeglOperation       *operation,
           cl_mem              in,
           cl_mem              out,
           size_t              global_worksize,
           const GeglRectangle *roi,
           gint                level)
{
    if (!cl_data)
    {
        const char *kernel_name[] = {"cl_invertgamma", NULL};
        cl_data = gegl_cl_compile_and_build(invert_gamma_cl_source, kernel_name);
        return TRUE;
    }
    else
    {
        cl_int cl_err = 0;
        const size_t global_ws[2] = {global_worksize, 1};
        cl_err = gegl_cl_set_kernel_args(cl_data->kernel[0], sizeof(cl_mem), in, sizeof(cl_mem), out, NULL);
        CL_CHECK;
        cl_err = gegl_clEnqueueNDRangeKernel(gegl_cl_get_command_queue(), cl_data->kernel[0], 2, NULL, global_ws, NULL, 0, NULL, NULL);
        CL_CHECK;
        cl_err = gegl_clFinish(gegl_cl_get_command_queue());
        CL_CHECK;
        return TRUE;
    }
error:
    return FALSE;
}

#endif
