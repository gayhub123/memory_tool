# gperftools test

## contents
1. src
source code of gperftools and libunwind as submodules

2. tests
tests of gperftools components

## src version
1. gperftools
ssh: git@github.com:gperftools/gperftools.git
branch: master
commit: fe85bbdf4cb891a67a8e2109c1c22a33aa958c7e

2. libunwind
ssh: git@github.com:libunwind/libunwind.git
branch: v1.2-stable
commit: ac028085563082ba9262a5fbd050c3422c7fcff0

## compile
```
sh build.sh
```

## target test
1. cpu profiler test
2. heap profiler test