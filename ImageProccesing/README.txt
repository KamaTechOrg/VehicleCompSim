To run image recognition, follow the following instructions: 
Connect from GitHub and download the branch MidIntegration
Next, go to the folder VeichleCompSim > ImageProcessing > CMakeProjectOPENCV > data 
Now, download and extract the following two folders into it:

https://drive.google.com/file/d/1lxavCr4hZobaFjD5TIhaserxwM3S0Prd/view?usp=sharing
https://drive.google.com/file/d/10l3b44mZ22tT-GPQNUBuW5-KwB82cjVB/view?usp=sharing

You should now have 3 subfolders inside the data folder.
Go back one folder above (CMakeProjectOPENCV), and add a Build folder.
enter it and build as a CMAKE project: load the following commands:
cmake ..
cmake --build .
Now, go into the Debug subfolder created during the build process,
 and run the MyApp.exe file.