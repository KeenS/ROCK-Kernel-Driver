/* SPDX-License-Identifier: MIT */
/*
 * Copyright © 2019 Intel Corporation
 */

#ifndef __I915_GEM_SHRINKER_H__
#define __I915_GEM_SHRINKER_H__

#include <linux/bits.h>

struct drm_i915_private;
struct mutex;

/* i915_gem_shrinker.c */
unsigned long i915_gem_shrink(struct drm_i915_private *i915,
			      unsigned long target,
			      unsigned long *nr_scanned,
			      unsigned flags);
#define I915_SHRINK_UNBOUND	BIT(0)
#define I915_SHRINK_BOUND	BIT(1)
#define I915_SHRINK_ACTIVE	BIT(2)
#define I915_SHRINK_VMAPS	BIT(3)
#define I915_SHRINK_WRITEBACK	BIT(4)

unsigned long i915_gem_shrink_all(struct drm_i915_private *i915);
void i915_gem_driver_register__shrinker(struct drm_i915_private *i915);
void i915_gem_driver_unregister__shrinker(struct drm_i915_private *i915);

#endif /* __I915_GEM_SHRINKER_H__ */
