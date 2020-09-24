/* SPDX-License-Identifier: MIT */
#ifndef AMDKCL_DRM_BACKPORT_H
#define AMDKCL_DRM_BACKPORT_H

#include <linux/ctype.h>
#include <drm/drm_fourcc.h>
#include <kcl/kcl_drm.h>
#include <kcl/header/kcl_drm_file_h.h>
#if defined(HAVE_CHUNK_ID_SYNOBJ_IN_OUT)
#include <drm/drm_syncobj.h>
#endif

#if !defined(HAVE_DRM_GET_FORMAT_NAME_I_P)
/**
 * struct drm_format_name_buf - name of a DRM format
 * @str: string buffer containing the format name
 */
struct drm_format_name_buf {
	char str[32];
};

static char printable_char(int c)
{
	return isascii(c) && isprint(c) ? c : '?';
}

static inline const char *_kcl_drm_get_format_name(uint32_t format, struct drm_format_name_buf *buf)
{
	snprintf(buf->str, sizeof(buf->str),
		 "%c%c%c%c %s-endian (0x%08x)",
		 printable_char(format & 0xff),
		 printable_char((format >> 8) & 0xff),
		 printable_char((format >> 16) & 0xff),
		 printable_char((format >> 24) & 0x7f),
		 format & DRM_FORMAT_BIG_ENDIAN ? "big" : "little",
		 format);

	return buf->str;
}
#define drm_get_format_name _kcl_drm_get_format_name
#endif

#if !defined(HAVE_DRM_GEM_OBJECT_PUT_LOCKED)
#if defined(HAVE_DRM_GEM_OBJECT_PUT_UNLOCKED)
#define drm_gem_object_put _kcl_drm_gem_object_put
#endif
#endif

#ifndef HAVE_DRM_GEM_OBJECT_LOOKUP_2ARGS
static inline struct drm_gem_object *
_kcl_drm_gem_object_lookup(struct drm_file *filp, u32 handle)
{
	return drm_gem_object_lookup(filp->minor->dev, filp, handle);
}
#define drm_gem_object_lookup _kcl_drm_gem_object_lookup
#endif

#if defined(HAVE_CHUNK_ID_SYNOBJ_IN_OUT)
static inline
int _kcl_drm_syncobj_find_fence(struct drm_file *file_private,
						u32 handle, u64 point, u64 flags,
						struct dma_fence **fence)
{
#if defined(HAVE_DRM_SYNCOBJ_FIND_FENCE)
#if defined(HAVE_DRM_SYNCOBJ_FIND_FENCE_5ARGS)
	return drm_syncobj_find_fence(file_private, handle, point, flags, fence);
#elif defined(HAVE_DRM_SYNCOBJ_FIND_FENCE_4ARGS)
	return drm_syncobj_find_fence(file_private, handle, point, fence);
#else
	return drm_syncobj_find_fence(file_private, handle, fence);
#endif
#elif defined(HAVE_DRM_SYNCOBJ_FENCE_GET)
	return drm_syncobj_fence_get(file_private, handle, fence);
#endif
}
#define drm_syncobj_find_fence _kcl_drm_syncobj_find_fence
#endif

#if DRM_VERSION_CODE >= DRM_VERSION(4, 17, 0)
#define AMDKCL_AMDGPU_DMABUF_OPS
#endif

/*
 * commit v5.4-rc4-1120-gb3fac52c5193
 * drm: share address space for dma bufs
 */
#if DRM_VERSION_CODE < DRM_VERSION(5, 5, 0)
#define AMDKCL_DMA_BUF_SHARE_ADDR_SPACE
#endif

#endif/*AMDKCL_DRM_BACKPORT_H*/
