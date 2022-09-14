PACKAGECONFIG[opengl]     = "--enable-video-opengl,--disable-video-opengl"
#PACKAGECONFIG[gles2]      = "--enable-video-opengles,--disable-video-opengles,gles-user-module"
PREFERRED_PROVIDER_virtual/libgles2 ?= "gles-user-module"
