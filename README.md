# Unity C# Template Setter
This tool allows you to set the Unity C# Script Template easily for multiple Unity installations.

  # INI Files  
  INI Files are used in this program to specify the Editor folder in the Unity Installation folder.
  
  ## Example INI File
    C:/Unity/2019.3.10f1/Editor/
    C:/Unity/2017.1.5f1/Edtior/
    
  ## Executable
  There must be a file named `default_cs.txt` in the directory the executable is in. This file must contain the contents of the new C# script template. If it does not exist already, it will automatically create one based on the example found within main.cpp.
    
  You can drag and drop the .ini file onto the exectuable, or use the command prompt to run it like this:
  
  `set_unity_template.exe [[-b]ackup] [file]`
  
  where `[[-b]ackup]` enables the current template script, that is to be overwritten, be backed up before overwriting.
