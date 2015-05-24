
# ============================================================
# change according to your options ===========================
# ============================================================

# one or the other
#CONFIG          *= application
#CONFIG          *= unitTests

# needed dependencies
EXTERNALS       =   $${PWD}/externals
CPPFORMAT_INC   =   $${EXTERNALS}/cppformat

# for unit tests
CATCH_INC       =   $${EXTERNALS}/catch

# ============================================================
# nothing to change below this line ==========================
# ============================================================



linux {
    contains(QMAKE_CC, gcc){
        SYSTEM      =   gcc
    }
    contains(QMAKE_CC, clang){
        SYSTEM      =   clang

#        equals(CLANG_SANITIZER, address) {
#            message("Address sanitizer")

#            system("export ASAN_SYMBOLIZER_PATH=/usr/bin/llvm-symbolizer-3.5")
#            QMAKE_CXXFLAGS  = -g -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls -stdlib=libc++
#            QMAKE_LFLAGS    = -g -fsanitize=address -lc++ -lc++abi #TODO remove gl functions but not working
#            QMAKE_LINK      = clang
#        }
#        else  { equals(CLANG_SANITIZER, memory) {
#            message("Memory sanitizer")

#            system("export MSAN_SYMBOLIZER_PATH=/usr/bin/llvm-symbolizer-3.5")
#            QMAKE_CXXFLAGS  = -g -fsanitize=memory -fsanitize-memory-track-origins -fno-omit-frame-pointer -fno-optimize-sibling-calls -stdlib=libc++
#            QMAKE_LFLAGS    = -g -fsanitize=memory -lc++ -lc++abi
#            QMAKE_LINK      = clang
#        }
#        else { equals(CLANG_SANITIZER, thread) {
#            message("Thread sanitizer")

#            QMAKE_CXXFLAGS  = -g -fsanitize=thread -stdlib=libc++
#            QMAKE_LFLAGS    = -g -fsanitize=thread -lc++ -lc++abi
#            QMAKE_LINK      = clang
#        }
#        else {
            QMAKE_CXXFLAGS  *= -stdlib=libc++ -Reverything -Wthread-safety
            QMAKE_LFLAGS    *= -lc++ -lc++abi

            equals(CLANG_SANITIZER, analyze) {
                QMAKE_CXXFLAGS  *=  --analyze
            }
#        }}}
    }

    BUILDDIR        =   $${PWD}/build/$${SYSTEM}
    SYSTEM          =   gcc ## clang is compatible with gcc for dependancies
}


# qmake config ----------------------------------
LANGUAGE            =   C++
TEMPLATE            =	app                         # executable app
TARGET              =   entity                      # executable name
DESTDIR             =   $$BUILDDIR                  # executable location
#CONFIG              *=  console
CONFIG              *=  precompile_header
PRECOMPILED_HEADER  =   precompiled.h
QMAKE_CXXFLAGS      *=  -std=c++14 #-flto #-fno-exceptions #-fopenmp
QT                  *=  core gui widgets
LIBS                *=  -lGLU -lGL

# include dirs ----------------------------------
INCLUDEPATH         *=  sources/
HEADERS             *=  sources/Log.h
SOURCES             *=  sources/Log.cpp

# defines --------------------------------------
CONFIG(release, release|debug) {
    DEFINES         *= NDEBUG
}

# dependencies
#INCLUDEPATH         *=  $${CPPFORMAT_INC}
INCLUDEPATH         *=  $${EXTERNALS}

include(sources/converse/converse.pri)
include(sources/display/display.pri)
include(sources/entity/entity.pri)
include(sources/signal/signal.pri)

application {
    TARGET          =   entity                      # executable name
    CONFIG          *=  exceptions_off
    SOURCES         *=  sources/main.cpp
    QMAKE_CXXFLAGS  *=  -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Weffc++
}

unitTests {
#    INCLUDEPATH     *=  $${CATCH_INC}
    TARGET          =   unit_test                      # executable name
#    CONFIG          *=  exceptions_off

    include(unitTests/unitTests.pri)
}






