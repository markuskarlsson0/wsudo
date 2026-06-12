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
