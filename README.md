# ProjectEsky_UnrealIntegration

This is experimental, and still a work in progress.

What is missing:

- MRTK Integration
- Relocalization

How to use:

- Clone project
- Open Config/DataConfig.json and update your v2 render float variables, and set your display offsets, as well as your leapmotion tracker and camera rig offsets
- Open ProjectEsky.uproject, allow it to build the plugins
- The 'Test Map' within the Esky plugin's content is your go to default scene setup, alternatively just drag a 'NSRig' actor from the esky content folder into the scene
- Hit play, have fun!

Known Issues
- Having the editor open will prevent standalone builds with `esky enabled` not to work

