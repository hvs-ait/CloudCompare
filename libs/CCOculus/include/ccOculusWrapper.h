#pragma once
#include "CCOculus.h"

#include <OVR_CAPI.h>
#include <OVR_CAPI_GL.h>
CCOCULUS_LIB_API unsigned int ovrGetConnectedControllerTypes(ovrSession session);
CCOCULUS_LIB_API double ovrGetPredictedDisplayTime(ovrSession session, long long frameIndex);
CCOCULUS_LIB_API ovrTrackingState ovrGetTrackingState(ovrSession session, double absTime, ovrBool latencyMarker);
CCOCULUS_LIB_API ovrResult ovrGetTextureSwapChainCurrentIndex(ovrSession session, ovrTextureSwapChain chain, int* out_Index);
CCOCULUS_LIB_API ovrResult ovrGetTextureSwapChainBufferGL(ovrSession session, ovrTextureSwapChain chain, int index, unsigned int* out_TexId);
CCOCULUS_LIB_API ovrResult ovrCommitTextureSwapChain(ovrSession session, ovrTextureSwapChain chain);
CCOCULUS_LIB_API ovrResult ovrSubmitFrame(ovrSession session, long long frameIndex, const ovrViewScaleDesc* viewScaleDesc, ovrLayerHeader const* const* layerPtrList, unsigned int layerCount);
CCOCULUS_LIB_API ovrResult ovrInitialize(const ovrInitParams* params);
CCOCULUS_LIB_API ovrResult ovrCreate(ovrSession* pSession, ovrGraphicsLuid* pLuid);
CCOCULUS_LIB_API ovrHmdDesc ovrGetHmdDesc(ovrSession session);
CCOCULUS_LIB_API ovrResult ovrRecenterTrackingOrigin(ovrSession session);
CCOCULUS_LIB_API void ovrCalcEyePoses(ovrPosef headPose, const ovrPosef HmdToEyePose[2], ovrPosef outEyePoses[2]);
CCOCULUS_LIB_API void ovrShutdown();
CCOCULUS_LIB_API ovrEyeRenderDesc ovrGetRenderDesc(ovrSession session, ovrEyeType eyeType, ovrFovPort fov);
CCOCULUS_LIB_API ovrSizei ovrGetFovTextureSize(ovrSession session, ovrEyeType eye, ovrFovPort fov, float pixelsPerDisplayPixel);
CCOCULUS_LIB_API ovrResult ovrGetTextureSwapChainLength(ovrSession session, ovrTextureSwapChain chain, int* out_Length);
CCOCULUS_LIB_API ovrResult ovrCreateMirrorTextureGL(ovrSession session, const ovrMirrorTextureDesc* desc, ovrMirrorTexture* out_MirrorTexture);
CCOCULUS_LIB_API ovrResult ovrGetMirrorTextureBufferGL(ovrSession session, ovrMirrorTexture mirrorTexture, unsigned int* out_TexId);
CCOCULUS_LIB_API void ovrDestroyMirrorTexture(ovrSession session, ovrMirrorTexture mirrorTexture);
CCOCULUS_LIB_API void ovrDestroy(ovrSession session);
CCOCULUS_LIB_API void ovrDestroyTextureSwapChain(ovrSession session, ovrTextureSwapChain chain);
CCOCULUS_LIB_API ovrResult ovrCreateTextureSwapChainGL(ovrSession session, const ovrTextureSwapChainDesc* desc, ovrTextureSwapChain* out_TextureSwapChain);
CCOCULUS_LIB_API ovrResult ovrGetDevicePoses(ovrSession session, ovrTrackedDeviceType* deviceTypes, int deviceCount, double absTime, ovrPoseStatef* outDevicePoses);
CCOCULUS_LIB_API double ovrGetTimeInSeconds();
CCOCULUS_LIB_API ovrResult ovrGetInputState(ovrSession session, ovrControllerType controllerType, ovrInputState* inputState);