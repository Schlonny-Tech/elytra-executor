# Polytoria Executor (Official Windows Source)

This is the official source code for the Polytoria Executor for Windows.  
It is a tool used to run custom code on the Polytoria platform.

Anyone can contribute. Submit pull requests, report issues, or help improve the code.

Repository: https://github.com/Schlonny-Tech/elytra-executor

---

## Disclaimer

This repository is for educational purposes only. Use responsibly and follow Polytoria's Terms of Service.

---

## Important Note About Injection

This project builds to an `InternalDLL.dll` file. This is not a standalone executable.

To use the executor, you need an external injector to load `InternalDLL.dll` into the Polytoria process. Examples of injectors you can use:

- Extreme Injector
- Xenos Injector
- Any other manual map injector

Do not ask for support on specific injectors. Choose one and use it at your own risk.

---
## Build from Source

Requirements: Windows 10/11, Visual Studio 2022, CMake, Git (linux can also compile thanks to me .schlonny :D)

1. Clone the repo  
   `git clone https://github.com/Schlonny-Tech/elytra-executor.git`

2. Open the solution in Visual Studio and build.

3. Locate `InternalDLL.dll` in the output folder.

4. Use an injector to load `InternalDLL.dll` into the Polytoria process.

---

## How to Contribute

1. Fork the repo
2. Create a branch (`git checkout -b my-fix`)
3. Commit your changes
4. Push and open a pull request

All contributions are welcome.
