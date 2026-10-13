/*
XDK_DSOUND.H

DirectSound (the Xbox's) declarations the game and the platform layer
use.
(README.md)
*/

#ifndef HALO_XDK_DSOUND_H
#define HALO_XDK_DSOUND_H

#include "xdk_xbox.h"

/* ---------- macros */

/* Results. DS_OK is S_OK; the errors are the COM and DirectSound codes
Microsoft documents for DirectSound (learn.microsoft.com, "DirectSound
return values").
Source: cachebeta.exe, dsound_error (0x5b93e0...): it compares with
0x80040110, 0x80004005, 0x80004001, 0x8007000e, 0x88780078, 0x88780032 and
0x8878001e, naming each by the strings "DSERR_NOAGGREGATION" ... */
#define DS_OK ((HRESULT)0x00000000L)
#define DSERR_GENERIC ((HRESULT)0x80004005L)
#define DSERR_UNSUPPORTED ((HRESULT)0x80004001L)
#define DSERR_OUTOFMEMORY ((HRESULT)0x8007000EL)
#define DSERR_NOAGGREGATION ((HRESULT)0x80040110L)
#define DSERR_CONTROLUNAVAIL ((HRESULT)0x8878001EL)
#define DSERR_INVALIDCALL ((HRESULT)0x88780032L)
#define DSERR_NODRIVER ((HRESULT)0x88780078L)

/* Sample formats (WAVEFORMATEX.wFormatTag).
Source: cachebeta.exe: the game's buffers and streams store 1 for PCM and
0x69 for the Xbox's 4-bit ADPCM (progress bar setup at 0x4d2913 and
dsound_initialize_channel at 0x5bad66); WAVE_FORMAT_PCM is also the
documented Win32 value. */
#define WAVE_FORMAT_PCM 1
#define WAVE_FORMAT_XBOX_ADPCM 0x0069

/* Buffer creation flags (DSBUFFERDESC.dwFlags) and play flags.
Source: cachebeta.exe: the progress bar's buffers are created with
0x40100 and played with 1 (0x4d2955, 0x4d29c2); the split of 0x40100 is
the documented DirectSound one (DSBCAPS_CTRLPOSITIONNOTIFY 0x100,
DSBCAPS_LOCDEFER 0x40000; learn.microsoft.com, DSBUFFERDESC). */
#define DSBCAPS_CTRLPOSITIONNOTIFY 0x00000100
#define DSBCAPS_LOCDEFER 0x00040000
#define DSBPLAY_LOOPING 0x00000001

/* Volume floor (hundredths of a decibel) and frequency limits (Hz).
Source: cachebeta.exe: dsound_channel_set_I3DL2_properties stores -10000
(0x5b9093); dsound_frequency_from_pitch (0x5b8cb0) clamps to the floats
188.0 and 191983.0. */
#define DSBVOLUME_MIN (-10000)
#define DSBFREQUENCY_MIN 188
#define DSBFREQUENCY_MAX 191983

/* 3D processing modes and when 3D settings apply.
Source: cachebeta.exe, dsound_channel_set_location (0x5ba933...): SetMode
gets 0 for a spatialized sound and 2 otherwise, with apply 1 for
DS3D_DEFERRED. DS3DMODE_HEADRELATIVE and DS3D_IMMEDIATE are the documented
DirectSound values (learn.microsoft.com, IDirectSound3DBuffer8::SetMode and
DirectSound 3D "deferred settings"). */
#define DS3DMODE_NORMAL 0x00000000
#define DS3DMODE_HEADRELATIVE 0x00000001
#define DS3DMODE_DISABLE 0x00000002
#define DS3D_IMMEDIATE 0x00000000
#define DS3D_DEFERRED 0x00000001

/* A 3D voice's default distances.
Source: cachebeta.exe, the DirectSound library's voice set-up (0x5f8f89...)
stores 1.0 and 1e9 (0x4e6e6b28) as the minimum and maximum distances; the
same as DirectSound's documented defaults. */
#define DS3D_DEFAULTMINDISTANCE 1.0f
#define DS3D_DEFAULTMAXDISTANCE 1000000000.0f

/* Mix bins (dwMixBinMask and the SetMixBins/SetMixBinVolumes masks): one
bit per bin, bins 0-5 the speakers, 6-9 the crosstalk (surround
virtualizer) inputs, 10 the I3DL2 reverb input, 11-30 the effect sends.
Source: cachebeta.exe: dsound_initialize_channel uses 0x7 (front left,
right and center), 0x1833 (front and back pairs and sends 0 and 1) and 0x3;
dsound_fix_rear_speakers 0x1f00 (crosstalk back pair, I3DL2, sends 0 and
1); dsound_initialize passes 0x7fffffff for the four masks together. So the
front pair is 0x3, center 0x4, the back pair 0x30, sends 0 and 1 0x1800,
and crosstalk back pair plus I3DL2 0x700. Within each pair left (or send 0)
is taken as the lower bit, I3DL2 as the bin after the four crosstalk bins;
the binary does not tell those apart. The speaker order within 0-5 agrees
with Cxbx-Reloaded (cross-check only). */
#define DSMIXBIN_FRONT_LEFT 0x00000001
#define DSMIXBIN_FRONT_RIGHT 0x00000002
#define DSMIXBIN_FRONT_CENTER 0x00000004
#define DSMIXBIN_BACK_LEFT 0x00000010
#define DSMIXBIN_BACK_RIGHT 0x00000020
#define DSMIXBIN_XTLK_BACK_LEFT 0x00000100
#define DSMIXBIN_XTLK_BACK_RIGHT 0x00000200
#define DSMIXBIN_I3DL2 0x00000400
#define DSMIXBIN_FXSEND_0 0x00000800
#define DSMIXBIN_FXSEND_1 0x00001000
#define DSMIXBIN_SPEAKER_MASK 0x0000003F
#define DSMIXBIN_XTLK_MASK 0x000003C0
#define DSMIXBIN_FXSEND_MASK 0x7FFFF800

/* IDirectSound_GetSpeakerConfig: the console's audio setting.
Source: cachebeta.exe, dsound_initialize_channel tests 0x10000 for AC-3;
the Xbox Dev Wiki's EEPROM page gives the audio setting word as 0 for
stereo and 0x10000 for "enable AC3", which the library reports. */
#define DSSPEAKER_STEREO 0x00000000
#define DSSPEAKER_ENABLE_AC3 0x00010000

/* Stream creation flags (DSSTREAMDESC.dwFlags).
Source: cachebeta.exe, dsound_initialize_channel stores 0x10 for a 3D
channel (0x5bade7). */
#define DSSTREAMCAPS_CTRL3D 0x00000010

/* IDirectSoundStream_Pause's argument. The game never pauses a stream and
the binary has no use of it; only port/linux/src/dsound_sdl.c interprets
it. Resume 0, pause 1 (agrees with Cxbx-Reloaded, cross-check only). */
#define DSSTREAMPAUSE_RESUME 0x00000000
#define DSSTREAMPAUSE_PAUSE 0x00000001

/* Media packet status (*XMEDIAPACKET.pdwStatus and the stream callback's
status): COM results (S_OK, E_PENDING, E_ABORT, E_FAIL).
Source: cachebeta.exe, dsound_channel_callback (0x5ba460): it treats 0 and
0x80004004 as finished (the latter flushed), 0x80004005 as "status is
failure" and 0x8000000a as "status is pending". */
#define XMEDIAPACKET_STATUS_SUCCESS ((DWORD)0x00000000L)
#define XMEDIAPACKET_STATUS_PENDING ((DWORD)0x8000000AL)
#define XMEDIAPACKET_STATUS_FLUSHED ((DWORD)0x80004004L)
#define XMEDIAPACKET_STATUS_FAILURE ((DWORD)0x80004005L)

/* Media object stream information (XMEDIAINFO.dwFlags) and status
(GetStatus).
Source: cachebeta.exe: dsound_channel_fill (at 0x5ba419) queues more data
while bit 0 of the status is set, and the library's stream GetStatus
(0x5f42b1) reports 1 or 0. The library's stream GetInfo
(0x5f420c) reports 0x15; which of those bits is fixed sample size and
which asynchronous input the binary does not show: 0x1 and 0x4 (agrees
with Cxbx-Reloaded, cross-check only). */
#define XMO_STATUSF_ACCEPT_INPUT_DATA 0x00000001
#define XMO_STREAMF_FIXED_SAMPLE_SIZE 0x00000001
#define XMO_STREAMF_INPUT_ASYNC 0x00000004

/* ---------- functions */

/* The stream's media object methods, called through its vtable
(IDirectSoundStreamVtbl, from cachebeta.pdb).
Source: cachebeta.exe has no functions of these names: the game calls
through lpVtbl, and port/linux/src/dsound_sdl.c implements them as the
vtable's entries. The ones with their own entry points (SetVolume,
SetMixBins, ...) are prototyped in xdk_pdb.h. */
#define IDirectSoundStream_AddRef(stream) \
	((stream)->lpVtbl->AddRef(stream))
#define IDirectSoundStream_Release(stream) \
	((stream)->lpVtbl->Release(stream))
#define IDirectSoundStream_GetInfo(stream, information) \
	((stream)->lpVtbl->GetInfo((stream), (information)))
#define IDirectSoundStream_GetStatus(stream, status) \
	((stream)->lpVtbl->GetStatus((stream), (status)))
#define IDirectSoundStream_Process(stream, input, output) \
	((stream)->lpVtbl->Process((stream), (input), (output)))
#define IDirectSoundStream_Discontinuity(stream) \
	((stream)->lpVtbl->Discontinuity(stream))
#define IDirectSoundStream_Flush(stream) \
	((stream)->lpVtbl->Flush(stream))

#endif
