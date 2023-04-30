# MO-SCENE EDITOR  
Members (Brooklyn Nets):  
Emerson Paul Celestial  
Aldrey Gaurana  
Joseph Christopher Santos  
Eryn Tallador  

To access the project, go to the master branch and clone it from there. The main entry point is located in the 'BNS_main.cpp' file.

Navigating Around the Level:
1. Go to the 'Scene Window'.
2. Rotate the camera view by dragging the mouse while pressing the right mouse button.
3. Press 'WASD' to move around the level.
4. Press 'QE' to move up and down the scene.
![image](https://user-images.githubusercontent.com/80930588/235347476-e7411054-90e7-453a-b30d-3db07f579041.png)

When Creating Objects:
1. In the menu toolbar, go to the 'Create Gameobject' tab and select a list of objects you want to spawn.
2. In the content browser, go to the mesh folder and find an object/mesh file. Double-click that file to spawn the object in the level.

When Deleting Objects:
1. Go to the 'Hierarchy Window' and select the gameobject you want to delete.
2. The properties of the object will show up in the 'Inspector Window'.
3. Press the 'Delete' button in the 'Inspector Window' to delete the object.
4. Alternatively, press the 'Delete' key on your keyboard to delete the object.

Modifying Gameobject's transform: 
1. Go to the 'Hierarchy Window' and select the gameobject you want to modify. The transform 
2. properties of the object will show up in the 'Inspector Window'. 
3. Modify the values in position, rotation, and scale by dragging the slider of each field.
![image](https://user-images.githubusercontent.com/80930588/235347423-e8062b6a-eba3-4c5c-8730-6ce97c5cc3b9.png)

Modifying Gameobject's components:
1. Go to the 'Hierarchy Window' and select the gameobject you want to modify.
2. The list of components will show up.
3. Modify the component's values.

Adding Textures to Meshes/Primitives:
1. Select the mesh/primitive using the Hierarchy Window.
2. Open the Textures folder in the Content Browser window at the bottom of the engine window.
3. Make sure the game object is selected.
4. Double-click on the .jpg or .png file you want to apply to the selected object.

Logging: 
1. Call BNS_Log::GetInstance()->WriteLog() when using the logging system. 
2. Include "BNS_Log.h" in the header/cpp file where you want to log. An example is BNS_Log::GetInstance()->WriteLog(BNS_Log::Warning, ""). 
3. The first parameter on WriteLog is the verbosity of the log: BNS_Log::Display, BNS_Log::Warning, and BNS_Log::Error. 
4. The second parameter is your log message formatted as an std::string.
![image](https://user-images.githubusercontent.com/80930588/235347384-512edc1e-ba00-470e-8c96-1a93238ab2e5.png)

"Multi-Threaded Task w/ Semaphores"
1. Load multiple files simultaneously.
2. Synchronously run threads.
3. Optimized instantiation of threads
![image](https://user-images.githubusercontent.com/80930588/235347338-b69711a1-086a-47a0-8f28-8e2cdf0c417b.png)
![image](https://user-images.githubusercontent.com/80930588/235347348-eaac65ee-1ec0-447a-bb33-248fc9fff7f3.png)


"Opening a Scene to the Editor"

FOR UNITY:
1. Ensure that the "exportedSceneUnity.json" file is in the Scene folder.
2. Go to SceneReader.cpp. On line 22, change "/exportedSceneUnreal" to "/exportedSceneUnity". "/exportedSceneUnreal" is used to open Unreal scenes.
3. Run the program and go to File -> Open Scene. The Unity scene will load automatically; just make sure to move the camera around.

FOR UNREAL:
1. Ensure that the "exportedSceneUnreal.json" file is in the Scene folder.
2. Go to SceneReader.cpp. On line 22, change "/exportedSceneUnity" to "/exportedSceneUnreal". "/exportedSceneUnity" is used to open Unity scenes.
3. Run the program and go to File -> Open Scene. The Unreal scene will load automatically; just make sure to move the camera around.

FOR SCENES MADE IN THE EDITOR:
1. Make sure that the "output.json" file is in the Scene folder.
2. Go to SceneReader.cpp. On line 22, change "/exportedSceneUnity" or "/exportedSceneUnreal" to "/output".
3. Run the program and go to File -> Open Scene. The editor scene will load automatically; just make sure to move the camera around.

Saving a Scene in the Editor:
1. Once you are done creating your scene, go to File -> Save. The scene data will be saved to a JSON file.
2. To make sure that it is saved, go to the "Scene" folder of the project directory, and look for "output.json". This file can now be used for loading engine scenes to Unity, Unreal, or on this Editor.

