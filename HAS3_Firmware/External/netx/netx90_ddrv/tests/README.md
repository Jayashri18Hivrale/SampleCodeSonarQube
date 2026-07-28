CppUTtest framework
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


## How to integrate tested code into the `netx90_ddrv` library.

* Test files and production code files should all be kept in version control. The `.gitignore` file manages exclusion of non-required build artifacts.
* It is recommended to follow Test-Driven Development (TDD), where test cases are written before or alongside the implementation of modules. TDD ensures better code design, maintainability, and reliability by encouraging developers to think about edge cases and expected behavior upfront.

### Folder structure and its recommended usage:

```
netx90_ddrv
    |-- build
    |-- include
    |-- src
    |-- tests
        |-- _build       - All build-related files, organized by compiler.
        |-- _hardware    - Contains small hardware projects for testing hardware-related modules.
        |-- helpers      - Contains utility modules and header files to organize and structure tests.
        |-- mocks        - Developed mocks to simulate module behavior for testing.
        |-- stubs        - Trivial implementations of modules to enable compilation during testing.
        |-- test_cases   - Contains all test files, with one test file per module (.cpp).
```

### Integrating a new module:

1. **Source Code Structure:**  
   Each module should be added as a **module pair** with:
   
   - `module.c`: The implementation file, placed inside the `src` directory.
   - `module.h`: The corresponding header file, placed inside the `include` directory. The header defines the module's public interface (functions, macros, types).

   For example, if you are creating a module for memory management:
   ```
   src/memory_manager.c
   include/memory_manager.h
   ```

2. **Test Structure:**  
   Each module should have a dedicated test file within the `tests/test_cases` directory, named after the module it tests.  
   Example:
   ```
   tests/test_cases/memory_managerTests.cpp
   ```

   The test file should follow a structured approach:
   
   - Include the module's header (`memory_manager.h`).
   - Include any necessary mock or stub headers from the `mocks` and `stubs` directories.
   - Write test cases that validate module behavior, covering different scenarios such as normal operation, edge cases, and error handling.
   - Use helper functions from the `helpers` directory if needed to keep test code clean and maintainable.

3. **Mocks, Stubs, and Helpers:**  
   - **Mocks:** Used when testing code that depends on external modules or services. Place them in the `mocks` directory.
   - **Stubs:** Provide minimal implementations to allow module compilation during testing. Place these in the `stubs` directory.
   - **Helpers:** Organize reusable code for setting up tests. Place these in the `helpers` directory.


### Test-Driven Development (TDD)

Although not required, Test-Driven Development is recommended. This means before writing the code, write the test and make it fail, then write the code that passes the test. Iterate until all functionality is implemented.

#### Benefits of using TDD:

- **Improved Code Quality:** Writing tests first forces developers to think through the expected functionality and edge cases before implementation.
- **Reduced Debugging Time:** Early detection of issues through automated tests.
- **Refactoring Support:** Well-written tests act as a safety net when making changes or optimizations to the codebase.
- **Documentation:** Test cases serve as live documentation for module functionality, making it easier for other developers to understand and use your code.

### Recommended Guidelines

1. **Use AAA (Arrange-Act-Assert) Methodology:**  
   - Structure your tests using Arrange (set up necessary objects), Act (perform the operation), and Assert (verify the result).
   
2. **Live Documentation:**  
   - Keep the tests simple enough that they can serve as live documentation of the production code. Use the `helpers` folder to create or clean up reusable components that improve test clarity and organization.

3. **Test Independence:**  
   - Try to keep tests as independent as possible from the code implementation. This means relying only on definitions and functions declared within the module's header file (`module.h`).

4. **Refactoring:**  
   - Once the code passes a test, take time to consider refactoring. Ask yourself, "Can the code be written better than it is now?"

5. **Reference Material:**  
   - For further reading, refer to the book *"Test-Driven Development for Embedded C"* by James W. Grenning.




## Running unit tests using Visual Studio 2019.
* Open the solution of the project (`_build\vs\netx90_ddrvTests.sln`)
* Build: `Debug | x64 | Local Windows Debugger`
* You should see these test results in the console window:

    ```
    ...............
        OK (XX tests, 15 ran, 50 checks, 0 ignored, 0 filtered out, 3 ms)
    ```

## Running Unit Tests Using a Linux-Based cppUTest Compiler (Makefile)

The Makefile provided in `_build/gnu_make` is designed to streamline the process of running tests in a Linux-based GNU compiler environment. This setup can later be used to perform continuous integration (CI) tests in the remote repository. Running tests locally using the Makefile ensures that they behave as consistently as possible with tests executed in a CI pipeline. The Makefile includes comments to explain its usage.

Additionally, a Docker file is provided to create a consistent testing environment, independent of the operating system on the local development machine. This approach helps maintain compatibility and reproducibility across different environments. 

### Running Linux-Based cppUTest on Windows

To run the tests on a Windows development computer, assuming Docker is installed, follow these steps:


**1. Create the container from the `test\_build\gnu_make\cpputest.dockerfile`**

The container must be created on first use or if the `.dockerfile` has been modified. Assuming that workspace is at the root of the repository and that you have a [GitLab personal access token](https://gitlab.com/-/user_settings/personal_access_tokens) properly configured, run the following commands in powershell:

```powershell
 docker build -t cpputest-ci `
      --build-arg GIT_USERNAME="your_gitlab_username" `
      --build-arg GIT_TOKEN="your_personal_access_token" `
      -f ci/dockerfiles/cpputest.dockerfile .
```
**2. Running the unit tests locally.**

A script is created to run in windows power-shell, this script is located in `tests/_build/gnu_make` and can be run using. Make sure docker is running and that the cpputest-ci image was created.

```powershell
.\run_unit_tests.ps1
```

If clean is needed use
```powershell
.\run_unit_tests.ps1 -Clean
```

### **Differences Between Visual Studio and Linux-Based cppUTest Compilers**  
Visual Studio and Linux-based cppUTest environments use different compilers, which can lead to varying test results. These discrepancies often stem from differences in compiler optimizations, standard library implementations, and platform-specific behaviors. By running tests on both platforms, you increase the robustness and portability of your code, catching potential issues early.

### **Benefits of Running Tests Locally on Linux**  
- **Early Issue Detection**: Running tests locally on Linux helps catch issues early, before they reach the main codebase, reducing the risk of breaking the build.  
- **Consistency Across Environments**: Local testing ensures compatibility with CI environments, improving consistency and reducing surprises during integration.  
- **Faster Feedback Loop**: Local tests provide immediate feedback, enabling faster iteration and development. This avoids delays caused by internet connectivity issues or cloud-based compile times, streamlining the merge process.
