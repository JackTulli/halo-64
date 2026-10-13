/*
GL.H

OpenGL entry points used by the renderer, resolved at run time through
SDL_GL_GetProcAddress once the context exists (gl_functions_load).
*/

#ifndef __HALO_LINUX_GL_H
#define __HALO_LINUX_GL_H

#ifdef HALO_XBOX
/* the Xbox build has no OpenGL: xgpu.h only needs the type names */
typedef unsigned int GLuint;
typedef unsigned int GLenum;
#else

/* prototypes are declared only to give each pointer its exact type */
#define GL_GLEXT_PROTOTYPES 1
/* the XDK defines APIENTRY as __stdcall; OpenGL on Linux uses cdecl (on
Windows it is __stdcall too, and SDL would include windows.h without it) */
#pragma push_macro("APIENTRY")
#ifndef _WIN32
#undef APIENTRY
#endif
#ifdef HALO_ANDROID
#include <GLES3/gl32.h>
#include <GLES2/gl2ext.h>
#define GLAPIENTRY GL_APIENTRY
#else
#include <SDL3/SDL_opengl.h>
#endif
#pragma pop_macro("APIENTRY")

#ifdef HALO_ANDROID
/* OpenGL ES 3.2 (port/android/README.md); tools/android_gl_stubs.py reads
this list to generate the guest's entry points */
/* ANDROID_GL_FUNCTIONS_BEGIN */
#define GL_FUNCTIONS(X) \
	X(glGetString) \
	X(glGetIntegerv) \
	X(glCopyImageSubData) \
	X(glGenerateMipmap) \
	X(glGetError) \
	X(glEnable) \
	X(glDisable) \
	X(glViewport) \
	X(glDepthRangef) \
	X(glScissor) \
	X(glClearColor) \
	X(glClearDepthf) \
	X(glClearStencil) \
	X(glClear) \
	X(glColorMask) \
	X(glDepthMask) \
	X(glDepthFunc) \
	X(glStencilFunc) \
	X(glStencilOp) \
	X(glStencilMask) \
	X(glBlendFunc) \
	X(glBlendEquation) \
	X(glBlendColor) \
	X(glCullFace) \
	X(glFrontFace) \
	X(glPolygonOffset) \
	X(glLineWidth) \
	X(glPixelStorei) \
	X(glReadPixels) \
	X(glFinish) \
	X(glFlush) \
	X(glGenTextures) \
	X(glDeleteTextures) \
	X(glBindTexture) \
	X(glActiveTexture) \
	X(glTexImage2D) \
	X(glTexImage3D) \
	X(glTexSubImage2D) \
	X(glCompressedTexImage2D) \
	X(glCompressedTexImage3D) \
	X(glTexParameteri) \
	X(glTexParameteriv) \
	X(glTexParameterf) \
	X(glTexParameterfv) \
	X(glGenSamplers) \
	X(glBindSampler) \
	X(glSamplerParameteri) \
	X(glSamplerParameterf) \
	X(glSamplerParameterfv) \
	X(glGenFramebuffers) \
	X(glDeleteFramebuffers) \
	X(glBindFramebuffer) \
	X(glFramebufferTexture2D) \
	X(glCheckFramebufferStatus) \
	X(glBlitFramebuffer) \
	X(glDrawBuffers) \
	X(glReadBuffer) \
	X(glInvalidateFramebuffer) \
	X(glGenBuffers) \
	X(glDeleteBuffers) \
	X(glBindBuffer) \
	X(glBufferData) \
	X(glBufferSubData) \
	X(glBindBufferBase) \
	X(glBindBufferRange) \
	X(glGenVertexArrays) \
	X(glBindVertexArray) \
	X(glEnableVertexAttribArray) \
	X(glDisableVertexAttribArray) \
	X(glVertexAttribPointer) \
	X(glVertexAttribIPointer) \
	X(glVertexAttrib4fv) \
	X(glVertexAttribI4ui) \
	X(glDrawArrays) \
	X(glDrawElements) \
	X(glDrawElementsBaseVertex) \
	X(glCreateShader) \
	X(glShaderSource) \
	X(glCompileShader) \
	X(glGetShaderiv) \
	X(glGetShaderInfoLog) \
	X(glDeleteShader) \
	X(glCreateProgram) \
	X(glAttachShader) \
	X(glBindAttribLocation) \
	X(glLinkProgram) \
	X(glGetProgramiv) \
	X(glGetProgramInfoLog) \
	X(glUseProgram) \
	X(glGetUniformLocation) \
	X(glUniform1i) \
	X(glUniform1iv) \
	X(glUniform1f) \
	X(glUniform4fv) \
	X(glUniform2f) \
	X(glGenQueries) \
	X(glBeginQuery) \
	X(glEndQuery) \
	X(glGetQueryObjectuiv)
/* ANDROID_GL_FUNCTIONS_END */
#else
#define GL_FUNCTIONS(X) \
	X(glGetString) \
	X(glGetIntegerv) \
	X(glGetTexImage) \
	X(glCopyImageSubData) \
	X(glGenerateMipmap) \
	X(glGetError) \
	X(glEnable) \
	X(glDisable) \
	X(glViewport) \
	X(glDepthRange) \
	X(glScissor) \
	X(glClearColor) \
	X(glClearDepth) \
	X(glClearStencil) \
	X(glClear) \
	X(glColorMask) \
	X(glDepthMask) \
	X(glDepthFunc) \
	X(glStencilFunc) \
	X(glStencilOp) \
	X(glStencilMask) \
	X(glBlendFunc) \
	X(glBlendEquation) \
	X(glBlendColor) \
	X(glCullFace) \
	X(glFrontFace) \
	X(glPolygonMode) \
	X(glPolygonOffset) \
	X(glLineWidth) \
	X(glPixelStorei) \
	X(glReadPixels) \
	X(glFinish) \
	X(glFlush) \
	X(glClipControl) \
	X(glGenTextures) \
	X(glDeleteTextures) \
	X(glBindTexture) \
	X(glActiveTexture) \
	X(glTexImage2D) \
	X(glTexImage3D) \
	X(glTexSubImage2D) \
	X(glCompressedTexImage2D) \
	X(glCompressedTexImage3D) \
	X(glTexParameteri) \
	X(glTexParameteriv) \
	X(glTexParameterf) \
	X(glTexParameterfv) \
	X(glGenSamplers) \
	X(glBindSampler) \
	X(glSamplerParameteri) \
	X(glSamplerParameterf) \
	X(glSamplerParameterfv) \
	X(glGenFramebuffers) \
	X(glDeleteFramebuffers) \
	X(glBindFramebuffer) \
	X(glFramebufferTexture2D) \
	X(glCheckFramebufferStatus) \
	X(glBlitFramebuffer) \
	X(glDrawBuffers) \
	X(glGenBuffers) \
	X(glDeleteBuffers) \
	X(glBindBuffer) \
	X(glBufferData) \
	X(glBufferSubData) \
	X(glBufferStorage) \
	X(glMapBufferRange) \
	X(glBindBufferBase) \
	X(glGenVertexArrays) \
	X(glBindVertexArray) \
	X(glEnableVertexAttribArray) \
	X(glDisableVertexAttribArray) \
	X(glVertexAttribPointer) \
	X(glVertexAttribIPointer) \
	X(glVertexAttrib4fv) \
	X(glVertexAttribI4ui) \
	X(glDrawArrays) \
	X(glDrawElements) \
	X(glDrawElementsBaseVertex) \
	X(glCreateShader) \
	X(glShaderSource) \
	X(glCompileShader) \
	X(glGetShaderiv) \
	X(glGetShaderInfoLog) \
	X(glDeleteShader) \
	X(glCreateProgram) \
	X(glAttachShader) \
	X(glBindAttribLocation) \
	X(glBindFragDataLocation) \
	X(glLinkProgram) \
	X(glGetProgramiv) \
	X(glGetProgramInfoLog) \
	X(glUseProgram) \
	X(glGetUniformLocation) \
	X(glUniform1i) \
	X(glUniform1iv) \
	X(glUniform1f) \
	X(glUniform4fv) \
	X(glUniform2f) \
	X(glGenQueries) \
	X(glBeginQuery) \
	X(glEndQuery) \
	X(glGetQueryObjectuiv) \
	X(glDebugMessageCallback)
#endif

#define GL_DECLARE_FUNCTION(name) extern __typeof__(&name) halo_##name;
GL_FUNCTIONS(GL_DECLARE_FUNCTION)
#undef GL_DECLARE_FUNCTION

/* call sites use the ordinary names; gl_functions.c, which defines the
pointers, sees the declarations without these aliases */
#ifndef GL_FUNCTIONS_DEFINE
#ifdef HALO_ANDROID
#define glGetString halo_glGetString
#define glGetIntegerv halo_glGetIntegerv
#define glCopyImageSubData halo_glCopyImageSubData
#define glGenerateMipmap halo_glGenerateMipmap
#define glGetError halo_glGetError
#define glEnable halo_glEnable
#define glDisable halo_glDisable
#define glViewport halo_glViewport
#define glDepthRangef halo_glDepthRangef
#define glScissor halo_glScissor
#define glClearColor halo_glClearColor
#define glClearDepthf halo_glClearDepthf
#define glClearStencil halo_glClearStencil
#define glClear halo_glClear
#define glColorMask halo_glColorMask
#define glDepthMask halo_glDepthMask
#define glDepthFunc halo_glDepthFunc
#define glStencilFunc halo_glStencilFunc
#define glStencilOp halo_glStencilOp
#define glStencilMask halo_glStencilMask
#define glBlendFunc halo_glBlendFunc
#define glBlendEquation halo_glBlendEquation
#define glBlendColor halo_glBlendColor
#define glCullFace halo_glCullFace
#define glFrontFace halo_glFrontFace
#define glPolygonOffset halo_glPolygonOffset
#define glLineWidth halo_glLineWidth
#define glPixelStorei halo_glPixelStorei
#define glReadPixels halo_glReadPixels
#define glFinish halo_glFinish
#define glFlush halo_glFlush
#define glGenTextures halo_glGenTextures
#define glDeleteTextures halo_glDeleteTextures
#define glBindTexture halo_glBindTexture
#define glActiveTexture halo_glActiveTexture
#define glTexImage2D halo_glTexImage2D
#define glTexImage3D halo_glTexImage3D
#define glTexSubImage2D halo_glTexSubImage2D
#define glCompressedTexImage2D halo_glCompressedTexImage2D
#define glCompressedTexImage3D halo_glCompressedTexImage3D
#define glTexParameteri halo_glTexParameteri
#define glTexParameteriv halo_glTexParameteriv
#define glTexParameterf halo_glTexParameterf
#define glTexParameterfv halo_glTexParameterfv
#define glGenSamplers halo_glGenSamplers
#define glBindSampler halo_glBindSampler
#define glSamplerParameteri halo_glSamplerParameteri
#define glSamplerParameterf halo_glSamplerParameterf
#define glSamplerParameterfv halo_glSamplerParameterfv
#define glGenFramebuffers halo_glGenFramebuffers
#define glDeleteFramebuffers halo_glDeleteFramebuffers
#define glBindFramebuffer halo_glBindFramebuffer
#define glFramebufferTexture2D halo_glFramebufferTexture2D
#define glCheckFramebufferStatus halo_glCheckFramebufferStatus
#define glBlitFramebuffer halo_glBlitFramebuffer
#define glDrawBuffers halo_glDrawBuffers
#define glReadBuffer halo_glReadBuffer
#define glInvalidateFramebuffer halo_glInvalidateFramebuffer
#define glGenBuffers halo_glGenBuffers
#define glDeleteBuffers halo_glDeleteBuffers
#define glBindBuffer halo_glBindBuffer
#define glBufferData halo_glBufferData
#define glBufferSubData halo_glBufferSubData
#define glBindBufferBase halo_glBindBufferBase
#define glBindBufferRange halo_glBindBufferRange
#define glGenVertexArrays halo_glGenVertexArrays
#define glBindVertexArray halo_glBindVertexArray
#define glEnableVertexAttribArray halo_glEnableVertexAttribArray
#define glDisableVertexAttribArray halo_glDisableVertexAttribArray
#define glVertexAttribPointer halo_glVertexAttribPointer
#define glVertexAttribIPointer halo_glVertexAttribIPointer
#define glVertexAttrib4fv halo_glVertexAttrib4fv
#define glVertexAttribI4ui halo_glVertexAttribI4ui
#define glDrawArrays halo_glDrawArrays
#define glDrawElements halo_glDrawElements
#define glDrawElementsBaseVertex halo_glDrawElementsBaseVertex
#define glCreateShader halo_glCreateShader
#define glShaderSource halo_glShaderSource
#define glCompileShader halo_glCompileShader
#define glGetShaderiv halo_glGetShaderiv
#define glGetShaderInfoLog halo_glGetShaderInfoLog
#define glDeleteShader halo_glDeleteShader
#define glCreateProgram halo_glCreateProgram
#define glAttachShader halo_glAttachShader
#define glBindAttribLocation halo_glBindAttribLocation
#define glLinkProgram halo_glLinkProgram
#define glGetProgramiv halo_glGetProgramiv
#define glGetProgramInfoLog halo_glGetProgramInfoLog
#define glUseProgram halo_glUseProgram
#define glGetUniformLocation halo_glGetUniformLocation
#define glUniform1i halo_glUniform1i
#define glUniform1iv halo_glUniform1iv
#define glUniform1f halo_glUniform1f
#define glUniform4fv halo_glUniform4fv
#define glUniform2f halo_glUniform2f
#define glGenQueries halo_glGenQueries
#define glBeginQuery halo_glBeginQuery
#define glEndQuery halo_glEndQuery
#define glGetQueryObjectuiv halo_glGetQueryObjectuiv
#else
#define glGetString halo_glGetString
#define glGetIntegerv halo_glGetIntegerv
#define glGetTexImage halo_glGetTexImage
#define glCopyImageSubData halo_glCopyImageSubData
#define glGenerateMipmap halo_glGenerateMipmap
#define glGetError halo_glGetError
#define glEnable halo_glEnable
#define glDisable halo_glDisable
#define glViewport halo_glViewport
#define glDepthRange halo_glDepthRange
#define glScissor halo_glScissor
#define glClearColor halo_glClearColor
#define glClearDepth halo_glClearDepth
#define glClearStencil halo_glClearStencil
#define glClear halo_glClear
#define glColorMask halo_glColorMask
#define glDepthMask halo_glDepthMask
#define glDepthFunc halo_glDepthFunc
#define glStencilFunc halo_glStencilFunc
#define glStencilOp halo_glStencilOp
#define glStencilMask halo_glStencilMask
#define glBlendFunc halo_glBlendFunc
#define glBlendEquation halo_glBlendEquation
#define glBlendColor halo_glBlendColor
#define glCullFace halo_glCullFace
#define glFrontFace halo_glFrontFace
#define glPolygonMode halo_glPolygonMode
#define glPolygonOffset halo_glPolygonOffset
#define glLineWidth halo_glLineWidth
#define glPixelStorei halo_glPixelStorei
#define glReadPixels halo_glReadPixels
#define glFinish halo_glFinish
#define glFlush halo_glFlush
#define glClipControl halo_glClipControl
#define glGenTextures halo_glGenTextures
#define glDeleteTextures halo_glDeleteTextures
#define glBindTexture halo_glBindTexture
#define glActiveTexture halo_glActiveTexture
#define glTexImage2D halo_glTexImage2D
#define glTexImage3D halo_glTexImage3D
#define glTexSubImage2D halo_glTexSubImage2D
#define glCompressedTexImage2D halo_glCompressedTexImage2D
#define glCompressedTexImage3D halo_glCompressedTexImage3D
#define glTexParameteri halo_glTexParameteri
#define glTexParameteriv halo_glTexParameteriv
#define glTexParameterf halo_glTexParameterf
#define glTexParameterfv halo_glTexParameterfv
#define glGenSamplers halo_glGenSamplers
#define glBindSampler halo_glBindSampler
#define glSamplerParameteri halo_glSamplerParameteri
#define glSamplerParameterf halo_glSamplerParameterf
#define glSamplerParameterfv halo_glSamplerParameterfv
#define glGenFramebuffers halo_glGenFramebuffers
#define glDeleteFramebuffers halo_glDeleteFramebuffers
#define glBindFramebuffer halo_glBindFramebuffer
#define glFramebufferTexture2D halo_glFramebufferTexture2D
#define glCheckFramebufferStatus halo_glCheckFramebufferStatus
#define glBlitFramebuffer halo_glBlitFramebuffer
#define glDrawBuffers halo_glDrawBuffers
#define glGenBuffers halo_glGenBuffers
#define glDeleteBuffers halo_glDeleteBuffers
#define glBindBuffer halo_glBindBuffer
#define glBufferData halo_glBufferData
#define glBufferSubData halo_glBufferSubData
#define glBufferStorage halo_glBufferStorage
#define glMapBufferRange halo_glMapBufferRange
#define glBindBufferBase halo_glBindBufferBase
#define glGenVertexArrays halo_glGenVertexArrays
#define glBindVertexArray halo_glBindVertexArray
#define glEnableVertexAttribArray halo_glEnableVertexAttribArray
#define glDisableVertexAttribArray halo_glDisableVertexAttribArray
#define glVertexAttribPointer halo_glVertexAttribPointer
#define glVertexAttribIPointer halo_glVertexAttribIPointer
#define glVertexAttrib4fv halo_glVertexAttrib4fv
#define glVertexAttribI4ui halo_glVertexAttribI4ui
#define glDrawArrays halo_glDrawArrays
#define glDrawElements halo_glDrawElements
#define glDrawElementsBaseVertex halo_glDrawElementsBaseVertex
#define glCreateShader halo_glCreateShader
#define glShaderSource halo_glShaderSource
#define glCompileShader halo_glCompileShader
#define glGetShaderiv halo_glGetShaderiv
#define glGetShaderInfoLog halo_glGetShaderInfoLog
#define glDeleteShader halo_glDeleteShader
#define glCreateProgram halo_glCreateProgram
#define glAttachShader halo_glAttachShader
#define glBindAttribLocation halo_glBindAttribLocation
#define glBindFragDataLocation halo_glBindFragDataLocation
#define glLinkProgram halo_glLinkProgram
#define glGetProgramiv halo_glGetProgramiv
#define glGetProgramInfoLog halo_glGetProgramInfoLog
#define glUseProgram halo_glUseProgram
#define glGetUniformLocation halo_glGetUniformLocation
#define glUniform1i halo_glUniform1i
#define glUniform1iv halo_glUniform1iv
#define glUniform1f halo_glUniform1f
#define glUniform4fv halo_glUniform4fv
#define glUniform2f halo_glUniform2f
#define glGenQueries halo_glGenQueries
#define glBeginQuery halo_glBeginQuery
#define glEndQuery halo_glEndQuery
#define glGetQueryObjectuiv halo_glGetQueryObjectuiv
#define glDebugMessageCallback halo_glDebugMessageCallback

#endif
#endif

/* returns FALSE (and logs) if a required function is missing */
int gl_functions_load(void);

#endif /* HALO_XBOX */

#endif
