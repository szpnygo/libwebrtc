webrtc\src\build\config\win\BUILD.gn
默认编译的静态运行时库，如果需要动态运行时库，需要修改BUILD.gn文件，如下所示：
```
# Configures how the runtime library (CRT) is going to be used.
# See https://msdn.microsoft.com/en-us/library/2kzt1wy3.aspx for a reference of
# what each value does.
config("default_crt") {
  if (is_component_build) {
    # Component mode: dynamic CRT. Since the library is shared, it requires
    # exceptions or will give errors about things not matching, so keep
    # exceptions on.
    configs = [ ":dynamic_crt" ]
  } else {
    if (current_os == "winuwp") {
      # https://blogs.msdn.microsoft.com/vcblog/2014/06/10/the-great-c-runtime-crt-refactoring/
      # contains a details explanation of what is happening with the Windows
      # CRT in Visual Studio releases related to Windows store applications.
      configs = [ ":dynamic_crt" ]
    } else {
      # Desktop Windows: static CRT.
      configs = [ ":static_crt" ]
    }
  }
}
```