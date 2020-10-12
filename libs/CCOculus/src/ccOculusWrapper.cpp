#include "ccOculusWrapper.h"

CCOCULUS_LIB_API unsigned int ovrGetConnectedControllerTypes(ovrSession session) {
	return ovr_GetConnectedControllerTypes(session);
}

CCOCULUS_LIB_API double ovrGetPredictedDisplayTime(ovrSession session, long long frameIndex)
{
	return ovr_GetPredictedDisplayTime(session, frameIndex);
}

CCOCULUS_LIB_API ovrTrackingState ovrGetTrackingState(ovrSession session, double absTime, ovrBool latencyMarker)
{
	return ovr_GetTrackingState(session, absTime, latencyMarker);
}

CCOCULUS_LIB_API ovrResult ovrGetTextureSwapChainCurrentIndex(ovrSession session, ovrTextureSwapChain chain, int * out_Index)
{
	return ovr_GetTextureSwapChainCurrentIndex(session, chain, out_Index);
}

CCOCULUS_LIB_API ovrResult ovrGetTextureSwapChainBufferGL(ovrSession session, ovrTextureSwapChain chain, int index, unsigned int * out_TexId)
{
	return ovr_GetTextureSwapChainBufferGL(session, chain, index, out_TexId);
}

CCOCULUS_LIB_API ovrResult ovrCommitTextureSwapChain(ovrSession session, ovrTextureSwapChain chain)
{
	return ovr_CommitTextureSwapChain(session, chain);
}

CCOCULUS_LIB_API ovrResult ovrSubmitFrame(ovrSession session, long long frameIndex, const ovrViewScaleDesc * viewScaleDesc, ovrLayerHeader const * const * layerPtrList, unsigned int layerCount)
{
	return ovr_SubmitFrame(session, frameIndex, viewScaleDesc, layerPtrList, layerCount);
}

CCOCULUS_LIB_API ovrResult ovrInitialize(const ovrInitParams * params)
{
	return ovr_Initialize(params);
}

CCOCULUS_LIB_API ovrResult ovrCreate(ovrSession * pSession, ovrGraphicsLuid * pLuid)
{
	return ovr_Create(pSession, pLuid);
}

CCOCULUS_LIB_API ovrHmdDesc ovrGetHmdDesc(ovrSession session)
{
	return ovr_GetHmdDesc(session);
}

CCOCULUS_LIB_API ovrResult ovrRecenterTrackingOrigin(ovrSession session)
{
	return ovr_RecenterTrackingOrigin(session);
}

CCOCULUS_LIB_API void ovrCalcEyePoses(ovrPosef headPose, const ovrPosef HmdToEyePose[2], ovrPosef outEyePoses[2])
{
	ovr_CalcEyePoses(headPose, HmdToEyePose, outEyePoses);
}

CCOCULUS_LIB_API void ovrShutdown()
{
	ovr_Shutdown();
}

CCOCULUS_LIB_API ovrEyeRenderDesc ovrGetRenderDesc(ovrSession session, ovrEyeType eyeType, ovrFovPort fov)
{
	return ovr_GetRenderDesc(session, eyeType, fov);
}

CCOCULUS_LIB_API ovrSizei ovrGetFovTextureSize(ovrSession session, ovrEyeType eye, ovrFovPort fov, float pixelsPerDisplayPixel)
{
	return ovr_GetFovTextureSize(session, eye, fov, pixelsPerDisplayPixel);
}

CCOCULUS_LIB_API ovrResult ovrGetTextureSwapChainLength(ovrSession session, ovrTextureSwapChain chain, int * out_Length)
{
	return ovr_GetTextureSwapChainLength(session, chain, out_Length);
}

CCOCULUS_LIB_API ovrResult ovrCreateMirrorTextureGL(ovrSession session, const ovrMirrorTextureDesc * desc, ovrMirrorTexture * out_MirrorTexture)
{
	return ovr_CreateMirrorTextureGL(session, desc, out_MirrorTexture);
}

CCOCULUS_LIB_API ovrResult ovrGetMirrorTextureBufferGL(ovrSession session, ovrMirrorTexture mirrorTexture, unsigned int * out_TexId)
{
	return ovr_GetMirrorTextureBufferGL(session, mirrorTexture, out_TexId);
}

CCOCULUS_LIB_API void ovrDestroyMirrorTexture(ovrSession session, ovrMirrorTexture mirrorTexture)
{
	ovr_DestroyMirrorTexture(session, mirrorTexture);
}

CCOCULUS_LIB_API void ovrDestroy(ovrSession session)
{
	ovr_Destroy(session);
}

CCOCULUS_LIB_API void ovrDestroyTextureSwapChain(ovrSession session, ovrTextureSwapChain chain)
{
	ovr_DestroyTextureSwapChain(session, chain);
}

CCOCULUS_LIB_API ovrResult ovrCreateTextureSwapChainGL(ovrSession session, const ovrTextureSwapChainDesc * desc, ovrTextureSwapChain * out_TextureSwapChain)
{
	return ovr_CreateTextureSwapChainGL(session, desc, out_TextureSwapChain);
}

CCOCULUS_LIB_API ovrResult ovrGetDevicePoses(ovrSession session, ovrTrackedDeviceType * deviceTypes, int deviceCount, double absTime, ovrPoseStatef * outDevicePoses)
{
	return ovr_GetDevicePoses(session, deviceTypes, deviceCount, absTime, outDevicePoses);
}

CCOCULUS_LIB_API double ovrGetTimeInSeconds()
{
	return ovr_GetTimeInSeconds();
}

CCOCULUS_LIB_API ovrResult ovrGetInputState(ovrSession session, ovrControllerType controllerType, ovrInputState * inputState)
{
	return ovr_GetInputState(session, controllerType, inputState);
}
