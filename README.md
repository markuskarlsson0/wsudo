# wsudo

A sudo-like utility for running commands with administrator privileges on Windows.

## Usage

```
wsudo [options] {command}
```

### Options

| Short Option | Long Option | Description |
|---|---|---|
| `-h` | `--help` | Display help message |
| `-v` | `--version` | Display version information |

### Examples

Start an administrator session in the current console:

```
wsudo
```

Exit administrator session:

```
exit
```

Run a command as administrator:

```
wsudo taskkill /f /im program.exe
```

## Development

### Requirements

- CMake 3.25+
- A C++20 compiler
- Ninja (when using the provided presets)

### Build

```sh
cmake --preset debug-x64
cmake --build --preset debug-x64
```

Other build configurations are available in `CMakePresets.json`.
