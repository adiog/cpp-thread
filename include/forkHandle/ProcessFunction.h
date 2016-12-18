// This file is a part of cpp-thread project.
// Copyright (c) 2016 Aleksander Gajewski <adiog@brainfuck.pl>.

#ifndef CPP_THREAD_PROCESSFUNCTION_H
#define CPP_THREAD_PROCESSFUNCTION_H

#include <functional>
#include "forkHandle/FileDescriptorInputReaderStream.h"
#include "forkHandle/FileDescriptorOutputWriterStream.h"


using ProcessFunction = std::function<int(FileDescriptorInputReaderStream,
                                          FileDescriptorOutputWriterStream)>;

#endif //CPP_THREAD_PROCESSFUNCTION_H
