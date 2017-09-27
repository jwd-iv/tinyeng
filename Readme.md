
A C++ component-based game engine, built using [RIKU]. This engine is currently
intended for small-scale game prototyping, but will eventually be extended to a
more general-purpose game engine and editor. Also included is the tiny_core
library, which uses [ttvfs] and [jsoncpp] to provide default filesystem and
serializer implementations.

**Features**
 - Introspective
 - Data driven
 - Component based
 - Highly extensible


Using tinyeng
----------------------------------------

### Download

```bash
git clone /path/to/tinyeng https://github.com/jwd-iv/tinyeng
```


### Build with CMake

```bash
cd /path/to/build/folder
cmake /path/to/tinyeng
cmake /path/to/tinyeng --build
```

As part of the build process, CMake will download additional libraries. At a
minimum [RIKU] will be downloaded adjacent to the `tinyeng` source folder. If
the `TINY_CORE` CMake flag is enabled (which it is by default), [ttvfs] will be
also be downloaded to `ext/ttvfs`, next to both `RIKU` and `tinyeng`.


### Add to an existing CMake project

```cmake
add_subdirectory("/path/to/tinyeng" "${CMAKE_CURRENT_BINARY_DIR}/tiny_build")

target_link_libraries(your_library tiny)
```

If you want to use the tiny core library, link `tiny_core` instead of `tiny` with
`target_link_libraries()`. That will automatically link `tiny` as well.


### Link statically

Build and add tiny.lib and/or tiny_core.lib from the `build` folder to your
project, and add `tinyeng/include` to the list of include directories.


[RIKU]: https://github.com/jwd-iv/RIKU
[ttvfs]: https://github.com/fgenesis/ttvfs
[jsoncpp]: https://github.com/open-source-parsers/jsoncpp
