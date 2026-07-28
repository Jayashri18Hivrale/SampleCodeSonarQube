cpputest XMC framework
===========================

This describes how to integrate CppUTest based testing into your production code base using the Visual Studio environment, and how to run the CI environment locally.  It is assumed that Visual Studio 2019 and Docker are installed in a Windows 10 machine.

### Install and build CppUTest locally (For Visual Studio)

To use Cpputest, it must first be built locally. To build it using Visual Studio:

1. Get CppUTest source using one of the following methods. Wherever you put the folder will be its "install" location, so choose a known, persistent location.
    * Clone from https://github.com/cpputest/cpputest.git to the "install" location. Use at least version [4.0](https://github.com/cpputest/cpputest/releases/download/v4.0/cpputest-4.0.zip).
2. Open the CppUTest Visual Studio Solution (`CppUTest_VS201x.sln`). Accept any Solution retargeting or conversions that VS prompts to do.
3. Click `Debug > Start Without Debugging`. VS should build and then execute CppUTest.
4. Confirm an overall status of `OK`, and 1000+ tests running. e.g.:
    ```
   OK (1085 tests, 1037 ran, 2505 checks, 48 ignored, 0 filtered out, 186 ms)
    ```
5. To make CppUTest available for use, [create environment variable](https://superuser.com/q/949560) `CPPUTEST_HOME` that points to the "install" location (the folder containing CppUTest_VS201x.sln) from (1).

See https://cpputest.github.io/#using-cpputest-with-visual-studio for additional information.


## Running the current CppuTest tests.
* Open the solution of the project (`TDD_CppuTest\vs-test-build\TDD.sln`)
* Build: `Debug > Start Without Debugging`
* You should see these test results in the console window:

    ```
    ...............
        OK (XX tests, 15 ran, 50 checks, 0 ignored, 0 filtered out, 3 ms)
    ```

## How to integrate tested code into the repository.
* Test files and production code files should all be kept in version control (.gitignore takes care of non required build files).
* The current folder structure and its recommended usage is:

    ```
    XMC
        |-- ci
        |-- Codesys
        |-- ...
        |-- TDD_CppuTests
                 |-- Hardware tests - includes small hardware projects that allows hardware related modules testing divided by folders
                 |-- inc            - unit test specific header files, can also hold production headers that do not fit in any folder of the XMC repository, good place to start a new module. Also this is the location to place header stubs and header mocks that are replaced from production code.
                 |-- src            - unit test specific source code, can also hold production headers that do not fit in any folder of the XMC repository, good place to start a new module. Also this is the location to place stubs and mocks that are replaced from production code.
                 |-- tests
                          |-- srctest      - all test files, one test file per module .cpp
                          |-- mocks        - any developed mock should be here (mock examples are here).
                          |-- helpers      - in order to have a space to create test specific modules and header files.
                 |-- vs-test-build  - visual studio project
    ```
* The current test approach is module based, so the solution has a library project (`ProductionLibCode`) and a Test project (`AllTests`). Any code to test has to be included as a module (`.h/.c` file pair) into the library project.
* To add code and tests, add the module source file (`.c`) to the `ProductionLibCode` project.
* It is recommended to start new modules in the `src\` and `inc\` folders of `TDD_CppuTests\` and move them when a proper location is defined to the production code. Notice that the `ProductionCodeLib` can include external source files as long as the required includes are added to the include directories paths.
* Add a corresponding `moduleTests.cpp` file to the `AllTests` project, use the example templates as support on how to do this.

## Recomended practices.
* Although not required, Test Driven Development is recommended. This means before writting the code, write the test and make it fail, then write the code that pass the test. Iterate until all functionality is implemented.
* If possible, use AAA (Arrange-Act-Assert) methodology for the test.
* Keep the tests simple enough that could be used as live documentation of the production code. Use the helper folder to create/clean.
* Try to keep tests as independent as possible of the code. This means try to use only definitions and function defined within the header of the module.
* Once the code passes a test, take some time to consider refactoring. Ask "can the code be written better than it is now?"
* Reference the book "Test-Driven Development for Embedded C" from James W. Greening.


## Testing hardware modules.

In order to emulate the hardware register definitions, cmsis files of the hardware modules are modified for two main purposes:

### 1. Compiler of Test CPU is not available.

CMSIS header files are designed to work for many embedded compilers, however common desktop compilers are not considered. All the compiler differences are handled in the file [`cmsis_compiler.h`](../Daughtercard/G4DC/Drivers/CMSIS/Include/cmsis_compiler.h#L275) which contains definitions to many of the compiler-specfic commands (primitives). Most of them are not available for desktop platforms. The modification made added "Test specific" (`__TDD_CPPUTEST__`) flag definition to override the embedded definitions in this file.

### 2. Embedded microcontroller memory is not available.

The physical hardware memory is not available in the test cpu, and must be emulated in RAM. Two main memory locations are emulated: 1) core processor, and 2) peripheral.

The **core processor** one can be emulated in processor specific header files such as [`core_cm0plus.h`](../Daughtercard/G4DC/Drivers/CMSIS/Include/core_cm0plus.h#L650) for ARM M0+ procesors and [`core_cm4.h`](../Daughtercard/G4DC/Drivers/CMSIS/Include/core_cm4.h#L1546) for ARM M4 processors. On these files the emulation is included using the same preprocessor flag (`__TDD_CPPUTEST__`).

For the **peripheral memory** is dependant on the specific microcontroller family or device. For the processor STM32G071xx, the header file that describe the peripheral memory map is [`stm32g071xx.h`](../G0/Drivers/CMSIS/Device/ST/STM32G0xx/Include/stm32g071xx.h#L648). This file as to be modified to emulate the physical memory for RAM when the preprocessor flag `__TDD_CPPUTEST__` is used.

The physical memory is replaced by an array of the same size in RAM that has to be defined in the test implementations, see [`cmsisMockStm32G0.cpp`](../TDD_CppuTest/tests/srctest/cmsisMockStm32G0.cpp) as example.

Once this is done, the emulated memory map can be accessed for testing purposes within the `AllTests` project.


## Verifing CI unit tests locally.

### Introduction
Running your unit tests locally before pushing your code to the CI pipeline is a crucial step to ensure the stability and reliability of your code. This process allows you to catch and fix issues early, ultimately saving time and reducing the risk of breaking the build.

### Differences Between Visual Studio and Linux-Based cpputest Compilers
It is important to note that Visual Studio and Linux-based cpputest use different compilers, which can lead to different results. The discrepancies may arise due to variations in compiler optimizations, standard library implementations, and platform-specific behaviors. By running tests in both environments, you ensure that your code is robust and portable.

### Benefits of Running CI Environment Locally
- **Early Detection of Issues**: Running CI tests locally helps identify issues before they are integrated into the main codebase, reducing the chances of breaking the build.
- **Consistency**: Ensuring tests pass locally helps maintain consistency between local and CI environments.
- **Faster Feedback Loop**: Immediate feedback from local tests speeds up the development process and enhances productivity.

### Using the Provided Makefile to Run CI Tests
The Makefile in `TDD_CppuTest/` is designed to streamline the process of running CI tests, both locally and in CI. Using the Makefile for local tests ensures that they will behave as similarly as possible to the tests run in the CI pipeline. The provided Makefile has comments that help explain its usage. Assuming that Docker is installed on the local machine, the steps to use it locally are:

#### 1. Create the container from the `ci\dockerfile\cpputest.dockerfile`
The container must be created on first use or if the `.dockerfile` has been modified. Assuming that workspace is at the root of the XMC repository and that you have a [GitLab personal access token](https://gitlab.com/-/user_settings/personal_access_tokens) properly configured, run the following commands in powershell:

```powershell
    docker build -t cpputest-ci `
      --build-arg GIT_USERNAME="your_gitlab_username" `
      --build-arg GIT_TOKEN="your_personal_access_token" `
      -f ci/dockerfiles/cpputest.dockerfile .
```
### 2. Running the unit tests locally.

Open a PowerShell terminal at the root of the XMC repository.
```powershell
cd "your_XMC_root_folder_path"
```
Run the docker container.
```powershell
 docker run --rm -v ${PWD}:/builds/dematic/pd-scs/xmc/XMC --workdir /builds/dematic/pd-scs/xmc/XMC -i cpputest-ci make -C TDD_CppuTest
```
If clean is needed use
```powershell
 docker run --rm -v ${PWD}:/builds/dematic/pd-scs/xmc/XMC --workdir /builds/dematic/pd-scs/xmc/XMC -i cpputest-ci make -C TDD_CppuTest clean
```
