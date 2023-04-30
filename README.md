# MO-SCENE EDITOR  
Members (Brooklyn Nets):  
Emerson Paul Celestial  
Aldrey Gaurana  
Joseph Christopher Santos  
Eryn Tallador  


Go to master branch. The project can be cloned at the master branch.  
Main entry point is in the 'BNS_main.cpp'  

Navigating Around the Level:  
1. Go to the 'Scene Window'.  
2. Drag your mouse while pressing the right mouse button to rotate the camera view.  
3. 'WASD' are the keys to press for moving around the level.  
4. 'QE' to move up and down the scene.  

When Creating Objects:  
1. In the menu toolbar, go to the 'Create Gameobject' tab and select a list of objects you want to spawn.
2. In the content browser, go to the mesh folder and find an object / mesh file. Double-click that file to spawn the object in the level.

When Deleting Objects:
1. Go to the 'Hierarchy Window' and select the gameobject you want to delete.
2. After selecting, the properties of the object will show up in the 'Inspector Window'.
3. Press the 'Delete' button in the 'Inspector Window' to delete the object.
4. You can also delete the object by pressing the 'Delete' key on your keyboard.

Modifying Gameobject's transform:
1. Go to the 'Hierarchy Window' and select the gameobject you want to modify its transform.
2. After selecting, the transform properties of the object will show up in the 'Inspector Window'.
3. Modify the values in position, rotation and scale by dragging the slider of each field.

Modifying Gameobject's components:
1. Go to the 'Hierarchy Window' and select the gameobject you want to modify its components.
2. After selecting, the list of components wil show.
3. Modify the component's values.

Adding Textures to Meshes/Primitives:
1. Once a mesh/primitive is created, select it using the Hierarchy Window.
2. Navigate the Content Browser window on the bottom of the engine window and open the Textures folder.
3. Make sure that the game object is selected
4. If it is selected, double click on the .jpg or .png file that you want as they can be applied onto the selected object.

Opening a Scene to the Editor

FOR UNITY
1. Make sure that the "exportedSceneUnity.json" file is in the Scene folder.
2. Go to SceneReader.cpp. On line 22, change "/exportedSceneUnreal" to "/exportedSceneUnity". "/exportedSceneUnreal" is used to open Unreal scenes.
3. Run the program and go to File -> Open Scene. The Unity scene will load automatically, just make sure to move the camera around.

FOR UNREAL 
1. Make sure that the "exportedSceneUnreal.json" file is in the Scene folder.
2. Go to SceneReader.cpp. On line 22, change "/exportedSceneUnity" to "/exportedSceneUnreal". "/exportedSceneUnity" is used to open Unity scenes.
3. Run the program and go to File -> Open Scene. The Unity scene will load automatically, just make sure to move the camera around.

FOR SCENES MADE IN THE EDITOR
1. Make sure that the "output.json" file is in the Scene folder.
2. Go to SceneReader.cpp. On line 22, change "/exportedSceneUnity" or "/exportedSceneUnreal" to "/output".
3. Run the program and go to File -> Open Scene. The editor scene will load automatically, just make sure to move the camera around.

Saving a Scene in the Editor
1. Once you are done creating your scene, go to File -> Save. The scene data will be saved to a JSON file.
2. To make sure that it is saved, go to the "Scene" folder of the project directory, and look for "output.json". This file can now be used for loading engine scenes to Unity, Unreal, or on this Editor.

Logging
1. Whenever a user wants to use the logging system, call BNS_Log::GetInstance()->WriteLog();
2. Make sure that "BNS_Log.h" is included in the header/cpp file on where you want to log.
3. An example is BNS_Log::GetInstance()->WriteLog(BNS_Log::Warning, "");
4. The first parameter on WriteLog is the verbosity of the log: BNS_Log::Display, BNS_Log::Warning, and BNS_Log::Error.
5. The second parameter is your log message formatted as an std::string.
