# ProjectEsky_UnrealIntegration

This is experimental, and still a work in progress.

What is working:

- Undistortion rendering
- Tracking (and relocalization)

What is not working:

- Temporal Reprojection

What is missing:

- MRTK Integration
- Config files

How to use:

- Clone project
- Open in unreal
- Copy the 'TestMap' resource (for you to edit)/Drag n Drop the 'NSRig' Actor into your map
- Edit the settings on the renderer component (Attached to the NSRig Actor)
- Copy your 4 float files (for v2 rendering) to the EskyNativeRenderer.h file
