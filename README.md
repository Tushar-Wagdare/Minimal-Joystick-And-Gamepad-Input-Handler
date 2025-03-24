# **Minimal Joystick and Gamepad Input Handler** 🎮  

## **Overview**  
This project is a **lightweight joystick and gamepad input handler** built using **C, C++, and the Win32 API**. It detects and processes controller inputs such as joystick movements and button presses, making it ideal for game development and real-time input tracking on Windows systems.  

## **Features**  
✅ Detects joystick movements and button presses  
✅ Built using **C, C++, and the Win32 API**  
✅ Minimal and optimized codebase  
✅ Compatible with various controllers  
✅ Easy to integrate into Windows-based C++ projects  
✅ Logs and processes input events efficiently  

## **Installation & Usage**  

### **1. Clone the Repository**  
```sh  
git clone https://github.com/Tushar-Wagdare/Minimal-Joystick-And-Gamepad-Input-Handler.git  
cd Minimal-Joystick-And-Gamepad-Input-Handler  
```

### **2. Build the Project**  
#### **Using the provided batch script:**
Use of x64 Native command prompt is recommended

Run `build.bat` to compile the project.  

#### **Manual Compilation:**  
If you want to compile manually, use a C++ compiler that supports the **Win32 API** (such as MinGW or MSVC).  
Example command using g++ (MinGW):  
```sh  
g++ Controller.cpp -o gamepad_handler.exe -mwindows  
```

### **3. Run the Application**  
Execute the compiled binary to start detecting controller inputs.  

## **Project Structure**  
```
📂 Minimal-Joystick-And-Gamepad-Input-Handler  
│── .gitignore          # Git ignore file  
│── Controller.cpp      # Main source file handling gamepad input  
│── Logger.h            # Header file for logging functionalities  
│── Window.h            # Header file for window handling  
│── Window.rc           # Resource script  
│── MyIcon.ico          # Application icon  
│── build.bat           # Build script for easy compilation  
│── README.md           # Project documentation  
```

## **How It Works**  
- Uses the **Win32 XInput API** to detect connected game controllers.  
- Captures **joystick movements** and **button presses** in real time.  
- Logs and processes input data efficiently.  

## **Requirements**  
✔ Windows OS  
✔ C++ Compiler with Win32 API support (MinGW, MSVC, etc.)  


