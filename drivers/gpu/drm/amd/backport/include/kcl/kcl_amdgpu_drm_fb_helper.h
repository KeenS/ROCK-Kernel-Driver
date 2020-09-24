/* SPDX-License-Identifier: MIT */
#ifndef AMDGPU_BACKPORT_KCL_AMDGPU_DRM_FB_HELPER_H
#define AMDGPU_BACKPORT_KCL_AMDGPU_DRM_FB_HELPER_H

#include <drm/drm_fb_helper.h>
#include <drm/drm_gem_framebuffer_helper.h>
#include "amdgpu.h"

#ifndef HAVE_DRM_FB_HELPER_LASTCLOSE
void drm_fb_helper_lastclose(struct drm_device *dev);
void drm_fb_helper_output_poll_changed(struct drm_device *dev);
#endif

static inline
void kcl_drm_gem_fb_set_obj(struct drm_framebuffer *fb, int index, struct drm_gem_object *obj)
{
#ifdef HAVE_DRM_DRM_GEM_FRAMEBUFFER_HELPER_H
	if (fb)
		fb->obj[index] = obj;
#else
	struct amdgpu_framebuffer *afb = to_amdgpu_framebuffer(fb);
	(void)index; /* for compile un-used warning */
	if (afb)
		afb->obj = obj;
#endif
}
#ifndef HAVE_DRM_DRM_GEM_FRAMEBUFFER_HELPER_H
struct drm_gem_object *drm_gem_fb_get_obj(struct drm_framebuffer *fb,
					  unsigned int plane);
#endif

#endif
