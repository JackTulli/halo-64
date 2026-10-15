# Optional Halo PC expansion in ROM Builder

Run the existing window from the repository root with Python 3.11 or newer:

```powershell
python -m tools.n64.rombuilder_main
```

The normal Xbox source and Multiplayer/Campaign selections are unchanged. Tick **PC expansion (optional)** to reveal its PC source controls. This creates `halo-pc-expansion.z64` separately from `halo-n64.z64` and `halo-campaign.z64`. Select your installed retail Halo PC `Maps` folder, its parent game folder, or an ISO9660 disc containing loose map files. PC Custom Edition caches and Xbox caches are rejected in the PC field. PC inputs are never read for a vanilla-only build.

A packed CAB/MSI installer disc cannot be used directly: install your own copy with its normal installer separately, then select its installed `Maps` folder. ROM Builder never runs autorun, MSI, setup executables or scripts from selected media. A loose-map ISO is read without mounting it. Only ISO9660 primary names are supported; unsupported layouts give an installed-folder fallback.

The six required expansion maps are Danger Canyon, Death Island, Gephyrophobia, Ice Fields, Infinity and Timberland, plus `ui.map` for their original thumbnails. Keep `bitmaps.map` and `sounds.map` beside them if supplied by your installation; these shared resource caches are staged too. The original Xbox data is still required for the base thirteen maps, menu, audio and Firefight source.

For a source run, point **Local PC conversion tools** at an explicitly prepared trusted folder containing `invader-extract.exe`, `invader-convert.exe`, `invader-refactor.exe`, `invader-bludgeon.exe`, `invader-edit.exe`, `invader-sound.exe`, `invader-build.exe`, `ffmpeg.exe`, and any DLLs those builds need. No tools are downloaded. Set `N64_TOOLS` to the native libdragon tool directory and `INVADER_DEFINITIONS` to the chosen Invader `src/tag/hek/definition` directory. Its matching `include/invader/hek/fourcc.hpp` and 1024-byte `src/bitmap/p8/p8_palette` must be present in the same source tree. The native libdragon tools folder must also include `mkasset` for the PC expansion. These are developer prerequisites; a complete packaged release supplies them.

Fresh ABI-compatible programs must exist at `build/n64/halo.elf.stripped` and `.sym`, `build/n64/campaign/halo.elf.stripped` and `.sym`, and, for PC expansion, `build/n64/pc-expansion/halo.elf.stripped` and `.sym`. Old programs/maps cannot substitute for the new H64M v2 MP32/CP16 format. Each installer run derives fresh cache files in its own work area. The PC program must include the admitted N64F v3 shared-effect-bank reader; a pre-pooling PC program is incompatible with the final packed filesystem.

CLI examples:

```powershell
# Original default: Xbox multiplayer and campaign only.
python -m tools.n64.rombuilder "D:\OwnedHaloXbox\maps" -o "D:\HaloROMs"

# Separate PC expansion only, using both owned source editions.
python -m tools.n64.rombuilder "D:\OwnedHaloXbox\maps" -o "D:\HaloROMs" --pc-expansion --pc-source "D:\OwnedHaloPC\Maps" --pc-tools "D:\TrustedHaloConversionTools"
```

The PC path is validated as retail version 7, copied and hashed, normalized with Invader and FFmpeg into six Xbox-version-5 converter intermediates, and explicitly passed to the PC expansion converters. The original thirteen Xbox inputs stay separate; no sibling folder enables PC imports automatically. Original PC UI thumbnails are extracted explicitly. All nineteen PC expansion maps then pass through libdragon `mkasset` at LZ4 level 1 with a 64 KiB window; the helper verifies the complete decoded map is byte-identical before replacing the generated map. After raw filesystem pruning, the reviewed FX helper derives a separate packed filesystem that shares only exact effect-record bytes. Raw source files stay unchanged, and no pruning or edits occur after pooling. The PC DFS uses that final packed tree; default multiplayer and campaign keep their existing installer asset flow. All final ROMs are checked against the 64 MiB cartridge maximum before publication; a failed or oversized build preserves the previous ROM and removes its own temporary output. PC and Xbox inputs are read-only. Temporary input/tag/audio/filesystem data is removed after completion or failure; converter logs remain in the work directory. Successful PC input, normalization and exact FX pooling manifests persist under `work/receipts/<run>/` with input/output/tool hashes.

To build the native Windows tools from a trusted libdragon source tree on a MinGW-w64 cross-build host, include the optional third flag for the PC compressor:

```sh
tools/n64/build_windows_tools.sh /path/to/libdragon build/windows-tools --pc-expansion
```

Omit that flag to build the original five-tool vanilla set. The PC packaging options require `mkasset.exe` in `--tools`; vanilla packaging does not.

To package a release using installed PyInstaller, Pillow, tkinterdnd2 and sv-ttk:

```powershell
python tools/n64/package_rombuilder.py --tools "D:\TrustedWindowsLibdragonTools" --elf build/n64 --invader "D:\InvaderSource" --out dist --pc-expansion-elf build/n64/pc-expansion --pc-tools "D:\TrustedHaloConversionTools" --pc-tool-notices "D:\MatchingToolLicences"
```

Omit the three PC packaging options together for a vanilla package. Keep Invader/FFmpeg licence and applicable source-offer information with the matching tool builds. Packaging includes program code, supplied tools, definitions and project-original paint art. It never bundles retail Halo maps, extracted tags, original PC UI pixels or a prebuilt Halo ROM. Copying a tool's DLLs is limited to the explicit supplied PC tools directory; dependency completeness still needs a clean-machine smoke test.

Source checks cover malformed/hostile input files, fresh same-size replacement, normalization command ordering and failures, explicit 13/19-map/campaign workers, optional GUI state and package composition. Native content conversion, a clean-machine executable test, rendered GUI review and hardware boot/play remain release gates.
