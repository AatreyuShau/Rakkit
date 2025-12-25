# Rakkit

Rakkit is an experimental compiled programming language and toolchain written in C++.

**Warning: Specific to Unix systems**

~Parses `.rk` source files
~Performs semantic analysis
~Transpiles Rakkit to `C`
~Compiles the generated `C` code
~Provides commands to run the generated `C` code

---

## Features

* `fn`-based syntax
* Integer and string literals _(so far)_ along with custom csv literals
* Variables (defined using `let`)
* Functions & function calls
* Built-in functions: `input`, `print`
* Automatic C code generation
* Single command compile & run
* File & directory inspection via `rak peep`

---

## Installation (macOS / Linux)

### 1️Prerequisites

Make sure you have:
* **Clang**
* **C++ compiler**

Check:

```bash
clang --version
```
Ensure its at least version 17.0.0

## 

### 2️Clone the repository

```bash
git clone https://github.com/AatreyuShau/Rakkit.git
cd Rakkit
```

## 

### 3️Build & Install Rakkit

```bash
./install.sh
```
Runnning this after cloning will auto install for you

*
    > for verification later on;
    > ```bash
    > rak
    > ```
    > You should see usage instructions.

---

## Example

### Rakkit code (`test/hello.rk`)

```rk
fn main() {
    let msg := "hello from rakkit"
    print(msg)
}
```

### Run it

```bash
rak race test/hello.rk
```

Output:

```text
Semantic analysis passed!
Generated output.c
[clang] compiling output.c...
[run]
hello world!!
input x here:
..
input y here:
..
.
```

---

## Commands

### `rak race <file.rk>`

Compiles and runs a Rakkit file.

---

### `rak peep <file | directory>`

Prints the contents of a file or **all files inside a directory**.

Examples:

```bash
rak peep test/hello.rk
```

---

## Removing / Cleaning Old Installs

Check where `rak` is coming from:

```bash
which rak
```

Remove it:

```bash
sudo rm /usr/local/bin/rak
```

---

## Platform Support

| Platform | Status      |
| -------- | ----------- |
| macOS    | Supported |
| Linux    | Supported |
| Windows  | Not yet   |

### Why Windows isn’t supported (yet)

* Uses POSIX APIs (`dirent.h`, `sys/stat.h`)
* Assumes Unix-style executables (`./output`)
* Relies on `clang` being available in PATH

---

## Notes & Limitations

* Strings are currently `const char*` in generated C
* No memory management (yet)
* No type inference beyond int/string
* No error recovery (compiler exits on first error)
* This is an **experimental language**, not production-ready

---

##LICENSE

[GNU GPL v3.0](https://github.com/AatreyuShau/Rakkit/blob/main/LICENSE)
